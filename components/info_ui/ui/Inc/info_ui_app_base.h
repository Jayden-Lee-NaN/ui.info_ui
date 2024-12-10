#pragma once
#include <string>
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "info_ui_item.h"

namespace info_ui {

class info_ui_app_base : public info_ui_item{

public:
    info_ui_app_base() {
        this->_icon_src = NULL;
        this->_app_panel = NULL;
    }

    void* get_icon_img() { return this->_icon_src; }
    lv_obj_t* get_app_panel() { return this->_app_panel; }
    bool is_app_available() { return this->_app_available_flag; }
    void panel_register(lv_obj_t* disp_layer) { this->_app_panel = lv_obj_create(disp_layer); }
    void info_lable_register(lv_obj_t* sys_popup_label) { this->_app_info_label = sys_popup_label; }
    void entry();
    virtual void run() = 0;

    void popup_info();
    void dropdown_info();
protected: 
    void*       _icon_src;                      // 图标源码
    lv_obj_t*   _app_panel;                     // 软件面板
    bool        _app_available_flag = false;    // 软件可用标志
    lv_obj_t*   _app_info_label;                // 系统的弹窗
};

}
