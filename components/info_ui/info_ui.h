#pragma once
#include "info_ui_config.h"

namespace info_ui{
    
class info_ui {
public:
    info_ui(info_ui_config_t* cfg);
    void start(void);
    ~info_ui(void);
private:
    info_ui_config_t* cfg;              // 屏幕的配置信息
};

}

