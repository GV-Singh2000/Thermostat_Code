/* 
 * File:   lcd.h
 * Author: Gurvansh Singh
 *
 * Created on November 30, 2021, 1:45 PM
 */

/*
 * Initialize LCD controller.  Performs a software reset.
 */
void	lcd_init(void);

/*
 * Send one character to the LCD.
 */
int	lcd_putchar(char c, FILE *stream);