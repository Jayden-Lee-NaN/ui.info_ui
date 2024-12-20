//------------------------------------------------------------
// Created Time :   2024.12.18
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------
#include "app_computer_controller.h"
#include "extra/widgets/animimg/lv_animimg.h"
#include "esp_log.h"

namespace info_ui {

static void app_computer_controller_button_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        auto app = static_cast<app_computer_controller*>(lv_event_get_user_data(e));
        app->stop();
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

    //------------------------------等待WI-FI连接成功------------------------------
    EventBits_t bits = xEventGroupWaitBits(app->_wifi_connect_event_group, _WIFI_ANY_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
    if (bits & _WIFI_CONNECTED_BIT) {
        ESP_LOGI(app->INFO_UI_TAG, "WI-FI connected");
        app->hidden_connect_wifi_layer();
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

    //------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_computer_controller);
    LV_IMG_DECLARE(picture_wifi_find);

    //------------------------------获取软件图标------------------------------
    this->_icon_src = (void*)&icon_computer_controller;

    //------------------------------获取查找WI-FI图片------------------------------
    this->_picture_wifi_find = (void*)&picture_wifi_find;

    //------------------------------创建WI-FI事件组------------------------------
    this->_wifi_connect_event_group = xEventGroupCreate();
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

    //------------------------------配置连接WI-FI面板------------------------------
    this->_layer_connect_wifi = lv_obj_create(this->_app_panel);
    lv_obj_add_flag(this->_layer_connect_wifi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(this->_layer_connect_wifi, this->_disp_width, this->_disp_height);
    lv_obj_align(this->_layer_connect_wifi, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(this->_layer_connect_wifi, LV_SCROLLBAR_MODE_OFF);

    //------------------------------配置用户控制面板------------------------------
    this->_layer_computer_control = lv_obj_create(this->_app_panel);
    lv_obj_add_flag(this->_layer_computer_control, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(this->_layer_computer_control, this->_disp_width, this->_disp_height);
    lv_obj_align(this->_layer_computer_control, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(this->_layer_computer_control, LV_SCROLLBAR_MODE_OFF);

    //------------------------------配置状态信息------------------------------
    this->_app_state = info_ui_app_state::INIT;
}

void app_computer_controller::run () {
    lvgl_port_lock(0);
    printf("run app_computer_controller\n");
    this->_app_state = info_ui_app_state::RUNNING;
    lv_obj_clear_flag(this->_app_panel, LV_OBJ_FLAG_HIDDEN);

    //------------------------------显示屏幕------------------------------
    this->_display_layer();
    this->display_connect_wifi_layer();

    //------------------------------等待WI-FI连接------------------------------
    // 创建连接任务
    xTaskCreate(wifi_connect_task, "wifi_connect_task", 4096, this, 5, NULL);

    //------------------------------创建显示主界面的任务------------------------------
    xTaskCreate(display_computer_control_layer_task, "display_computer_control_layer_task", 4096, this, 5, NULL);


    // esp_err_t ret = this->_wifi->connect();     
    // if (ret == ESP_OK) {
    //     // 连接成功
    //     ESP_LOGI(this->INFO_UI_TAG, "WI-FI Connected Successfully");
    // }
    // else {
    //     ESP_LOGI(this->INFO_UI_TAG, "WI-FI Connected Fail");
    // }
    lvgl_port_unlock();
}

void app_computer_controller::stop() {
    printf("stop app_computer_controller\n");
    this->_app_state = info_ui_app_state::STOPPED;
    this->_hidden_layer();
}


void app_computer_controller::display_connect_wifi_layer() {
    lvgl_port_lock(0);
    //------------------------------添加查找WI-FI图片------------------------------
    lv_obj_t* img_wifi_find = lv_img_create(this->_layer_connect_wifi);
    lv_img_set_src(img_wifi_find, this->_picture_wifi_find);
    lv_obj_set_size(img_wifi_find, ((lv_img_dsc_t*)this->_picture_wifi_find)->header.w, ((lv_img_dsc_t*)this->_picture_wifi_find)->header.h);
    lv_obj_align(img_wifi_find, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(this->_layer_connect_wifi, LV_OBJ_FLAG_HIDDEN);
    lvgl_port_unlock();
}

void app_computer_controller::hidden_connect_wifi_layer() {
    lvgl_port_lock(0);
    lv_obj_add_flag(this->_layer_connect_wifi, LV_OBJ_FLAG_HIDDEN);
    lvgl_port_unlock();
}

void app_computer_controller::display_computer_control_layer() {

}

void app_computer_controller::hidden_computer_control_layer() {

}

}
