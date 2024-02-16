//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103ZET6,����ԭ��ELITE STM32������,��Ƶ72MHZ������12MHZ
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567
//�ֻ�:15989313508���빤��
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/09
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��        DC5V/3.3V      //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI����
//     LCDģ��                STM32��Ƭ��
//    SDI(MOSI)      ��          PB15         //Һ����SPI��������д�ź�
//    SDO(MISO)      ��          PB14         //Һ����SPI�������ݶ��źţ��������Ҫ�������Բ�����
//=======================================Һ���������߽���==========================================//
//     LCDģ��                        STM32��Ƭ��
//       LED         ��          PB9          //Һ������������źţ��������Ҫ���ƣ���5V��3.3V
//       SCK         ��          PB13         //Һ����SPI����ʱ���ź�
//      DC/RS        ��          PB10         //Һ��������/��������ź�
//       RST         ��          PB12         //Һ������λ�����ź�
//       CS          ��          PB11         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//     LCDģ��                STM32��Ƭ��
//      T_IRQ        ��          PC10         //�����������ж��ź�
//      T_DO         ��          PC2          //������SPI���߶��ź�
//      T_DIN        ��          PC3          //������SPI����д�ź�
//      T_CS         ��          PC13         //������Ƭѡ�����ź�
//      T_CLK        ��          PC0          //������SPI����ʱ���ź�
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

void LCD_ShowChar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size);
void LCD_Show2Num(u16 x, u16 y, u16 num, u8 len, u8 size, u8 mode);
void LCD_ShowString(u16 x, u16 y, u8 size, u8 *p, u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode);
void LCD_ShowHzString(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode);
void LCD_ShowStringCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode);

u16 gui_alpha_blend565(u16 src, u16 dst, u8 alpha);                     //alpha blending
u16 gui_color_chg(u32 rgb);                                             //��ɫת��.��32λɫתΪ16λɫ
u16 gui_rgb332torgb565(u16 rgb332);                                     //��rgb332תΪrgb565
long gui_pow(u8 m, u8 n);                                           //�õ�m^n

u32 gui_disabs(u32 x1, u32 x2);                                         //��ľ���ֵ
void gui_alphablend_area(u16 x, u16 y, u16 width, u16 height, u16 color, u8 aval);
void gui_draw_bigpoint(u16 x0, u16 y0, u16 color);                      //�����
void gui_draw_line(u16 x0, u16 y0, u16 x1, u16 y1, u16 color);          //��������
void gui_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color); //��һ������
void gui_draw_bline1(u16 x0, u16 y0, u16 x1, u16 y1, u8 size, u16 color); //��һ������,����1
void gui_draw_rectangle(u16 x0, u16 y0, u16 width, u16 height, u16 color); //������
void gui_draw_arcrectangle(u16 x, u16 y, u16 width, u16 height, u8 r, u8 mode, u16 upcolor, u16 downcolor); //��Բ�Ǿ���
void gui_draw_vline(u16 x0, u16 y0, u16 len, u16 color);                            //����ֱ��
void gui_draw_hline(u16 x0, u16 y0, u16 len, u16 color);                            //��ˮƽ��
void gui_fill_colorblock(u16 x0, u16 y0, u16 width, u16 height, u16 *ctbl, u8 mode); //�����ɫ��
void gui_smooth_color(u32 srgb, u32 ergb, u16 *cbuf, u16 len);                      //��ɫƽ������
void gui_draw_smooth_rectangle(u16 x, u16 y, u16 width, u16 height, u32 srgb, u32 ergb); //����ɫƽ�����ɵľ���/����
void gui_fill_rectangle(u16 x0, u16 y0, u16 width, u16 height, u16 color);          //�������
void gui_fill_circle(u16 x0, u16 y0, u16 r, u16 color);
void gui_draw_ellipse(u16 x0, u16 y0, u16 rx, u16 ry, u16 color);
void gui_fill_ellipse(u16 x0, u16 y0, u16 rx, u16 ry, u16 color);
void gui_draw_argrec(u16 x0, u16 y0, u16 width, u16 height, u16 color);
void gui_show_strmid(u16 x, u16 y, u16 width, u16 height, u16 color, u8 size, u8 *str);
void gui_show_strmidex(u16 x, u16 y, u16 width, u16 height, u16 color, u8 size, u8 *str, u8 mode);
void gui_show_ptchar(u16 x, u16 y, u16 xend, u16 yend, u16 offset, u16 color, u16 size, u8 chr, u8 mode);
void gui_show_ptfont(u16 x, u16 y, u16 xend, u16 yend, u16 offset, u16 color, u16 size, u8 *chr, u8 mode);
void gui_show_ptstr(u16 x, u16 y, u16 xend, u16 yend, u16 offset, u16 color, u8 size, u8 *str, u8 mode);
void gui_show_ptstrwhiterim(u16 x, u16 y, u16 xend, u16 yend, u16 offset, u16 color, u16 rimcolor, u8 size, u8 *str); //��Ч��ʾ�ַ���
void gui_draw_icos(u16 x, u16 y, u8 size, u8 index);
void gui_draw_icosalpha(u16 x, u16 y, u8 size, u8 index);
void gui_show_num(u16 x, u16 y, u8 len, u16 color, u8 size, long num, u8 mode);
u8 *gui_num2str(u8 *str, u32 num);
void gui_num2strex(int n, char *str);
void gui_draw_arc(u16 sx, u16 sy, u16 ex, u16 ey, u16 rx, u16 ry, u16 r, u16 color, u8 mode);

u32 gui_get_stringline(u8 *str, u16 linelenth, u8 font);
void gui_show_string(u8 *str, u16 x, u16 y, u16 width, u16 height, u8 font, u16 fcolor);
void gui_show_stringex(u16 x, u16 y, u16 width, u16 height, u16 fcolor, u8 font, u8 *str, u8 mode);
u16 gui_string_forwardgbk_count(u8 *str, u16 pos);


#endif

