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
    ERROR,
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
    info_ui_app_state get_app_state() { return this->_app_state; }

    //------------------------------系统注册相关------------------------------
    void panel_register(lv_obj_t* disp_layer) { this->_app_panel = lv_obj_create(disp_layer); }
    void info_lable_register(lv_obj_t* sys_popup_label) { this->_app_info_label = sys_popup_label; }
    
    //------------------------------软件运行相关------------------------------
    virtual void init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_indev_t* encoder_indev) = 0;
    virtual void run() = 0;
    virtual void stop() = 0;

    //------------------------------系统提示相关------------------------------
    void popup_info(std::string info);                  // 弹窗信息
    void dropdown_info();                               // 弹窗退出

protected:
    //------------------------------内部函数------------------------------
    void _hidden_layer();                               // 隐藏软件顶层
    void _display_layer();                              // 显示软件顶层
    void _encoder_group_enable();                       // 获取编码器使用权

protected: 

    //------------------------------面板相关------------------------------
    void*               _icon_src;                      // 图标源码
    lv_obj_t*           _app_panel;                     // 软件面板

    //------------------------------系统弹窗设置------------------------------
    lv_obj_t*           _app_info_label;                // 系统的弹窗

    //------------------------------系统状态信息------------------------------
    info_ui_app_state   _app_state = info_ui_app_state::DEFAULT;
                                                        // 软件运行的状态
    std::string         _app_name = "unknown";          // 软件名称

    //------------------------------外设-编码器组------------------------------
    lv_group_t*         _encoder_group;                 // 编码器组
    lv_indev_t*         _encoder_indev;                 // 编码器外部的设备

    //------------------------------外设-按钮------------------------------

};

}
