set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
file(GLOB UI_SOURCES 
    "ui/Src/*.cpp"
    "ui/Src/*.c"
)

file (GLOB CONFIG_SOURCES
    "config/Src/*.cpp"
    "config/Src/*.c"
)


file (GLOB APP_SOURCES
    "app/sys_info/Src/*.c"
    "app/sys_info/Src/*.cpp"
    "app/music/Src/*.c"
    "app/music/Src/*.cpp"
    "app/imu/Src/*.c"
    "app/imu/Src/*.cpp"
    "app/temperature/Src/*.c"
    "app/temperature/Src/*.cpp"
    "app/clock/Src/*.c"
    "app/clock/Src/*.cpp"
    "app/computer_controller/Src/*.c"
    "app/computer_controller/Src/*.cpp"
) 

file (GLOB APP_ICON
    "app/sys_info/Icon/*.c"
    "app/music/Icon/*.c"
    "app/imu/Icon/*.c"
    "app/temperature/Icon/*.c"
    "app/clock/Icon/*.c"
    "app/computer_controller/Icon/*.c"
)

idf_component_register(
    SRCS 
        ${UI_SOURCES} 
        ${CONFIG_SOURCES}
        ${APP_SOURCES}
        ${APP_ICON}
        "./info_ui.cpp"
    INCLUDE_DIRS 
        "ui/Inc"
        "config/Inc"
        "app/sys_info/Inc"
        "app/music/Inc"
        "app/imu/Inc"
        "app/temperature/Inc"
        "app/clock/Inc"
        "app/computer_controller/Inc"
        "."
    REQUIRES
        esp_wifi
)
