#pragma once
#include "info_ui_config.h"
#include <string>

namespace info_ui{
    
class info_ui {
public:
    info_ui(info_ui_config_t* cfg);
    void popup_info(std::string info);
    void dropdown_info();
    void update();
    ~info_ui(void);
private:
    static void _dropdown_info_anim_ready_cb(lv_anim_t* a);

public:
private:
    info_ui_config_t*   _cfg;               // 屏幕的配置信息
    lv_disp_t*          _disp;              // 屏幕
    lv_obj_t*           _layer_top;         // 顶层

    lv_obj_t*           _layer_pop;         // 信息弹窗层
    // static bool         _dropdown_anim_finish_flag;
};

}

