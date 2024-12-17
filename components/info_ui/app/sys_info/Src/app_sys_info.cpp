//------------------------------------------------------------
// Created Time :   2024.12.07
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include "app_sys_info.h"
#include "core/lv_event.h"
#include "core/lv_group.h"
#include "core/lv_indev.h"
#include "core/lv_obj.h"
#include "core/lv_obj_pos.h"
#include "esp_lvgl_port_button.h"
#include "lv_api_map.h"

namespace info_ui {

static void app_sys_info_button_cb(lv_event_t* e) {
    printf("Hello World\n");
}

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

    //------------------------------创建按钮组------------------------------
    // this->_sys_button_group = lv_group_create();
}


void app_sys_info::init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_group_t* button_group) {
    //------------------------------创建屏幕------------------------------
    this->_app_panel = lv_obj_create(disp_layer);
    lv_obj_add_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(this->_app_panel, this->_disp_width, this->_disp_height);

    //------------------------------配置弹窗面板------------------------------
    this->_app_info_label = sys_popup_label;

    //------------------------------添加按钮------------------------------
    this->_button_group = button_group;

    //------------------------------配置状态信息------------------------------
    this->_app_state = info_ui_app_state::INIT;


    //------------------------------绘制面板------------------------------
    this->draw_interface();
}

void app_sys_info::run() {
    lvgl_port_lock(0);
    printf("run app_sys_info\n");
    this->_app_state = info_ui_app_state::RUNNING;
    lv_obj_clear_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
    // lv_scr_load(this->_app_panel);

    this->_sys_button_group = lv_group_create();
    lv_obj_t* btn = lv_btn_create(this->_app_panel);
    lv_obj_set_size(btn, 1, 1);
    lv_indev_set_group(this->_button_indev, this->_sys_button_group);
    lv_group_add_obj(this->_sys_button_group, btn);

    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn, app_sys_info_button_cb, LV_EVENT_CLICKED, NULL);


    lvgl_port_unlock();
}

void app_sys_info::stop() {
    printf("stop app_sys_info\n");
    // lv_obj_add_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
    // lv_group_remove_obj(this->_app_panel);
    this->_app_state = info_ui_app_state::STOPPED;
}

void app_sys_info::draw_interface() {
    lv_obj_t* author_img_obj = lv_img_create(this->_app_panel);
    lv_img_set_src(author_img_obj, this->_author_img);

    lv_obj_align(author_img_obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(author_img_obj, 64, 64);
}

}
