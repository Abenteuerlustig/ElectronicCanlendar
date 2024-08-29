/*
 * oled.c
 *
 *  Created on: 2023年6月26日
 *      Author: dell
 */

#include "oled.h"
#include "string.h"
#include "stdlib.h"

/*
===========================
全局变量定义
===========================
*/
//OLED缓存128*64bit
// 由数据手册得知SSD1306有8页，将屏幕纵向分成64 / 8 = 8个块
//每页有128个字节竖向排列代表128列
//
//以上图页2为例，要点亮第2页第0行第0列（屏幕16行0列）交叉的像素时，向第2页第0列写入0x01;
//要点亮第2页第5行第4列（屏幕21行4列）交叉的像素时，向第2页第4列写入0x20;
static uint8_t g_oled_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
//OLED实时信息
static SSD1306_t oled;
//OLED是否正在显示，1显示，0等待
static unsigned char is_show_str =0;
/*
===========================
函数定义
===========================
*/


/**
 * 向oled写命令
 * @param[in]   command
 * @retval
 */

void oled_write_cmd(uint8_t command)
{
    HAL_I2C_Mem_Write(&hi2c1,OLED_WRITE_ADDR,WRITE_CMD,I2C_MEMADD_SIZE_8BIT,&command,1,100);
}

/**
 * 向oled写数据
 * @param[in]   data
 * @retval
 *              - ESP_OK
 * @par         修改日志
 *               Ver0.0.1:
                     XinC_Guo, 2018/07/18, 初始化版本\n
 */
void oled_write_data(uint8_t data)
{
    HAL_I2C_Mem_Write(&hi2c1,OLED_WRITE_ADDR,WRITE_DATA,I2C_MEMADD_SIZE_8BIT,&data,1,100);
}
/**
 * 向oled写长数据
 * @param[in]   data   要写入的数据
 * @param[in]   len     数据长度
 * @retval
 *              - ESP_OK
 * @par         修改日志
 *               Ver0.0.1:
                     XinC_Guo, 2018/07/18, 初始化版本\n
 */
void oled_write_long_data(uint8_t *data,uint16_t len)
{
    HAL_I2C_Mem_Write(&hi2c1,OLED_WRITE_ADDR,WRITE_DATA,I2C_MEMADD_SIZE_8BIT,data,len,100);
}

/**
 * 初始化 oled
 * @param[in]   NULL
 * @retval
 *              NULL
 * @par         修改日志
 *               Ver0.0.1:
                     XinC_Guo, 2018/07/18, 初始化版本\n
 */
void oled_init(void)
{

    //oled配置
    oled_write_cmd(TURN_OFF_CMD);
    oled_write_cmd(0xAE);//关显示
    oled_write_cmd(0X20);//低列地址
    oled_write_cmd(0X10);//高列地址
    oled_write_cmd(0XB0);//
    oled_write_cmd(0XC8);
    oled_write_cmd(0X00);
    oled_write_cmd(0X10);
    //设置行显示的开始地址(0-63)
    //40-47: (01xxxxx)
    oled_write_cmd(0X40);
    //设置对比度
    oled_write_cmd(0X81);
    oled_write_cmd(0XFF);//这个值越大，屏幕越亮(和上条指令一起使用)(0x00-0xff)

    oled_write_cmd(0XA1);//0xA1: 左右反置，  0xA0: 正常显示（默认0xA0）
    //0xA6: 表示正常显示（在面板上1表示点亮，0表示不亮）
    //0xA7: 表示逆显示（在面板上0表示点亮，1表示不亮）
    oled_write_cmd(0XA6);

    oled_write_cmd(0XA8);//设置多路复用率（1-64）
    oled_write_cmd(0X3F);//（0x01-0x3f）(默认为3f)
    oled_write_cmd(0XA4);
    //设置显示抵消移位映射内存计数器
    oled_write_cmd(0XD3);
    oled_write_cmd(0X00);
    //设置显示时钟分频因子/振荡器频率
    oled_write_cmd(0XD5);
    //低4位定义显示时钟(屏幕的刷新时间)（默认：0000）分频因子= [3:0]+1
    //高4位定义振荡器频率（默认：1000）
    oled_write_cmd(0XF0);
    //时钟预充电周期
    oled_write_cmd(0XD9);
    oled_write_cmd(0X22);
    //设置COM硬件应脚配置
    oled_write_cmd(0XDA);
    oled_write_cmd(0X12);
    oled_write_cmd(0XDB);
    oled_write_cmd(0X20);
    //电荷泵设置（初始化时必须打开，否则看不到显示）
    oled_write_cmd(0X8D);
    oled_write_cmd(0X14);
    //开显示
    oled_write_cmd(0XAF);

}

/**
 * 将显存内容刷新到oled显示区
 * @param[in]   NULL
 * @retval
 *              NULL
 * @par         修改日志
 *               Ver0.0.1:
                     XinC_Guo, 2018/07/18, 初始化版本\n
 */
void oled_update_screen(void)
{
    uint8_t line_index;
    for(line_index=0    ;   line_index<8   ;  line_index++)
    {
        oled_write_cmd(0xb0+line_index);
        oled_write_cmd(0x00);
        oled_write_cmd(0x10);

        //数组设置的时候是按一行一行设置，但是写入的时候是按1列8行为一个字节写入
        oled_write_long_data(&g_oled_buffer[SSD1306_WIDTH * line_index],SSD1306_WIDTH);
    }
}


/**
 * 清屏
 * @param[in]   NULL
 * @retval
 *              NULL
 * @par         修改日志
 *               Ver0.0.1:
                     XinC_Guo, 2018/07/18, 初始化版本\n
 */

void oled_clear(void)
{
    //清0缓存
    memset(g_oled_buffer,SSD1306_COLOR_BLACK,sizeof(g_oled_buffer));
    oled_update_screen();
}
/**
 * 填屏
 * @param[in]   NULL
 * @retval
 *              NULL
 * @par         修改日志
 *               Ver0.0.1:
                     XinC_Guo, 2018/07/18, 初始化版本\n
 */
void oled_all_on(void)
{
    //置ff缓存
    memset(g_oled_buffer,0xff,sizeof(g_oled_buffer));
    oled_update_screen();
}
/**
 * 移动坐标
 * @param[in]   x   显示区坐标 x
 * @param[in]   y   显示去坐标 y
 * @retval
 *              其它
 * @par         修改日志
 *               Ver0.0.1:
                     XinC_Guo, 2018/07/18, 初始化版本\n
 */
void oled_gotoXY(uint16_t x, uint16_t y)
{
    oled.CurrentX = x;
    oled.CurrentY = y;
}
/**
 * 向显存写入
 * @param[in]   x   坐标
 * @param[in]   y   坐标
 * @param[in]   color   色值0/1
 * @retval
 */
void oled_drawpixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color)
{
    if (
            x >= SSD1306_WIDTH ||
            y >= SSD1306_HEIGHT
            )
    {
        return;
    }
    if (color == SSD1306_COLOR_WHITE)
    {
        g_oled_buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);//保留同一列的其他数字，值其中一位为1
    }
    else
    {
        g_oled_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}
/**
 * 在x，y位置显示字符
 * @param[in]   x    显示坐标x
 * @param[in]   y    显示坐标y
 * @param[in]   ch   要显示的字符
 * @param[in]   font 显示的字形
 * @param[in]   color 颜色  1显示 0不显示
 * @retval
*/
char oled_show_char(uint16_t x,uint16_t y,char ch, FontDef_t* Font, SSD1306_COLOR_t color)
{
    uint32_t i, b, j;
    if ( SSD1306_WIDTH <= (oled.CurrentX + Font->FontWidth) || SSD1306_HEIGHT <= (oled.CurrentY + Font->FontHeight) )
    {
        return 0;
    }
    if(0 == is_show_str)
    {
        oled_gotoXY(x,y);
    }

    for (i = 0; i < Font->FontHeight; i++)
    {

        b = DATA [(ch - 32) * Font->FontHeight + i];
        for (j = 0; j < Font->FontWidth; j++)
        {
            if ((b << j) & 0x80)
            {
                oled_drawpixel(oled.CurrentX + j, (oled.CurrentY + i), (SSD1306_COLOR_t) color);
            }
            else
            {
                oled_drawpixel(oled.CurrentX + j, (oled.CurrentY + i), (SSD1306_COLOR_t)!color);
            }
        }
    }
    oled.CurrentX += Font->FontWidth;
    if(0 == is_show_str)
    {
        //oled_update_screen();
    }
    return ch;
}

/**
 * 在x，y位置显示汉字
 * @param[in]   x    显示坐标x
 * @param[in]   y    显示坐标y
 * @param[in]   ch   要显示的字符
 * @param[in]   font 显示的字形
 * @param[in]   color 颜色  1显示 0不显示
 * @retval
*/
char oled_show_china(uint16_t x,uint16_t y,int ch, SSD1306_COLOR_t color)
{
    uint32_t i, b, j;

	  if(0 == is_show_str)
    {
        oled_gotoXY(x,y);
    }
    if ( SSD1306_WIDTH <= (oled.CurrentX + 16) || SSD1306_HEIGHT <= (oled.CurrentY + 16) )
    {
        return 0;
    }

    for (i = 0; i < 16; i++)
    {

        b = hanzi [ch*32 + i*2];
        for (j = 0; j < 8; j++)
        {
            if ((b << j) & 0x80)
            {
                oled_drawpixel(oled.CurrentX + j, (oled.CurrentY + i), (SSD1306_COLOR_t) color);
            }
            else
            {
                oled_drawpixel(oled.CurrentX + j, (oled.CurrentY + i), (SSD1306_COLOR_t)!color);
            }
        }
				b = hanzi [ch*32 + i*2 +1];
				for (j = 0; j < 8; j++)
        {
            if ((b << j) & 0x80)
            {
                oled_drawpixel(oled.CurrentX + j + 8, (oled.CurrentY + i), (SSD1306_COLOR_t) color);
            }
            else
            {
                oled_drawpixel(oled.CurrentX + j + 8, (oled.CurrentY + i), (SSD1306_COLOR_t)!color);
            }
        }
    }
    oled.CurrentX += 16;
    if(0 == is_show_str)
    {
        //oled_update_screen();
    }
    return ch;
}

/**
 * 在x，y位置显示字符串
 * @param[in]   x    显示坐标x
 * @param[in]   y    显示坐标y
 * @param[in]   str   要显示的字符串
 * @param[in]   font 显示的字形
 * @param[in]   color 颜色  1显示 0不显示
 * @retval
 */
char oled_show_str(uint16_t x,uint16_t y, char* str, FontDef_t* Font, SSD1306_COLOR_t color)
{
    is_show_str=1;
    oled_gotoXY(x,y);
    while (*str)
    {
        if (oled_show_char(x,y,*str, Font, color) != *str)
        {
            is_show_str=0;
            return *str;
        }
        str++;
    }
    is_show_str=0;
    //oled_update_screen();
    return *str;
}

/**************************************************************************
Function: Find m to the NTH power
Input   : m: base number, n: power number
Output  : none
函数功能：求m的n次方的函数
入口参数：m：底数，n：次方数
返回  值：无
**************************************************************************/
unsigned int oled_pow(unsigned char m,unsigned char n)
{
    unsigned int result=1;
    while(n--)result*=m;
    return result;
}
/**************************************************************************
Function: Displays 2 numbers
Input   : x,y: starting coordinate;Len: The number of digits;Size: font size;Mode: mode, 0, fill mode, 1, overlay mode;Num: value (0 ~ 4294967295);
Output  : none
函数功能：显示2个数字
入口参数：x,y :起点坐标; len :数字的位数; size:字体大小; mode:模式, 0,填充模式, 1,叠加模式; num:数值(0~4294967295);
返回  值：无
**************************************************************************/
void OLED_ShowNumber(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size)
{
    unsigned char t,temp;
    unsigned char enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                oled_show_char(x+(size/2)*t,y,' ',&fontone,SSD1306_COLOR_WHITE);
                continue;
            }else enshow=1;

        }
        oled_show_char(x+(size/2)*t,y,temp+'0',&fontone,SSD1306_COLOR_WHITE);
    }
}

