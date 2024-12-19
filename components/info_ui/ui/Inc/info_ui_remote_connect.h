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
    tcp_socket() : _tcp_socket(-1), _tcp_is_connected(false) {
        memset(&_tcp_dest_addr, 0, sizeof(_tcp_dest_addr));
    }
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
    int32_t                     _tcp_socket;
    bool                        _tcp_is_connected;
    struct sockaddr_in          _tcp_dest_addr;
    static const int32_t        _tcp_buffer_size = 1024;
};

class wifi {
public:
    wifi(std::string ssid, std::string password, std::string server_ip, std::string server_port);
    void        init(); 
    esp_err_t   connect();
    void        disconnect();
    bool        is_connected() const { return this->_wifi_is_connected; }
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
