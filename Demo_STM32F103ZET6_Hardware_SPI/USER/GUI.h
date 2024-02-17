//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103ZET6,正点原子ELITE STM32开发板,主频72MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567
//手机:15989313508（冯工）
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机
//    SDI(MOSI)      接          PB15         //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PB14         //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块                        STM32单片机
//       LED         接          PB9          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB13         //液晶屏SPI总线时钟信号
//      DC/RS        接          PB10         //液晶屏数据/命令控制信号
//       RST         接          PB12         //液晶屏复位控制信号
//       CS          接          PB11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//     LCD模块                STM32单片机
//      T_IRQ        接          PC10         //触摸屏触摸中断信号
//      T_DO         接          PC2          //触摸屏SPI总线读信号
//      T_DIN        接          PC3          //触摸屏SPI总线写信号
//      T_CS         接          PC13         //触摸屏片选控制信号
//      T_CLK        接          PC0          //触摸屏SPI总线时钟信号
**************************************************************************************************/
/* @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/
#ifndef __GUI_H__
#define __GUI_H__

void gui_show_ptchar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode);
void gui_show_num(u16 x, u16 y, u32 num, u8 len, u8 size);
void gui_show_ptstring(u16 x, u16 y, u8 size, u8 *p, u8 mode);
void gui_draw_font16(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
void gui_draw_font24(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
void gui_draw_font32(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
void gui_show_string(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode);
void gui_show_string_center(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode);
void gui_fill_rectangle_color(u16 x0, u16 y0, u16 width, u16 height, u16 color);          //填充区域
void gui_fill_rectangle(u16 x0, u16 y0, u16 width, u16 height);
void gui_draw_rectangle(u16 x0, u16 y0, u16 width, u16 height);
u16 gui_alpha_blend565(u16 src, u16 dst, u8 alpha);                     //alpha blending
u16 gui_color_chg(u32 rgb);                                             //颜色转换.将32位色转为16位色
u16 gui_rgb332torgb565(u16 rgb332);                                     //将rgb332转为rgb565
void gui_alphablend_area(u16 x, u16 y, u16 width, u16 height, u16 color, u8 aval);
void gui_draw_bigpoint(u16 x0, u16 y0, u16 color);                      //画大点
void gui_draw_line_color(u16 x0, u16 y0, u16 x1, u16 y1, u16 color);          //画任意线
void gui_draw_line(u16 x1, u16 y1, u16 x2, u16 y2);
void gui_draw_arcrectangle(u16 x, u16 y, u16 width, u16 height, u8 r, u8 mode, u16 upcolor, u16 downcolor); //画圆角矩形
void gui_draw_vline(u16 x0, u16 y0, u16 len, u16 color);                            //画垂直线
void gui_draw_hline(u16 x0, u16 y0, u16 len, u16 color);                            //画水平线
void gui_smooth_color(u32 srgb, u32 ergb, u16 *cbuf, u16 len);                      //颜色平滑过渡
void gui_draw_smooth_rectangle(u16 x, u16 y, u16 width, u16 height, u32 srgb, u32 ergb); //画颜色平滑过渡的矩形/线条
void gui_draw_ellipse(u16 x0, u16 y0, u16 rx, u16 ry, u16 color);
void gui_fill_ellipse(u16 x0, u16 y0, u16 rx, u16 ry, u16 color);
void gui_draw_argrec(u16 x0, u16 y0, u16 width, u16 height, u16 color);
void gui_show_ptfont(u16 x, u16 y, u16 xend, u16 yend, u16 offset, u16 color, u16 size, u8 *chr, u8 mode);
void gui_draw_arc(u16 sx, u16 sy, u16 ex, u16 ey, u16 rx, u16 ry, u16 r, u16 color, u8 mode);
void gui_draw_point_color(u16 x, u16 y, u16 color);
void gui_fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);
void gui_draw_line(u16 x1, u16 y1, u16 x2, u16 y2);
void gui_draw_triangle(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2);
void gui_fill_triangle(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2);
void gui_draw_circle(int xc, int yc, u16 c, int r, int fill);
void gui_draw_image(u16 x, u16 y, u16 width, u16 height, const u8 *pic);
void gui_clear(u16 Color);
void gui_draw_point(u16 x, u16 y); //画点


#endif

