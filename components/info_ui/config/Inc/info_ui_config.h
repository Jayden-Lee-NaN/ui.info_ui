#pragma once
#include <stdint.h>
#include "esp_lvgl_port_disp.h"

namespace info_ui{
    struct info_ui_config_t {
        const lvgl_port_display_cfg_t*    disp_cfg;          // 屏幕的配置信息
        uint32_t                    disp_width;             // 屏幕的宽
        uint32_t                    disp_height;
    };
}
