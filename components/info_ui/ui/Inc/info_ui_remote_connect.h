#pragma once
#include <string>
#include "esp_wifi.h"
#include "lwip/sockets.h"

#define WIFI_CONNECTED_BIT      BIT0
#define WIFI_FAIL_BIT           BIT1

namespace info_ui {
namespace remote_connect {

class tcp_socket {
public:
    tcp_socket();
    ~tcp_socket() {
        disconnect();
    }
    bool is_connected() const { return this->_tcp_is_connected; }
    bool connect(std::string ip, std::string port);

    void disconnect() {
        if (this->_tcp_socket != -1) {
            closesocket(this->_tcp_socket);
            this->_tcp_socket = -1;
        }
        this->_tcp_is_connected = false;
    }

    int printf(const char* formart, ...);
    int send_data(const void* data, size_t len);
    int receive(void* buffer, size_t len, uint32_t timeout_ms = 0);

private:
    bool connect_and_wait(TickType_t timeout);                  
    static void connect_task(void* pvParameters);
    void do_connect();
    
private:
    //------------------------------基本参数定义------------------------------
    int32_t                     _tcp_socket;                    // tcp的socket号
    bool                        _tcp_is_connected = false;      // tcp是否连接
    struct sockaddr_in          _tcp_dest_addr;                 // 连接目标地址
    static const int32_t        _tcp_buffer_size = 1024;        // tcp发送缓冲区大小

    //------------------------------事件组定义------------------------------
    EventGroupHandle_t          _tcp_event_group;               // tcp连接事件组
    static constexpr uint32_t SOCKET_CONNECTED_BIT      = BIT0; // 判断socket连接的标志位
    static constexpr uint32_t SOCKET_FAILED_BIT         = BIT1; // socket连接失败的标志位
    static constexpr uint32_t SOCKET_DISCONNECTED_BIT   = BIT2; // socket未连接的标志位
    static constexpr uint32_t SOCKET_ANY_BIT            = (SOCKET_CONNECTED_BIT | SOCKET_FAILED_BIT | SOCKET_DISCONNECTED_BIT);

    //------------------------------socket重连配置------------------------------
    struct {
        uint32_t                retry_count = 5;
        uint32_t                retry_interval = 1000;          // 重连间隔(ms)
        uint32_t                connect_timeout = 5000;         // 连接超时(ms)
    }cfg;
}; 

class wifi {
public:
    wifi(std::string ssid, std::string password, std::string server_ip, std::string server_port);
    void        init(); 
    esp_err_t   connect();
    void        disconnect();
    bool        is_wifi_connected() const { return this->_wifi_is_connected; }
    bool        is_socket_connected() const { return this->_wifi_tcp_socket.is_connected(); } 
    template<typename... Args>
    int         printf(const char* format, Args... args) { return this->_wifi_tcp_socket.printf(format, args...); }

private: 
    
    std::string                 _wifi_ssid;                     // wifi的名称
    std::string                 _wifi_password;                 // wifi的密码
    std::string                 _wifi_server_ip;                // wifi的ip地址
    std::string                 _wifi_server_port;              // wifi的端口
    EventGroupHandle_t          _wifi_event_group;              // wifi事件组

    bool                        _wifi_is_connected = false;     // 是否连接的标志
    tcp_socket                  _wifi_tcp_socket;               // wifi的tcp_socket
};

}
}
