#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

// Edit these
#define LCD_DDR  DDRD
#define LCD_PORT PORTD

#define LCD_RS 0	//RS - PD0
#define LCD_RW 3	//RW - PD1
#define LCD_EN 1	//EN - PD3
#define LCD_D4 4	//D4 - PD4
#define LCD_D5 5	//D5 - PD5
#define LCD_D6 6 	//D6 - PD6
#define LCD_D7 7	//D7 - PD7

#define LCD_COL_COUNT 16
#define LCD_ROW_COUNT 2

// The rest should be left alone
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS  0x00

void LCD_Init(void);

void LCD_Command(uint8_t command);
void lcd_write(uint8_t value);

void LCD_On(void);
void LCD_Off(void);

void LCD_Clear(void);
void LCD_Return_Home(void);

void LCD_Enable_Blinking(void);
void LCD_Disable_Blinking(void);

void LCD_Enable_Cursor(void);
void LCD_Disable_Cursor(void);

void LCD_Scroll_Left(void);
void LCD_Scroll_Right(void);

void LCD_Set_Left_to_Right(void);
void LCD_Set_Right_to_Left(void);

void LCD_Enable_Autoscroll(void);
void LCD_Disable_Autoscroll(void);

void LCD_Create_Char(uint8_t location, uint8_t *charmap);

void LCD_Set_Cursor(uint8_t col, uint8_t row);

void LCD_Puts(char *string);
void LCD_Printf(char *format, ...);

#endif
