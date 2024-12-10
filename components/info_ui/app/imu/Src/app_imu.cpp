//------------------------------------------------------------
// Created Time :   2024.12.07
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------
#include "app_imu.h"

extern "C" info_ui::app_imu::app_imu(uint32_t disp_width, uint32_t disp_height) {
    //------------------------------挂载信息------------------------------
    this->_disp_width = disp_width;
    this->_disp_height = disp_height;

    //------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_imu);

    //------------------------------获取软件图标------------------------------
    this->_icon_src = (void*)&icon_imu;

}

void info_ui::app_imu::run () {
    this->_app_available_flag = true;    
}
