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
#ifndef __LCD_H__
#define __LCD_H__

#include "sys.h"

//LCD��Ҫ������
typedef struct
{
    u16 width;          //LCD ���
    u16 height;         //LCD �߶�
    u16 id;               //LCD ID
    u8  direction;              //���������������ƣ�0��������1��������
} lcd_dev_t;

//LCD����
extern lcd_dev_t lcddev; //����LCD��Ҫ����

/////////////////////////////////////�û�������///////////////////////////////////
#define USE_HORIZONTAL       0//����Һ����˳ʱ����ת����    0-0����ת��1-90����ת��2-180����ת��3-270����ת

//////////////////////////////////////////////////////////////////////////////////
//TFTLCD������Ҫ���õĺ���
extern u16  PAINT_COLOR;//Ĭ�Ϻ�ɫ
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���----------------
#define LED      9        //�����������        PB9
#define LCD_CS   11       //Ƭѡ����            PB11
#define LCD_RS   10       //�Ĵ���/����ѡ������ PB10 
#define LCD_RST  12       //��λ����            PB12


//QDtechȫϵ��ģ������������ܿ��Ʊ��������û�Ҳ���Խ�PWM���ڱ�������
#define LCD_BL_ON()      PBout(LED) = 1 //LCD����             PB9
#define LCD_BL_OFF()     PBout(LED) = 0 //LCD����             PB9
//���ʹ�ùٷ��⺯���������еײ㣬�ٶȽ����½���14֡ÿ�룬���������˾�Ƽ�����
//����IO����ֱ�Ӳ����Ĵ���������IO������ˢ�����ʿ��Դﵽ28֡ÿ�룡

//GPIO��λ�����ߣ�
#define LCD_CS_SET()    GPIOB->BSRR=1<<LCD_CS    //Ƭѡ�˿�   PB11
#define LCD_RS_SET()    GPIOB->BSRR=1<<LCD_RS    //����/����  PB10    
#define LCD_RST_SET()   GPIOB->BSRR=1<<LCD_RST   //��λ           PB12

//GPIO��λ�����ͣ�
#define LCD_CS_CLR()    GPIOB->BRR=1<<LCD_CS     //Ƭѡ�˿�   PB11
#define LCD_RS_CLR()    GPIOB->BRR=1<<LCD_RS     //����/����  PB10   
#define LCD_RST_CLR()   GPIOB->BRR=1<<LCD_RST    //��λ           PB12

//������ɫ
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN       0XBC40 //��ɫ
#define BRRED       0XFC07 //�غ�ɫ
#define GRAY        0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE    0X01CF //����ɫ
#define LIGHTBLUE   0X7D7C //ǳ��ɫ  
#define GRAYBLUE    0X5458 //����ɫ
//������ɫΪPANEL����ɫ

#define LIGHTGREEN  0X841F //ǳ��ɫ
#define LIGHTGRAY   0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY       0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE   0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE      0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
u16  LCD_ReadPoint(u16 x, u16 y); //����
u16 LCD_ReadReg(u8 reg);
void LCD_SetWindow(u16 sx, u16 sy, u16 width, u16 height);
void LCD_WriteReg(u8 reg, u16 value);
void LCD_WriteByte(u8 data);
void LCD_WriteWord(u16 Data);
void LCD_WriteRAM_Prepare(void);
void LCD_Direction(u8 direction);

#endif





