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
#include "lcd.h"
#include "string.h"
#include "font.h"
#include "delay.h"
#include "gui.h"

//��ɫת��
//rgb:32λ��ɫ
//����ֵ:16λɫ.
u16 gui_color_chg(u32 rgb)
{
    u16 r, g, b;

    r = rgb >> 19;
    g = (rgb >> 10) & 0x3f;
    b = (rgb >> 3) & 0x1f;

    return ((r << 11) | (g << 5) | b);
}

//��rgb332��ɫת��Ϊrgb565.
//rgb332:rgb332��ʽ����
//����ֵ:rgb565��ʽ����.
u16 gui_rgb332torgb565(u16 rgb332)
{
    u16 rgb565 = 0x18E7;

    rgb565 |= (rgb332 & 0x03) << 3;
    rgb565 |= (rgb332 & 0x1c) << 6;
    rgb565 |= (rgb332 & 0xe0) << 8;

    return rgb565;
}

//////////////////////////ͳһ��׼����//////////////////////////////
//���ٻ���
//x,y:����
//color:��ɫ
void gui_draw_point_color(u16 x, u16 y, u16 color)
{
    LCD_SetWindow(x, y, 1, 1);  //���õ��λ��
    LCD_WriteWord(color);
}

//����
//x,y:����
//PAINT_COLOR:�˵����ɫ
void gui_draw_point(u16 x, u16 y)
{
    gui_draw_point_color(x, y, PAINT_COLOR);
}

//��һ�����
//2*2�ĵ�
void gui_draw_big_point(u16 x, u16 y, u16 color)
{
    gui_draw_point_color(x, y, color); //���ĵ�
    gui_draw_point_color(x + 1, y, color);
    gui_draw_point_color(x, y + 1, color);
    gui_draw_point_color(x + 1, y + 1, color);
}

//��������
//x0,y0:��� x1,y1:�յ�
//color:��ɫ
void gui_draw_line_color(u16 x0, u16 y0, u16 x1, u16 y1, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    delta_x = x1 - x0; //������������
    delta_y = y1 - y0;
    row = x0;
    col = y0;

    if (delta_x > 0)
    {
        incx = 1;    //���õ�������
    }
    else if (delta_x == 0)
    {
        incx = 0;    //��ֱ��
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
        incy = 0;    //ˮƽ��
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
    {
        distance = delta_x;    //ѡȡ��������������
    }
    else
    {
        distance = delta_y;
    }

    for (t = 0; t <= distance + 1; t++) //�������
    {
        gui_draw_point_color(row, col, color); //����
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


//����
//x0,y0:�������
//x1,y1:�յ�����
void gui_draw_line(u16 x0, u16 y0, u16 x1, u16 y1)
{
    gui_draw_line_color(x0, y0, x1, y1, PAINT_COLOR);
}

//����ֱ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void gui_draw_vline(u16 x0, u16 y0, u16 len, u16 color)
{
    if ((len == 0) || (x0 > lcddev.width) || (y0 > lcddev.height))
    {
        return;
    }

    gui_fill(x0, y0, x0, y0 + len - 1, color);
}

//��ˮƽ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void gui_draw_hline(u16 x0, u16 y0, u16 len, u16 color)
{
    if ((len == 0) || (x0 > lcddev.width) || (y0 > lcddev.height))
    {
        return;
    }

    gui_fill(x0, y0, x0 + len - 1, y0, color);
}

//��������
//color:Ҫ���������ɫ
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

//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void gui_fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
    u16 i, j;
    u16 width = ex - sx + 1; //�õ����Ŀ��
    u16 height = ey - sy + 1; //�߶�

    LCD_SetWindow(sx, sy, width, height); //������ʾ����

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            LCD_WriteWord(color);    //д������
        }
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //�ָ���������Ϊȫ��
}

// ��һ��ʮ�ֵı��
// x����ǵ�X����
// y����ǵ�Y����
// color����ǵ���ɫ
void gui_draw_sign(u16 x, u16 y, u16 color)
{
    u8 i;

    /* ���� */
    LCD_SetWindow(x - 1, y - 1, x + 1, y + 1);
    for (i = 0; i < 9; i++)
    {
        LCD_WriteWord(color);
    }

    /* ���� */
    LCD_SetWindow(x - 4, y, x + 4, y);
    for (i = 0; i < 9; i++)
    {
        LCD_WriteWord(color);
    }

    /* ���� */
    LCD_SetWindow(x, y - 4, x, y + 4);
    for (i = 0; i < 9; i++)
    {
        LCD_WriteWord(color);
    }
}

//������
//(x1,y1),(x2,y2):���εĶԽ�����
void gui_draw_rectangle(u16 x0, u16 y0, u16 width, u16 height)
{
    gui_draw_line(x0, y0, x0 + width - 1, y0);
    gui_draw_line(x0, y0, x0, y0 + height - 1);
    gui_draw_line(x0, y0 + height - 1, x0 + width - 1, y0 + height - 1);
    gui_draw_line(x0 + width - 1, y0, x0 + width - 1, y0 + height - 1);
}

//������
//x0,y0:���ε����Ͻ�����
//width,height:���εĳߴ�
//color:��ɫ
void gui_fill_rectangle_color(u16 x0, u16 y0, u16 width, u16 height, u16 color)
{
    if (width == 0 || height == 0)
    {
        return;    //�Ƿ�.
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

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void gui_draw_circle(int xc, int yc, u16 c, int r, int fill)
{
    int x = 0, y = r, yi, d;

    d = 3 - 2 * r;


    if (fill)
    {
        // �����䣨��ʵ��Բ��
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
        // �������䣨������Բ��
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

//�������ڻ���
//(sx,sy),(ex,ey):�趨����ʾ��Χ
//(x,y):������
//color:��ɫ
void gui_draw_expoint(u16 sx, u16 sy, u16 ex, u16 ey, u16 x, u16 y, u16 color)
{
    if (x <= ex && x >= sx && y <= ey && y >= sy)
    {
        gui_draw_point_color(x, y, color);
    }
}

//��8��(Bresenham�㷨)
//(sx,sy),(ex,ey):�趨����ʾ��Χ
//(rx,ry,a,b):����
//color:��ɫ
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

//��ָ��λ�û�һ��ָ����С��Բ
//(rx,ry):Բ��
//(sx,sy),(ex,ey):�趨����ʾ��Χ
//r    :�뾶
//color:��ɫ
//mode :0,�����;1,���
void gui_draw_arc(u16 sx, u16 sy, u16 ex, u16 ey, u16 rx, u16 ry, u16 r, u16 color, u8 mode)
{
    int a, b, c;
    int di;

    a = 0;
    b = r;
    di = 3 - (r << 1); //�ж��¸���λ�õı�־

    while (a <= b)
    {
        if (mode)
            for (c = a; c <= b; c++)
            {
                gui_draw_circle8(sx, sy, ex, ey, rx, ry, a, c, color);    //��ʵ��Բ
            }
        else
        {
            gui_draw_circle8(sx, sy, ex, ey, rx, ry, a, b, color);    //������Բ
        }

        a++;

        //ʹ��Bresenham�㷨��Բ
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

//����Բ
//x0,y0:����
//rx:x����뾶
//ry:y����뾶
//color:��Բ����ɫ
void gui_draw_ellipse(u16 x0, u16 y0, u16 rx, u16 ry, u16 color)
{
    u32 OutConst, Sum, SumY;
    u16 x, y;
    u16 xOld;
    u32 _rx = rx;
    u32 _ry = ry;

    if (rx > x0 || ry > y0)
    {
        return;    //�Ƿ�.
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

//�����Բ
//x0,y0:����
//rx:x����뾶
//ry:y����뾶
//color:��Բ����ɫ
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

//��һ����ɫƽ�����ɵ�����һ����ɫ
//srgb,ergb:��ʼ��ɫ����ֹ��ɫ.
//cbuf:��ɫ������(��len)
//len:���ɼ���(�����������ɫֵ����)
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

//��һ��ƽ�����ɵĲ�ɫ����(����)
//x,y,width,height:���ε�����ߴ�
//srgb,ergb:��ʼ����ֹ����ɫ
void gui_draw_smooth_rectangle(u16 x, u16 y, u16 width, u16 height, u32 srgb, u32 ergb)
{
    u16 i, j;
    u16 *colortbl;

    gui_smooth_color(srgb, ergb, colortbl, width); //�����ɫ��

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            gui_draw_point_color(x + i, y + j, colortbl[i]); //����
        }
    }
}

//��Բ�Ǿ���/���Բ�Ǿ���
//x,y,width,height:Բ�Ǿ��ε�λ�úͳߴ�
//r:Բ�ǵİ뾶.
//mode:0,�����ο�;1,������.
//upcolor:�ϰ벿����ɫ
//downcolor:�°벿����ɫ
void gui_draw_arcrectangle(u16 x, u16 y, u16 width, u16 height, u8 r, u8 mode, u16 upcolor, u16 downcolor)
{
    u16 btnxh = 0;

    if (height % 2)
    {
        btnxh = height + 1;    //��ż������
    }
    else
    {
        btnxh = height;
    }

    if (mode) //���
    {
        gui_fill_rectangle_color(x + r, y, width - 2 * r, btnxh / 2, upcolor); //����
        gui_fill_rectangle_color(x + r, y + btnxh / 2, width - 2 * r, btnxh / 2, downcolor); //����
        gui_fill_rectangle_color(x, y + r, r, btnxh / 2 - r, upcolor);        //����
        gui_fill_rectangle_color(x, y + btnxh / 2, r, btnxh / 2 - r, downcolor); //����
        gui_fill_rectangle_color(x + width - r, y + r, r, btnxh / 2 - r, upcolor); //����
        gui_fill_rectangle_color(x + width - r, y + btnxh / 2, r, btnxh / 2 - r, downcolor); //����
    }
    else
    {
        gui_draw_hline(x + r, y, width - 2 * r, upcolor);           //��
        gui_draw_hline(x + r, y + btnxh - 1, width - 2 * r, downcolor); //��
        gui_draw_vline(x, y + r, btnxh / 2 - r, upcolor);           //����
        gui_draw_vline(x, y + btnxh / 2, btnxh / 2 - r, downcolor); //����
        gui_draw_vline(x + width - 1, y + r, btnxh / 2 - r, upcolor); //����
        gui_draw_vline(x + width - 1, y + btnxh / 2, btnxh / 2 - r, downcolor); //����
    }

    gui_draw_arc(x, y, x + r, y + r, x + r, y + r, r, upcolor, mode); //����
    gui_draw_arc(x, y + btnxh - r, x + r, y + btnxh - 1, x + r, y + btnxh - r - 1, r, downcolor, mode); //����
    gui_draw_arc(x + width - r, y, x + width, y + r, x + width - r - 1, y + r, r, upcolor, mode); //����
    gui_draw_arc(x + width - r, y + btnxh - r, x + width, y + btnxh - 1, x + width - r - 1, y + btnxh - r - 1, r, downcolor, mode); //����
}

//����ALPHA BLENDING�㷨.
//src:Դ��ɫ
//dst:Ŀ����ɫ
//alpha:͸���̶�(0~32)
//����ֵ:��Ϻ����ɫ.
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

//��ָ���������alphablend
//x,y,width,height:��������
//color:alphablend����ɫ
//aval:͸����(0~32)
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
        LCD_WriteWord(temp);//�����ʾ
        tmp += 2;
    } while (tmp < num);
}

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void gui_show_ptchar(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 size, u8 mode)
{
    u8 temp;
    u8 pos, t;

    num = num - ' '; //�õ�ƫ�ƺ��ֵ
    LCD_SetWindow(x, y, size / 2, size); //���õ���������ʾ����

    if (!mode) //�ǵ��ӷ�ʽ
    {
        for (pos = 0; pos < size; pos++)
        {
            if (size == 12)
            {
                temp = ascii_1206[num][pos];    //����1206����
            }
            else
            {
                temp = ascii_1608[num][pos];    //����1608����
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
    else //���ӷ�ʽ
    {
        for (pos = 0; pos < size; pos++)
        {
            if (size == 12)
            {
                temp = ascii_1206[num][pos];    //����1206����
            }
            else
            {
                temp = ascii_1608[num][pos];    //����1608����
            }

            for (t = 0; t < size / 2; t++)
            {
                if (temp & 0x01)
                {
                    gui_draw_point_color(x + t, y + pos, fc);    //��һ����
                }

                temp >>= 1;
            }
        }
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //�ָ�����Ϊȫ��
}

//��ָ��λ�ÿ�ʼ,��ʾָ�����ȷ�Χ���ַ���.
//x,y:��ʾ��ʼ����.
//xend:x�������ֹ����
//offset:��ʼ��ʾ��ƫ��
//color:������ɫ.
//size:���ִ�С
//str:�ַ���
//mode:0,�ǵ�����ʾ;1,������ʾ,2,������(ֻ�����ڵ���ģʽ)
void gui_show_ptstring(u16 x, u16 y, u8 size, u8 *p, u8 mode)
{
    while ((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
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

//m^n����
//����ֵ:m^n�η�.
u32 mypow(u8 m, u8 n)
{
    u32 result = 1;

    while (n--)
    {
        result *= m;
    }

    return result;
}

//��ʾ����,��λΪ0,����ʾ
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);
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

    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); //�Զ�ͳ�ƺ�����Ŀ

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
        {
            LCD_SetWindow(x, y, 16, 16);
            for (i = 0; i < 16 * 2; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //�ǵ��ӷ�ʽ
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
                            gui_draw_point_color(x, y, fc);    //��һ����
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
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //�ָ�����Ϊȫ��
}

void gui_draw_font24(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); //�Զ�ͳ�ƺ�����Ŀ

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
        {
            LCD_SetWindow(x, y, 24, 24);
            for (i = 0; i < 24 * 3; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //�ǵ��ӷ�ʽ
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
                            gui_draw_point_color(x, y, fc);    //��һ����
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
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //�ָ�����Ϊȫ��
}

void gui_draw_font32(u16 x, u16 y, u16 fc, u16 bc, u8 *s, u8 mode)
{
    u8 i, j;
    u16 k;
    u16 HZnum;
    u16 x0 = x;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); //�Զ�ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
        {
            LCD_SetWindow(x, y, 32, 32);
            for (i = 0; i < 32 * 4; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //�ǵ��ӷ�ʽ
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
                            gui_draw_point_color(x, y, fc);    //��һ����
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
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }

    LCD_SetWindow(0, 0, lcddev.width, lcddev.height); //�ָ�����Ϊȫ��
}

//���趨��������ʾ�ַ���,�������������ʾ
//str:�ַ���ָ��;
//x,y,width,height:��ʾ����
//font:�����С
//fcolor:������ɫ,�Ե��ӷ�ʽд��.
void gui_show_string(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode)
{
    u16 x0 = x;
    u8 bHz = 0;   //�ַ���������
    while (*str != 0) //����δ����
    {
        if (!bHz)
        {
            if (x > (lcddev.width - size / 2) || y > (lcddev.height - size))
            {
                return;
            }
            if (*str > 0x80)
            {
                bHz = 1;    //����
            }
            else              //�ַ�
            {
                if (*str == 0x0D) //���з���
                {
                    y += size;
                    x = x0;
                    str++;
                }
                else
                {
                    if (size > 16) //�ֿ���û�м���12X24 16X32��Ӣ������,��8X16����
                    {
                        gui_show_ptchar(x, y, fc, bc, *str, 16, mode);
                        x += 8; //�ַ�,Ϊȫ�ֵ�һ��
                    }
                    else
                    {
                        gui_show_ptchar(x, y, fc, bc, *str, size, mode);
                        x += size / 2; //�ַ�,Ϊȫ�ֵ�һ��
                    }
                }

                str++;
            }
        }
        else //����
        {
            if (x > (lcddev.width - size) || y > (lcddev.height - size))
            {
                return;
            }

            bHz = 0; //�к��ֿ�
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
            x += size; //��һ������ƫ��
        }
    }
}

void gui_show_string_center(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode)
{
    u16 len = strlen((const char *)str);
    u16 x1 = (lcddev.width - len * 8) / 2;
    
    gui_show_string(x1, y, fc, bc, str, size, mode);
}


