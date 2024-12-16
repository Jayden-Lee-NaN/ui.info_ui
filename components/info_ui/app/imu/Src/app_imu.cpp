//------------------------------------------------------------
// Created Time :   2024.12.07
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------
#include "app_imu.h"

namespace info_ui {

extern "C" app_imu::app_imu(uint32_t disp_width, uint32_t disp_height) {
    //------------------------------声明软件名称------------------------------
    this->_app_name = "app_imu";

    //------------------------------挂载信息------------------------------
    this->_disp_width = disp_width;
    this->_disp_height = disp_height;

    //------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_imu);

    //------------------------------获取软件图标------------------------------
    this->_icon_src = (void*)&icon_imu;
}

void app_imu::init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_group_t* button_group) {
    this->_app_panel = lv_obj_create(disp_layer);
    lv_obj_add_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);

    this->_app_info_label = sys_popup_label;
    this->_app_state = info_ui_app_state::INIT;
}

void app_imu::run () {
    printf("run app_imu\n");
    this->_app_state = info_ui_app_state::RUNNING;

    lv_obj_clear_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
}

void app_imu::stop() {
    printf("stop app_imu\n");
    lv_obj_add_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
    this->_app_state = info_ui_app_state::STOPPED;
}

}
