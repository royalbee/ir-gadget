#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"

#define IR_CARRIER 38400

int main(void) {
    unsigned long ir_period;

    // 40 MHz system clock
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|
        SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Configure PB6 as T0CCP0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
    GPIOPinConfigure(GPIO_PF1_T0CCP1);
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_1);

    // Configure timer
    ir_period = SysCtlClockGet() / (IR_CARRIER / 2);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER0_BASE, TIMER_B, ir_period);
    TimerMatchSet(TIMER0_BASE, TIMER_B, ir_period / 2); // PWM
    TimerEnable(TIMER0_BASE, TIMER_B);

    while(1) {
        SysCtlDelay(50000);
    }
}
