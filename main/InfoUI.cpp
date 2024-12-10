#include <stdio.h>
#include <string>

#include "core/lv_obj_pos.h"
#include "esp_lcd_types.h"
#include "esp_log_level.h"
#include "esp_lvgl_port.h"
#include "info_ui.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "misc/lv_anim.h"
#include "misc/lv_area.h"
#include "driver/gpio.h"
#include "iot_button.h"

//------------------------------LOG参数设置------------------------------
static const char* TAG = "TEST";

//------------------------------LCD参数设置------------------------------
#define TEST_LCD_PIXEL_CLOCK_HZ     (400 * 1000)
#define TEST_PIN_NUM_SDA            (18)
#define TEST_PIN_NUM_SCL            (15)
#define TEST_PIN_NUM_RST            (-1)
#define TEST_I2C_HW_ADDR            (0x3C)
#define TEST_LCD_H_RES              (128)
#define TEST_LCD_V_RES              (64)
#define TEST_LCD_CMD_BITS           (8)
#define TEST_LCD_PARAM_BITS         (8)

//------------------------------I2C设置------------------------------
#define I2C_HOST                    (I2C_NUM_0)


//------------------------------测试代码------------------------------
void test_lvgl_demo_ui(lv_disp_t* disp) {
    lv_obj_t* scr = lv_disp_get_scr_act(disp);
    lv_obj_t* label = lv_label_create(scr);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label, "Hello World!");
    lv_obj_set_width(label, TEST_LCD_V_RES);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

static void button_49_cb(void* arg, void* usr_data) {
    info_ui::info_ui* ui = (info_ui::info_ui*)usr_data;
    ui->popup_info("Hello World");
}

static void button_37_cb(void* arg, void* usr_data) {
    info_ui::info_ui* ui = (info_ui::info_ui*)usr_data;
    ui->popup_info("Hello JaydenLee");
}

extern "C" void app_main(void)
{
    //------------------------------I2C配置------------------------------
    ESP_LOGI(TAG, "Initialize I2C bus");
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = TEST_PIN_NUM_SDA,
        .scl_io_num = TEST_PIN_NUM_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
    };
    i2c_conf.master.clk_speed = TEST_LCD_PIXEL_CLOCK_HZ;

    ESP_ERROR_CHECK(i2c_param_config(I2C_HOST, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_HOST, I2C_MODE_MASTER, 0, 0, 0));

    //------------------------------安装面板IO------------------------------
    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = TEST_I2C_HW_ADDR,
        .control_phase_bytes = 1,
        .dc_bit_offset = (unsigned int)6,
        .lcd_cmd_bits = TEST_LCD_CMD_BITS,
        .lcd_param_bits = TEST_LCD_PARAM_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(I2C_HOST, &io_config, &io_handle));

    //------------------------------安装SSD1306面板驱动------------------------------
    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = TEST_PIN_NUM_RST,
        .bits_per_pixel = 1,
    };
    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = TEST_LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    //------------------------------初始化LVGL------------------------------
    ESP_LOGI(TAG, "Initialize LVGL");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    esp_err_t err = lvgl_port_init(&lvgl_cfg);

    //------------------------------添加屏幕------------------------------
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = TEST_LCD_H_RES * TEST_LCD_V_RES,
        .double_buffer = true,
        .hres = TEST_LCD_H_RES,
        .vres = TEST_LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };
    lv_disp_t* disp = lvgl_port_add_disp(&disp_cfg);
    lv_disp_set_rotation(disp, LV_DISP_ROT_180);

    //------------------------------初始化info ui------------------------------
    info_ui::info_ui_config_t info_ui_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .disp_width = TEST_LCD_V_RES,
        .disp_height = TEST_LCD_H_RES,
    };
    info_ui::info_ui ui(&info_ui_cfg, 35, 40, 37);
    ui.start();

    while (1) {
        ui.update();
    }

}
