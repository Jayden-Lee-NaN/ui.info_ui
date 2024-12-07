//------------------------------------------------------------
// Created Time :    2024.11.23
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include <stdio.h>
#include "info_ui.h" 
#include "core/lv_disp.h"
#include "core/lv_event.h"
#include "core/lv_obj_scroll.h"
#include "esp_lvgl_port_button.h"
#include "extra/widgets/imgbtn/lv_imgbtn.h"
#include <string>
#include "app_sys_info.h"
#include "app_music.h"

namespace info_ui {

static lv_style_t   _app_selected_style;    // 软件图标被选择的样式

info_ui::info_ui(info_ui_config_t* cfg, int32_t button_prev_num, int32_t button_next_num, int32_t button_enter_num) {
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

    //------------------------------添加定位按键------------------------------
    const button_config_t button_config[] = {
        {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
            .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
            .gpio_button_config = {
                .gpio_num = button_prev_num,
                .active_level = 0,
            },
        },

        {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
            .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
            .gpio_button_config = {
                .gpio_num = button_next_num,
                .active_level = 0,
            },
        },

        {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
            .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
            .gpio_button_config = {
                .gpio_num = button_enter_num,
                .active_level = 0,
            },
        },
    };

    lvgl_port_nav_btns_cfg_t btns = {
        .disp = this->_disp,
        .button_prev = &button_config[0],
        .button_next = &button_config[1],
        .button_enter = &button_config[2],
    };
    this->_button_handle = lvgl_port_add_navigation_buttons(&btns);
    this->_button_group = lv_group_create();
    lv_indev_set_group(this->_button_handle, this->_button_group);

    //------------------------------获取屏幕------------------------------
    this->_disp_layer = lv_disp_get_scr_act(this->_disp);
    this->_top = lv_disp_get_layer_top(this->_disp);

    //------------------------------设置info label------------------------------
    this->_info_label = lv_label_create(this->_top);
    lv_label_set_long_mode(this->_info_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(this->_info_label, this->_cfg->disp_height - 20);
    lv_obj_align(this->_info_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(this->_info_label, 2, LV_PART_MAIN);
    lv_obj_set_style_pad_all(this->_info_label, 5, LV_PART_MAIN);
    lv_obj_set_y(this->_info_label, this->_cfg->disp_width);

    //------------------------------配置软件选择层------------------------------
    this->_app_select_layer = lv_obj_create(this->_disp_layer);
    lv_obj_set_size(this->_app_select_layer, 128, 64);
    lv_obj_set_scroll_snap_x(this->_app_select_layer, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(this->_app_select_layer, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(this->_app_select_layer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(this->_app_select_layer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(this->_app_select_layer, LV_ALIGN_CENTER, 0, 0);

    //------------------------------配置软件被选择后的样式------------------------------
    lv_style_init(&_app_selected_style);
    lv_style_set_border_width(&_app_selected_style, 1);
    lv_style_set_border_color(&_app_selected_style, lv_color_hex(0));
    lv_style_set_border_post(&_app_selected_style, true);
    lv_style_set_radius(&_app_selected_style, 10);

    //------------------------------注册软件------------------------------
    app_sys_info sys_info;
    this->app_register((info_ui_app_base*)&sys_info);
    
    app_music music;
    this->app_register((info_ui_app_base*)&music);
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

    lv_anim_set_time(&anim, 1000);                // 动画时长 1000 ms
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
        if (lvgl_port_lock(0)) {
            // TODO:tbd
            lvgl_port_unlock();
        }
}

extern "C" void info_ui::test_button_input() {
    lvgl_port_lock(0);

    LV_IMG_DECLARE(icon_message);
    
    LV_IMG_DECLARE(icon_chat);

    LV_IMG_DECLARE(icon_voice);

    LV_IMG_DECLARE(icon_windows);

    
    void* img_src[] = {
        (void*)&icon_message,
        (void*)&icon_chat,
        (void*)&icon_voice,
        (void*)&icon_windows,
    };

    lv_obj_t* panel = lv_obj_create(this->_disp_layer);
    lv_obj_set_size(panel, 128, 64);
    lv_obj_set_scroll_snap_x(panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(panel, LV_ALIGN_CENTER, 0, 0);

    static lv_style_t style_focused;
    lv_style_init(&style_focused);
    lv_style_set_border_width(&style_focused, 1);
    lv_style_set_border_color(&style_focused, lv_color_hex(0));
    lv_style_set_border_post(&style_focused, true);
    lv_style_set_radius(&style_focused, 10);


    for (int i = 0; i < 4; ++i) {
        lv_obj_t* btn = lv_btn_create(panel);
        lv_obj_set_size(btn, 60, 60);
        lv_obj_center(btn);
        lv_obj_add_style(btn, &style_focused, LV_STATE_FOCUSED);

        lv_obj_t* img = lv_img_create(btn);
        lv_img_set_src(img, img_src[i]);
        lv_obj_set_size(img, 56, 56);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

        lv_group_add_obj(this->_button_group, btn);
    }
    lv_obj_update_snap(panel, LV_ANIM_ON);

     
    lvgl_port_unlock();
}


void info_ui::app_register(info_ui_app_base* app) {
    lvgl_port_lock(0);
    //------------------------------添加选择的按钮------------------------------
    lv_obj_t* btn = lv_btn_create(this->_app_select_layer);
    lv_obj_set_size(btn, 60, 60);
    lv_obj_center(btn);
    lv_obj_add_style(btn, &_app_selected_style, LV_STATE_FOCUSED);

    //------------------------------添加图标------------------------------
    lv_obj_t* icon = lv_img_create(btn);
    lv_img_set_src(icon, app->get_icon_img());
    lv_obj_set_size(icon, 56, 56);
    lv_obj_align(icon, LV_ALIGN_CENTER, 0, 0);
    lv_group_add_obj(this->_button_group, btn);

    //------------------------------注册面板------------------------------
    app->panel_register(this->_app_select_layer);

    //------------------------------刷新面板------------------------------
    lv_obj_update_snap(this->_app_select_layer, LV_ANIM_ON);
    lvgl_port_unlock();
}

}

