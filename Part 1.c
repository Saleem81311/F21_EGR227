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

int dutyVar = 0;
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Pin_init();
    Timer_Init();

    NVIC_EnableIRQ(PORT6_IRQn);  // set up to run code
    __enable_interrupts();

    while(1)
    {

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

/****************************************************************************************
/**
 * 7-Segment LED test code
 *

void SysTick_Init();
void SysTick_Delay(uint16_t delay);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	P2->SEL0 &= ~BIT5;
	P2->SEL1 &= ~BIT5;
	P2->DIR |= BIT5;
	P2->REN |= BIT5;
	P2->OUT &= ~BIT5;
    P3->SEL0 &= ~BIT0;
    P3->SEL1 &= ~BIT0;
    P3->DIR |= BIT0;
    P3->REN |= BIT0;
    P3->OUT &= ~BIT0;
	int val = 100000000000000000;
	int n = 5;

   /* for(i = 0; i < n; i++)
    {

    }

	while(1)
	{
	      P2->OUT |= 01111111;
	      //P3->OUT |= BIT0;
	      SysTick_Delay(3000);

	}

}

void SysTick_Init()
{
    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 32;
    SysTick->CTRL = 0x00000005;
}

void SysTick_Delay(uint16_t delay)
{
    SysTick->LOAD=((delay*3000)-1);
    SysTick->VAL = 5;
    while((SysTick->CTRL & 0x00010000)==0);
    **********************************************************************************************/
