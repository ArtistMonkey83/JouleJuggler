/*
 * Joule Juggler V2: PLL.c
 *
 *  Created on: Mar 16, 2025
 *      Author: Yolie Reyes
 */


#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"

#define SYSCTL_RCC2_USERCC2     0x80000000  // Use RCC2
#define SYSCTL_RCC2_BYPASS2     0x00000800  // PLL Bypass 2
#define SYSCTL_RCC_XTAL_M       0x000007C0  // Crystal Value
#define SYSCTL_RCC_XTAL_16MHZ   0x00000540  // 16 MHz Crystal
#define SYSCTL_RCC2_OSCSRC2_M   0x00000070  // Oscillator Source 2
#define SYSCTL_RCC2_OSCSRC2_MO  0x00000000  // MOSC
#define SYSCTL_RCC2_PWRDN2      0x00002000  // Power-Down PLL 2
#define SYSCTL_RCC2_DIV400      0x40000000  // Divide PLL as 400 MHz vs. 200

#define SYSDIV2                 0x7			// the value to come up with 50 MHz.

#define SYSCTL_RIS_PLLLRIS      0x00000040  // PLL Lock Raw Interrupt Status

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

// configure the system to get its clock from the PLL
int PLL_Init(void){
	uint32_t counter = 0;

	SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;      		// 0) configure the system to use RCC2 for advanced features
	                                           	    //    such as 400 MHz PLL and non-integer System Clock Divisor

  SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;      	    // 1) bypass PLL while initializing

          										    // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R  &= ~SYSCTL_RCC_XTAL_M;          	//    clear XTAL field
  SYSCTL_RCC_R  += SYSCTL_RCC_XTAL_16MHZ;           //    configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;     		//    clear oscillator source field
  SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;     		//    configure for main oscillator source

  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;        		// 3) activate PLL by clearing PWRDN

          											// 4) set the desired system 		significant bit
  SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;         		//    use 400 MHz PLL
  SYSCTL_RCC2_R  = (SYSCTL_RCC2_R&~0x1FC00000) 		//    clear system clock divider field
                   + (SYSDIV2<<22);            		//    configure for 50 MHz clock

          										    // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS) == 0){};
	while(counter<20){
			++counter;
	}
	counter = 0;
	  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;   	    // 6) enable use of PLL by clearing BYPASS

	return 0;
}
