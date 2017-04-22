#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#define SEQ 3
void InitConsole(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 16000000);
}

void ADC_InitHW(void) {
    uint32_t ui32Strength;
    uint32_t ui32PinType;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // ADC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // GPIO Pin PE3
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
//
//    GPIOPadConfigGet(GPIO_PORTL_BASE, GPIO_PIN_3, &ui32Strength, &ui32PinType);
//    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_3, ui32Strength, GPIO_PIN_TYPE_STD);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); // Configure GPIO Pin PE3 as an ADC pin
//    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE,GPIO_PIN_3);

    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL, ADC_CLOCK_RATE_FULL);
    ADCSequenceDisable(ADC0_BASE, SEQ);
    ADCSequenceConfigure(ADC0_BASE, SEQ, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, SEQ, 0,
                    ADC_CTL_IE | // Issue interrupt upon completion
                    ADC_CTL_END | // End of sample sequence
                    ADC_CTL_CH0 // Sample the value of Channel 0
                    );
    ADCSequenceEnable(ADC0_BASE, SEQ);

//    GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, 0);
}

void ADC_InitInt(void) {
    ADCIntClear(ADC0_BASE, SEQ);
    ADCIntEnable(ADC0_BASE, SEQ);
}

int main(void)
{
    uint32_t ui32SysClock;
    uint32_t pui32ADC0Value[1];

    ui32SysClock = SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 10*1000*1000);

    InitConsole();
    UARTprintf("ADC ->\n");
    UARTprintf("  Type: Single Ended\n");
    UARTprintf("  Samples: One\n");
    UARTprintf("  Update Rate: 250ms\n");
    UARTprintf("  Input Pin: AIN0/PE3\n\n");
    
    ADC_InitHW();
    ADC_InitInt();

    while(1)
    {

        ADCProcessorTrigger(ADC0_BASE, SEQ);

        while(!ADCIntStatus(ADC0_BASE, SEQ, false))
        {
        }

        ADCIntClear(ADC0_BASE, SEQ);

        ADCSequenceDataGet(ADC0_BASE, SEQ, pui32ADC0Value);

        UARTprintf("AIN0 = %4d\r", pui32ADC0Value[0]);

        SysCtlDelay(ui32SysClock / 100);
    }
}
