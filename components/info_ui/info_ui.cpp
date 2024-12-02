//------------------------------------------------------------
// Created Time :    2024.11.23
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include <stdio.h>
#include "info_ui.h"
#include <string>

namespace info_ui {

static void _pop_info_anim_ready_cb(lv_anim_t* a) {
    lv_obj_del((lv_obj_t*)a->var);
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
}

lv_obj_t* info_ui::pop_info(std::string info) {
    
    //------------------------------获取屏幕------------------------------
    lv_obj_t* popup = lv_disp_get_scr_act(this->_disp);
    lv_obj_set_size(popup, this->_cfg->disp_height * 0.5, this->_cfg->disp_width * 0.5);
    lv_obj_align(popup, LV_ALIGN_CENTER, 0, 0);
    
    static lv_style_t style;
    lv_style_init(&style);


    lv_obj_t* label = lv_label_create(popup);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(label, this->_cfg->disp_height - 20);
    lv_label_set_text(label, info.c_str());
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_border_width(label, 2, LV_PART_MAIN);
    lv_obj_set_style_pad_all(label, 5, LV_PART_MAIN);

    //------------------------------创建动画------------------------------
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, popup);

    lv_anim_set_values(&anim, this->_cfg->disp_width, 0);
    
    lv_anim_set_time(&anim, 500);       // 动画市场500ms
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

    // lv_anim_set_ready_cb(&anim, _pop_info_anim_ready_cb);

    lv_anim_start(&anim);

    return popup;
}

info_ui::~info_ui(void) {

}


}

