#pragma once
#include "info_ui_app_base.h"
#include <string>
#include <list>

namespace info_ui {

class info_ui_app_registry {
public:
    static const uint32_t app_max_val = 64;

public:
    typedef info_ui_app_base* (*create_app_func)();

    //------------------------------获取单例对象------------------------------
    static info_ui_app_registry& get_instance() {
        static info_ui_app_registry instance;
        return instance;
    }

    //------------------------------注册新应用------------------------------
    bool register_app(std::string app_name, create_app_func creator);

    //------------------------------创建应用实例------------------------------
    info_ui_app_base* create_app(std::string app_name);

public:
    info_ui_app_registry() = default;         
    ~info_ui_app_registry() = default;        
    // 禁止外部拷贝构造
    info_ui_app_registry(const info_ui_app_registry &app_registry) = delete;
    // 禁止外部赋值操作
    const info_ui_app_registry &operator=(const info_ui_app_registry &app_registry) = delete;

private:
    std::list<std::string>  _app_name_list;
    create_app_func         _app_creators[info_ui_app_registry::app_max_val];
    uint32_t                _app_count = 0;
};
}
