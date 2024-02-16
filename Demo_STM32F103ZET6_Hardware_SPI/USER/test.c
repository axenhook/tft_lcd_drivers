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
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "key.h"
#include "led.h"
#include "pic.h"

//========================variable==========================//
u16 ColorTab[5] = {RED, GREEN, BLUE, YELLOW, BRED}; //������ɫ����
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
    //���ƹ̶���up
    LCD_Clear(WHITE);
    gui_fill(0, 0, lcddev.width, 20, BLUE);
    //���ƹ̶���down
    gui_fill(0, lcddev.height - 20, lcddev.width, lcddev.height, BLUE);
    PAINT_COLOR = WHITE;
    gui_show_string_center(0, 2, WHITE, BLUE, str, 16, 1); //������ʾ
    gui_show_string_center(0, lcddev.height - 18, WHITE, BLUE, "http://www.lcdwiki.com", 16, 1); //������ʾ
    //���Ʋ�������
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
    DrawTestPage("ȫ�������ۺϲ��Գ���");
    gui_show_string_center(0, 30, RED, BLUE, "ȫ������", 16, 1); //������ʾ
    gui_show_string_center(0, 60, RED, BLUE, "�ۺϲ��Գ���", 16, 1); //������ʾ
    gui_show_string_center(0, 90, GREEN, BLUE, "2.4\" ILI9341 240X320", 16, 1); //������ʾ
    gui_show_string_center(0, 120, BLUE, BLUE, "xiaoFeng@QDtech 2018-08-20", 16, 1); //������ʾ
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
    //DrawTestPage("����1:��ɫ������");
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
    DrawTestPage("����2:GUI����������");
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
    DrawTestPage("����3:GUI��Բ������");
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
    DrawTestPage("����5:Ӣ����ʾ����");
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
    DrawTestPage("����4:GUI Triangle������");
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
    DrawTestPage("����6:������ʾ����");
    gui_show_string(10, 30, BLUE, YELLOW, "16X16:ȫ�����Ӽ������޹�˾��ӭ��", 16, 0);
    gui_show_string(10, 50, BLUE, YELLOW, "16X16:Welcomeȫ������", 16, 0);
    gui_show_string(10, 70, BLUE, YELLOW, "24X24:���������Ĳ���", 24, 1);
    gui_show_string(10, 100, BLUE, YELLOW, "32X32:�������", 32, 1);
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
    DrawTestPage("����7:ͼƬ��ʾ����");
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
        DrawTestPage("����8:��Ļ��ת����");
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
    DrawTestPage("����9:Touch(��KEY0У׼)      ");
    gui_show_ptstring(lcddev.width - 24, 0, 16, "RST", 1); //��ʾ��������
    PAINT_COLOR = RED;
    gui_fill(lcddev.width - 52, 2, lcddev.width - 50 + 20, 18, RED);
    while (1)
    {
        key = KEY_Scan();
        tp_dev.scan(0);
        if (tp_dev.sta & TP_PRES_DOWN)      //������������
        {
            if (tp_dev.x < lcddev.width && tp_dev.y < lcddev.height)
            {
                if (tp_dev.x > (lcddev.width - 24) && tp_dev.y < 16)
                {
                    DrawTestPage("����9:Touch(��KEY0У׼)      ");//���
                    gui_show_ptstring(lcddev.width - 24, 0, 16, "RST", 1); //��ʾ��������
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
                    gui_draw_big_point(tp_dev.x, tp_dev.y, PAINT_COLOR);    //��ͼ
                }
            }
        }
        else
        {
            delay_ms(10);    //û�а������µ�ʱ��
        }
        if (key == 1) //KEY_RIGHT����,��ִ��У׼����
        {

            LCD_Clear(WHITE);//����
            TP_Adjust();  //��ĻУ׼
            TP_Save_Adjdata();
            DrawTestPage("����9:Touch(��KEY0У׼)      ");
            gui_show_ptstring(lcddev.width - 24, 0, 16, "RST", 1); //��ʾ��������
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




