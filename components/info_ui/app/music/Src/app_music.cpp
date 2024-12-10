#include "app_music.h"

extern "C" info_ui::app_music::app_music(uint32_t disp_width, uint32_t disp_height) {
    //------------------------------挂载信息------------------------------
    this->_disp_width = disp_width;
    this->_disp_height = disp_height;

    // ------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_voice);
    this->_icon_src = (void*)&icon_voice;
}

void info_ui::app_music::run() {
    this->_app_available_flag = true;    
}
