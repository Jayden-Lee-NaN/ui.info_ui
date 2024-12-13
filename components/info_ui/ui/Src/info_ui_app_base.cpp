#include "info_ui_app_base.h"
namespace info_ui {


static void timer_cb_load_scr(lv_timer_t* timer) {
    // 判断屏幕是否被加载完成的定时器
    lv_obj_t* current_scr = lv_scr_act(); 
    info_ui_app_base* app = (info_ui_app_base*)(timer->user_data);
    if (current_scr == app->get_app_panel()) {
        app->set_app_load_finish();
        lv_timer_del(timer);
    }
}

void info_ui_app_base::entry() {
    // lv_obj_move_foreground(this->_app_panel);

    // 500ms的动画,当前屏幕向下滑出,新屏幕从上滑入
    lv_scr_load_anim(this->_app_panel, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 500, 0, false);

    // 用定时器去监测软件是否被加载完成
    lv_timer_create(timer_cb_load_scr, 100, (void*)this);
}

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

}

