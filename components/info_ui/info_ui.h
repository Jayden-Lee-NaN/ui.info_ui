#pragma once
#include "info_ui_config.h"
#include "info_ui_app_base.h"
#include <string>

namespace info_ui {
    
class info_ui {
public:
    info_ui(info_ui_config_t* cfg, int32_t button_prev_num, int32_t button_next_num, int32_t button_enter);

    void popup_info(std::string info);
    void dropdown_info();
    void start();
    void update();
    void test_button_input();
    ~info_ui(void);
private:
    void app_register(info_ui_app_base* app);


private:
    //------------------------------屏幕相关配置------------------------------
    info_ui_config_t*   _cfg;                   // 屏幕的配置信息
    lv_disp_t*          _disp;                  // 屏幕
    lv_obj_t*           _disp_layer;            // 屏幕层
    lv_obj_t*           _info_label;            // 信息弹窗层
    lv_obj_t*           _top;                   // 顶层

    //------------------------------按键配置相关------------------------------
    lv_indev_t*         _button_handle;
    lv_group_t*         _button_group;

    //------------------------------软件相关配置------------------------------
    lv_obj_t*           _app_select_layer;      // 软件选择层
    lv_obj_t*           _app_panel_layer;       // 软件层的头
    
};

}

