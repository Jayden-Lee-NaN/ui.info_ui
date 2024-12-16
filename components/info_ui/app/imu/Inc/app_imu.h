#pragma once
#include "info_ui_app_base.h"
#include "lvgl.h"

namespace info_ui {

class app_imu : public info_ui_app_base {
public:
    app_imu(uint32_t disp_width, uint32_t disp_height);
    void init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_group_t* button_group) override;
    void run() override;
    void stop() override;
};

}
