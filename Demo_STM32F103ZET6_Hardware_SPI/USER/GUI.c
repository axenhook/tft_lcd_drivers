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

//////////////////////////统一标准函数//////////////////////////////
//快速画点
//x,y:坐标
//color:颜色
void gui_draw_point_color(u16 x, u16 y, u16 color)
{
    LCD_SetWindow(x, y, 1, 1);  //设置点的位置
    LCD_WriteWord(color);
}

//画点
//x,y:坐标
//PAINT_COLOR:此点的颜色
void gui_draw_point(u16 x, u16 y)
{
    gui_draw_point_color(x, y, PAINT_COLOR);
}

//画一个大点
//2*2的点
void gui_draw_big_point(u16 x, u16 y, u16 color)
{
    gui_draw_point_color(x, y, color); //中心点
    gui_draw_point_color(x + 1, y, color);
    gui_draw_point_color(x, y + 1, color);
    gui_draw_point_color(x + 1, y + 1, color);
}

//画任意线
//x0,y0:起点 x1,y1:终点
//color:颜色
void gui_draw_line_color(u16 x0, u16 y0, u16 x1, u16 y1, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    delta_x = x1 - x0; //计算坐标增量
    delta_y = y1 - y0;
    row = x0;
    col = y0;

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
        gui_draw_point_color(row, col, color); //画点
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}


//画线
//x0,y0:起点坐标
//x1,y1:终点坐标
void gui_draw_line(u16 x0, u16 y0, u16 x1, u16 y1)
{
    gui_draw_line_color(x0, y0, x1, y1, PAINT_COLOR);
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

    gui_fill(x0, y0, x0, y0 + len - 1, color);
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

    gui_fill(x0, y0, x0 + len - 1, y0, color);
}

//清屏函数
//color:要清屏的填充色
void gui_clear(u16 Color)
{
    unsigned int i, j;

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height);

    for (i = 0; i < lcddev.height; i++)
    {
        for (j = 0; j < lcddev.width; j++)
        {
            LCD_WriteWord(Color);
        }
    }
}

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void gui_fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
    u16 i, j;
    u16 width = ex - sx + 1; //得到填充的宽度
    u16 height = ey - sy + 1; //高度

    LCD_SetWindow(sx, sy, width, height); //设置显示窗口

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            LCD_WriteWord(color);    //写入数据
        }
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //恢复窗口设置为全屏
}

// 画一个十字的标记
// x：标记的X坐标
// y：标记的Y坐标
// color：标记的颜色
void gui_draw_sign(u16 x, u16 y, u16 color)
{
    u8 i;

    /* 画点 */
    LCD_SetWindow(x - 1, y - 1, x + 1, y + 1);
    for (i = 0; i < 9; i++)
    {
        LCD_WriteWord(color);
    }

    /* 画竖 */
    LCD_SetWindow(x - 4, y, x + 4, y);
    for (i = 0; i < 9; i++)
    {
        LCD_WriteWord(color);
    }

    /* 画横 */
    LCD_SetWindow(x, y - 4, x, y + 4);
    for (i = 0; i < 9; i++)
    {
        LCD_WriteWord(color);
    }
}

//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void gui_draw_rectangle(u16 x0, u16 y0, u16 width, u16 height)
{
    gui_draw_line(x0, y0, x0 + width - 1, y0);
    gui_draw_line(x0, y0, x0, y0 + height - 1);
    gui_draw_line(x0, y0 + height - 1, x0 + width - 1, y0 + height - 1);
    gui_draw_line(x0 + width - 1, y0, x0 + width - 1, y0 + height - 1);
}

//填充矩形
//x0,y0:矩形的左上角坐标
//width,height:矩形的尺寸
//color:颜色
void gui_fill_rectangle_color(u16 x0, u16 y0, u16 width, u16 height, u16 color)
{
    if (width == 0 || height == 0)
    {
        return;    //非法.
    }

    gui_fill(x0, y0, x0 + width - 1, y0 + height - 1, color);
}

void gui_fill_rectangle(u16 x0, u16 y0, u16 width, u16 height)
{
    gui_fill_rectangle_color(x0, y0, width, height, PAINT_COLOR);
}


void gui_draw_triangle(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2)
{
    gui_draw_line(x0, y0, x1, y1);
    gui_draw_line(x1, y1, x2, y2);
    gui_draw_line(x2, y2, x0, y0);
}

static void _swap(u16 *a, u16 *b)
{
    u16 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void gui_fill_triangle(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 a, b, y, last;
    int dx01, dy01, dx02, dy02, dx12, dy12;
    long sa = 0;
    long sb = 0;
    if (y0 > y1)
    {
        _swap(&y0, &y1);
        _swap(&x0, &x1);
    }
    if (y1 > y2)
    {
        _swap(&y2, &y1);
        _swap(&x2, &x1);
    }
    if (y0 > y1)
    {
        _swap(&y0, &y1);
        _swap(&x0, &x1);
    }
    if (y0 == y2)
    {
        a = b = x0;
        if (x1 < a)
        {
            a = x1;
        }
        else if (x1 > b)
        {
            b = x1;
        }
        if (x2 < a)
        {
            a = x2;
        }
        else if (x2 > b)
        {
            b = x2;
        }
        gui_fill(a, y0, b, y0, PAINT_COLOR);
        return;
    }
    dx01 = x1 - x0;
    dy01 = y1 - y0;
    dx02 = x2 - x0;
    dy02 = y2 - y0;
    dx12 = x2 - x1;
    dy12 = y2 - y1;

    if (y1 == y2)
    {
        last = y1;
    }
    else
    {
        last = y1 - 1;
    }
    for (y = y0; y <= last; y++)
    {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if (a > b)
        {
            _swap(&a, &b);
        }
        gui_fill(a, y, b, y, PAINT_COLOR);
    }
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y <= y2; y++)
    {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (a > b)
        {
            _swap(&a, &b);
        }
        gui_fill(a, y, b, y, PAINT_COLOR);
    }
}

void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
    gui_draw_point_color(xc + x, yc + y, c);
    gui_draw_point_color(xc - x, yc + y, c);
    gui_draw_point_color(xc + x, yc - y, c);
    gui_draw_point_color(xc - x, yc - y, c);
    gui_draw_point_color(xc + y, yc + x, c);
    gui_draw_point_color(xc - y, yc + x, c);
    gui_draw_point_color(xc + y, yc - x, c);
    gui_draw_point_color(xc - y, yc - x, c);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void gui_draw_circle(int xc, int yc, u16 c, int r, int fill)
{
    int x = 0, y = r, yi, d;

    d = 3 - 2 * r;


    if (fill)
    {
        // 如果填充（画实心圆）
        while (x <= y)
        {
            for (yi = x; yi <= y; yi++)
            {
                _draw_circle_8(xc, yc, x, yi, c);
            }

            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
    else
    {
        // 如果不填充（画空心圆）
        while (x <= y)
        {
            _draw_circle_8(xc, yc, x, y, c);
            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}

//在区域内画点
//(sx,sy),(ex,ey):设定的显示范围
//(x,y):点坐标
//color:颜色
void gui_draw_expoint(u16 sx, u16 sy, u16 ex, u16 ey, u16 x, u16 y, u16 color)
{
    if (x <= ex && x >= sx && y <= ey && y >= sy)
    {
        gui_draw_point_color(x, y, color);
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
            gui_draw_line_color(x0 - xOld, y0 - y + 1, x0 - x, y0 - y, color);
            gui_draw_line_color(x0 - xOld, y0 + y - 1, x0 - x, y0 + y, color);
            gui_draw_line_color(x0 + xOld, y0 - y + 1, x0 + x, y0 - y, color);
            gui_draw_line_color(x0 + xOld, y0 + y - 1, x0 + x, y0 + y, color);
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
            gui_draw_point_color(x + i, y + j, colortbl[i]); //画点
        }
    }
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
        gui_fill_rectangle_color(x + r, y, width - 2 * r, btnxh / 2, upcolor); //中上
        gui_fill_rectangle_color(x + r, y + btnxh / 2, width - 2 * r, btnxh / 2, downcolor); //中下
        gui_fill_rectangle_color(x, y + r, r, btnxh / 2 - r, upcolor);        //左上
        gui_fill_rectangle_color(x, y + btnxh / 2, r, btnxh / 2 - r, downcolor); //左下
        gui_fill_rectangle_color(x + width - r, y + r, r, btnxh / 2 - r, upcolor); //右上
        gui_fill_rectangle_color(x + width - r, y + btnxh / 2, r, btnxh / 2 - r, downcolor); //右下
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
            gui_draw_point_color(x + i, y + j, tempcolor);
        }
    }
}

void gui_draw_image(u16 x, u16 y, u16 width, u16 height, const u8 *img)
{
    u16 temp = 0;
    long tmp = 0, num = 0;

    LCD_SetWindow(x, y, width, height);

    num = width * height * 2 ;
    do
    {
        temp = img[tmp + 1];
        temp = temp << 8;
        temp = temp | img[tmp];
        LCD_WriteWord(temp);//逐点显示
        tmp += 2;
    } while (tmp < num);
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void gui_show_ptchar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode)
{
    u8 temp;
    u8 pos, t;

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
                if (temp & 0x01)
                {
                    gui_draw_point_color(x + t, y + pos, fc);    //画一个点
                }

                temp >>= 1;
            }
        }
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //恢复窗口为全屏
}

//在指定位置开始,显示指定长度范围的字符串.
//x,y:显示开始坐标.
//xend:x方向的终止坐标
//offset:开始显示的偏移
//color:文字颜色.
//size:文字大小
//str:字符串
//mode:0,非叠加显示;1,叠加显示,2,大点叠加(只适用于叠加模式)
void gui_show_ptstring(u16 x, u16 y, u8 size, u8 *p, u8 mode)
{
    while ((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if (x > (lcddev.width - 1) || y > (lcddev.height - 1))
        {
            return;
        }

        gui_show_ptchar(x, y, PAINT_COLOR, BACK_COLOR, *p, size, mode);
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
void gui_show_num(u16 x, u16 y, u32 num, u8 len, u8 size)
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
                gui_show_ptchar(x + (size / 2)*t, y, PAINT_COLOR, BACK_COLOR, ' ', size, 0);
                continue;
            }
            else
            {
                enshow = 1;
            }

        }

        gui_show_ptchar(x + (size / 2)*t, y, PAINT_COLOR, BACK_COLOR, temp + '0', size, 0);
    }
}

void gui_draw_font16(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode)
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
                        if (tfont16[k].Msk[i] & (0x80 >> j))
                        {
                            gui_draw_point_color(x, y, fc);    //画一个点
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

void gui_draw_font24(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode)
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
                        if (tfont24[k].Msk[i] & (0x80 >> j))
                        {
                            gui_draw_point_color(x, y, fc);    //画一个点
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

void gui_draw_font32(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode)
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
                        if (tfont32[k].Msk[i] & (0x80 >> j))
                        {
                            gui_draw_point_color(x, y, fc);    //画一个点
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

//在设定区域内显示字符串,超过区域后不再显示
//str:字符串指针;
//x,y,width,height:显示区域
//font:字体大小
//fcolor:字体颜色,以叠加方式写字.
void gui_show_string(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode)
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
                        gui_show_ptchar(x, y, fc, bc, *str, 16, mode);
                        x += 8; //字符,为全字的一半
                    }
                    else
                    {
                        gui_show_ptchar(x, y, fc, bc, *str, size, mode);
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
                gui_draw_font32(x, y, fc, bc, str, mode);
            }
            else if (size == 24)
            {
                gui_draw_font24(x, y, fc, bc, str, mode);
            }
            else
            {
                gui_draw_font16(x, y, fc, bc, str, mode);
            }

            str += 2;
            x += size; //下一个汉字偏移
        }
    }
}

void gui_show_string_center(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode)
{
    u16 len = strlen((const char *)str);
    u16 x1 = (lcddev.width - len * 8) / 2;
    
    gui_show_string(x1, y, fc, bc, str, size, mode);
}


