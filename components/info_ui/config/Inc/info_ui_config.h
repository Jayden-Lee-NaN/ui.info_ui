#pragma once
#include <stdint.h>
#include "esp_lcd_panel_vendor.h"
#include "esp_lvgl_port.h"
#include "esp_lcd_types.h"


namespace info_ui{
    struct info_ui_config_t {
        esp_lcd_panel_io_handle_t           io_handle;              // io驱动
        esp_lcd_panel_handle_t              panel_handle;           // 面板驱动
        uint32_t                            disp_width;             // 屏幕的宽
        uint32_t                            disp_height;            // 屏幕的高
    };
}
