# A2TimBlink
Blink LED with internal TIM

+ The main clock is HSE@8.000MHz
+ CPU is working at 62.5kHz
+ TIM2 is working at 31.25kHz, each half second(15625 ticks) trigger an interrupt to flip GPIO