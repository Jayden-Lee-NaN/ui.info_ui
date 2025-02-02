#pragma once

#include "info_ui_app_base.h"
#include "lvgl.h"


namespace info_ui {

class app_sys_info : public info_ui_app_base {
public:
    app_sys_info(uint32_t disp_width, uint32_t disp_height);
    void init(lv_obj_t* disp_layer, lv_obj_t* sys_popup_label, lv_indev_t* encoder_indev) override;
    void run() override;
    void stop() override;
private:
    void draw_interface();

private:
    void* _author_img;
    lv_group_t*     _sys_button_group;
    lv_indev_t*     _button_indev;
    lvgl_port_nav_btns_cfg_t* _btns;
};

}
