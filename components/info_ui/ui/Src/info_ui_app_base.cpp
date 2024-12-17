#include "info_ui_app_base.h"
#include "core/lv_group.h"
#include "lv_api_map.h"
namespace info_ui {

void info_ui_app_base::popup_info(std::string info) {
    lvgl_port_lock(0);

    //------------------------------设置文字------------------------------
    lv_label_set_text(this->_app_info_label, info.c_str());

    //------------------------------初始化弹窗动画------------------------------
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, this->_app_info_label);

    lv_anim_set_values(&anim, this->_disp_width, 0);

    lv_anim_set_time(&anim, 1000);                // 动画时长 1000 ms
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

    //------------------------------启动动画------------------------------
    lv_anim_start(&anim);

    lvgl_port_unlock();
}

void info_ui_app_base::dropdown_info() {
    lvgl_port_lock(0);
    //------------------------------初始化弹窗动画------------------------------

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, this->_app_info_label);

    lv_anim_set_values(&anim, 0, this->_disp_width);

    lv_anim_set_time(&anim, 1000);                // 动画时长 1000 ms
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

    //------------------------------启动动画------------------------------
    lv_anim_start(&anim);
    lvgl_port_unlock();
}

void info_ui_app_base::_hidden_layer() {
    auto info_ui_app_panel = lv_obj_get_parent(this->_app_panel);
    lv_obj_add_flag(info_ui_app_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
}

void info_ui_app_base::_display_layer() {
    auto info_ui_app_panel = lv_obj_get_parent(this->_app_panel);
    lv_obj_clear_flag(info_ui_app_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(info_ui_app_panel);
}

void info_ui_app_base::_encoder_group_enable() {
    //------------------------------如果没有创建组的话创建一个组------------------------------
    if (this->_encoder_group == NULL) 
        this->_encoder_group = lv_group_create();
    lv_indev_set_group(this->_encoder_indev, this->_encoder_group);
}

}

