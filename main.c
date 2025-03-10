/*
 * Joule Juggler V1: main.c
 *
 *  Latest Revision on: Feb 16, 2025
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
//                           Charging        LCD
//                                   Discharging

// The Layout of Pins for a GPIO Port  -----------
//         b7  b6  b5  b4 ||  b3  b2  b1  b0     |
// Pin      7   6   5   4 ||   3   2   1   0     |
//                   0000 || 0000                |
//                        ||                     |
//                        ||                     |
// Control:               ||                     |
//                        ||                     |
//                        ||                     |
//  LCD    D7  D6  D5  D4 ||                     |
//                        ||       En  R/W  RS   |
//                        ||                     |

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

