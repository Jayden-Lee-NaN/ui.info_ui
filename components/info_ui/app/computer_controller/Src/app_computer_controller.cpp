//------------------------------------------------------------
// Created Time :   2024.12.18
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------
#include "app_computer_controller.h"
#include "extra/widgets/animimg/lv_animimg.h"
#include "esp_log.h"

namespace info_ui {

static lv_style_t           _slider_selected_style;

static void app_computer_controller_button_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        auto app = static_cast<app_computer_controller*>(lv_event_get_user_data(e));
        app->stop();
    }
}

static void app_computer_controller_send_data_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        auto btn = static_cast<app_computer_controller_btn*>(lv_event_get_user_data(e));
        auto app = btn->app;
        auto precent = btn->precent;
        std::string cmd = "bright: ";
        std::string val = "10%";
        switch (precent) {
            case info_ui::app_computer_controller::p10:
                val = "10%";
                break;
            case info_ui::app_computer_controller::p20:
                val = "20%";
                break;
            case info_ui::app_computer_controller::p30:
                val = "30%";
                break;
            case info_ui::app_computer_controller::p40:
                val = "40%";
                break;
            case info_ui::app_computer_controller::p50:
                val = "50%";
                break;
            case info_ui::app_computer_controller::p60:
                val = "60%";
                break;
            case info_ui::app_computer_controller::p70:
                val = "70%";
                break;
            case info_ui::app_computer_controller::p80:
                val = "80%";
                break;
            case info_ui::app_computer_controller::p90:
                val = "90%";
                break;
            case info_ui::app_computer_controller::p100:
                val = "100%";
                break;
            default:
                break;
        }
        cmd = cmd + val;
        if (app->get_wifi()->is_wifi_connected()) {
            app->get_wifi()->printf(cmd.c_str());
            printf("Send: ");
        }
        else {
            printf("Not Send: ");
        }
            printf("%s\n", cmd.c_str());
    }
        
}

void app_computer_controller::wifi_connect_task(void* pvParameters) {
    app_computer_controller* app = (app_computer_controller*)pvParameters;
    esp_err_t ret = app->_wifi->connect();
    if (ret == ESP_OK) {
        xEventGroupSetBits(app->_wifi_connect_event_group, _WIFI_CONNECTED_BIT);
    }
    else
        xEventGroupSetBits(app->_wifi_connect_event_group, _WIFI_FAILED_BIT);
    vTaskDelete(NULL);
}

void app_computer_controller::display_computer_control_layer_task(void* pvParameters) {
    app_computer_controller* app = (app_computer_controller*)pvParameters;
    EventBits_t bits = xEventGroupWaitBits(app->_wifi_connect_event_group, _WIFI_ANY_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
    if (bits & _WIFI_CONNECTED_BIT) {
        ESP_LOGI(app->INFO_UI_TAG, "WI-FI connected");
        app->hidden_connect_wifi_layer();
        app->display_computer_control_layer();
        app->enable_computer_control_group();
    }
    else {
        ESP_LOGI(app->INFO_UI_TAG, "WI-FI connect failed");
    }
    vTaskDelete(NULL);
}

extern "C" app_computer_controller::app_computer_controller(uint32_t disp_width, uint32_t disp_height){
    //------------------------------声明软件名称------------------------------
    this->_app_name = "app_computer_controller";

    //------------------------------挂载信息------------------------------
    this->_disp_width = disp_width;
    this->_disp_height = disp_height;

    //------------------------------初始化输入设备组------------------------------
    this->_indev_group_computer_control = lv_group_create();

    //------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_computer_controller);
    LV_IMG_DECLARE(picture_wifi_find);

    //------------------------------获取软件图标------------------------------
    this->_icon_src = (void*)&icon_computer_controller;

    //------------------------------获取查找WI-FI图片------------------------------
    this->_picture_wifi_find = (void*)&picture_wifi_find;

    //------------------------------创建WI-FI事件组------------------------------
    this->_wifi_connect_event_group = xEventGroupCreate();

    //------------------------------初始化选中的style------------------------------
    lv_style_init(&_slider_selected_style);
    lv_style_set_border_width(&_slider_selected_style, 1);
    lv_style_set_border_color(&_slider_selected_style, lv_color_hex(0));
    lv_style_set_border_post(&_slider_selected_style, true);
    lv_style_set_radius(&_slider_selected_style, 10);
}



void app_computer_controller::init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_indev_t* encoder_indev) { 
    //------------------------------创建屏幕------------------------------
    this->_app_panel = lv_obj_create(disp_layer);
    lv_obj_add_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(this->_app_panel, this->_disp_width, this->_disp_height);
    lv_obj_align(this->_app_panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(this->_app_panel, LV_SCROLLBAR_MODE_OFF);

    //------------------------------配置弹窗面板------------------------------
    this->_app_info_label = sys_popup_label;

    //------------------------------添加编码器------------------------------
    this->_encoder_indev = encoder_indev;
    this->_encoder_group = lv_group_create();

    //------------------------------绘制WI-FI连接层------------------------------
    this->draw_connect_wifi_layer();

    //------------------------------绘制用户面板层------------------------------
    this->create_computer_control_layer();
    this->draw_computer_control_layer();

    //------------------------------配置状态信息------------------------------
    this->_app_state = info_ui_app_state::INIT;
}

void app_computer_controller::run () {
    lvgl_port_lock(0);
    printf("run app_computer_controller\n");
    this->_app_state = info_ui_app_state::RUNNING;
    //------------------------------显示屏幕------------------------------
    this->_display_layer();

    //------------------------------等待WI-FI连接------------------------------
    // 创建连接任务
    if (!this->_wifi->is_socket_connected()) {
        this->hidden_computer_control_layer();
        this->display_connect_wifi_layer();
        xTaskCreate(wifi_connect_task, "wifi_connect_task", 4096, this, 5, NULL);
        xTaskCreate(display_computer_control_layer_task, "display_computer_control_layer_task", 4096, this, 5, NULL);
    }
    else {
        this->hidden_connect_wifi_layer();
        this->display_computer_control_layer();
        this->enable_computer_control_group();
    }


    // this->display_computer_control_layer();
    // this->enable_computer_control_group();




    // else {

    // }

    //------------------------------创建显示主界面的任务------------------------------
    // if (bits & _WIFI_CONNECTED_BIT) {
    //     ESP_LOGI(this->INFO_UI_TAG, "WI-FI connected");
    //     this->hidden_connect_wifi_layer();
    //     this->_encoder_group_enable();
    //     this->display_computer_control_layer();
    // }

    // this->hidden_connect_wifi_layer();

    lvgl_port_unlock();
}

void app_computer_controller::stop() {
    printf("stop app_computer_controller\n");
    this->_app_state = info_ui_app_state::STOPPED;
    this->_hidden_layer();
}

void app_computer_controller::draw_connect_wifi_layer() {
    //------------------------------配置连接WI-FI面板------------------------------
    this->_layer_connect_wifi = lv_obj_create(this->_app_panel);
    lv_obj_add_flag(this->_layer_connect_wifi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(this->_layer_connect_wifi, this->_disp_width, this->_disp_height);
    lv_obj_align(this->_layer_connect_wifi, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(this->_layer_connect_wifi, LV_SCROLLBAR_MODE_OFF);

    //------------------------------绘制WI-FI查询图片------------------------------
    lv_obj_t* img_wifi_find = lv_img_create(this->_layer_connect_wifi);
    lv_img_set_src(img_wifi_find, this->_picture_wifi_find);
    lv_obj_set_size(img_wifi_find, ((lv_img_dsc_t*)this->_picture_wifi_find)->header.w, ((lv_img_dsc_t*)this->_picture_wifi_find)->header.h);
    lv_obj_align(img_wifi_find, LV_ALIGN_CENTER, 0, 0);
}

void app_computer_controller::display_connect_wifi_layer() {
    lvgl_port_lock(0);
    lv_obj_clear_flag(this->_layer_connect_wifi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(this->_layer_connect_wifi);
    lvgl_port_unlock();
}

void app_computer_controller::hidden_connect_wifi_layer() {
    lvgl_port_lock(0);
    lv_obj_add_flag(this->_layer_connect_wifi, LV_OBJ_FLAG_HIDDEN);
    lvgl_port_unlock();
}


void app_computer_controller::create_computer_control_layer() {
    lvgl_port_lock(0);
    //------------------------------配置用户控制面板------------------------------
    // 基本设置
    this->_layer_computer_control = lv_obj_create(this->_app_panel);
    lv_obj_add_flag(this->_layer_computer_control, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_scrollbar_mode(this->_layer_computer_control, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(this->_layer_computer_control, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scroll_snap_y(this->_layer_computer_control, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(this->_layer_computer_control, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(this->_layer_computer_control, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // this->_layer_computer_control = lv_menu_create(this->_app_panel);
    // lv_menu_set_mode_root_back_btn(this->_layer_computer_control, LV_MENU_ROOT_BACK_BTN_ENABLED);
    // lv_obj_add_event_cb(this->_layer_computer_control, app_computer_controller_button_cb, LV_EVENT_CLICKED, this);
    // lv_obj_set_size(this->_layer_computer_control, this->_disp_width, this->_disp_height);
    // lv_obj_center(this->_layer_computer_control);
    lvgl_port_unlock();
}

extern "C" void app_computer_controller::draw_computer_control_layer() {
    lvgl_port_lock(0);
    this->_display_layer();
    this->display_computer_control_layer();

    this->create_btn(this->_layer_computer_control, 80, 30, p10);
    this->create_btn(this->_layer_computer_control, 80, 30, p20);
    this->create_btn(this->_layer_computer_control, 80, 30, p30);
    this->create_btn(this->_layer_computer_control, 80, 30, p40);
    this->create_btn(this->_layer_computer_control, 80, 30, p50);
    this->create_btn(this->_layer_computer_control, 80, 30, p60);
    this->create_btn(this->_layer_computer_control, 80, 30, p70);
    this->create_btn(this->_layer_computer_control, 80, 30, p80);
    this->create_btn(this->_layer_computer_control, 80, 30, p90);
    this->create_btn(this->_layer_computer_control, 80, 30, p100);

    //------------------------------创建退出按钮------------------------------
    LV_IMG_DECLARE(picture_exit);
    lv_obj_t* btn = lv_btn_create(this->_layer_computer_control);
    lv_obj_set_size(btn, 80, 30);
    lv_obj_center(btn);
    lv_obj_add_style(btn, &_slider_selected_style, LV_STATE_FOCUSED);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn, app_computer_controller_button_cb, LV_EVENT_CLICKED, this);

    lv_obj_t* img = lv_img_create(btn);
    lv_img_set_src(img, &picture_exit);
    lv_obj_set_size(img, 80, 30);
    lv_obj_center(img);
    lv_group_add_obj(this->_indev_group_computer_control, btn);
        
    this->_hidden_layer();
    lvgl_port_unlock();

}

void app_computer_controller::display_computer_control_layer() {
    lvgl_port_lock(0);
    lv_obj_clear_flag(this->_layer_computer_control, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(this->_layer_computer_control);
    lvgl_port_unlock();
}


void app_computer_controller::hidden_computer_control_layer() {
    lvgl_port_lock(0);
    lv_obj_add_flag(this->_layer_computer_control, LV_OBJ_FLAG_HIDDEN);
    lvgl_port_unlock();
}

extern "C" void app_computer_controller::create_btn(lv_obj_t* parent, lv_coord_t w, lv_coord_t h, precentage precent) {
    lvgl_port_lock(0);
    // this->_select_precent = precent;
    LV_IMG_DECLARE(picture_p10);
    LV_IMG_DECLARE(picture_p20);
    LV_IMG_DECLARE(picture_p30);
    LV_IMG_DECLARE(picture_p40);
    LV_IMG_DECLARE(picture_p50);
    LV_IMG_DECLARE(picture_p60);
    LV_IMG_DECLARE(picture_p70);
    LV_IMG_DECLARE(picture_p80);
    LV_IMG_DECLARE(picture_p90);
    LV_IMG_DECLARE(picture_p100);
    LV_IMG_DECLARE(picture_exit);

    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, w, h);
    lv_obj_center(btn);
    auto btn_struct = new(app_computer_controller_btn);
    btn_struct->precent = precent;
    btn_struct->app = this;
    lv_obj_add_event_cb(btn, app_computer_controller_send_data_cb, LV_EVENT_CLICKED, btn_struct);
    lv_obj_add_style(btn, &_slider_selected_style, LV_STATE_FOCUSED);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* img = lv_img_create(btn);
    switch (precent) {
        case p10:
            lv_img_set_src(img, &picture_p10);
            break;
        case p20:
            lv_img_set_src(img, &picture_p20);
            break;
        case p30:
            lv_img_set_src(img, &picture_p30);
            break;
        case p40:
            lv_img_set_src(img, &picture_p40);
            break;
        case p50:
            lv_img_set_src(img, &picture_p50);
            break;
        case p60:
            lv_img_set_src(img, &picture_p60);
            break;
        case p70:
            lv_img_set_src(img, &picture_p70);
            break;
        case p80:
            lv_img_set_src(img, &picture_p80);
            break;
        case p90:
            lv_img_set_src(img, &picture_p90);
            break;
        case p100:
            lv_img_set_src(img, &picture_p100);
            break;
        default:
            break;
    }
    lv_obj_set_size(img, w, h);
    lv_obj_center(img);
    lv_group_add_obj(this->_indev_group_computer_control, btn);
    lvgl_port_unlock();
}

}
