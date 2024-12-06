//------------------------------------------------------------
// Created Time :    2024.11.23
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include <stdio.h>
#include "info_ui.h"
#include "core/lv_disp.h"
#include "extra/widgets/imgbtn/lv_imgbtn.h"
#include <string>

namespace info_ui {

info_ui::info_ui(info_ui_config_t* cfg) {
    //------------------------------挂载数据------------------------------
    this->_cfg = cfg;    

    //------------------------------初始化屏幕------------------------------
    lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = this->_cfg->io_handle,
        .panel_handle = this->_cfg->panel_handle,
        .buffer_size = this->_cfg->disp_width * this->_cfg->disp_height,
        .double_buffer = true,
        .hres = this->_cfg->disp_height,
        .vres = this->_cfg->disp_width,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };
    this->_disp = lvgl_port_add_disp(&disp_cfg);
    lv_disp_set_rotation(this->_disp, LV_DISP_ROT_180);

    //------------------------------获取屏幕------------------------------
    this->_layer = lv_disp_get_scr_act(this->_disp);

    //------------------------------设置info label------------------------------
    this->_info_label = lv_label_create(this->_layer);
    lv_label_set_long_mode(this->_info_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(this->_info_label, this->_cfg->disp_height - 20);
    lv_obj_align(this->_info_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(this->_info_label, 2, LV_PART_MAIN);
    lv_obj_set_style_pad_all(this->_info_label, 5, LV_PART_MAIN);
}

/*
 * @brief               弹出指定的信息
 * @param info          需要显示的信息
 * @return              无
 */
void info_ui::popup_info(std::string info) {

    lvgl_port_lock(0);

    //------------------------------设置文字------------------------------
    lv_label_set_text(this->_info_label, info.c_str());


    //------------------------------初始化弹窗动画------------------------------
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, this->_info_label);

    lv_anim_set_values(&anim, this->_cfg->disp_width, 0);

    lv_anim_set_time(&anim, 1000);                // 动画时长 500 ms
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

    //------------------------------启动动画------------------------------
    lv_anim_start(&anim);

    lvgl_port_unlock();
}

/*
 * @brief               收回弹窗
 * @return              无
 */
void info_ui::dropdown_info() {
    lvgl_port_lock(0);
    //------------------------------初始化弹窗动画------------------------------

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, this->_info_label);

    lv_anim_set_values(&anim, 0, this->_cfg->disp_width);

    lv_anim_set_time(&anim, 1000);                // 动画时长 1000 ms
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

    //------------------------------启动动画------------------------------
    lv_anim_start(&anim);
    lvgl_port_unlock();
}

/*
 * @brief               放在主循环中,做标志位的处理
 * @return              无
 */
void info_ui::update() {
    if (_dropdown_anim_finish_flag == true) {
        if (lvgl_port_lock(0)) {
            _dropdown_anim_finish_flag = false;
            // lv_obj_clean(this->_layer);
            this->_layer = NULL;
            lvgl_port_unlock();
        }
    }
}


extern "C" void info_ui::test_show_image() {
    lvgl_port_lock(0);
    LV_IMG_DECLARE(icon_jayden_lee);
    lv_obj_t* icon = lv_img_create(lv_disp_get_scr_act(this->_disp));
    lv_img_set_src(icon, &icon_jayden_lee);
    lvgl_port_unlock();
}

info_ui::~info_ui(void) {

}


}

