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
//     LCDģ�� 					      STM32��Ƭ��
//       LED         ��          PB9          //Һ������������źţ��������Ҫ���ƣ���5V��3.3V
//       SCK         ��          PB13         //Һ����SPI����ʱ���ź�
//      DC/RS        ��          PB10         //Һ��������/��������ź�
//       RST         ��          PB12         //Һ������λ�����ź�
//       CS          ��          PB11         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ��
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
#include "stdlib.h"
#include "delay.h"
#include "SPI.h"


//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 PAINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
u16 DeviceCode;

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
    LCD_SelectReg(lcddev.wramcmd);
}

//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 Color)
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
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
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


//��ָ�����������ָ����ɫ��
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_FillColor(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
    u16 height, width;
    u16 i, j;
    
    width = ex - sx + 1;    //�õ����Ŀ��
    height = ey - sy + 1;   //�߶�
    
    LCD_SetWindow(sx, sy, ex, ey);
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            LCD_WriteWord(color[i * width + j]);
        }
    }
}

//����
//x,y:����
//PAINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x, u16 y)
{
    LCD_SetWindow(x, y, 1, 1);  //���õ��λ��
    LCD_WriteWord(PAINT_COLOR);
}

//���ٻ���
//x,y:����
//color:��ɫ
void LCD_DrawPointColor(u16 x, u16 y, u16 color)
{
    LCD_SetWindow(x, y, 1, 1);  //���õ��λ��
    LCD_WriteWord(color);
}

//��һ�����
//2*2�ĵ�
void LCD_DrawBigPoint(u16 x, u16 y, u16 color)
{
    LCD_DrawPointColor(x, y, color); //���ĵ�
    LCD_DrawPointColor(x + 1, y, color);
    LCD_DrawPointColor(x, y + 1, color);
    LCD_DrawPointColor(x + 1, y + 1, color);
}

//����
//x1,y1:�������
//x2,y2:�յ�����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1; //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
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
        LCD_DrawPoint(uRow, uCol); //����
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

void LCD_DrawLineColor(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
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
        LCD_DrawPointColor(uRow, uCol, color); //����
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


// ��һ��ʮ�ֵı��
// x����ǵ�X����
// y����ǵ�Y����
// color����ǵ���ɫ
void LCD_DrawSign(u16 x, u16 y, u16 color)
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
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}

/*****************************************************************************
 * @name       :void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09
 * @function   :Filled a rectangle
 * @parameters :x1:the bebinning x coordinate of the filled rectangle
                y1:the bebinning y coordinate of the filled rectangle
                                x2:the ending x coordinate of the filled rectangle
                                y2:the ending y coordinate of the filled rectangle
 * @retvalue   :None
******************************************************************************/
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_Fill(x1, y1, x2, y2, PAINT_COLOR);
}

/*****************************************************************************
 * @name       :void LCD_DrawTriangle(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge
                y0:the bebinning y coordinate of the triangular edge
                                x1:the vertex x coordinate of the triangular
                                y1:the vertex y coordinate of the triangular
                                x2:the ending x coordinate of the triangular edge
                                y2:the ending y coordinate of the triangular edge
 * @retvalue   :None
******************************************************************************/
void LCD_DrawTriangle(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x0, y0, x1, y1);
    LCD_DrawLine(x1, y1, x2, y2);
    LCD_DrawLine(x2, y2, x0, y0);
}

static void _swap(u16 *a, u16 *b)
{
    u16 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/*****************************************************************************
 * @name       :void LCD_DrawTriangleFill(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09
 * @function   :filling a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge
                y0:the bebinning y coordinate of the triangular edge
                                x1:the vertex x coordinate of the triangular
                                y1:the vertex y coordinate of the triangular
                                x2:the ending x coordinate of the triangular edge
                                y2:the ending y coordinate of the triangular edge
 * @retvalue   :None
******************************************************************************/
void LCD_DrawTriangleFill(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2)
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
        LCD_Fill(a, y0, b, y0, PAINT_COLOR);
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
        LCD_Fill(a, y, b, y, PAINT_COLOR);
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
        LCD_Fill(a, y, b, y, PAINT_COLOR);
    }
}

/*****************************************************************************
 * @name       :void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
 * @date       :2018-08-09
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center
                yc:the y coordinate of the Circular center
                                x:the x coordinate relative to the Circular center
                                y:the y coordinate relative to the Circular center
                                c:the color value of the circle
 * @retvalue   :None
******************************************************************************/
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
    LCD_DrawPointColor(xc + x, yc + y, c);

    LCD_DrawPointColor(xc - x, yc + y, c);

    LCD_DrawPointColor(xc + x, yc - y, c);

    LCD_DrawPointColor(xc - x, yc - y, c);

    LCD_DrawPointColor(xc + y, yc + x, c);

    LCD_DrawPointColor(xc - y, yc + x, c);

    LCD_DrawPointColor(xc + y, yc - x, c);

    LCD_DrawPointColor(xc - y, yc - x, c);
}

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_DrawCircle(int xc, int yc, u16 c, int r, int fill)
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; //GPIOB9,10,11,12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
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
    LCD_WriteByte(0x08); 	 //30
    LCD_WriteByte(0x26); 	 //30
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
    LCD_Clear(WHITE);//��ȫ����ɫ
}

//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height.
void LCD_SetWindow(u16 sx, u16 sy, u16 width, u16 height)
{
    LCD_SelectReg(lcddev.setxcmd);
    LCD_WriteByte(sx >> 8);
    LCD_WriteByte(0x00FF & sx);
    LCD_WriteByte((sx + width - 1) >> 8);
    LCD_WriteByte(0x00FF & (sx + width - 1));

    LCD_SelectReg(lcddev.setycmd);
    LCD_WriteByte(sy >> 8);
    LCD_WriteByte(0x00FF & sy);
    LCD_WriteByte((sy + height - 1) >> 8);
    LCD_WriteByte(0x00FF & (sy + height - 1));

    LCD_WriteRAM_Prepare();	//��ʼд��GRAM
}

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_SetWindow(Xpos, Ypos, 1, 1);
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
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    lcddev.wramcmd = 0x2C;
    switch (direction)
    {
        case 0:
            lcddev.width = LCD_W;
            lcddev.height = LCD_H;
            LCD_WriteReg(0x36, (1 << 3) | (0 << 6) | (0 << 7)); //BGR==1,MY==0,MX==0,MV==0
            break;
        case 1:
            lcddev.width = LCD_H;
            lcddev.height = LCD_W;
            LCD_WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
            break;
        case 2:
            lcddev.width = LCD_W;
            lcddev.height = LCD_H;
            LCD_WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 7)); //BGR==1,MY==0,MX==0,MV==0
            break;
        case 3:
            lcddev.width = LCD_H;
            lcddev.height = LCD_W;
            LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
            break;
        default:
            break;
    }
}

void LCD_ShowPicture(u16 x, u16 y, u16 width, u16 height, const u8 *pic)
{
    u16 temp = 0;
    long tmp = 0, num = 0;
    
    LCD_SetWindow(x, y, width, height);
    
    num = width * height * 2 ;
    do
    {
        temp = pic[tmp + 1];
        temp = temp << 8;
        temp = temp | pic[tmp];
        LCD_WriteWord(temp);//�����ʾ
        tmp += 2;
    } while (tmp < num);
}


