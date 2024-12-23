#pragma once
#include "info_ui_app_base.h"
#include "lvgl.h"
#include "info_ui_remote_connect.h"

namespace info_ui {

struct app_computer_controller_slider {
    lv_obj_t*               slider; 
    lv_obj_t*               label;
    std::string             cmd;
    remote_connect::wifi*   wifi;
};

class app_computer_controller : public info_ui_app_base {

public:
enum precentage{
    p10,
    p20,
    p30,
    p40,
    p50,
    p60,
    p70,
    p80,
    p90,
    p100,
};

public:
    app_computer_controller(uint32_t disp_width, uint32_t disp_height);
    void init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_indev_t* encoder_indev) override;
    void set_wifi(remote_connect::wifi* wifi) { 
        this->_wifi = wifi; 
        this->_wifi_is_set = true;
    }
    void run() override;
    void stop() override;
    precentage get_precent() { return this->_select_precent; }
    remote_connect::wifi* get_wifi() { return this->_wifi; }

private:
    void draw_connect_wifi_layer();
    void display_connect_wifi_layer();
    void hidden_connect_wifi_layer();

    void create_computer_control_layer();
    void draw_computer_control_layer();
    void display_computer_control_layer();
    void hidden_computer_control_layer();

    void slider_register(app_computer_controller_slider* cc_slider, lv_obj_t* flex_cont);
    void create_btn(lv_obj_t* parent, lv_coord_t w, lv_coord_t h, precentage precent);

    static void wifi_connect_task(void* pvParameters);
    static void display_computer_control_layer_task(void* pvParameters);

    void enable_computer_control_group() {
        if (this->_indev_group_computer_control == NULL) 
            this->_indev_group_computer_control = lv_group_create();
        lv_indev_set_group(this->_encoder_indev, this->_indev_group_computer_control);
    }
    
private:
    remote_connect::wifi*   _wifi;
    bool                    _wifi_is_set = false;
    lv_obj_t*               _layer_connect_wifi = NULL;
    lv_obj_t*               _layer_computer_control = NULL;
    lv_obj_t*               _exit_btn = NULL;
    void*                   _picture_wifi_find;

    //------------------------------设备组------------------------------
    lv_group_t*             _indev_group_computer_control = NULL;

    //------------------------------事件组定义------------------------------
    EventGroupHandle_t      _wifi_connect_event_group;
    static constexpr uint32_t _WIFI_CONNECTED_BIT = BIT0;
    static constexpr uint32_t _WIFI_FAILED_BIT = BIT1;
    static constexpr uint32_t _WIFI_ANY_BIT = (_WIFI_CONNECTED_BIT | _WIFI_FAILED_BIT);

    precentage              _select_precent = p10;

};

struct app_computer_controller_btn {
    app_computer_controller*                app;
    app_computer_controller::precentage     precent;
};

}
