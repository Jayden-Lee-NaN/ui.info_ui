#include "info_ui_app_registry.h"
namespace info_ui {

/*
 * @brief               注册app
 */
bool info_ui_app_registry::register_app(std::string app_name, create_app_func creator) {
    // 检查软件数量是否超过最大值
    if (this->_app_count > info_ui_app_registry::app_max_val) 
        return false;
    
    // 检查是否已经存在
    for (const auto& str : this->_app_name_list) {
        if (str == app_name)
            return false;
    }

    // 添加应用
    this->_app_name_list.push_back(app_name);
    this->_app_creators[this->_app_count] = creator;
    ++this->_app_count;
    return true;
}

/*
 * @brief               创建实例
 */
info_ui_app_base* info_ui_app_registry::create_app(std::string app_name) {
    uint32_t i = 0;
    for (auto it = this->_app_name_list.begin(); it != this->_app_name_list.end(); ++it, ++i) {
        if (*it == app_name) {
            return this->_app_creators[i]();        // 创建一个实例返回出去
        }
    }
    return nullptr;
}

}
