#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"

#include "utils/uartstdio.h"

#define IR_CARRIER 38400

void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

#define SW1 4
int on = 0;

#define IR_PORT GPIO_PORTA_BASE
#define IR_PIN GPIO_PIN_2

void ir_input_isr(void)
{
	GPIOIntClear(IR_PORT, IR_PIN);
	on ^= 1;
	UARTprintf("on:%d\n", on);
}

int main(void) {
    unsigned long ir_period;

    // 40 MHz system clock
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|
        SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	ConfigureUART();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    //
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

	// Interrupt on sw1
    GPIODirModeSet(IR_PORT, IR_PIN, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(IR_PORT, IR_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
	GPIOPinTypeGPIOInput(IR_PORT, IR_PIN);
	GPIOIntTypeSet(IR_PORT, IR_PIN, GPIO_FALLING_EDGE);
	GPIOIntRegister(IR_PORT, ir_input_isr);
	GPIOIntEnable(IR_PORT, IR_PIN );

	IntMasterEnable();

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
    GPIOPinConfigure(GPIO_PF1_T0CCP1);
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_1);

    // Configure timer
    ir_period = SysCtlClockGet() / (IR_CARRIER / 2);

	UARTprintf("ir_period:%d\n", ir_period);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER0_BASE, TIMER_B, ir_period);
    TimerMatchSet(TIMER0_BASE, TIMER_B, ir_period / 2); // PWM
    TimerEnable(TIMER0_BASE, TIMER_B);

	unsigned long m = 0;
    while(1) {
    TimerMatchSet(TIMER0_BASE, TIMER_B, m++); // PWM
	if (m > ir_period) m = 0;
        SysCtlDelay(5000);
    }
}
