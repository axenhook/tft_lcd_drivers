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
#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "key.h"
#include "led.h"
#include "pic.h"

//========================variable==========================//
u16 ColorTab[5] = {RED, GREEN, BLUE, YELLOW, BRED}; //定义颜色数组
//=====================end of variable======================//

/*****************************************************************************
 * @name       :void DrawTestPage(u8 *str)
 * @date       :2018-08-09
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/
void DrawTestPage(u8 *str)
{
    //绘制固定栏up
    LCD_Clear(WHITE);
    gui_fill(0, 0, lcddev.width, 20, BLUE);
    //绘制固定栏down
    gui_fill(0, lcddev.height - 20, lcddev.width, lcddev.height, BLUE);
    PAINT_COLOR = WHITE;
    gui_show_string_center(0, 2, WHITE, BLUE, str, 16, 1); //居中显示
    gui_show_string_center(0, lcddev.height - 18, WHITE, BLUE, "http://www.lcdwiki.com", 16, 1); //居中显示
    //绘制测试区域
    //gui_fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

/*****************************************************************************
 * @name       :void main_test(void)
 * @date       :2018-08-09
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void main_test(void)
{
    DrawTestPage("全动电子综合测试程序");
    gui_show_string_center(0, 30, RED, BLUE, "全动电子", 16, 1); //居中显示
    gui_show_string_center(0, 60, RED, BLUE, "综合测试程序", 16, 1); //居中显示
    gui_show_string_center(0, 90, GREEN, BLUE, "2.4\" ILI9341 240X320", 16, 1); //居中显示
    gui_show_string_center(0, 120, BLUE, BLUE, "xiaoFeng@QDtech 2018-08-20", 16, 1); //居中显示
    delay_ms(1500);
    delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-09
 * @function   :Color fill test(white,black,red,green,blue)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
    //DrawTestPage("测试1:纯色填充测试");
    gui_fill(0, 0, lcddev.width, lcddev.height, WHITE);
    gui_show_string(20, 30, BLUE, YELLOW, "BL Test", 16, 1);
    delay_ms(800);
    gui_fill(0, 0, lcddev.width, lcddev.height, RED);
    gui_show_string(20, 30, BLUE, YELLOW, "RED ", 16, 1);
    delay_ms(800);
    gui_fill(0, 0, lcddev.width, lcddev.height, GREEN);
    gui_show_string(20, 30, BLUE, YELLOW, "GREEN ", 16, 1);
    delay_ms(800);
    gui_fill(0, 0, lcddev.width, lcddev.height, BLUE);
    gui_show_string(20, 30, RED, YELLOW, "BLUE ", 16, 1);
    delay_ms(800);
}

/*****************************************************************************
 * @name       :void Test_FillRec(void)
 * @date       :2018-08-09
 * @function   :Rectangular display and fill test
                                Display red,green,blue,yellow,pink rectangular boxes in turn,
                                1500 milliseconds later,
                                Fill the rectangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_FillRec(void)
{
    u8 i = 0;
    DrawTestPage("测试2:GUI矩形填充测试");
    gui_fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++)
    {
        PAINT_COLOR = ColorTab[i];
        gui_draw_rectangle(lcddev.width / 2 - 80 + (i * 15), lcddev.height / 2 - 80 + (i * 15), 60, 60);
    }
    delay_ms(1500);
    gui_fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++)
    {
        PAINT_COLOR = ColorTab[i];
        gui_fill_rectangle(lcddev.width / 2 - 80 + (i * 15), lcddev.height / 2 - 80 + (i * 15), 60, 60);
    }
    delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-09
 * @function   :circular display and fill test
                                Display red,green,blue,yellow,pink circular boxes in turn,
                                1500 milliseconds later,
                                Fill the circular in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
    u8 i = 0;
    DrawTestPage("测试3:GUI画圆填充测试");
    gui_fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++)
    {
        gui_draw_circle(lcddev.width / 2 - 80 + (i * 25), lcddev.height / 2 - 50 + (i * 25), ColorTab[i], 30, 0);
    }
    delay_ms(1500);
    gui_fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++)
    {
        gui_draw_circle(lcddev.width / 2 - 80 + (i * 25), lcddev.height / 2 - 50 + (i * 25), ColorTab[i], 30, 1);
    }
    delay_ms(1500);
}

/*****************************************************************************
 * @name       :void English_Font_test(void)
 * @date       :2018-08-09
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void English_Font_test(void)
{
    DrawTestPage("测试5:英文显示测试");
    gui_show_string(10, 30, BLUE, YELLOW, "6X12:abcdefghijklmnopqrstuvwxyz0123456789", 12, 0);
    gui_show_string(10, 45, BLUE, YELLOW, "6X12:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 12, 1);
    gui_show_string(10, 60, BLUE, YELLOW, "6X12:~!@#$%^&*()_+{}:<>?/|-+.", 12, 0);
    gui_show_string(10, 80, BLUE, YELLOW, "8X16:abcdefghijklmnopqrstuvwxyz0123456789", 16, 0);
    gui_show_string(10, 100, BLUE, YELLOW, "8X16:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 16, 1);
    gui_show_string(10, 120, BLUE, YELLOW, "8X16:~!@#$%^&*()_+{}:<>?/|-+.", 16, 0);
    delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-09
 * @function   :triangle display and fill test
                                Display red,green,blue,yellow,pink triangle boxes in turn,
                                1500 milliseconds later,
                                Fill the triangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
    u8 i = 0;
    DrawTestPage("测试4:GUI Triangle填充测试");
    gui_fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++)
    {
        PAINT_COLOR = ColorTab[i];
        gui_draw_triangle(lcddev.width / 2 - 80 + (i * 20), lcddev.height / 2 - 20 + (i * 15), lcddev.width / 2 - 50 - 1 + (i * 20), lcddev.height / 2 - 20 - 52 - 1 + (i * 15), lcddev.width / 2 - 20 - 1 + (i * 20), lcddev.height / 2 - 20 + (i * 15));
    }
    delay_ms(1500);
    gui_fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++)
    {
        PAINT_COLOR = ColorTab[i];
        gui_fill_triangle(lcddev.width / 2 - 80 + (i * 20), lcddev.height / 2 - 20 + (i * 15), lcddev.width / 2 - 50 - 1 + (i * 20), lcddev.height / 2 - 20 - 52 - 1 + (i * 15), lcddev.width / 2 - 20 - 1 + (i * 20), lcddev.height / 2 - 20 + (i * 15));
    }
    delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Chinese_Font_test(void)
 * @date       :2018-08-09
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Chinese_Font_test(void)
{
    DrawTestPage("测试6:中文显示测试");
    gui_show_string(10, 30, BLUE, YELLOW, "16X16:全动电子技术有限公司欢迎您", 16, 0);
    gui_show_string(10, 50, BLUE, YELLOW, "16X16:Welcome全动电子", 16, 0);
    gui_show_string(10, 70, BLUE, YELLOW, "24X24:深圳市中文测试", 24, 1);
    gui_show_string(10, 100, BLUE, YELLOW, "32X32:字体测试", 32, 1);
    delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Pic_test(void)
 * @date       :2018-08-09
 * @function   :picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Pic_test(void)
{
    DrawTestPage("测试7:图片显示测试");
    //gui_fill(0,20,lcddev.width,lcddev.height-20,WHITE);
    LCD_ShowPicture(30, 30, 40, 40, gImage_qq);
    gui_show_string(30 + 12, 75, BLUE, YELLOW, "QQ", 16, 1);
    LCD_ShowPicture(90, 30, 40, 40, gImage_qq);
    gui_show_string(90 + 12, 75, BLUE, YELLOW, "QQ", 16, 1);
    LCD_ShowPicture(150, 30, 40, 40, gImage_qq);
    gui_show_string(150 + 12, 75, BLUE, YELLOW, "QQ", 16, 1);
    delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Rotate_Test(void)
{
    u8 i = 0;
    u8 *Direction[4] = {"Rotation:0", "Rotation:90", "Rotation:180", "Rotation:270"};

    for (i = 0; i < 4; i++)
    {
        LCD_Direction(i);
        DrawTestPage("测试8:屏幕旋转测试");
        gui_show_string(20, 30, BLUE, YELLOW, Direction[i], 16, 1);
        LCD_ShowPicture(30, 50, 40, 40, gImage_qq);
        delay_ms(1000);
        delay_ms(1000);
    }
    LCD_Direction(USE_HORIZONTAL);
}

/*****************************************************************************
 * @name       :void Touch_Test(void)
 * @date       :2018-08-09
 * @function   :touch test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Touch_Test(void)
{
    u8 key;
    u16 i = 0;
    u16 j = 0;
    u16 colorTemp = 0;
    TP_Init();
    KEY_Init();
    LED_Init();
    DrawTestPage("测试9:Touch(按KEY0校准)      ");
    gui_show_ptstring(lcddev.width - 24, 0, 16, "RST", 1); //显示清屏区域
    PAINT_COLOR = RED;
    gui_fill(lcddev.width - 52, 2, lcddev.width - 50 + 20, 18, RED);
    while (1)
    {
        key = KEY_Scan();
        tp_dev.scan(0);
        if (tp_dev.sta & TP_PRES_DOWN)      //触摸屏被按下
        {
            if (tp_dev.x < lcddev.width && tp_dev.y < lcddev.height)
            {
                if (tp_dev.x > (lcddev.width - 24) && tp_dev.y < 16)
                {
                    DrawTestPage("测试9:Touch(按KEY0校准)      ");//清除
                    gui_show_ptstring(lcddev.width - 24, 0, 16, "RST", 1); //显示清屏区域
                    PAINT_COLOR = colorTemp;
                    gui_fill(lcddev.width - 52, 2, lcddev.width - 50 + 20, 18, PAINT_COLOR);
                }
                else if ((tp_dev.x > (lcddev.width - 60) && tp_dev.x < (lcddev.width - 50 + 20)) && tp_dev.y < 20)
                {
                    gui_fill(lcddev.width - 52, 2, lcddev.width - 50 + 20, 18, ColorTab[j % 5]);
                    PAINT_COLOR = ColorTab[(j++) % 5];
                    colorTemp = PAINT_COLOR;
                    delay_ms(10);
                }

                else
                {
                    gui_draw_big_point(tp_dev.x, tp_dev.y, PAINT_COLOR);    //画图
                }
            }
        }
        else
        {
            delay_ms(10);    //没有按键按下的时候
        }
        if (key == 1) //KEY_RIGHT按下,则执行校准程序
        {

            LCD_Clear(WHITE);//清屏
            TP_Adjust();  //屏幕校准
            TP_Save_Adjdata();
            DrawTestPage("测试9:Touch(按KEY0校准)      ");
            gui_show_ptstring(lcddev.width - 24, 0, 16, "RST", 1); //显示清屏区域
            PAINT_COLOR = colorTemp;
            gui_fill(lcddev.width - 52, 2, lcddev.width - 50 + 20, 18, PAINT_COLOR);
        }
        i++;
        if (i == 30)
        {
            i = 0;
            LED0 = !LED0;
            //break;
        }
    }
}




