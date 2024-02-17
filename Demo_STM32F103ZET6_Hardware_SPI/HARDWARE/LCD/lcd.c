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
#include "sys.h"
#include "stdlib.h"
#include "lcd.h"
#include "delay.h"
#include "SPI.h"

//定义LCD的尺寸
#define LCD_W 240
#define LCD_H 320


//管理LCD重要参数
//默认为竖屏
lcd_dev_t lcddev;

//画笔颜色,背景颜色
u16 PAINT_COLOR = BLACK, BACK_COLOR = WHITE;

//写寄存器函数
//reg:寄存器值
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //使能GPIOB时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; //GPIOB9,10,11,12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
}

void LCD_WriteReg(u8 reg, u16 value)
{
    LCD_SelectReg(reg);
    LCD_WriteByte(value);
}

//读数据
//返回值:读到的值
u16 LCD_ReadRamData(void)
{
    return 0;
}

//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x, u16 y)
{
    if (x >= lcddev.width || y >= lcddev.height)
    {
        return 0;    //超过了范围,直接返回
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
    SPI2_Init(); //硬件SPI2初始化
    LCD_GPIOInit();//LCD GPIO初始化
    LCD_RESET(); //LCD 复位
    
    //*************2.4inch ILI9341初始化**********//
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

    LCD_Direction(USE_HORIZONTAL);//设置LCD显示方向
    LCD_BL_ON(); //点亮背光
}

//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.
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

    LCD_WriteRAM_Prepare(); //开始写入GRAM
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


