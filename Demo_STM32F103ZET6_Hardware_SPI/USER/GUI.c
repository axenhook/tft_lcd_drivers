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
#include "string.h"
#include "font.h"
#include "delay.h"
#include "gui.h"

/*****************************************************************************
 * @name       :void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
 * @date       :2018-08-09
 * @function   :Display a single English character
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
                                fc:the color value of display character
                                bc:the background color of display character
                                num:the ascii code of display character(0~94)
                                size:the size of display character
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode)
{
    u8 temp;
    u8 pos, t;
    u16 colortemp = PAINT_COLOR;

    num = num - ' '; //得到偏移后的值
    LCD_SetWindow(x, y, size / 2, size); //设置单个文字显示窗口
    if (!mode) //非叠加方式
    {
        for (pos = 0; pos < size; pos++)
        {
            if (size == 12)
            {
                temp = ascii_1206[num][pos];    //调用1206字体
            }
            else
            {
                temp = ascii_1608[num][pos];    //调用1608字体
            }
            for (t = 0; t < size / 2; t++)
            {
                if (temp & 0x01)
                {
                    LCD_WriteWord(fc);
                }
                else
                {
                    LCD_WriteWord(bc);
                }
                temp >>= 1;

            }

        }
    }
    else //叠加方式
    {
        for (pos = 0; pos < size; pos++)
        {
            if (size == 12)
            {
                temp = ascii_1206[num][pos];    //调用1206字体
            }
            else
            {
                temp = ascii_1608[num][pos];    //调用1608字体
            }
            for (t = 0; t < size / 2; t++)
            {
                PAINT_COLOR = fc;
                if (temp & 0x01)
                {
                    LCD_DrawPoint(x + t, y + pos);    //画一个点
                }
                temp >>= 1;
            }
        }
    }
    PAINT_COLOR = colortemp;
    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //恢复窗口为全屏
}

/*****************************************************************************
 * @name       :void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
 * @date       :2018-08-09
 * @function   :Display English string
 * @parameters :x:the bebinning x coordinate of the English string
                y:the bebinning y coordinate of the English string
                                p:the start address of the English string
                                size:the size of display character
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowString(u16 x, u16 y, u8 size, u8 *p, u8 mode)
{
    while ((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if (x > (lcddev.width - 1) || y > (lcddev.height - 1))
        {
            return;
        }
        LCD_ShowChar(x, y, PAINT_COLOR, BACK_COLOR, *p, size, mode);
        x += size / 2;
        p++;
    }
}

//m^n函数
//返回值:m^n次方.
u32 mypow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}

//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + (size / 2)*t, y, PAINT_COLOR, BACK_COLOR, ' ', size, 0);
                continue;
            }
            else
            {
                enshow = 1;
            }

        }
        LCD_ShowChar(x + (size / 2)*t, y, PAINT_COLOR, BACK_COLOR, temp + '0', size, 0);
    }
}

/*****************************************************************************
 * @name       :void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
 * @date       :2018-08-09
 * @function   :Display a single 16x16 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
                                fc:the color value of Chinese character
                                bc:the background color of Chinese character
                                s:the start address of the Chinese character
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); //自动统计汉字数目


    for (k = 0; k < HZnum; k++)
    {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
        {
            LCD_SetWindow(x, y, 16, 16);
            for (i = 0; i < 16 * 2; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //非叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_WriteWord(fc);
                        }
                        else
                        {
                            LCD_WriteWord(bc);
                        }
                    }
                    else
                    {
                        PAINT_COLOR = fc;
                        if (tfont16[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);    //画一个点
                        }
                        x++;
                        if ((x - x0) == 16)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }

                }

            }


        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //恢复窗口为全屏
}

/*****************************************************************************
 * @name       :void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
 * @date       :2018-08-09
 * @function   :Display a single 24x24 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
                                fc:the color value of Chinese character
                                bc:the background color of Chinese character
                                s:the start address of the Chinese character
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); //自动统计汉字数目

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
        {
            LCD_SetWindow(x, y, 24, 24);
            for (i = 0; i < 24 * 3; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //非叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_WriteWord(fc);
                        }
                        else
                        {
                            LCD_WriteWord(bc);
                        }
                    }
                    else
                    {
                        PAINT_COLOR = fc;
                        if (tfont24[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);    //画一个点
                        }
                        x++;
                        if ((x - x0) == 24)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }


        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //恢复窗口为全屏
}

/*****************************************************************************
 * @name       :void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
 * @date       :2018-08-09
 * @function   :Display a single 32x32 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
                                fc:the color value of Chinese character
                                bc:the background color of Chinese character
                                s:the start address of the Chinese character
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); //自动统计汉字数目
    for (k = 0; k < HZnum; k++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
        {
            LCD_SetWindow(x, y, 32, 32);
            for (i = 0; i < 32 * 4; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //非叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_WriteWord(fc);
                        }
                        else
                        {
                            LCD_WriteWord(bc);
                        }
                    }
                    else
                    {
                        PAINT_COLOR = fc;
                        if (tfont32[k].Msk[i] & (0x80 >> j))
                        {
                            LCD_DrawPoint(x, y);    //画一个点
                        }
                        x++;
                        if ((x - x0) == 32)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }


        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //恢复窗口为全屏
}

/*****************************************************************************
 * @name       :void LCD_ShowString(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
 * @date       :2018-08-09
 * @function   :Display Chinese and English strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
                                fc:the color value of Chinese and English strings
                                bc:the background color of Chinese and English strings
                                str:the start address of the Chinese and English strings
                                size:the size of Chinese and English strings
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowHzString(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode)
{
    u16 x0 = x;
    u8 bHz = 0;   //字符或者中文
    while (*str != 0) //数据未结束
    {
        if (!bHz)
        {
            if (x > (lcddev.width - size / 2) || y > (lcddev.height - size))
            {
                return;
            }
            if (*str > 0x80)
            {
                bHz = 1;    //中文
            }
            else              //字符
            {
                if (*str == 0x0D) //换行符号
                {
                    y += size;
                    x = x0;
                    str++;
                }
                else
                {
                    if (size > 16) //字库中没有集成12X24 16X32的英文字体,用8X16代替
                    {
                        LCD_ShowChar(x, y, fc, bc, *str, 16, mode);
                        x += 8; //字符,为全字的一半
                    }
                    else
                    {
                        LCD_ShowChar(x, y, fc, bc, *str, size, mode);
                        x += size / 2; //字符,为全字的一半
                    }
                }
                str++;

            }
        }
        else //中文
        {
            if (x > (lcddev.width - size) || y > (lcddev.height - size))
            {
                return;
            }
            bHz = 0; //有汉字库
            if (size == 32)
            {
                GUI_DrawFont32(x, y, fc, bc, str, mode);
            }
            else if (size == 24)
            {
                GUI_DrawFont24(x, y, fc, bc, str, mode);
            }
            else
            {
                GUI_DrawFont16(x, y, fc, bc, str, mode);
            }

            str += 2;
            x += size; //下一个汉字偏移
        }
    }
}

/*****************************************************************************
 * @name       :void LCD_ShowStringCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
 * @date       :2018-08-09
 * @function   :Centered display of English and Chinese strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
                                fc:the color value of Chinese and English strings
                                bc:the background color of Chinese and English strings
                                str:the start address of the Chinese and English strings
                                size:the size of Chinese and English strings
                                mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowStringCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode)
{
    u16 len = strlen((const char *)str);
    u16 x1 = (lcddev.width - len * 8) / 2;
    LCD_ShowHzString(x1, y, fc, bc, str, size, mode);
}


//extern code const unsigned char ascii_1206[95][12];
//extern code const unsigned char ascii_1608[95][16];
//extern code const unsigned char ascii_2412[95][36];



//颜色转换
//rgb:32位颜色
//返回值:16位色.
u16 gui_color_chg(u32 rgb)
{
    u16 r, g, b;
    r = rgb >> 19;
    g = (rgb >> 10) & 0x3f;
    b = (rgb >> 3) & 0x1f;
    return ((r << 11) | (g << 5) | b);
}
//将rgb332颜色转换为rgb565.
//rgb332:rgb332格式数据
//返回值:rgb565格式数据.
u16 gui_rgb332torgb565(u16 rgb332)
{
    u16 rgb565 = 0x18E7;
    rgb565 |= (rgb332 & 0x03) << 3;
    rgb565 |= (rgb332 & 0x1c) << 6;
    rgb565 |= (rgb332 & 0xe0) << 8;
    return rgb565;
}
//m^n函数
long gui_pow(u8 m, u8 n)
{
    long result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}
//两个数的差的绝对值
u32 gui_disabs(u32 x1, u32 x2)
{
    return x1 > x2 ? x1 - x2 : x2 - x1;
}

//////////////////////////统一标准函数//////////////////////////////
//画点函数
//x0,y0:坐标
//color:颜色
void gui_draw_point(u16 x0, u16 y0, u16 color)
{
    u16 tempcolor = PAINT_COLOR;
    PAINT_COLOR = color;
    LCD_DrawPoint(x0, y0);
    PAINT_COLOR = tempcolor;
}

//对指定区域进行alphablend
//x,y,width,height:区域设置
//color:alphablend的颜色
//aval:透明度(0~32)
void gui_alphablend_area(u16 x, u16 y, u16 width, u16 height, u16 color, u8 aval)
{
    u16 i, j;
    u16 tempcolor;
    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            tempcolor = LCD_ReadPoint(x + i, y + j);
            tempcolor = gui_alpha_blend565(tempcolor, color, aval);
            LCD_DrawPointColor(x + i, y + j, tempcolor);
        }
    }
}

//画大点函数
//x0,y0:坐标
//color:颜色
//以(x0,y0)为中心,画一个9个点的大点
void gui_draw_bigpoint(u16 x0, u16 y0, u16 color)
{
    u16 i, j;
    u16 x, y;
    if (x0 >= 1)
    {
        x = x0 - 1;
    }
    else
    {
        x = x0;
    }
    if (y0 >= 1)
    {
        y = y0 - 1;
    }
    else
    {
        y = y0;
    }
    for (i = y; i < y0 + 2; i++)
    {
        for (j = x; j < x0 + 2; j++)
        {
            LCD_DrawPointColor(j, i, color);
        }
    }
}
//画任意线
//x0,y0:起点 x1,y1:终点
//color:颜色
void gui_draw_line(u16 x0, u16 y0, u16 x1, u16 y1, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x1 - x0; //计算坐标增量
    delta_y = y1 - y0;
    uRow = x0;
    uCol = y0;
    if (delta_x > 0)
    {
        incx = 1;    //设置单步方向
    }
    else if (delta_x == 0)
    {
        incx = 0;    //垂直线
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;    //水平线
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
    {
        distance = delta_x;    //选取基本增量坐标轴
    }
    else
    {
        distance = delta_y;
    }
    for (t = 0; t <= distance + 1; t++) //画线输出
    {
        LCD_DrawPointColor(uRow, uCol, color); //画点
        xerr += delta_x ;
        yerr += delta_y ;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
//画一条粗线(方法0)
//(x1,y1),(x2,y2):线条的起止坐标
//size：线条的粗细程度
//color：线条的颜色
void gui_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    if (x1 < size || x2 < size || y1 < size || y2 < size)
    {
        return;
    }
    if (size == 0)
    {
        LCD_DrawPointColor(x1, y1, color);
        return;
    }
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
    {
        incx = 1;    //设置单步方向
    }
    else if (delta_x == 0)
    {
        incx = 0;    //垂直线
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;    //水平线
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
    {
        distance = delta_x;    //选取基本增量坐标轴
    }
    else
    {
        distance = delta_y;
    }
    for (t = 0; t <= distance + 1; t++) //画线输出
    {
        gui_fill_circle(uRow, uCol, size, color); //画点
        xerr += delta_x ;
        yerr += delta_y ;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

//画一条粗线(方法1)
//x0,y0:起点 x1,y1:终点
//size:线粗细,仅支持:0~2.
//color:颜色
void gui_draw_bline1(u16 x0, u16 y0, u16 x1, u16 y1, u8 size, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x1 - x0; //计算坐标增量
    delta_y = y1 - y0;
    uRow = x0;
    uCol = y0;
    if (delta_x > 0)
    {
        incx = 1;    //设置单步方向
    }
    else if (delta_x == 0)
    {
        incx = 0;    //垂直线
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;    //水平线
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
    {
        distance = delta_x;    //选取基本增量坐标轴
    }
    else
    {
        distance = delta_y;
    }
    for (t = 0; t <= distance + 1; t++) //画线输出
    {
        if (size == 0)
        {
            LCD_DrawPointColor(uRow, uCol, color);    //画点
        }
        if (size == 1)
        {
            LCD_DrawPointColor(uRow, uCol, color); //画点
            LCD_DrawPointColor(uRow + 1, uCol, color); //画点
            LCD_DrawPointColor(uRow, uCol + 1, color); //画点
            LCD_DrawPointColor(uRow + 1, uCol + 1, color); //画点
        }
        if (size == 2)
        {
            LCD_DrawPointColor(uRow, uCol, color); //画点
            LCD_DrawPointColor(uRow + 1, uCol, color); //画点
            LCD_DrawPointColor(uRow, uCol + 1, color); //画点
            LCD_DrawPointColor(uRow + 1, uCol + 1, color); //画点
            LCD_DrawPointColor(uRow - 1, uCol + 1, color); //画点
            LCD_DrawPointColor(uRow + 1, uCol - 1, color); //画点
            LCD_DrawPointColor(uRow - 1, uCol - 1, color); //画点
            LCD_DrawPointColor(uRow - 1, uCol, color); //画点
            LCD_DrawPointColor(uRow, uCol - 1, color); //画点
        }
        xerr += delta_x ;
        yerr += delta_y ;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
//画垂直线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_vline(u16 x0, u16 y0, u16 len, u16 color)
{
    if ((len == 0) || (x0 > lcddev.width) || (y0 > lcddev.height))
    {
        return;
    }
    LCD_Fill(x0, y0, x0, y0 + len - 1, color);
}
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0, u16 y0, u16 len, u16 color)
{
    if ((len == 0) || (x0 > lcddev.width) || (y0 > lcddev.height))
    {
        return;
    }
    LCD_Fill(x0, y0, x0 + len - 1, y0, color);
}
//填充一个色块.
//x0,y0,width,height:坐标及填充尺寸
//ctbl:颜色表,横向填充时width,纵向填充时大小为height
//mode:填充模式(纵向:从左到右,从上到下填充;横向:从上到下,从左到右填充)
//[7:1]:保留
//[bit0]:0,纵向;1,横向
void gui_fill_colorblock(u16 x0, u16 y0, u16 width, u16 height, u16 *ctbl, u8 mode)
{
    u16 i;
    if (height == 0 || width == 0)
    {
        return;
    }
    if (mode & 0x01) //横向
    {
        for (i = 0; i < width; i++)
        {
            gui_draw_vline(x0 + i, y0, height - 1, ctbl[i]);
        }
    }
    else
    {
        for (i = 0; i < height; i++)
        {
            gui_draw_hline(x0, y0 + i, width - 1, ctbl[i]);
        }
    }
}

//将一种颜色平滑过渡到另外一种颜色
//srgb,ergb:起始颜色和终止颜色.
//cbuf:颜色缓存区(≥len)
//len:过渡级数(就是输出的颜色值总数)
void gui_smooth_color(u32 srgb, u32 ergb, u16 *cbuf, u16 len)
{
    u8 sr, sg, sb;
    short dr, dg, db;
    u16 r, g, b;
    u16 i = 0;
    sr = srgb >> 16;
    sg = (srgb >> 8) & 0XFF;
    sb = srgb & 0XFF;
    dr = (ergb >> 16) - sr;
    dg = ((ergb >> 8) & 0XFF) - sg;
    db = (ergb & 0XFF) - sb;
    for (i = 0; i < len; i++)
    {
        r = sr + (dr * i) / len;
        g = sg + (dg * i) / len;
        b = sb + (db * i) / len;
        r >>= 3;
        g >>= 2;
        b >>= 3;
        cbuf[i] = ((r << 11) | (g << 5) | b);
    }
}

//画一条平滑过渡的彩色矩形(或线)
//x,y,width,height:矩形的坐标尺寸
//srgb,ergb:起始和终止的颜色
void gui_draw_smooth_rectangle(u16 x, u16 y, u16 width, u16 height, u32 srgb, u32 ergb)
{
    u16 i, j;
    u16 *colortbl;

    gui_smooth_color(srgb, ergb, colortbl, width); //获得颜色组
    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            LCD_DrawPointColor(x + i, y + j, colortbl[i]); //画点
        }
    }
}

//画矩形
//x0,y0:矩形的左上角坐标
//width,height:矩形的尺寸
//color:颜色
void gui_draw_rectangle(u16 x0, u16 y0, u16 width, u16 height, u16 color)
{
    gui_draw_hline(x0, y0, width, color);
    gui_draw_hline(x0, y0 + height - 1, width, color);
    gui_draw_vline(x0, y0, height, color);
    gui_draw_vline(x0 + width - 1, y0, height, color);
}
//画圆角矩形/填充圆角矩形
//x,y,width,height:圆角矩形的位置和尺寸
//r:圆角的半径.
//mode:0,画矩形框;1,填充矩形.
//upcolor:上半部分颜色
//downcolor:下半部分颜色
void gui_draw_arcrectangle(u16 x, u16 y, u16 width, u16 height, u8 r, u8 mode, u16 upcolor, u16 downcolor)
{
    u16 btnxh = 0;
    if (height % 2)
    {
        btnxh = height + 1;    //基偶数处理
    }
    else
    {
        btnxh = height;
    }
    if (mode) //填充
    {
        gui_fill_rectangle(x + r, y, width - 2 * r, btnxh / 2, upcolor); //中上
        gui_fill_rectangle(x + r, y + btnxh / 2, width - 2 * r, btnxh / 2, downcolor); //中下
        gui_fill_rectangle(x, y + r, r, btnxh / 2 - r, upcolor);        //左上
        gui_fill_rectangle(x, y + btnxh / 2, r, btnxh / 2 - r, downcolor); //左下
        gui_fill_rectangle(x + width - r, y + r, r, btnxh / 2 - r, upcolor); //右上
        gui_fill_rectangle(x + width - r, y + btnxh / 2, r, btnxh / 2 - r, downcolor); //右下
    }
    else
    {
        gui_draw_hline(x + r, y, width - 2 * r, upcolor);           //上
        gui_draw_hline(x + r, y + btnxh - 1, width - 2 * r, downcolor); //下
        gui_draw_vline(x, y + r, btnxh / 2 - r, upcolor);           //左上
        gui_draw_vline(x, y + btnxh / 2, btnxh / 2 - r, downcolor); //左下
        gui_draw_vline(x + width - 1, y + r, btnxh / 2 - r, upcolor); //右上
        gui_draw_vline(x + width - 1, y + btnxh / 2, btnxh / 2 - r, downcolor); //右下
    }
    gui_draw_arc(x, y, x + r, y + r, x + r, y + r, r, upcolor, mode); //左上
    gui_draw_arc(x, y + btnxh - r, x + r, y + btnxh - 1, x + r, y + btnxh - r - 1, r, downcolor, mode); //左下
    gui_draw_arc(x + width - r, y, x + width, y + r, x + width - r - 1, y + r, r, upcolor, mode); //右上
    gui_draw_arc(x + width - r, y + btnxh - r, x + width, y + btnxh - 1, x + width - r - 1, y + btnxh - r - 1, r, downcolor, mode); //右下
}

//填充矩形
//x0,y0:矩形的左上角坐标
//width,height:矩形的尺寸
//color:颜色
void gui_fill_rectangle(u16 x0, u16 y0, u16 width, u16 height, u16 color)
{
    if (width == 0 || height == 0)
    {
        return;    //非法.
    }
    LCD_Fill(x0, y0, x0 + width - 1, y0 + height - 1, color);
}
//画实心圆
//x0,y0:坐标
//r半径
//color:颜色
void gui_fill_circle(u16 x0, u16 y0, u16 r, u16 color)
{
    u32 i;
    u32 imax = ((u32)r * 707) / 1000 + 1;
    u32 sqmax = (u32)r * (u32)r + (u32)r / 2;
    u32 x = r;
    gui_draw_hline(x0 - r, y0, 2 * r, color);
    for (i = 1; i <= imax; i++)
    {
        if ((i * i + x * x) > sqmax)
        {
            // draw lines from outside
            if (x > imax)
            {
                gui_draw_hline(x0 - i + 1, y0 + x, 2 * (i - 1), color);
                gui_draw_hline(x0 - i + 1, y0 - x, 2 * (i - 1), color);
            }
            x--;
        }
        // draw lines from inside (center)
        gui_draw_hline(x0 - x, y0 + i, 2 * x, color);
        gui_draw_hline(x0 - x, y0 - i, 2 * x, color);
    }
}
//在区域内画点
//(sx,sy),(ex,ey):设定的显示范围
//(x,y):点坐标
//color:颜色
void gui_draw_expoint(u16 sx, u16 sy, u16 ex, u16 ey, u16 x, u16 y, u16 color)
{
    u16 tempcolor;
    if (x <= ex && x >= sx && y <= ey && y >= sy)
    {
        tempcolor = PAINT_COLOR;
        PAINT_COLOR = color;
        LCD_DrawPoint(x, y);
        PAINT_COLOR = tempcolor;
    }
}
//画8点(Bresenham算法)
//(sx,sy),(ex,ey):设定的显示范围
//(rx,ry,a,b):参数
//color:颜色
void gui_draw_circle8(u16 sx, u16 sy, u16 ex, u16 ey, u16 rx, u16 ry, int a, int b, u16 color)
{
    gui_draw_expoint(sx, sy, ex, ey, rx + a, ry - b, color);
    gui_draw_expoint(sx, sy, ex, ey, rx + b, ry - a, color);
    gui_draw_expoint(sx, sy, ex, ey, rx + b, ry + a, color);
    gui_draw_expoint(sx, sy, ex, ey, rx + a, ry + b, color);
    gui_draw_expoint(sx, sy, ex, ey, rx - a, ry + b, color);
    gui_draw_expoint(sx, sy, ex, ey, rx - b, ry + a, color);
    gui_draw_expoint(sx, sy, ex, ey, rx - b, ry - a, color);
    gui_draw_expoint(sx, sy, ex, ey, rx - a, ry - b, color);
}
//在指定位置画一个指定大小的圆
//(rx,ry):圆心
//(sx,sy),(ex,ey):设定的显示范围
//r    :半径
//color:颜色
//mode :0,不填充;1,填充
void gui_draw_arc(u16 sx, u16 sy, u16 ex, u16 ey, u16 rx, u16 ry, u16 r, u16 color, u8 mode)
{
    int a, b, c;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1); //判断下个点位置的标志
    while (a <= b)
    {
        if (mode)
            for (c = a; c <= b; c++)
            {
                gui_draw_circle8(sx, sy, ex, ey, rx, ry, a, c, color);    //画实心圆
            }
        else
        {
            gui_draw_circle8(sx, sy, ex, ey, rx, ry, a, b, color);    //画空心圆
        }
        a++;
        //使用Bresenham算法画圆
        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}
//画椭圆
//x0,y0:坐标
//rx:x方向半径
//ry:y方向半径
//color:椭圆的颜色
void gui_draw_ellipse(u16 x0, u16 y0, u16 rx, u16 ry, u16 color)
{
    u32 OutConst, Sum, SumY;
    u16 x, y;
    u16 xOld;
    u32 _rx = rx;
    u32 _ry = ry;
    if (rx > x0 || ry > y0)
    {
        return;    //非法.
    }
    OutConst = _rx * _rx * _ry * _ry + (_rx * _rx * _ry >> 1); // Constant as explaint above
    // To compensate for rounding
    xOld = x = rx;
    for (y = 0; y <= ry; y++)
    {
        if (y == ry)
        {
            x = 0;
        }
        else
        {
            SumY = ((u32)(rx * rx)) * ((u32)(y * y)); // Does not change in loop
            while (Sum = SumY + ((u32)(ry * ry)) * ((u32)(x * x)), (x > 0) && (Sum > OutConst))
            {
                x--;
            }
        }
        // Since we draw lines, we can not draw on the first iteration
        if (y)
        {
            gui_draw_line(x0 - xOld, y0 - y + 1, x0 - x, y0 - y, color);
            gui_draw_line(x0 - xOld, y0 + y - 1, x0 - x, y0 + y, color);
            gui_draw_line(x0 + xOld, y0 - y + 1, x0 + x, y0 - y, color);
            gui_draw_line(x0 + xOld, y0 + y - 1, x0 + x, y0 + y, color);
        }
        xOld = x;
    }
}
//填充椭圆
//x0,y0:坐标
//rx:x方向半径
//ry:y方向半径
//color:椭圆的颜色
void gui_fill_ellipse(u16 x0, u16 y0, u16 rx, u16 ry, u16 color)
{
    u32 OutConst, Sum, SumY;
    u16 x, y;
    u32 _rx = rx;
    u32 _ry = ry;
    OutConst = _rx * _rx * _ry * _ry + (_rx * _rx * _ry >> 1); // Constant as explaint above
    // To compensate for rounding
    x = rx;
    for (y = 0; y <= ry; y++)
    {
        SumY = ((u32)(rx * rx)) * ((u32)(y * y)); // Does not change in loop
        while (Sum = SumY + ((u32)(ry * ry)) * ((u32)(x * x)), (x > 0) && (Sum > OutConst))
        {
            x--;
        }
        gui_draw_hline(x0 - x, y0 + y, 2 * x, color);
        if (y)
        {
            gui_draw_hline(x0 - x, y0 - y, 2 * x, color);
        }
    }
}
//快速ALPHA BLENDING算法.
//src:源颜色
//dst:目标颜色
//alpha:透明程度(0~32)
//返回值:混合后的颜色.
u16 gui_alpha_blend565(u16 src, u16 dst, u8 alpha)
{
    u32 src2;
    u32 dst2;
    //Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
    src2 = ((src << 16) | src) & 0x07E0F81F;
    dst2 = ((dst << 16) | dst) & 0x07E0F81F;
    //Perform blending R:G:B with alpha in range 0..32
    //Note that the reason that alpha may not exceed 32 is that there are only
    //5bits of space between each R:G:B value, any higher value will overflow
    //into the next component and deliver ugly result.
    dst2 = ((((dst2 - src2) * alpha) >> 5) + src2) & 0x07E0F81F;
    return (dst2 >> 16) | dst2;
}

//在中间位置显示一个字符串
//x,y,width,height:显示区域坐标.
//color:文字颜色.
//size:文字大小
//str:字符串
void gui_show_strmid(u16 x, u16 y, u16 width, u16 height, u16 color, u8 size, u8 *str)
{
    u16 xoff = 0, yoff = 0;
    u16 strlenth;
    u16 strwidth;
    if (str == '\0')
    {
        return;
    }
    strlenth = strlen((const char *)str); //得到字符串长度
    strwidth = strlenth * size / 2; //字符串显示占用宽度
    if (height > size)
    {
        yoff = (height - size) / 2;
    }
    if (strwidth <= width) //字符串没超过宽度
    {
        xoff = (width - strwidth) / 2;
    }
    gui_show_ptstr(x + xoff, y + yoff, x + width - 1, y + height - 1, 0, color, size, str, 1);
}

//在中间位置显示一个字符串
//x,y,width,height:显示区域坐标.
//color:文字颜色.
//size:文字大小
//str:字符串
//mode:0,非叠加显示;1,叠加显示,2,大点叠加(只适用于叠加模式)
void gui_show_strmidex(u16 x, u16 y, u16 width, u16 height, u16 color, u8 size, u8 *str, u8 mode)
{
    u16 xoff = 0, yoff = 0;
    u16 strlenth;
    u16 strwidth;
    if (str == '\0')
    {
        return;
    }
    strlenth = strlen((const char *)str); //得到字符串长度
    strwidth = strlenth * size / 2; //字符串显示占用宽度
    if (height > size)
    {
        yoff = (height - size) / 2;
    }
    if (strwidth <= width) //字符串没超过宽度
    {
        xoff = (width - strwidth) / 2;
    }
    gui_show_ptstr(x + xoff, y + yoff, x + width - 1, y + height - 1, 0, color, size, str, mode);
}

//在指定地址开始显示一个ASCII字符
//x,y:显示开始坐标.
//xend,yend:x,y 方向的终点坐标
//offset:开始显示的偏移
//color:文字颜色.
//size:文字大小
//chr:字符
//mode:0,非叠加显示;1,叠加显示.2,大点叠加(只适用于叠加模式)
void gui_show_ptchar(u16 x, u16 y, u16 xend, u16 yend, u16 offset, u16 color, u16 size, u8 chr, u8 mode)
{
    u8 temp;
    u8 t1, t;
    u16 tempoff;
    u16 y0 = y;
    u16 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //得到字体一个字符对应点阵集所占的字节数
    if (chr > ' ')
    {
        chr = chr - ' ';    //得到偏移后的值
    }
    else
    {
        chr = 0;    //小于空格的一律用空格代替,比如TAB键(键值为9)
    }
    for (t = 0; t < csize; t++)
    {
        switch (size)
        {
            case 12:
                temp = ascii_1206[chr][t];  //调用1206字体
                break;
            case 16:
                //temp = ascii_1608[chr][t];      //调用1608字体
                break;
            case 24:
                //temp = ascii_2412[chr][t];      //调用2412字体
                break;
            default://不支持的字体
                return;
        }
        tempoff = offset;
        if (x > xend)
        {
            return;    //超区域了
        }
        if (tempoff == 0) //偏移地址到了
        {
            for (t1 = 0; t1 < 8; t1++)
            {
                if (y <= yend)
                {
                    if (temp & 0x80)
                    {
                        if (mode == 0x02)
                        {
                            gui_draw_bigpoint(x, y, color);
                        }
                        else
                        {
                            LCD_DrawPointColor(x, y, color);
                        }
                    }
                    else if (mode == 0)
                    {
                        LCD_DrawPointColor(x, y, BACK_COLOR);
                    }
                }
                temp <<= 1;
                y++;
                if ((y - y0) == size)
                {
                    y = y0;
                    x++;
                    break;
                }
            }
        }
        else
        {
            y += 8;
            if ((y - y0) >= size) //大于一个字的高度了
            {
                y = y0;     //y坐标归零
                tempoff--;
            }
        }
    }
}

//显示len个数字
//x,y :起点坐标
//len :数字的位数
//color:颜色
//size:字体大小
//num:数值(0~2^64);
//mode:模式.
//[7]:0,不填充;1,填充0.
//[3:0]:0,非叠加显示;1,叠加显示.2,大点叠加(只适用于叠加模式)
void gui_show_num(u16 x, u16 y, u8 len, u16 color, u8 size, long num, u8 mode)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / gui_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode & 0X80)
                {
                    gui_show_ptchar(x + (size / 2)*t, y, lcddev.width, lcddev.height, 0, color, size, '0', mode & 0xf);    //填充0
                }
                else
                {
                    gui_show_ptchar(x + (size / 2)*t, y, lcddev.width, lcddev.height, 0, color, size, ' ', mode & 0xf);
                }
                continue;
            }
            else
            {
                enshow = 1;
            }

        }
        gui_show_ptchar(x + (size / 2)*t, y, lcddev.width, lcddev.height, 0, color, size, temp + '0', mode & 0xf);
    }
}

//将数字转为字符串
//str:字符串存放地址.
//num:数字
u8 *gui_num2str(u8 *str, u32 num)
{
    u8 t, temp;
    u8 enrec = 0;
    for (t = 0; t < 10; t++)
    {
        temp = (num / gui_pow(10, 10 - t - 1)) % 10;
        if (enrec == 0 && t < 9)
        {
            if (temp == 0)
            {
                continue;    //还不需要记录
            }
            else
            {
                enrec = 1;    //需要记录了.
            }
        }
        *str = temp + '0';
        str++;
    }
    *str = '\0'; //加入结束符.
    return str;
}

//将数字转为字符串
//num:数字
//str:字符串存放地址
void gui_num2strex(int n, char *str)
{
    char i = 0, j = 0, k = 0;
    char s[6];

    do
    {
        s[i++] = n % 10 + '0'; //取下一个数字
    } while ((n /= 10) > 0); //删除该数字
//    for (j = i - 1; j >= 0; j--) //生成的数字是逆序的，所以要逆序输出
    {
        str[k++] = s[j];
    }
    str[k] = '\0';
}

//在指定位置开始,显示指定长度范围的字符串.
//x,y:显示开始坐标.
//xend:x方向的终止坐标
//offset:开始显示的偏移
//color:文字颜色.
//size:文字大小
//str:字符串
//mode:0,非叠加显示;1,叠加显示,2,大点叠加(只适用于叠加模式)
void gui_show_ptstr(u16 x, u16 y, u16 xend, u16 yend, u16 offset, u16 color, u8 size, u8 *str, u8 mode)
{
    u8 bHz = 0;     //字符或者中文
    u8 EnDisp = 0;  //使能显示
    while (*str != 0) //数据未结束
    {
        if (!bHz)
        {
            if (*str > 0x80)
            {
                bHz = 1;    //中文
            }
            else                //字符
            {
                if (EnDisp == 0) //还未使能显示
                {
                    if (offset >= size / 2) //超过了一个字符
                    {
                        offset -= size / 2; //减少一个字符的偏移
                    }
                    else                //未超过一个字符的偏移
                    {
                        offset = offset % (size / 2); //得到字符偏移量
                        EnDisp = 1;             //可以开始显示了
                    }
                }
                if (EnDisp == 1) //使能显示
                {
                    gui_show_ptchar(x, y, xend, yend, offset, color, size, *str, mode); //显示一个字符
                    if ((xend - x) > size / 2)
                    {
                        x += size / 2;    //字符,为全字的一半
                    }
                    else
                    {
                        x += xend - x;    //未完全显示
                    }
                    if (offset)
                    {
                        x -= offset;
                        offset = 0; //清除偏移
                    }
                }
                if (x >= xend)
                {
                    return;    //超过了,退出
                }
                str++;
            }
        }
        else //中文
        {
            bHz = 0; //有汉字库
            if (EnDisp == 0) //还未使能
            {
                if (offset >= size) //超过了一个字
                {
                    offset -= size; //减少
                }
                else
                {
                    offset = offset % (size); //得到字符偏移
                    EnDisp = 1; //可以开始显示了
                }
            }
            if (EnDisp)
            {
                //gui_show_ptfont(x,y,xend,yend,offset,color,size,str,mode); //显示这个汉字,空心显示
                if ((xend - x) > size)
                {
                    x += size;    //字符,为全字的一半
                }
                else
                {
                    x += xend - x;    //未完全显示
                }
                if (offset)
                {
                    x -= offset;
                    offset = 0; //清除偏移
                }
            }
            if (x >= xend)
            {
                return;    //超过了,退出
            }
            str += 2;
        }
    }
}

//在指定位置开始,显示指定长度范围的字符串.(带边特效显示)
//x,y:显示开始坐标.
//width:最大显示宽度
//offset:开始显示的偏移
//color:文字颜色.
//rimcolor:文字边的颜色
//size:文字大小
//str:字符串
void gui_show_ptstrwhiterim(u16 x, u16 y, u16 xend, u16 yend, u16 offset, u16 color, u16 rimcolor, u8 size, u8 *str)
{
    gui_show_ptstr(x, y, xend, yend, offset, rimcolor, size, str, 2); //先画边(用大号点画)
    gui_show_ptstr(x, y, xend, yend, offset, color, size, str, 1); //画字(用小点画)
}

//得到字符串所占的行数
//str:字符串指针;
//linelenth:每行的长度
//font:字体大小
//返回值:字符串所占的行数
u32 gui_get_stringline(u8 *str, u16 linelenth, u8 font)
{
    u16 xpos = 0;
    u32 lincnt = 1; //最少就是1行.
    if (linelenth < font / 2)
    {
        return 0XFFFFFFFF;    //无法统计完成
    }
    while (*str != '\0' && lincnt != 0xffffffff)
    {
        if ((*str == 0x0D && (*(str + 1) == 0X0A)) || (*str == 0X0A)) //是回车/换行符
        {
            if (*str == 0X0D)
            {
                str += 2;
            }
            else
            {
                str += 1;
            }
            lincnt++;//行数加1
            xpos = 0;
        }
        else if (*str >= 0X81 && (*(str + 1) >= 0X40)) //是gbk汉字
        {
            xpos += font;
            str += 2;
            if (xpos > linelenth) //已经不在本行之内
            {
                xpos = font;
                lincnt++;//行数加1
            }
        }
        else //是字符
        {
            xpos += font / 2;
            str += 1;
            if (xpos > linelenth) //已经不在本行之内
            {
                xpos = font / 2;
                lincnt++;//行数加1
            }
        }
    }
    return lincnt;
}

//在设定区域内显示字符串,超过区域后不再显示
//str:字符串指针;
//x,y,width,height:显示区域
//font:字体大小
//fcolor:字体颜色,以叠加方式写字.
void gui_show_string(u8 *str, u16 x, u16 y, u16 width, u16 height, u8 font, u16 fcolor)
{
    u16 xpos = x;
    u16 ypos = y;
    u16 endx = x + width - 1;
    u16 endy = y + height - 1;
    if (width < font / 2)
    {
        return ;    //无法显示完成
    }
    while (*str != '\0') //未结束
    {
        if ((*str == 0x0D && (*(str + 1) == 0X0A)) || (*str == 0X0A)) //是回车/换行符
        {
            if (*str == 0X0D)
            {
                str += 2;
            }
            else
            {
                str += 1;
            }
            xpos = x;
            ypos += font; //y增加16
        }
        else if (*str >= 0X81 && (*(str + 1) >= 0X40)) //是gbk汉字
        {
            if ((xpos + font) > (endx + 1)) //已经不在本行之内
            {
                xpos = x;
                ypos += font;
            }
            //gui_show_ptfont(xpos,ypos,endx,endy,0,fcolor,font,str,1);
            xpos += font; //偏移
            str += 2;

        }
        else //是字符
        {
            if ((xpos + font / 2) > (endx + 1))
            {
                xpos = x;
                ypos += font;
            }
            gui_show_ptchar(xpos, ypos, endx, endy, 0, fcolor, font, *str, 1);
            xpos += font / 2;
            str += 1;
        }
        if (ypos > endy)
        {
            break;    //超过了显示区域了.
        }
    }
}

//在设定区域内显示字符串,超过区域后不再显示
//x,y,width,height:显示区域
//fcolor:字体颜色
//font:字体大小
//str:字符串指针
//mode:0,非叠加显示;1,叠加显示,2,大点叠加(只适用于叠加模式)
void gui_show_stringex(u16 x, u16 y, u16 width, u16 height, u16 fcolor, u8 font, u8 *str, u8 mode)
{
    u16 xpos = x;
    u16 ypos = y;
    u16 endx = x + width - 1;
    u16 endy = y + height - 1;
    if (width < font / 2)
    {
        return ;    //无法显示完成
    }
    while (*str != '\0') //未结束
    {
        if ((*str == 0x0D && (*(str + 1) == 0X0A)) || (*str == 0X0A)) //是回车/换行符
        {
            if (*str == 0X0D)
            {
                str += 2;
            }
            else
            {
                str += 1;
            }
            xpos = x;
            ypos += font; //y增加16
        }
        else if (*str >= 0X81 && (*(str + 1) >= 0X40)) //是gbk汉字
        {
            if ((xpos + font) > (endx + 1)) //已经不在本行之内
            {
                xpos = x;
                ypos += font;
            }
            //gui_show_ptfont(xpos,ypos,endx,endy,0,fcolor,font,str,mode);
            xpos += font; //偏移
            str += 2;

        }
        else //是字符
        {
            if ((xpos + font / 2) > (endx + 1))
            {
                xpos = x;
                ypos += font;
            }
            gui_show_ptchar(xpos, ypos, endx, endy, 0, fcolor, font, *str, mode);
            xpos += font / 2;
            str += 1;
        }
        if (ypos > endy)
        {
            break;    //超过了显示区域了.
        }
    }
}


