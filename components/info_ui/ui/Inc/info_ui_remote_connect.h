#pragma once
#include <string>
#include "esp_wifi.h"

namespace info_ui {
namespace remote_connect {

class wifi {
public:

private: 
    
    std::string                 _wifi_ssid;                     // wifi的名称
    std::string                 _wifi_password;                 // wifi的密码
    std::string                 _wifi_server_ip;                // wifi的ip地址
    std::string                 _wifi_server_port;              // wifi的端口
};

}
}
