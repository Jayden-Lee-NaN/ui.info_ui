#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_ICON_CLOCK
#define LV_ATTRIBUTE_IMG_ICON_CLOCK
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_ICON_CLOCK uint8_t icon_clock_map[] = {
  0xf8, 0xf8, 0xf8, 0xff, 	/*Color of index 0*/
  0x28, 0x28, 0x28, 0xff, 	/*Color of index 1*/

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xeb, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x0f, 0x00, 0x78, 0x00, 0x00, 
  0x00, 0x00, 0x1c, 0x3e, 0x1c, 0x00, 0x00, 
  0x00, 0x00, 0x19, 0xff, 0xcc, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xc1, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0x00, 0x70, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0x7f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xc1, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0xc0, 0x00, 0x01, 0x80, 0x00, 
  0x00, 0x01, 0xc0, 0x00, 0x01, 0xc0, 0x00, 
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x3a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xae, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x31, 0xfe, 0x7f, 0x80, 0xff, 0x3f, 0xc6, 
  0x31, 0xfe, 0x7f, 0x80, 0xff, 0x3f, 0xc6, 
  0x31, 0x86, 0x61, 0x88, 0x03, 0x30, 0xc6, 
  0x31, 0x86, 0x61, 0x8c, 0x03, 0x30, 0xc6, 
  0x31, 0x86, 0x61, 0x88, 0x03, 0x30, 0xc6, 
  0x31, 0x86, 0x01, 0x80, 0x2f, 0x30, 0xc6, 
  0x31, 0x86, 0x01, 0x80, 0x3f, 0x30, 0xc6, 
  0x31, 0x86, 0x01, 0x80, 0x03, 0x30, 0xc6, 
  0x31, 0x86, 0x01, 0x88, 0x03, 0x30, 0xc6, 
  0x31, 0x86, 0x01, 0x8c, 0x03, 0x30, 0xc6, 
  0x31, 0x86, 0x01, 0x88, 0x03, 0x30, 0xc6, 
  0x31, 0xfe, 0x01, 0x80, 0xff, 0x3f, 0xc6, 
  0x30, 0xfa, 0x01, 0x00, 0x7d, 0x1f, 0x86, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x06, 0x0c, 0x00, 0x00, 0x00, 0x18, 0x30, 
  0x06, 0xac, 0x00, 0x00, 0x00, 0x1a, 0x70, 
  0x07, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xf0, 
  0x02, 0xa8, 0x00, 0x00, 0x00, 0x15, 0x40, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const lv_img_dsc_t icon_clock = {
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 56,
  .header.h = 56,
  .data_size = 400,
  .data = icon_clock_map,
};
