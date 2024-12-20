#pragma once
#include <stdint.h>

namespace info_ui {

class info_ui_item {
public:        
    uint32_t            _disp_width;             // 屏幕的宽
    uint32_t            _disp_height;            // 屏幕的高
    const char* INFO_UI_TAG  = "INFO_UI_TAG";
};

}
