#include "lcd.h"

#define F_CPU 8000000UL
#include <stdarg.h>
#include <stdio.h>
#include <util/delay.h>

void LCD_Send(uint8_t value, uint8_t mode);
void LCD_Write_Nibble(uint8_t nibble);

static uint8_t lcd_displayparams;
static char lcd_buffer[LCD_COL_COUNT + 1];

void LCD_Command(uint8_t command) {
  LCD_Send(command, 0);
}

void LCD_Write(uint8_t value) {
  LCD_Send(value, 1);
}

void LCD_Send(uint8_t value, uint8_t mode) {
  if (mode) {
    LCD_PORT = LCD_PORT | (1 << LCD_RS);
  } else {
    LCD_PORT = LCD_PORT & ~(1 << LCD_RS);
  }

  LCD_PORT = LCD_PORT & ~(1 << LCD_RW);

  LCD_Write_Nibble(value >> 4);
  LCD_Write_Nibble(value);
}

void LCD_Write_Nibble(uint8_t nibble) {
  LCD_PORT = (LCD_PORT & 0xff & ~(0x0f << LCD_D4)) | ((nibble & 0x0f) << LCD_D4);

  LCD_PORT = LCD_PORT & ~(1 << LCD_EN);
  LCD_PORT = LCD_PORT | (1 << LCD_EN);
  LCD_PORT = LCD_PORT & ~(1 << LCD_EN);
  _delay_ms(0.3);																// If delay less than this value, the data is not correctly displayed  
}

void LCD_Init(void) {
  // Configure pins as output
  LCD_DDR = LCD_DDR
    | (1 << LCD_RS)
    | (1 << LCD_RW)
    | (1 << LCD_EN)
    | (1 << LCD_D4)
    | (1 << LCD_D5)
    | (1 << LCD_D6)
    | (1 << LCD_D7);

  // Wait for LCD to become ready (docs say 15ms+)
  _delay_ms(15);

  LCD_PORT = LCD_PORT
    & ~(1 << LCD_EN)
    & ~(1 << LCD_RS)
    & ~(1 << LCD_RW);

  _delay_ms(4.1);

  LCD_Write_Nibble(0x03); // Switch to 4 bit mode
  _delay_ms(4.1);

  LCD_Write_Nibble(0x03); // 2nd time
  _delay_ms(4.1);

  LCD_Write_Nibble(0x03); // 3rd time
  _delay_ms(4.1);

  LCD_Write_Nibble(0x02); // Set 8-bit mode (?)

  LCD_Command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);

  lcd_displayparams = LCD_CURSOROFF | LCD_BLINKOFF;
  LCD_Command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void LCD_On(void) {
  lcd_displayparams |= LCD_DISPLAYON;
  LCD_Command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void LCD_Off(void) {
  lcd_displayparams &= ~LCD_DISPLAYON;
  LCD_Command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void LCD_Clear(void) {
  LCD_Command(LCD_CLEARDISPLAY);
  _delay_ms(2);
}

void LCD_Return_Home(void) {
  LCD_Command(LCD_RETURNHOME);
  _delay_ms(2);
}

void LCD_Enable_Blinking(void) {
  lcd_displayparams |= LCD_BLINKON;
  LCD_Command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void LCD_Disable_Blinking(void) {
  lcd_displayparams &= ~LCD_BLINKON;
  LCD_Command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void LCD_Enable_Cursor(void) {
  lcd_displayparams |= LCD_CURSORON;
  LCD_Command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void LCD_Disable_Cursor(void) {
  lcd_displayparams &= ~LCD_CURSORON;
  LCD_Command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void LCD_Scroll_Left(void) {
  LCD_Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCD_Scroll_Right(void) {
  LCD_Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCD_Set_Left_to_Right(void) {
  lcd_displayparams |= LCD_ENTRYLEFT;
  LCD_Command(LCD_ENTRYMODESET | lcd_displayparams);
}

void LCD_Set_Right_to_Left(void) {
  lcd_displayparams &= ~LCD_ENTRYLEFT;
  LCD_Command(LCD_ENTRYMODESET | lcd_displayparams);
}

void LCD_Enable_Autoscroll(void) {
  lcd_displayparams |= LCD_ENTRYSHIFTINCREMENT;
  LCD_Command(LCD_ENTRYMODESET | lcd_displayparams);
}

void LCD_Disable_Autoscroll(void) {
  lcd_displayparams &= ~LCD_ENTRYSHIFTINCREMENT;
  LCD_Command(LCD_ENTRYMODESET | lcd_displayparams);
}

void LCD_Create_Char(uint8_t location, uint8_t *charmap) {
  LCD_Command(LCD_SETCGRAMADDR | ((location & 0x7) << 3));
  for (int i = 0; i < 8; i++) {
    LCD_Write(charmap[i]);
  }
}

void LCD_Set_Cursor(uint8_t col, uint8_t row) {
  static uint8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };

  LCD_Command(LCD_SETDDRAMADDR | (col + offsets[row]));
}

void LCD_Puts(char *string) {
  for (char *it = string; *it; it++) {
    LCD_Write(*it);
  }
}

void LCD_Printf(char *format, ...) {
  va_list args;

  va_start(args, format);
  vsnprintf(lcd_buffer, LCD_COL_COUNT + 1, format, args);
  va_end(args);

  LCD_Puts(lcd_buffer);
}
