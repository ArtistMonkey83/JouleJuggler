/*
 * Joule Juggler V2: main.c
 *
 *  Latest Revision on: Mar 19, 2025
 *              Author: Yolie Reyes
 */

#include <JouleDriver.h>
#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"

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


/**
 * main.c
 */


int main(void)
{
    SysTick_Init();             // Initialization of SysTick for delays
    PLL_Init();                 // Initialization of Phase Lock Loop

    LCD4bits_Init();            // Initialization of Display in 4 bit mode

    //SysTick_Wait50ms(2);        // Short Delay VALUES = {1,20,100,80
    delayMs(500);
    LCDon();
    delayMs(500);

    Timer0A_Init();             // LEDoff should be called at some point... 1 second if my math is correct
}
