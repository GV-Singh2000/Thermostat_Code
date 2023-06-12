/* 
 * File:   Lab4.c
 * Author: Gurvansh Singh
 *
 * Created on November 30, 2021, 11:42 AM
 */

#define F_CPU 1000000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "defines.h"
#include "hd44780.h"
#include "lcd.h"

/*
 * 
 */


float Numi, Num1i, Num2i, Nume, Num1e; // initializing variables
int user_temp = 15, Control = 0, arrow_direc = 0;
;


FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE); // Using stdio stream routine FOR LCD use

void internal() {


    ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
    ADMUX = 0;
    ADMUX |= (1 << REFS1) | (1 << REFS0) | (1 << MUX3);
    ADCSRA |= (1 << ADSC);

    while (!(ADCSRA & (1 << ADIF))) {
    };

    Numi = ADC;
    Num1i = ((Numi * 1.1) / 10.24);
    Num2i = ((4.4 * Numi) - 1530.2);

} // Configuration and calibration for internal sensor

void external() {


    ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
    ADMUX = 0;
    ADMUX |= (1 << REFS1) | (1 << REFS0);
    ADCSRA |= (1 << ADSC);

    while (!(ADCSRA & (1 << ADIF))) {
    };

    Nume = ADC;
    Num1e = ((Nume * 1.1) / 10.24);

} // Configuration and calibration for external sensor

void deadband() {



    if (arrow_direc == 0) {

        if (user_temp > (Num2i + 0.5)) {

            fprintf(& lcd_str, "On \xb1\x80");
            PORTB |= (1 << PB0);
            Control = 0;

        } else if (user_temp < (Num2i - 0.5)) {

            fprintf(& lcd_str, "Off \xb1\x80");
            PORTB &= ~(1 << PORTB0);
            Control = 1;

        } else {
            if (Control == 1) {
                fprintf(& lcd_str, "On \xb1\x80");
                PORTB |= (1 << PB0);
            } else {
                fprintf(& lcd_str, "Off\xb1\x80");
                PORTB &= ~(1 << PORTB0);
            }

        }

    }

    if (arrow_direc == 1) {

        if (user_temp > (Num1e + 0.5)) {

            fprintf(& lcd_str, "On \xb1\x80");
            PORTB |= (1 << PB0);
            Control = 0;
        } else if (user_temp < (Num1e - 0.5)) {

            fprintf(& lcd_str, "Off\xb1\x80");
            PORTB &= ~(1 << PORTB0);
            Control = 1;
        } else {
            if (Control == 1) {
                fprintf(& lcd_str, "On \xb1\x80");
                PORTB |= (1 << PB0);
            } else {
                fprintf(& lcd_str, "Off\xb1\x80");
                PORTB &= ~(1 << PORTB0);
            }

        }
    }


} // Implementing dead band control

void LCD_display() {

    fprintf(& lcd_str, "%3.1f\xDF", Num2i);
    fprintf(& lcd_str, "C");

    if (arrow_direc == 0) {

        fprintf(& lcd_str, " <- ");

    } else {

        fprintf(& lcd_str, " -> ");

    }

    fprintf(& lcd_str, "%3.1f\xDF", Num1e);
    fprintf(& lcd_str, "C\x1b\xc0      ");

    fprintf(& lcd_str, "%2d\xDF", user_temp);
    fprintf(& lcd_str, "C   ");

} // Formatting our information on LCD

int sw1() // Function for Switch/Button 1 in Pressed State
{
    int x;

    x = !(PINC & (1 << PINC3));
    return (x);
}

int sw2() // Function for Switch/Button 2 in Pressed State
{
    int x;

    x = !(PINC & (1 << PINC4));
    return (x); // returns an identity/number for 
}

int sw3() // Function for Switch/Button 3 in Pressed State
{
    int x;

    x = !(PINC & (1 << PINC5));
    return (x);
}

int main(void) {


    DDRB |= (1 << DDB0); // Setting LED as output

    DDRC &= ~(1 << DDC5); // Setting Switches as input
    DDRC &= ~(1 << DDC4);
    DDRC &= ~(1 << DDC3);

    PORTB &= ~(1 << PORTB0); // Initializing our LED

    PORTC |= (1 << PORTC5); // Turning on the Pull-up Network for Switches 3,2 and 1 
    PORTC |= (1 << PORTC4);
    PORTC |= (1 << PORTC3);



    lcd_init();

    PCMSK1 |= (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13); // initializing interrupts
    sei();


    while (1) {


        PCICR |= (1 << PCIE1); // Turn on Interrupt again
        fprintf(&lcd_str, "\x1b\x80");

        internal(); // Initializing our sensors
        external();

        LCD_display();
        deadband();

    }
    return (0);
}

ISR(PCINT1_vect) {

    PCICR &= ~(1 << PCIE1); // Turn off Interrupt to avoid double toggling when using a switch

    if (sw1()) {

        if (user_temp < 35) { // Applying Constraints for User Temperature Setting
            user_temp++;
        }
    } // Incrementing and decrementing when pressing switch 1 & 2
    if (sw2()) {

        if (user_temp > 10) {
            user_temp--;
        }
    }
    if (sw3()) {

        arrow_direc ^= 1; // Toggling Arrow direction
    }

};

