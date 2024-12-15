//------------------------------------------------------------
// Created Time :   2024.12.07
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include "app_sys_info.h"

namespace info_ui {

extern "C" app_sys_info::app_sys_info(uint32_t disp_width, uint32_t disp_height) {
    //------------------------------声明软件名称------------------------------
    this->_app_name = "app_sys_info";

    //------------------------------挂载信息------------------------------
    this->_disp_width = disp_width;
    this->_disp_height = disp_height;

    //------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_message);
    LV_IMG_DECLARE(icon_jayden_lee);

    //------------------------------获取软件图标和软件面板------------------------------
    this->_icon_src = (void*)&icon_message;

    //------------------------------添加作者图标------------------------------
    this->_author_img = (void*)&icon_jayden_lee;
}


void app_sys_info::init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label) {
    this->_app_panel = lv_obj_create(disp_layer);
    this->_app_info_label = sys_popup_label;
    this->_app_state = info_ui_app_state::INIT;
}

void app_sys_info::run() {
    printf("run app_sys_info\n");
}

void app_sys_info::stop() {
    printf("stop app_sys_info\n");
}

}
