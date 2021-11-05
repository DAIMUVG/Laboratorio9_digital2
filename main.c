
///************
//librerias
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
//*************

/**
 * main.c
 */
#define XTAL 1600000

void initGPIOF (void);
unsigned char letra;
uint8_t bandera;
uint8_t bandera1;
uint8_t cont;
uint8_t Push;
void Timer0Config(void);
int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Reloj de 40MHz
    initGPIOF();
    Timer0Config(); // Configuracion del TIMER 0 //configuracion de leds y puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R = 0x0f;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);


    while (1){
        Push = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
        if(Push == 0)
             {
                 bandera1 = 1;
             }else{
                 bandera1 = 0;
             }

        if(cont > 3 & bandera1 == 1 ){
             cont = 0;
           }
    }
}

void Timer0Config(void){

    uint32_t ui32Period;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
    {
    }
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    ui32Period = ( SysCtlClockGet() ) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);


    IntEnable(INT_TIMER0A);   // Configuro las interrupciones
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Configuro las interrupciones
    IntMasterEnable();   // Configuro las interrupciones


    TimerEnable(TIMER0_BASE, TIMER_A); // timers.
}

//********Interrupcion de TIMER 0*****************
void Timer0IntHandler(void){
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);


        if(cont == 0)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
        }
        if(cont == 1)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        }
        if(cont == 2)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
        }
        cont = cont+1;
    }


//**********INICIALIZA PUERTO F Y LEDS OUTPUTS-------------------
void initGPIOF (void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

}
