//------------------------------------------------------------
// Created Time :    2024.12.07
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include "app_sys_info.h"

extern "C" info_ui::app_sys_info::app_sys_info() {
    //------------------------------获取图片------------------------------
    LV_IMG_DECLARE(icon_message);
    LV_IMG_DECLARE(icon_jayden_lee);

    //------------------------------获取软件图标和软件面板------------------------------
    this->_icon_src = (void*)&icon_message;

    //------------------------------添加作者图标------------------------------
    // this->_authors_list.push_back((void*)&icon_jayden_lee);

    
}
