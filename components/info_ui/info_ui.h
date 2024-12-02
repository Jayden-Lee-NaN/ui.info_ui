#pragma once
#include "info_ui_config.h"
#include <string>

namespace info_ui{
    
class info_ui {
public:
    info_ui(info_ui_config_t* cfg);
    lv_obj_t* pop_info(std::string info);
    // void start(void);
    ~info_ui(void);
private:
private:
    info_ui_config_t*   _cfg;               // 屏幕的配置信息
    lv_disp_t*          _disp;              // 屏幕

};

}

