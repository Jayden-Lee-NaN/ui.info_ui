//------------------------------------------------------------
// Created Time :    2024.11.23
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include <stdio.h>
#include <string>
#include "info_ui.h" 
#include "core/lv_disp.h"
#include "core/lv_event.h"
#include "core/lv_obj_scroll.h"
#include "esp_lvgl_port_button.h"
#include "extra/widgets/imgbtn/lv_imgbtn.h"
#include "info_ui_app_registry.h"
#include "app_sys_info.h"
#include "app_music.h"
#include "app_imu.h"
#include "app_temperature.h"
#include "lvgl__lvgl/src/core/lv_obj_tree.h"

namespace info_ui {

static lv_style_t           _app_selected_style;    // 软件图标被选择的样式
//------------------------------当前所在的页面------------------------------ 
static info_ui_page         _sys_page = INFO_UI_PAGE_HOME;      // 系统当前所在的页面
lv_obj_t* app_select_panel = NULL;

//------------------------------软件注册宏------------------------------
#define REGISTER_APP(class_name, app_name) \
    static info_ui_app_base* create_##class_name() { \
        return new class_name(128, 64); \
    }\
    static bool class_name##_registered = \
        info_ui_app_registry::get_instance().register_app(app_name, create_##class_name);

//------------------------------注册软件------------------------------
REGISTER_APP(app_sys_info, "app_sys_info");             
REGISTER_APP(app_music, "app_music");
REGISTER_APP(app_imu, "app_imu");
REGISTER_APP(app_temperature, "app_temperature");

//------------------------------当前所运行的软件------------------------------
static void*                _app_running = NULL;   // 正在运行的app指针

static void app_selected_button_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);

    //------------------------------进入应用------------------------------
    if (code == LV_EVENT_CLICKED) {
        info_ui_app_base* _app = (info_ui_app_base*)lv_event_get_user_data(e);
        _sys_page = INFO_UI_PAGE_APP;
        if (_app->get_app_name() == "app_sys_info") {
            app_sys_info* app = static_cast<app_sys_info*>(_app);
            lv_obj_add_flag(app_select_panel, LV_OBJ_FLAG_HIDDEN);
            app->run();
            _app_running = (void*)_app;
        }
        else if (_app->get_app_name() == "app_temperature") {
            app_temperature* app = static_cast<app_temperature*>(_app);
            lv_obj_add_flag(app_select_panel, LV_OBJ_FLAG_HIDDEN);
            app->run();
            _app_running = (void*)_app;
        }
        else if (_app->get_app_name() == "app_imu") {
            app_imu* app = static_cast<app_imu*>(_app);
            lv_obj_add_flag(app_select_panel, LV_OBJ_FLAG_HIDDEN);
            app->run();
            _app_running = (void*)_app;
        }
        else if (_app->get_app_name() == "app_music") {
            app_music* app = static_cast<app_music*>(_app);
            lv_obj_add_flag(app_select_panel, LV_OBJ_FLAG_HIDDEN);
            app->run();
            _app_running = (void*)_app;
        }
    }

}

info_ui::info_ui(info_ui_config_t* cfg, int32_t button_prev_num, int32_t button_next_num, int32_t button_enter_num) {
    lvgl_port_lock(0);
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

    //------------------------------不优雅的设计------------------------------
    app_select_panel = this->_app_select_layer;
    

    //------------------------------配置软件被选择后的样式------------------------------
    lv_style_init(&_app_selected_style);
    lv_style_set_border_width(&_app_selected_style, 1);
    lv_style_set_border_color(&_app_selected_style, lv_color_hex(0));
    lv_style_set_border_post(&_app_selected_style, true);
    lv_style_set_radius(&_app_selected_style, 10);

    //------------------------------配置软件面板层------------------------------
    this->_app_panel_layer = lv_obj_create(this->_disp_layer);
    lv_obj_set_size(this->_app_panel_layer, 128, 64);
    lv_obj_align(this->_app_panel_layer, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(this->_app_panel_layer, LV_OBJ_FLAG_HIDDEN);

    //------------------------------初始化app------------------------------
    // 初始化sys_info
    app_sys_info* sys_info = static_cast<app_sys_info*>(info_ui_app_registry::get_instance().create_app("app_sys_info"));
    if (sys_info) {
        sys_info->init(this->_app_panel_layer, this->_info_label, this->_button_handle);
        this->app_register((info_ui_app_base*)sys_info);
    }

    app_music* music = static_cast<app_music*>(info_ui_app_registry::get_instance().create_app("app_music"));
    if (music) {
        music->init(this->_app_panel_layer, this->_info_label, this->_button_handle);
        this->app_register((info_ui_app_base*)music);
    }

    app_imu* imu = static_cast<app_imu*>(info_ui_app_registry::get_instance().create_app("app_imu"));
    if (imu) {
        imu->init(this->_app_panel_layer, this->_info_label, this->_button_handle);
        this->app_register((info_ui_app_base*)imu);
    }

    app_temperature* temperature = static_cast<app_temperature*>(info_ui_app_registry::get_instance().create_app("app_temperature"));
    if (temperature) {
        temperature->init(this->_app_panel_layer, this->_info_label, this->_button_handle);
        this->app_register((info_ui_app_base*)temperature);
    }

    lvgl_port_unlock();
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

void info_ui::start() {
    lvgl_port_lock(0);
    lv_obj_move_foreground(this->_app_select_layer);
    // lv_obj_move_foreground(this->_app_panel_layer);
    lvgl_port_unlock();
}


void info_ui::app_register(info_ui_app_base* app) {
    lvgl_port_lock(0);
    //------------------------------添加选择的按钮------------------------------
    lv_obj_t* btn = lv_btn_create(this->_app_select_layer);
    lv_obj_set_size(btn, 60, 60);
    lv_obj_center(btn);
    lv_obj_add_style(btn, &_app_selected_style, LV_STATE_FOCUSED);
    
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(btn, app_selected_button_cb, LV_EVENT_CLICKED, app);

    //------------------------------添加图标------------------------------
    lv_obj_t* icon = lv_img_create(btn);
    lv_img_set_src(icon, app->get_icon_img());
    lv_obj_set_size(icon, 56, 56);
    lv_obj_align(icon, LV_ALIGN_CENTER, 0, 0);
    lv_group_add_obj(this->_button_group, btn);

    //------------------------------刷新面板------------------------------
    lv_obj_update_snap(this->_app_select_layer, LV_ANIM_ON);
    lvgl_port_unlock();
}

void info_ui::disply_app_select_layer() {
    lv_obj_clear_flag(this->_app_select_layer, LV_OBJ_FLAG_HIDDEN);
    lv_indev_set_group(this->_button_handle, this->_button_group);
    lv_obj_move_foreground(this->_app_select_layer);
}

void info_ui::hiden_app_select_layer() {

}

/*
 * @brief               放在主循环中,做标志位的处理
 * @return              无
 */
void info_ui::update() {
        if (lvgl_port_lock(0)) {
            if (_app_running != NULL) {
                if (((info_ui_app_base*)_app_running)->get_app_state() == info_ui_app_state::STOPPED) {
                    ((info_ui_app_base*)_app_running)->exit();              // 退出app
                    this->disply_app_select_layer();
                    _app_running = NULL;
                }
            }
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(1));
}

}

