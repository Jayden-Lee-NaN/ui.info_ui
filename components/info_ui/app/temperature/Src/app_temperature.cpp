//------------------------------------------------------------
// Created Time :   2024.12.07
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------
#include "app_temperature.h"

namespace info_ui {

extern "C" app_temperature::app_temperature(uint32_t disp_width, uint32_t disp_height) {
    //------------------------------声明软件名称------------------------------
    this->_app_name = "app_temperature";

    //------------------------------挂载信息------------------------------
    this->_disp_width = disp_width;
    this->_disp_height = disp_height;

    //------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_temperature);

    //------------------------------获取图标------------------------------
    this->_icon_src = (void*)&icon_temperature;
}


void app_temperature::init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label) {
    this->_app_panel = lv_obj_create(disp_layer);
    this->_app_info_label = sys_popup_label;
    this->_app_state = info_ui_app_state::INIT;
}

void app_temperature::run() {
    printf("run app_temperature\n");
}

void app_temperature::stop() {
    printf("stop app_temperature\n");
}

}
