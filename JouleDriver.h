/*
 * Joule Juggler V1: Joule_Driver.h
 *
 *  Latest Revision on: Feb 16, 2025
 *      Author: Yolie Reyes
 */

#include "stdint.h"
#include "tm4c123gh6pm.h"

#ifndef JOULE_DRIVER_H_
#define JOULE_DRIVER_H_


// The Layout of Ports on the Tiva TM4C123G
//             b31 ... b6 || b5  b4  b3  b2  b1  b0
//              RESERVED  || F   E   D   C   B   A
//                  0x...    20  10  08  04  02  01
//                           Charging        LCD
//                                  Discharging

// The Layout of Pins for a GPIO Port  -----------
//         b7  b6  b5  b4 ||  b3  b2  b1  b0     |
// Pin      7   6   5   4 ||   3   2   1   0     |
//                   0000 || 0000                |
//                                               |
//                                               |
// Control:               ||                     |
//                        ||                     |
//                        ||                     |
//  LCD    D7  D6  D5  D4 ||                     |
//                        ||       En  R/W  RS   |
//                        ||                     |

// The layout of the 16x2 LCD addresses
//
//       1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
//
// 0x... 80  81  82  83  84  85  86  87  88 89  8A  8B  8C  8D  8E  8F  LINE 1
// 0x... C0  C1  C2  C3  C4  C5  C6  C7  C8 C9  CA  CB  CC  CD  CE  CF  LINE 2


/*
 * Joule Juggler GPIO Ports
 */

#define PA 0x01     //SPI: Pin2 SCK[0], Pin3 CS[0], Pin4 MISO[0], Pin5 MOSI[0] I2C: Pin6 SCL[1], Pin7 SDA[1]
#define PB 0x02     //HW Serial: Pin0 Rx[1], Pin1 Tx[1] I2C: Pin2 SCL[0], Pin3 SDA[0] SPI: Pin4 SCK[2], Pin5 CS[2], Pin6 MISO[2], Pin7 MOSI[2]
#define PC 0x04     //HW Serial: Pin4 Rx[1], Pin5 Tx[1] Pin6 Rx[3], Pin7 Tx[3]
#define PD 0x08     //I2C: Pin0 SCL[3], Pin1 SDA[3] SPI: Pin0 SCK[3], Pin1 CS[3], Pin2 MISO[3], Pin3 MOSI[3] HW Serial: Pin6 Rx[2], Pin7 Tx[2]
#define PF 0x20     //SPI: Pin0 MISO[1], Pin1 MOSI[1], Pin2 SCK[1], Pin3 CS[1]

/*
 * ADC Values and Functions
 */


/*
 * Charging Control Values and Functions
 */
int toggle = 3;           // Toggle is initialized to 3 for system setup purposes, 0 = "gain" 1 = "drain"
void charging_Init(void); // Trigger Interrupt Control
void GPIOF_Handler();     // Handler for the charging interrupts coming in on Port F pin 4 and 0



/*
 * Discharging Control Values and Functions
 *
 */


/*
 * Joule Juggler LCD Control Values Functions
 */
#define RS 1     // 0x01, PB0
#define RW 2     // 0x02, PB1
#define EN 4     // 0x04, PB2

void LCD4bits_Init(void);                           // Initialization of LCD Display
void LCD_Write4bits(unsigned char, unsigned char);  // Write data as (4 bits) to LCD
void LCD_WriteString(char*);                        // Write a string to LCD
void LCD4bits_Cmd(unsigned char);                   // Write command
void LCD4bits_Data(unsigned char);                  // Write a character

void LCDon(void);
void LCDoff(void);
void LCDbattHealth(int p1, int p2);

char* LCDintConversion(int integer);

void delayUs(int);                                   // Delay in Micro Seconds
void delayMs(int);                                   // Delay in Milli Seconds

/*
 * Joule Timer Values and Functions
 */
int timer0ACounter;    // Variable to hold the current time for triggering timed transistions

void Timer0A_Init();    // Initialization for Timer0A ISR
void Timer0A_Handler(); // Timer0A ISR will run for the entire game


#endif /* JOULE_DRIVER_H_ */
