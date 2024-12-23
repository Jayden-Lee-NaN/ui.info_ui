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

#ifndef LV_ATTRIBUTE_IMG_PICTURE_P10
#define LV_ATTRIBUTE_IMG_PICTURE_P10
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_PICTURE_P10 uint8_t picture_p10_map[] = {
  0xfc, 0xfc, 0xfc, 0xff, 	/*Color of index 0*/
  0x17, 0x17, 0x17, 0xff, 	/*Color of index 1*/

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x80, 0x07, 0xe0, 0x3e, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xc0, 0x1f, 0xf8, 0x7f, 0x06, 0x00, 0x00, 
  0x00, 0x00, 0x3f, 0x80, 0x3e, 0x78, 0xe3, 0x0e, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xc0, 0x38, 0x1c, 0xe3, 0x1c, 0x00, 0x00, 
  0x00, 0x00, 0xf3, 0x80, 0x38, 0x1c, 0xe3, 0x18, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xc0, 0x70, 0x0e, 0xe3, 0x38, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x80, 0x73, 0x8e, 0xe7, 0x70, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xc0, 0x73, 0xce, 0x7e, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x80, 0x73, 0xce, 0x3c, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xc0, 0x73, 0xce, 0x01, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x80, 0x70, 0x0e, 0x03, 0xbf, 0x80, 0x00, 
  0x00, 0x00, 0x03, 0xc0, 0x70, 0x0e, 0x03, 0x73, 0x80, 0x00, 
  0x00, 0x00, 0x03, 0x80, 0x70, 0x1e, 0x07, 0x71, 0x80, 0x00, 
  0x00, 0x00, 0x03, 0xc0, 0x38, 0x1c, 0x0e, 0x71, 0x80, 0x00, 
  0x00, 0x00, 0x03, 0x80, 0x38, 0x1c, 0x1c, 0x71, 0x80, 0x00, 
  0x00, 0x00, 0x03, 0xc0, 0x3e, 0x78, 0x18, 0x73, 0x80, 0x00, 
  0x00, 0x00, 0xff, 0xfc, 0x1f, 0xf8, 0x38, 0x3f, 0x80, 0x00, 
  0x00, 0x00, 0xff, 0xfc, 0x07, 0xe0, 0x70, 0x1f, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const lv_img_dsc_t picture_p10 = {
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 80,
  .header.h = 30,
  .data_size = 308,
  .data = picture_p10_map,
};
