#include <stdbool.h> /*driverlib header requires stdbool.h to be included a first header file before any driverlib header*/
#include <stdint.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>/*supplies GPIO_PIN_x*/
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>/*supplies GPIO_PORTx_BASE*/
/*Controller is initially clocked with 16MHz*/
#define F_CPU 16000000

/*
 * main.c
 */
void main(void) {
    uint32_t ui32Strength;
    uint32_t ui32PinType;
    uint8_t ui8val = 1;
    volatile uint32_t ui32Loop = 0;

    /*activate gpio port n*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    /*configure GPIO_PIN_1 on port N as std pin*/
    GPIOPadConfigGet(GPIO_PORTN_BASE, GPIO_PIN_1, &ui32Strength, &ui32PinType);
    GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_1,ui32Strength,GPIO_PIN_TYPE_STD);

    /*set pin 1 of gpio port n to output*/
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE,GPIO_PIN_1);

	while(1)
	{
	    ui8val = ui8val ^ 2;/*toggle bit 1 in variable val*/
        /*write value of val to pin 1 of port n*/
        GPIOPinWrite (GPIO_PORTN_BASE, GPIO_PIN_1, 0);
        // for(ui32Loop = 0; ui32Loop < 400000; ui32Loop++){}
        SysCtlDelay(F_CPU/6);  //approx 0,5 s
	}
}
