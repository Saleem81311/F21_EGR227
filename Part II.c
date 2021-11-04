/************************************************************************************************************************
 * Name: Saleem Griggs-Taylor, Kevin Figurski, and Tennison Hoffmann
 * Start Date: 11/3/21
 * Last modified: 11/3/21
 * Description: This code is made to control a PWM motor with buttons and display numbers to a 7-segment LED
 ************************************************************************************************************************/
#include "msp.h"
#include <stdlib.h>
#include <stdio.h>

void Pin_init(void);
void SysTick_Init(void);
void delay_sec(void);
void LCD_countdownUp (void);

void main(void)
    {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    SysTick_Init();
    Pin_init();

    NVIC_EnableIRQ(PORT6_IRQn);  // set up to run code
    __enable_interrupts();

    while(1)
    {
        LCD_countdownUp();
    }

}

void Pin_init(void){
    P4 -> SEL0 &=~ 0xFF; //initializes LED pins as GP
    P4 -> SEL1 &=~ 0xFF; //Pins 4.0-4.7 used, all P4 pins setup
    P4 -> DIR |= 0xFF;
    P4 -> OUT &=~ 0xFF;
}

void SysTick_Init(void){
    SysTick->CTRL = 0;
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
}
void delay_sec(void)
{
    SysTick -> LOAD = 3000000-1; //3MHz clock, 1 sec = 30000000 cycles
    SysTick -> VAL = 0;
    while((SysTick -> CTRL & 0x00010000)==0); //wait for flag to be set
}


void LCD_countdownUp (void){

    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b0111111; //0
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b0000110; //1
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b1011011; //2
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b1001111; //3
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b1100110; //4
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b1101101; //5
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b1111101; //6
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b0000111; //7
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b1111111; //8
    delay_sec();
    P4->OUT &=~ 0xFF;
    P4->OUT |= 0b1100111; //9
    delay_sec();
}
