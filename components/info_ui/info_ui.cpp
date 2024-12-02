//------------------------------------------------------------
// Created Time :    2024.11.23
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include <stdio.h>
#include "info_ui.h"
#include "core/lv_disp.h"
#include <string>

namespace info_ui {

static bool _dropdown_anim_finish_flag = false;
void info_ui::_dropdown_info_anim_deleted_cb(lv_anim_t* a) {
    _dropdown_anim_finish_flag = true;
}

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
    this->_layer_pop = NULL;
    // this->_layer_top = lv_layer_top();
}

/*
 * @brief               弹出指定的信息
 * @param info          需要显示的信息
 * @return              无
 */
void info_ui::popup_info(std::string info) {

    // lv_obj_t* layer = lv_disp_get_scr_act(this->_disp);
    this->_layer_pop = lv_disp_get_scr_act(this->_disp);

    //------------------------------设置弹窗的基本属性------------------------------
    lv_obj_set_size(this->_layer_pop, this->_cfg->disp_height * 0.5, this->_cfg->disp_width * 0.5);
    lv_obj_align(this->_layer_pop, LV_ALIGN_CENTER, 0, 0);

    //------------------------------设置文字------------------------------
    lv_obj_t* label = lv_label_create(this->_layer_pop);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(label, this->_cfg->disp_height - 20);
    lv_label_set_text(label, info.c_str());
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_border_width(label, 2, LV_PART_MAIN);
    lv_obj_set_style_pad_all(label, 5, LV_PART_MAIN);

    //------------------------------初始化弹窗动画------------------------------
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, this->_layer_pop);

    lv_anim_set_values(&anim, this->_cfg->disp_width, 0);

    lv_anim_set_time(&anim, 500);                // 动画时长 500 ms
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

    //------------------------------启动动画------------------------------
    lv_anim_start(&anim);
}

/*
 * @brief               收回弹窗
 * @return              无
 */
void info_ui::dropdown_info() {
    //------------------------------初始化弹窗动画------------------------------
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, this->_layer_pop);

    lv_anim_set_values(&anim, 0, this->_cfg->disp_width);

    lv_anim_set_time(&anim, 500);                // 动画时长 500 ms
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
    lv_anim_set_deleted_cb(&anim, _dropdown_info_anim_deleted_cb);

    //------------------------------启动动画------------------------------
    lv_anim_start(&anim);
    _dropdown_anim_finish_flag = false;
}

/*
 * @brief               放在主循环中,做标志位的处理
 * @return              无
 */
void info_ui::update() {
    if (_dropdown_anim_finish_flag == true) {
        if (lvgl_port_lock(0)) {
            _dropdown_anim_finish_flag = false;
            lv_obj_clean(this->_layer_pop);
            this->_layer_pop = NULL;
            lvgl_port_unlock();
        }
    }
}

info_ui::~info_ui(void) {

}


}

