/*
 * Joule Juggler V2: SysTick.c
 *
 * Latest Revision on: Mar 19, 2025
 *      Author: Yolie Reyes
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

// The Layout of Ports on the Tiva TM4C123G
//             b31 ... b6 || b5  b4  b3  b2  b1  b0
//              RESERVED  || F   E   D   C   B   A
//                  0x...    20  10  08  04  02  01
//                               ADCs
//                                       Cycling
//                                           LCD

// The Layout of Pins for a GPIO Port  -----------
//         b7  b6  b5  b4 ||  b3  b2  b1  b0     |
// Pin      7   6   5   4 ||   3   2   1   0     |
//                   0000 || 0000                |
//                        ||                     |
//                        ||                     |
// Control:               ||ADC0  ADC1 ADC0      |
//                        ||AIN0  AIN1 AIN2      |
//  ADCs                  ||Volt  Amp  Temp      |
//  Cycling        -   +  ||                     |
//  LCD    D7  D6  D5  D4 ||       En  R/W  RS   |
//                        ||                     |
//                        ||                     |


// The layout of the 16x2 LCD addresses
//
//       1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
//
// 0x... 80  81  82  83  84  85  86  87  88 89  8A  8B  8C  8D  8E  8F  LINE 1
// 0x... C0  C1  C2  C3  C4  C5  C6  C7  C8 C9  CA  CB  CC  CD  CE  CF  LINE 2

// Initialize SysTick with busy wait running at bus clock.

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
}

// Time delay using busy wait.
// The delay parameter is in units of the core clock.
void SysTick_Wait(uint32_t delay){
    // number of counts to wait;
        NVIC_ST_RELOAD_R = delay - 1;
        NVIC_ST_CTRL_R = 5;
      // clear previous setting;
        NVIC_ST_CURRENT_R = 0;
    // wait for the count flag
        while ((NVIC_ST_CTRL_R & 0x10000) == 0 ) {}
}

void SysTick_Wait200ms(uint32_t delay){
  // local variable to control the for loop
    uint32_t j;
  for(j = 0; j < delay; j++){
      // wait 200ms (assumes 50 MHz clock)
        SysTick_Wait(10000000);
  }
}

void SysTick_Wait08ms(uint32_t delay){
  // local variable to control the for loop
    uint32_t j;
  for(j = 0; j < delay; j++){
      // wait 0.8ms (assumes 50 MHz clock)
        SysTick_Wait(40000);  // 0.8 / 20*10^-7
  }
}

void SysTick_Wait02ms(uint32_t delay){
  // local variable to control the for loop
    uint32_t j;
  for(j = 0; j < delay; j++){
      // wait 02ms (assumes 50 MHz clock)
        SysTick_Wait(10000); //0.2 / 20*10^-7
  }
}

void SysTick_Wait50ms(uint32_t delay){
  // local variable to control the for loop
    uint32_t j;
  for(j = 0; j < delay; j++){
      // wait 50ms (assumes 50 MHz clock)
        SysTick_Wait(25000000); // 50 / 20*10^-7
  }
}

