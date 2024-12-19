#include "esp_log.h"
#include "info_ui_remote_connect.h"
#include "lwip/sockets.h"
#include <fcntl.h>
#include <sys/select.h>

namespace info_ui {
namespace remote_connect {

static const char* TCP_TAG = "TCP_TAG";

// tcp_socket 事件的回调函数
static void tcp_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    
}

void tcp_socket::connect_task(void* pvParameters) {
    tcp_socket* client = (tcp_socket*)pvParameters;
    client->do_connect();
    vTaskDelete(NULL);
}

tcp_socket::tcp_socket() : _tcp_socket(-1), _tcp_is_connected(false) {
    //------------------------------给目标地址分配内存------------------------------
    memset(&_tcp_dest_addr, 0, sizeof(_tcp_dest_addr));

    //------------------------------创建事件组------------------------------
    this->_tcp_event_group = xEventGroupCreate();
}

void tcp_socket::do_connect() {
    // 设置 socket 为非阻塞模式
    int flags = fcntl(this->_tcp_socket, F_GETFL, 0);
    fcntl(this->_tcp_socket, F_SETFL, flags | O_NONBLOCK);
    
    // 开始连接
    int ret = ::connect(this->_tcp_socket, (struct sockaddr*)&(this->_tcp_dest_addr), sizeof(this->_tcp_dest_addr));
    
    if (ret == 0) {
        // 连接立即成功
        this->_tcp_is_connected = true;
        xEventGroupSetBits(this->_tcp_event_group, SOCKET_CONNECTED_BIT);
    }
    else if (errno == EINPROGRESS) {
        // 连接正在进行中
        fd_set write_fds;
        struct timeval tv;

        FD_ZERO(&write_fds);
        FD_SET(this->_tcp_socket, &write_fds);

        // 设置超时时间
        tv.tv_sec = this->cfg.connect_timeout / 1000;
        tv.tv_usec = (this->cfg.connect_timeout % 1000) * 1000;

        // 等待连接完成或超时
        ret = select(this->_tcp_socket + 1, NULL, &write_fds, NULL, &tv);

        if (ret > 0) {
            // 检查连接是否成功
            int error;
            socklen_t len = sizeof(error);
            if (getsockopt(this->_tcp_socket, SOL_SOCKET, SO_ERROR, &error, &len) == 0 && error == 0) {
                this->_tcp_is_connected = true;
                xEventGroupSetBits(this->_tcp_event_group, SOCKET_CONNECTED_BIT);
            }
            else 
                xEventGroupSetBits(this->_tcp_event_group, SOCKET_FAILED_BIT);
        }
        else {
            // 超时或错误
            xEventGroupSetBits(this->_tcp_event_group, SOCKET_FAILED_BIT);
        }
    }
    else {
        // 连接直接失败
        xEventGroupSetBits(this->_tcp_event_group, SOCKET_FAILED_BIT);
    }

    // 恢复阻塞模式
    fcntl(this->_tcp_socket, F_SETFL, flags);
}

bool tcp_socket::connect_and_wait(TickType_t timeout = portMAX_DELAY) {
    // 清除之前的事件位  
    xEventGroupClearBits(this->_tcp_event_group, SOCKET_ANY_BIT);

    // 创建连接任务
    xTaskCreate(connect_task, "socket_connect", 4096, this, 5, NULL);

    // 等待连接结果
    EventBits_t bits = xEventGroupWaitBits(this->_tcp_event_group, SOCKET_ANY_BIT, pdTRUE, pdFALSE, timeout);

    // 检查连接结果
    if (bits & SOCKET_CONNECTED_BIT) {
        ESP_LOGI(TCP_TAG, "Socket connected successfully");
        return true;
    }
    else {
        ESP_LOGI(TCP_TAG, "Socket connect failed");
        return false;
    }

    return true;    
}

bool tcp_socket::connect(std::string ip, std::string port) {
    this->_tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (this->_tcp_socket < 0) {
        ESP_LOGW(TCP_TAG, "Failed to create socket");
        return false;
    }

    //------------------------------设置目标地址------------------------------
    this->_tcp_dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    this->_tcp_dest_addr.sin_family = AF_INET;
    this->_tcp_dest_addr.sin_port = htons(std::stoul(port));

    uint32_t retry_cnt = 0;
    while (retry_cnt < cfg.retry_count) {
        ESP_LOGI(TCP_TAG, "Connecting to server (attempt %lu/%lu...)", (retry_cnt + 1), (cfg.retry_count));
        
        if (connect_and_wait(pdMS_TO_TICKS(this->cfg.connect_timeout))) {
            return true;
        }
        ++retry_cnt;

        if (retry_cnt < this->cfg.retry_count) {
            vTaskDelay(pdMS_TO_TICKS(this->cfg.retry_interval));
        }
    }
    ESP_LOGE(TCP_TAG, "Failed to connect after %lu attempts", this->cfg.retry_count);
    this->disconnect();
    return false;
}

int tcp_socket::printf(const char* format, ...) {
    if (!this->_tcp_is_connected) {
        ESP_LOGE(TCP_TAG, "Not connected");
        return -1;
    }
    
    char buffer[_tcp_buffer_size];

    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, _tcp_buffer_size, format, args);
    va_end(args);

    if (len < 0 || len >= _tcp_buffer_size) {
        ESP_LOGE(TCP_TAG, "Buffer overflow");
        return -1;
    }

    int sent = send(this->_tcp_socket, buffer, len, 0);
    if (sent < 0) {
        ESP_LOGE(TCP_TAG, "Send failed: errno %d", errno);
        disconnect();
        return -1;
    }
    return sent;
}


int tcp_socket::send_data(const void* data, size_t len) {
    if (!this->_tcp_is_connected) {
        ESP_LOGE(TCP_TAG, "Not connected");
        return -1;
    }

    int sent = send(this->_tcp_socket, data, len, 0);
    if (sent < 0) {
        ESP_LOGE(TCP_TAG, "Send failed: errno %d", errno);
        disconnect();
        return -1;
    }
    return sent;

}

int tcp_socket::receive(void* buffer, size_t len, uint32_t timeout_ms) {
    if (!this->_tcp_is_connected) {
        ESP_LOGE(TCP_TAG, "Not connected");
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
            ESP_LOGW(TCP_TAG, "Receive timeout");
        }
        else {
            ESP_LOGE(TCP_TAG, "Receive failed: errno %d", errno);
            disconnect();
        }
    }
    return received;
}

}
}
