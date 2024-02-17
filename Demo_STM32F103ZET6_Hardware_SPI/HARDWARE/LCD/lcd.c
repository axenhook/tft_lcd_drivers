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
#include "sys.h"
#include "stdlib.h"
#include "lcd.h"
#include "delay.h"
#include "SPI.h"

//����LCD�ĳߴ�
#define LCD_W 240
#define LCD_H 320


//����LCD��Ҫ����
//Ĭ��Ϊ����
lcd_dev_t lcddev;

//������ɫ,������ɫ
u16 PAINT_COLOR = BLACK, BACK_COLOR = WHITE;

//д�Ĵ�������
//reg:�Ĵ���ֵ
void LCD_SelectReg(u8 reg)
{
    LCD_CS_CLR();
    LCD_RS_CLR();
    SPI_WriteByte(SPI2, reg);
    LCD_CS_SET();
}

void LCD_WriteByte(u8 data)
{
    LCD_CS_CLR();
    LCD_RS_SET();
    SPI_WriteByte(SPI2, data);
    LCD_CS_SET();
}

void LCD_WriteWord(u16 color)
{
    LCD_CS_CLR();
    LCD_RS_SET();
    SPI_WriteByte(SPI2, color >> 8);
    SPI_WriteByte(SPI2, color);
    LCD_CS_SET();
}

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_GPIOInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //ʹ��GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; //GPIOB9,10,11,12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
}

void LCD_WriteReg(u8 reg, u16 value)
{
    LCD_SelectReg(reg);
    LCD_WriteByte(value);
}

//������
//����ֵ:������ֵ
u16 LCD_ReadRamData(void)
{
    return 0;
}

//��ȡ��ĳ�����ɫֵ
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x, u16 y)
{
    if (x >= lcddev.width || y >= lcddev.height)
    {
        return 0;    //�����˷�Χ,ֱ�ӷ���
    }

    LCD_SetWindow(x, y, 1, 1);

    return LCD_ReadRamData();
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
    LCD_SelectReg(0x2C);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_RESET(void)
{
    LCD_RST_CLR();
    delay_ms(100);
    LCD_RST_SET();
    delay_ms(50);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_Init(void)
{
    SPI2_Init(); //Ӳ��SPI2��ʼ��
    LCD_GPIOInit();//LCD GPIO��ʼ��
    LCD_RESET(); //LCD ��λ
    
    //*************2.4inch ILI9341��ʼ��**********//
    LCD_SelectReg(0xCF);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0xD9); //0xC1
    LCD_WriteByte(0X30);
    LCD_SelectReg(0xED);
    LCD_WriteByte(0x64);
    LCD_WriteByte(0x03);
    LCD_WriteByte(0X12);
    LCD_WriteByte(0X81);
    LCD_SelectReg(0xE8);
    LCD_WriteByte(0x85);
    LCD_WriteByte(0x10);
    LCD_WriteByte(0x7A);
    LCD_SelectReg(0xCB);
    LCD_WriteByte(0x39);
    LCD_WriteByte(0x2C);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x34);
    LCD_WriteByte(0x02);
    LCD_SelectReg(0xF7);
    LCD_WriteByte(0x20);
    LCD_SelectReg(0xEA);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x00);
    LCD_SelectReg(0xC0);    //Power control
    LCD_WriteByte(0x1B);   //VRH[5:0]
    LCD_SelectReg(0xC1);    //Power control
    LCD_WriteByte(0x12);   //SAP[2:0];BT[3:0] 0x01
    LCD_SelectReg(0xC5);    //VCM control
    LCD_WriteByte(0x08);     //30
    LCD_WriteByte(0x26);     //30
    LCD_SelectReg(0xC7);    //VCM control2
    LCD_WriteByte(0XB7);
    LCD_SelectReg(0x36);    // Memory Access Control
    LCD_WriteByte(0x08);
    LCD_SelectReg(0x3A);
    LCD_WriteByte(0x55);
    LCD_SelectReg(0xB1);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x1A);
    LCD_SelectReg(0xB6);    // Display Function Control
    LCD_WriteByte(0x0A);
    LCD_WriteByte(0xA2);
    LCD_SelectReg(0xF2);    // 3Gamma Function Disable
    LCD_WriteByte(0x00);
    LCD_SelectReg(0x26);    //Gamma curve selected
    LCD_WriteByte(0x01);
    LCD_SelectReg(0xE0);    //Set Gamma
    LCD_WriteByte(0x0F);
    LCD_WriteByte(0x1D);
    LCD_WriteByte(0x1A);
    LCD_WriteByte(0x0A);
    LCD_WriteByte(0x0D);
    LCD_WriteByte(0x07);
    LCD_WriteByte(0x49);
    LCD_WriteByte(0X66);
    LCD_WriteByte(0x3B);
    LCD_WriteByte(0x07);
    LCD_WriteByte(0x11);
    LCD_WriteByte(0x01);
    LCD_WriteByte(0x09);
    LCD_WriteByte(0x05);
    LCD_WriteByte(0x04);
    LCD_SelectReg(0XE1);    //Set Gamma
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x18);
    LCD_WriteByte(0x1D);
    LCD_WriteByte(0x02);
    LCD_WriteByte(0x0F);
    LCD_WriteByte(0x04);
    LCD_WriteByte(0x36);
    LCD_WriteByte(0x13);
    LCD_WriteByte(0x4C);
    LCD_WriteByte(0x07);
    LCD_WriteByte(0x13);
    LCD_WriteByte(0x0F);
    LCD_WriteByte(0x2E);
    LCD_WriteByte(0x2F);
    LCD_WriteByte(0x05);
    LCD_SelectReg(0x2B);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x01);
    LCD_WriteByte(0x3f);
    LCD_SelectReg(0x2A);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0xef);
    LCD_SelectReg(0x11); //Exit Sleep
    delay_ms(120);
    LCD_SelectReg(0x29); //display on

    LCD_Direction(USE_HORIZONTAL);//����LCD��ʾ����
    LCD_BL_ON(); //��������
}

//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height.
void LCD_SetWindow(u16 sx, u16 sy, u16 width, u16 height)
{
    LCD_SelectReg(0x2A);
    LCD_WriteByte(sx >> 8);
    LCD_WriteByte(0x00FF & sx);
    LCD_WriteByte((sx + width - 1) >> 8);
    LCD_WriteByte(0x00FF & (sx + width - 1));

    LCD_SelectReg(0x2B);
    LCD_WriteByte(sy >> 8);
    LCD_WriteByte(0x00FF & sy);
    LCD_WriteByte((sy + height - 1) >> 8);
    LCD_WriteByte(0x00FF & (sy + height - 1));

    LCD_WriteRAM_Prepare(); //��ʼд��GRAM
}

/*****************************************************************************
 * @name       :void LCD_Direction(u8 direction)
 * @date       :2018-08-09
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
                                                    2-180 degree
                                                    3-270 degree
 * @retvalue   :None
******************************************************************************/
void LCD_Direction(u8 direction)
{
    switch (direction)
    {
        case 0:
            lcddev.width = LCD_W;
            lcddev.height = LCD_H;
            lcddev.direction = direction;
            LCD_WriteReg(0x36, (1 << 3) | (0 << 6) | (0 << 7)); //BGR==1,MY==0,MX==0,MV==0
            break;
        case 1:
            lcddev.width = LCD_H;
            lcddev.height = LCD_W;
            lcddev.direction = direction;
            LCD_WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
            break;
        case 2:
            lcddev.width = LCD_W;
            lcddev.height = LCD_H;
            lcddev.direction = direction;
            LCD_WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 7)); //BGR==1,MY==0,MX==0,MV==0
            break;
        case 3:
            lcddev.width = LCD_H;
            lcddev.height = LCD_W;
            lcddev.direction = direction;
            LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
            break;
        default:
            break;
    }
}


