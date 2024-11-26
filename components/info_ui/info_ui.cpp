//------------------------------------------------------------
// Created Time :    2024.11.23
// Author       :   JaydenLee
// Email        :   jayden.lee.nan@gmail.com
//------------------------------------------------------------

#include <stdio.h>
#include "info_ui.h"

namespace info_ui {

info_ui::info_ui(info_ui_config_t* cfg) {
    //------------------------------挂载数据------------------------------
    this->cfg = cfg;    
}

info_ui::~info_ui(void) {

}

}

