/*
 * Joule Juggler V2: Joule_Driver.h
 *
 *  Latest Revision on: Mar 19, 2025
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


/*
 * Joule Juggler GPIO Ports
 */

#define PA 0x01     //SPI: Pin2 SCK[0], Pin3 CS[0], Pin4 MISO[0], Pin5 MOSI[0] I2C: Pin6 SCL[1], Pin7 SDA[1]
#define PB 0x02     //HW Serial: Pin0 Rx[1], Pin1 Tx[1] I2C: Pin2 SCL[0], Pin3 SDA[0] SPI: Pin4 SCK[2], Pin5 CS[2], Pin6 MISO[2], Pin7 MOSI[2]
#define PC 0x04     //HW Serial: Pin4 Rx[1], Pin5 Tx[1] Pin6 Rx[3], Pin7 Tx[3]
#define PD 0x08     //I2C: Pin0 SCL[3], Pin1 SDA[3] SPI: Pin0 SCK[3], Pin1 CS[3], Pin2 MISO[3], Pin3 MOSI[3] HW Serial: Pin6 Rx[2], Pin7 Tx[2]
#define PE 0x10     // ADCs
#define PF 0x20     //SPI: Pin0 MISO[1], Pin1 MOSI[1], Pin2 SCK[1], Pin3 CS[1]

/*
 * ADC Values and Functions
 */
#define Draining 32 // 0x20, PC5
#define Gaining 16  // 0x10, PC4
#define DrainingRest 1  // Rest after draining to reach equilibrium
#define GainingRest 1   // Rest after gaining to reach equilibrium
#define Rest 1          // General rest to be used during cycling if needed
double I_batt;      // Variable for R_senseC
double V_batt;      // Variable for R_vref
double Batt_temp;   // Variable for thermistor
int shutdown;       // Flag in case of an emergency shut it down!
int toggle = 3;         // Used to flag what state the Joule Juggler is in, i.e. Gaining or Draining

void ADC_Init(void);// Initialization of ADC0: AIN0, AIN2 ADC1: AIN1
void current();     // Take a reading of the R_senseC resistor
void temperature(); // Take a reading of the thermistor
void voltage();     // Take a reading of the R_vref resistor
double ADC_Conversion(double analogValue);      // Conversion for outputting user friendly units

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
#define TV  8   // Time Value short for demo purposes == 19 ~30 seconds
#define TV2 8   // Time Value long for operational purposes

int timer0ACounter;    // Variable to hold the current time for triggering timed transistions

void Timer0A_Init();    // Initialization for Timer0A ISR
void Timer0A_Handler(); // Timer0A ISR will run for the entire game


#endif /* JOULE_DRIVER_H_ */
