#include "app_music.h"
extern "C" info_ui::app_music::app_music() {
    // ------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_voice);
    this->_icon_src = (void*)&icon_voice;
}
