//------------------------------------------------------------
// Created Time :   2024.12.07
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------
#include "app_temperature.h"

extern "C" info_ui::app_temperature::app_temperature(uint32_t disp_width, uint32_t disp_height) {
    //------------------------------挂载信息------------------------------
    this->_disp_width = disp_width;
    this->_disp_height = disp_height;

    //------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_temperature);

    //------------------------------获取图标------------------------------
    this->_icon_src = (void*)&icon_temperature;
}

void info_ui::app_temperature::run() {
    this->_app_available_flag = true;
}
