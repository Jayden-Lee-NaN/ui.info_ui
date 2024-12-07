#pragma once
#include "lvgl.h"

namespace info_ui {

class info_ui_app_base {

public:
    info_ui_app_base() {
        this->_icon_src = NULL;
        this->_app_panel = NULL;
    }
    void* get_icon_img() { return this->_icon_src; }
    lv_obj_t* get_app_panel() { return this->_app_panel; }
    void panel_register(lv_obj_t* disp_layer) { this->_app_panel = lv_obj_create(disp_layer); }

protected: 
    void*       _icon_src;                      // 图标源码
    lv_obj_t*   _app_panel;                     // 软件面板

};

}
