#include "info_ui_remote_connect.h"
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_interface.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_wifi_types_generic.h"
#include "cstring"
#include "lwip/sockets.h"
#include <string>

namespace info_ui {
namespace remote_connect {

#define WIFI_MAXINUM_TETRY      (5)                 // WI-FI 最大尝试连接次数
static int32_t _wifi_retry_num = 0;                 // WI-FI 重连的次数
static const char* WIFI_TAG = "WIFI_TAG";


// wifi事件的回调函数
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:              // WI-FI站点模式启动
                esp_wifi_connect();                 // 开始连接WI-FI
                break;
            case WIFI_EVENT_STA_DISCONNECTED:       // WI-FI断开连接
                if (_wifi_retry_num < WIFI_MAXINUM_TETRY)  {
                    esp_wifi_connect();
                    _wifi_retry_num++;
                }
                else {
                    printf("WI-FI Connect Fail\n");
                    xEventGroupSetBits(*((EventGroupHandle_t*)arg), WIFI_FAIL_BIT);
                }
                break;
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        // 获取IP成功
        printf("WI-FI Connect Success\n");
        _wifi_retry_num = 0;
        xEventGroupSetBits(*(EventGroupHandle_t*)arg, WIFI_CONNECTED_BIT);
    }
}

wifi::wifi(std::string ssid, std::string password, std::string server_ip, std::string server_port) {
    this->_wifi_ssid = ssid;    
    this->_wifi_password = password;
    this->_wifi_server_ip = server_ip;
    this->_wifi_server_port = server_port;
}

void wifi::init() {
    this->_wifi_event_group = xEventGroupCreate();    

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();                // 创建默认wifi站点接口
    
    // 初始化WI-FI
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 注册事件处理函数   
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, &(this->_wifi_event_group)));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, &(this->_wifi_event_group)));
}

esp_err_t wifi::connect() {
    //------------------------------配置WI-FI参数------------------------------
    wifi_config_t cfg;
    strcpy((char*)cfg.sta.ssid, this->_wifi_ssid.c_str());
    strcpy((char*)cfg.sta.password, this->_wifi_password.c_str());

    //------------------------------设置WI-FI模式并启动------------------------------
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &cfg));
    ESP_ERROR_CHECK(esp_wifi_start());              // 启动!

    //------------------------------等待连接结果------------------------------
    EventBits_t bits = xEventGroupWaitBits(this->_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        this->_wifi_is_connected = true;
        // 进行socket连接
        if (!this->_wifi_tcp_socket.connect(this->_wifi_server_ip, this->_wifi_server_port)) 
            return ESP_FAIL;
        
        return ESP_OK;
    }
    else if (bits & WIFI_FAIL_BIT) {
        this->_wifi_is_connected = false;
        return ESP_FAIL;
    }

    this->_wifi_is_connected = false;
    return ESP_ERR_TIMEOUT;
}

void wifi::disconnect() {
    this->_wifi_tcp_socket.disconnect();
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    ESP_ERROR_CHECK(esp_wifi_stop());
}

//------------------------------tcp_socket代码------------------------------

bool tcp_socket::connect(std::string ip, std::string port) {
    this->_tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (this->_tcp_socket < 0) {
        printf("Failed to create socket");
        return false;
    }

    //------------------------------设置目标地址------------------------------
    this->_tcp_dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    this->_tcp_dest_addr.sin_family = AF_INET;
    this->_tcp_dest_addr.sin_port = htons(std::stoul(port));

    //------------------------------连接服务器------------------------------
    int err = ::connect(this->_tcp_socket, (struct sockaddr *)&this->_tcp_dest_addr, sizeof(this->_tcp_dest_addr));
    if (err != 0) {
        ESP_LOGE(WIFI_TAG, "Socket connect failed: errno %d", errno);
        closesocket(this->_tcp_socket);
        this->_tcp_socket = -1;
        return false;
    }

    this->_tcp_is_connected = true;
    ESP_LOGI(WIFI_TAG, "Socket connected to %s, %s", ip.c_str(), port.c_str());
    return true;
}

int tcp_socket::printf(const char* format, ...) {
    if (!this->_tcp_is_connected) {
        ESP_LOGE(WIFI_TAG, "Not connected");
        return -1;
    }
    
    char buffer[_tcp_buffer_size];

    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, _tcp_buffer_size, format, args);
    va_end(args);

    if (len < 0 || len >= _tcp_buffer_size) {
        ESP_LOGE(WIFI_TAG, "Buffer overflow");
        return -1;
    }

    int sent = send(this->_tcp_socket, buffer, len, 0);
    if (sent < 0) {
        ESP_LOGE(WIFI_TAG, "Send failed: errno %d", errno);
        disconnect();
        return -1;
    }
    return sent;
}


int tcp_socket::send_data(const void* data, size_t len) {
    if (!this->_tcp_is_connected) {
        ESP_LOGE(WIFI_TAG, "Not connected");
        return -1;
    }

    int sent = send(this->_tcp_socket, data, len, 0);
    if (sent < 0) {
        ESP_LOGE(WIFI_TAG, "Send failed: errno %d", errno);
        disconnect();
        return -1;
    }
    return sent;

}

int tcp_socket::receive(void* buffer, size_t len, uint32_t timeout_ms) {
    if (!this->_tcp_is_connected) {
        ESP_LOGE(WIFI_TAG, "Not connected");
        return -1;
    }
    
    if (timeout_ms > 0) {
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(this->_tcp_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }

    int received = recv(this->_tcp_socket, buffer, len, 0);
    if (received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            ESP_LOGW(WIFI_TAG, "Receive timeout");
        }
        else {
            ESP_LOGE(WIFI_TAG, "Receive failed: errno %d", errno);
            disconnect();
        }
    }
    return received;
}



}
}
