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
) 

file (GLOB APP_ICON
    "app/sys_info/Icon/*.c"
    "app/music/Icon/*.c"
)

idf_component_register(
    SRCS 
        ${UTILITY_SOURCE} 
        ${CONFIG_SOURCES}
        ${APP_SOURCES}
        ${APP_ICON}
        "./info_ui.cpp"
    INCLUDE_DIRS 
        "ui/Inc"
        "config/Inc"
        "app/sys_info/Inc"
        "app/music/Inc"
        "."
)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
