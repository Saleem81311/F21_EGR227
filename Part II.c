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
void PORT6_IRQHandler(void);
void Timer_Init(void);
void Timer_PWM(int duty);
void LCD_init(void);
void SysTick_Init(void);
void delay_sec(void);
void LCD_countdownUp (void);


int dutyVar = 0;
void main(void)
    {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    
    SysTick_Init();
    Pin_init();
    Timer_Init();
    LCD_init();

    NVIC_EnableIRQ(PORT6_IRQn);  // set up to run code
    __enable_interrupts();

    while(1)
    {
  //      P4 -> OUT |= 0xFF; //pin test
    //  __delay_cycles(500000);
      //  P4 -> OUT &=~ 0xFF; //pin test
        //__delay_cycles(500000);
        LCD_countdownUp();
    }

}
void Timer_PWM(int duty){               //runs timer given the specified duty cycle

    TIMER_A0-> CTL = 0b1001010100;                      //Count up using smclk, clears TAOR register, /2
    TIMER_A0-> CCR[0] = 37500 - 1;                      //TimerA will count up to 37500-1
    if (duty == 0)
        TIMER_A0-> CCR[4] = 0;
    else
        TIMER_A0-> CCR[4] = (37500 * duty / 100) - 1;   //Sets the duty cycle.
    TIMER_A0-> CCTL[4] = 0b11100000;                    //reset/set mode
}


void Timer_Init(void){              //Initializes the timer
    TIMER_A0-> CTL = 0b1001010100;                      //Count up using smclk, clears TAOR register, /2
    TIMER_A0-> CCR[0] = 37500 - 1;                      //TimerA will count up to 37500-1
    TIMER_A0-> CCR[4] = 0;                              //motor off to start
    TIMER_A0-> CCTL[4] = 0b11100000;                    //reset/set mode
}


void Pin_init(void){
    P6-> SEL0 &=~ (BIT5|BIT1|BIT4);
    P6-> SEL1 &=~ (BIT5|BIT1|BIT4);
    P6-> DIR &=~ (BIT5|BIT1|BIT4);
    P6-> REN |= (BIT5|BIT1|BIT4); //pullup resistor
    P6-> OUT |= (BIT5|BIT1|BIT4);
    P6-> IE |= (BIT5|BIT1|BIT4); //enable interupts for P1.1
    P6-> IES |= (BIT5|BIT1|BIT4); // Set pin to interrupt to trigger when it goes high->low
    P6-> IFG &= ~(BIT5|BIT1|BIT4); //clear P1 interrupt flags

    P2->SEL0 |= BIT7;
    P2->SEL1 &=~ BIT7;
    P2->DIR |= BIT7;
}

void PORT6_IRQHandler(void){
    if(P6->IFG & BIT1){
        printf("B1 Pressed!\n");
        if (dutyVar > 90)
            dutyVar = 100;
        else
            dutyVar += 10;
        P6->IFG  &= ~BIT1;  //clear the flag
    }
    if(P6->IFG & BIT4){
        printf("B2 Pressed!\n");
        if (dutyVar < 10)
            dutyVar = 0;
        else
            dutyVar -= 10;
        P6->IFG  &= ~BIT4;  //clear the flag
    }
    if(P6->IFG & BIT5){
        printf("B3 Pressed!\n");
        dutyVar = 0;
        P6->IFG  &= ~BIT5;  //clear the flag
    }
    Timer_PWM(dutyVar);
    P6->IFG = 0;
}

void LCD_init(void){

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
