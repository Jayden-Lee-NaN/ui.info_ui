#pragma once
#include <string>
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "info_ui_item.h"
#include <functional>

namespace info_ui {

enum class info_ui_app_state {
    DEFAULT,
    INIT,
    RUNNING,
    STOPPED,
    ERROR
};

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
    std::string get_app_name() { return this->_app_name; }

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

    virtual void init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_group_t* button_group) = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
    void exit();

    //------------------------------系统提示相关------------------------------
    void popup_info(std::string info);
    void dropdown_info();

protected: 
    void*               _icon_src;                      // 图标源码
    lv_obj_t*           _app_panel;                     // 软件面板
    bool                _app_available_flag = false;    // 软件可用标志
    bool                _app_load_finish_flag = false;  // 软件加载完成标志
    lv_obj_t*           _app_info_label;                // 系统的弹窗
    info_ui_app_state   _app_state = info_ui_app_state::DEFAULT;
                                                        // 软件运行的状态
    std::string         _app_name = "unknown";          // 软件名称
    lv_group_t*         _button_group;                  // 按键组
};

}
