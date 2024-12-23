//------------------------------------------------------------
// Created Time :    2024.12.07
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include "app_music.h"
#include "core/lv_event.h"

namespace info_ui {


extern "C" app_music::app_music(uint32_t disp_width, uint32_t disp_height) {
    //------------------------------声明软件名称------------------------------
    this->_app_name = "app_music";

    //------------------------------挂载信息------------------------------
    this->_disp_width = disp_width;
    this->_disp_height = disp_height;

    // ------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_voice);
    this->_icon_src = (void*)&icon_voice;
}


void app_music::init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_indev_t* encoder_indev) {
    //------------------------------创建屏幕------------------------------
    this->_app_panel = lv_obj_create(disp_layer);
    lv_obj_align(this->_app_panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(this->_app_panel, this->_disp_width, this->_disp_height);
    lv_obj_set_scrollbar_mode(this->_app_panel, LV_SCROLLBAR_MODE_OFF);

    //------------------------------配置弹窗面板------------------------------
    this->_app_info_label = sys_popup_label;

    //------------------------------添加编码器------------------------------
    this->_encoder_indev = encoder_indev;
    this->_encoder_group = lv_group_create();

    //------------------------------配置状态信息------------------------------
    this->_app_state = info_ui_app_state::INIT;

}

void app_music::run() {
    lvgl_port_lock(0);
    printf("run app_music\n");
    this->_app_state = info_ui_app_state::RUNNING;

    //------------------------------显示屏幕------------------------------
    this->_display_layer();
    

    lvgl_port_unlock();
}

void app_music::stop() {
    printf("stop app_music\n");
    this->_app_state = info_ui_app_state::STOPPED;
    this->_hidden_layer();
}

}
