/* 
 * File:   defines.h
 * Author: Gurvansh Singh
 *
 * Created on November 30, 2021, 1:33 PM
 */

/* CPU frequency */
#define F_CPU 1000000UL

/* UART baud rate */
#define UART_BAUD  9600

/* HD44780 LCD port connections */
#define HD44780_RS D, 0
#define HD44780_RW D, 1
#define HD44780_E  D, 2
/* The data bits have to be not only in ascending order but also consecutive. */
#define HD44780_D4 D, 4

/* Whether to read the busy flag, or fall back to
   worst-time delays. */
#define USE_BUSY_BIT 1