#include "info_ui_app_base.h"

void info_ui::info_ui_app_base::entry() {
    // 进入到指定的APP面板
    lvgl_port_lock(0);
    lv_obj_move_foreground(this->_app_panel);
    lvgl_port_unlock();
}

void info_ui::info_ui_app_base::popup_info() {
    lvgl_port_lock(0);

    //------------------------------设置文字------------------------------
    lv_label_set_text(this->_app_info_label, "No App");

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

void info_ui::info_ui_app_base::dropdown_info() {
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
