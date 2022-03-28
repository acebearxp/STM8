/* The simplest project
 * For testing compiler env is okay
 */
#include <stm8s.h>

#define LED_GPIO_PORT    (GPIOE)
#define LED_GPIO_PIN     (GPIO_PIN_5)

main()
{
    uint8_t u8Count = 0;

    CLK_LSICmd(ENABLE); // LSI@128kHz
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV128); // 1kHz
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_LSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

    GPIO_DeInit(LED_GPIO_PORT);
    GPIO_Init(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    
	while(TRUE){
        if(u8Count % 20 == 0){
            GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);
        }

        u8Count++;
    }
}