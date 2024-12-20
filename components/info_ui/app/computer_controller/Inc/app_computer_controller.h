#pragma once
#include "info_ui_app_base.h"
#include "lvgl.h"
#include "info_ui_remote_connect.h"

namespace info_ui {


class app_computer_controller : public info_ui_app_base {

public:
    app_computer_controller(uint32_t disp_width, uint32_t disp_height);
    void init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_indev_t* encoder_indev) override;
    void set_wifi(remote_connect::wifi* wifi) { 
        this->_wifi = wifi; 
        this->_wifi_is_set = true;
    }
    void run() override;
    void stop() override;

private:
    void display_connect_wifi_layer();
    void hidden_connect_wifi_layer();
    void display_computer_control_layer();
    void hidden_computer_control_layer();
    void display_loading_anim();
    static void wifi_connect_task(void* pvParameters);
    static void display_computer_control_layer_task(void* pvParameters);
private:
    remote_connect::wifi*   _wifi;
    bool                    _wifi_is_set = false;
    lv_obj_t*               _layer_connect_wifi;
    lv_obj_t*               _layer_computer_control;
    void*                   _picture_wifi_find;
    //------------------------------事件组定义------------------------------
    EventGroupHandle_t      _wifi_connect_event_group;
    static constexpr uint32_t _WIFI_CONNECTED_BIT = BIT0;
    static constexpr uint32_t _WIFI_FAILED_BIT = BIT1;
    static constexpr uint32_t _WIFI_ANY_BIT = (_WIFI_CONNECTED_BIT | _WIFI_FAILED_BIT);
};

}
