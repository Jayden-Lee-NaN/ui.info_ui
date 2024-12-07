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

#ifndef LV_ATTRIBUTE_IMG_ICON_MESSAGE
#define LV_ATTRIBUTE_IMG_ICON_MESSAGE
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_ICON_MESSAGE uint8_t icon_message_map[] = {
  0xf9, 0xf9, 0xf9, 0xff, 	/*Color of index 0*/
  0x72, 0x72, 0x72, 0xff, 	/*Color of index 1*/

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0xff, 0xff, 0xff, 0xc0, 0x00, 
  0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x00, 
  0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x00, 
  0x00, 0xe1, 0x55, 0x55, 0x54, 0x0e, 0x00, 
  0x00, 0xe3, 0xaa, 0xaa, 0xaa, 0x03, 0x00, 
  0x01, 0xcc, 0x00, 0x00, 0x00, 0x03, 0x00, 
  0x01, 0x98, 0x00, 0x00, 0x00, 0x01, 0x80, 
  0x01, 0x90, 0x00, 0x00, 0x00, 0x01, 0x80, 
  0x01, 0xa0, 0x00, 0x00, 0x00, 0x01, 0x80, 
  0x01, 0xa0, 0x00, 0x00, 0x00, 0x01, 0x80, 
  0x01, 0xa0, 0x00, 0x3c, 0x00, 0x01, 0x80, 
  0x03, 0xa0, 0x00, 0x42, 0x00, 0x01, 0x80, 
  0x01, 0xa0, 0x00, 0x42, 0x00, 0x01, 0x80, 
  0x03, 0xa0, 0x00, 0x42, 0x00, 0x05, 0x80, 
  0x01, 0xa0, 0x00, 0x36, 0x00, 0x01, 0x80, 
  0x03, 0xa0, 0x00, 0x08, 0x00, 0x05, 0x80, 
  0x01, 0xa0, 0x00, 0x00, 0x00, 0x09, 0x80, 
  0x03, 0xa0, 0x01, 0xfe, 0x00, 0x05, 0x80, 
  0x01, 0xa0, 0x01, 0x02, 0x00, 0x01, 0x80, 
  0x03, 0xa0, 0x01, 0xc2, 0x00, 0x01, 0x80, 
  0x01, 0xa0, 0x00, 0x42, 0x00, 0x05, 0x80, 
  0x03, 0xa0, 0x00, 0x22, 0x00, 0x05, 0x80, 
  0x01, 0xa0, 0x00, 0x42, 0x00, 0x09, 0x80, 
  0x03, 0xa0, 0x00, 0x22, 0x00, 0x05, 0x80, 
  0x01, 0xa0, 0x00, 0x42, 0x00, 0x09, 0x80, 
  0x03, 0xa0, 0x00, 0x22, 0x00, 0x05, 0x80, 
  0x01, 0xa0, 0x00, 0x42, 0x00, 0x09, 0x80, 
  0x03, 0xa0, 0x00, 0x22, 0x00, 0x05, 0x80, 
  0x01, 0xa0, 0x00, 0x42, 0x00, 0x09, 0x80, 
  0x03, 0xa0, 0x00, 0x62, 0x00, 0x05, 0x80, 
  0x01, 0xa0, 0x01, 0xc3, 0x80, 0x09, 0x80, 
  0x03, 0xa0, 0x01, 0x80, 0xc0, 0x05, 0x80, 
  0x01, 0xa0, 0x01, 0xff, 0x80, 0x09, 0x80, 
  0x01, 0xa0, 0x00, 0x00, 0x00, 0x05, 0x80, 
  0x01, 0x90, 0x00, 0x00, 0x00, 0x09, 0x80, 
  0x01, 0x90, 0x00, 0x00, 0x00, 0x19, 0x80, 
  0x01, 0x88, 0x00, 0x00, 0x00, 0x23, 0x80, 
  0x00, 0xc6, 0x00, 0x00, 0x00, 0x63, 0x00, 
  0x00, 0xe3, 0xff, 0xff, 0xff, 0x87, 0x00, 
  0x00, 0x70, 0x00, 0x00, 0x00, 0x0e, 0x00, 
  0x00, 0x3d, 0x55, 0x55, 0x55, 0x7c, 0x00, 
  0x00, 0x1f, 0xff, 0xff, 0xff, 0xf0, 0x00, 
  0x00, 0x02, 0xaa, 0xaa, 0xaa, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const lv_img_dsc_t icon_message = {
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 56,
  .header.h = 56,
  .data_size = 400,
  .data = icon_message_map,
};
