#pragma once
#include <string>
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "info_ui_item.h"
#include <functional>

namespace info_ui {

class info_ui_app_base : public info_ui_item{

public:
    info_ui_app_base() {
        this->_icon_src = NULL;
        this->_app_panel = NULL;
    }

    virtual ~info_ui_app_base() = default;

    //------------------------------获取参数相关------------------------------
    void* get_icon_img() { return this->_icon_src; }
    lv_obj_t* get_app_panel() { return this->_app_panel; }

    //------------------------------状态设置相关------------------------------
    void set_app_load_finish() { this->_app_load_finish_flag = true; }

    //------------------------------判断状态相关------------------------------
    bool is_app_available() { return this->_app_available_flag; }
    bool is_app_load_finish() { return this->_app_load_finish_flag; }

    //------------------------------系统注册相关------------------------------
    void panel_register(lv_obj_t* disp_layer) { this->_app_panel = lv_obj_create(disp_layer); }
    void info_lable_register(lv_obj_t* sys_popup_label) { this->_app_info_label = sys_popup_label; }
    void app_enable() { this->_app_available_flag = true; }
    void app_disable() { this->_app_available_flag = false; }
    
    //------------------------------软件运行相关------------------------------
    void entry();
    virtual void run() = 0;
    void exit();

    //------------------------------系统提示相关------------------------------
    void popup_info(std::string info);
    void dropdown_info();

protected: 
    void*       _icon_src;                      // 图标源码
    lv_obj_t*   _app_panel;                     // 软件面板
    bool        _app_available_flag = false;    // 软件可用标志
    bool        _app_load_finish_flag = false;  // 软件加载完成标志
    lv_obj_t*   _app_info_label;                // 系统的弹窗
};

}
