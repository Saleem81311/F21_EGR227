/************************************************************************************************************************
 * Name: Saleem Griggs-Taylor, Kevin Figurski, and Tennison Hoffmann
 * Start Date: 11/3/21
 * Last modified: 11/3/21
 * Description: This code is made to control a PWM motor with buttons and display numbers to a 7-segment LED
 ************************************************************************************************************************/
#include "msp.h"
#include <math.h>

void Pin_init(void);
void PORT6_IRQHandler(void);
void Timer_Init(void);
void Timer_PWM(int duty);

int dutyVar = 0;
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

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
    P6-> SEL0 &=~ (BIT0|BIT1|BIT4);
    P6-> SEL1 &=~ (BIT0|BIT1|BIT4);
    P6-> DIR &=~ (BIT0|BIT1|BIT4);
    P6-> REN = (BIT0|BIT1|BIT4); //pullup resistor
    P6-> OUT = (BIT0|BIT1|BIT4);
    P6-> IES = (BIT0|BIT1|BIT4); // Set pin to interrupt to trigger when it goes high->low
    P6-> IE = (BIT0|BIT1|BIT4); //enable interupts for P1.1
    P6-> IFG = (BIT0|BIT1|BIT4); //clear P1 interrupt flags
    
    P2->SEL0 |= BIT6;
    P2->SEL1 &=~ BIT6;
    P2->DIR |= BIT6;
}

void PORT6_IRQHandler()
{
    if(P6->IFG & BIT0)
    {
        dutyVar += 10;
        P1->IFG  &= ~BIT0;  //clear the flag
    }
    if(P6->IFG & BIT1)
    {
        dutyVar -= 10;
        P1->IFG  &= ~BIT1;  //clear the flag
    }
    if(P6->IFG & BIT4){
    	dutyVar = 0;
	P6->IFG  &= ~BIT4;  //clear the flag
    }
    Timer_PWM(dutyVar);
    P6->IFG = 0;
}
