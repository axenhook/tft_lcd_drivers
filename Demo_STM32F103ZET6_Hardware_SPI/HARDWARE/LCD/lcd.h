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
#ifndef __LCD_H__
#define __LCD_H__

#include "sys.h"

//LCD重要参数集
typedef struct
{
    u16 width;          //LCD 宽度
    u16 height;         //LCD 高度
    u16 id;               //LCD ID
    u8  direction;              //横屏还是竖屏控制：0，竖屏；1，横屏。
} lcd_dev_t;

//LCD参数
extern lcd_dev_t lcddev; //管理LCD重要参数

/////////////////////////////////////用户配置区///////////////////////////////////
#define USE_HORIZONTAL       0//定义液晶屏顺时针旋转方向    0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////
//TFTLCD部分外要调用的函数
extern u16  PAINT_COLOR;//默认红色
extern u16  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义----------------
#define LED      9        //背光控制引脚        PB9
#define LCD_CS   11       //片选引脚            PB11
#define LCD_RS   10       //寄存器/数据选择引脚 PB10 
#define LCD_RST  12       //复位引脚            PB12


//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
#define LCD_BL_ON()      PBout(LED) = 1 //LCD背光             PB9
#define LCD_BL_OFF()     PBout(LED) = 0 //LCD背光             PB9
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！

//GPIO置位（拉高）
#define LCD_CS_SET()    GPIOB->BSRR=1<<LCD_CS    //片选端口   PB11
#define LCD_RS_SET()    GPIOB->BSRR=1<<LCD_RS    //数据/命令  PB10    
#define LCD_RST_SET()   GPIOB->BSRR=1<<LCD_RST   //复位           PB12

//GPIO复位（拉低）
#define LCD_CS_CLR()    GPIOB->BRR=1<<LCD_CS     //片选端口   PB11
#define LCD_RS_CLR()    GPIOB->BRR=1<<LCD_RS     //数据/命令  PB10   
#define LCD_RST_CLR()   GPIOB->BRR=1<<LCD_RST    //复位           PB12

//画笔颜色
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
#define BROWN       0XBC40 //棕色
#define BRRED       0XFC07 //棕红色
#define GRAY        0X8430 //灰色
//GUI颜色

#define DARKBLUE    0X01CF //深蓝色
#define LIGHTBLUE   0X7D7C //浅蓝色  
#define GRAYBLUE    0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN  0X841F //浅绿色
#define LIGHTGRAY   0XEF5B //浅灰色(PANNEL)
#define LGRAY       0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE   0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE      0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
u16  LCD_ReadPoint(u16 x, u16 y); //读点
u16 LCD_ReadReg(u8 reg);
void LCD_SetWindow(u16 sx, u16 sy, u16 width, u16 height);
void LCD_WriteReg(u8 reg, u16 value);
void LCD_WriteByte(u8 data);
void LCD_WriteWord(u16 Data);
void LCD_WriteRAM_Prepare(void);
void LCD_Direction(u8 direction);

#endif





