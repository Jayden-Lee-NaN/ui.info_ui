set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
file(GLOB UI_SOURCES 
    "ui/Src/*.cpp"
    "ui/Src/*.c"
)

file (GLOB CONFIG_SOURCES
    "config/Src/*.cpp"
    "config/Src/*.c"
)

file (GLOB DRIVER_SOURCES
    "driver/Src/*.cpp"
    "driver/Src/*.c"
)

idf_component_register(
    SRCS 
        ${UTILITY_SOURCE} 
        ${CONFIG_SOURCES}
        ${DRIVER_SOURCES}
        "./info_ui.cpp"
    INCLUDE_DIRS 
        "ui/Inc"
        "config/Inc"
        "driver/Inc"
        "."
)
