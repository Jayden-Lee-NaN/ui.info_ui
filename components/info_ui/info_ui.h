#pragma once
#include "info_ui_config.h"
#include "info_ui_app_base.h"
#include <string>

namespace info_ui{
    
class info_ui {
public:
    info_ui(info_ui_config_t* cfg);
    void popup_info(std::string info);
    void dropdown_info();
    void update();
    void test_show_image();
    ~info_ui(void);
private:
    static void _dropdown_info_anim_deleted_cb(lv_anim_t* a);

public:
private:
    info_ui_config_t*   _cfg;               // 屏幕的配置信息
    lv_disp_t*          _disp;              // 屏幕

    lv_obj_t*           _layer;             // 屏幕
    lv_obj_t*           _info_label;        // 信息弹窗层
};

}

