/*
 * Joule Juggler V2: Joule_Driver.c
 *
 *  Latest Revision on: Mar 9, 2025
 *      Author: Yolie Reyes
 */

#include <JouleDriver.h>
#include "stdint.h"
#include "stdio.h"
#include "SysTick.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"

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
 * ADC Values and Functions
 */
void ADC_Init(void){ // Initialization of ADC0: AIN0, AIN2 ADC1: AIN1

}

void current(){     // Take a reading of the R_senseC resistor

}

void temperature(){ // Take a reading of the thermistor

}

void voltage(){     // Take a reading of the R_vref resistor

}

double ADC_Conversion(double analogValue){      // Conversion for outputting user friendly units

}

/*
 * Joule Juggler LCD Control Values Functions
 */
void LCD4bits_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x02;   // Enable the clock for Port B
    delayMs(50);                 // delay 50 ms for enable the clock of PORTB
    GPIO_PORTB_DIR_R = 0xFF;     // Set all pins on Port B for output
    GPIO_PORTB_DEN_R = 0xFF;     // Set all pins as digital
    LCD4bits_Cmd(0x28);          // 2 lines and 5x7 character (4-bit data, D4 to D7)
    LCD4bits_Cmd(0x06);          // Automatic Increment cursor (shift cursor to right)
    LCD4bits_Cmd(0x01);          // Clear display screen
    LCD4bits_Cmd(0x0F);          // Display on, cursor blinking
}

void LCD_Write4bits(unsigned char data, unsigned char control)
{
    data &= 0xF0;                                 // Clear lower nibble for control
    control &= 0x0F;                              // Clear upper nibble for data
    GPIO_PORTB_DATA_R  = data | control;          // Include RS value (command or data ) with data
    GPIO_PORTB_DATA_R  = data | control | EN;     // Pulse EN
    delayUs(0);                                   // Delay for pulsing EN
    GPIO_PORTB_DATA_R  = data | control;          // Turn off the pulse EN
    GPIO_PORTB_DATA_R  = 0;                       // Clear the Data
}

void LCD_WriteString(char * str)
{
    volatile int i = 0;             // Volatile is important

    while(*(str+i) != '\0')         // Until the end of the string
    {
        LCD4bits_Data(*(str+i));    // Write each character of string
        i++;                        // Increment for next character
    }
}

void LCD4bits_Cmd(unsigned char command)
{
    LCD_Write4bits(command & 0xF0 , 0);    // Upper nibble first
    LCD_Write4bits(command << 4 , 0);      // Then lower nibble

    if(command < 4){ delayMs(2); }   // Commands 1 and 2 need up to 1.64ms
    else{ delayUs(40);}              // All others 40 us
}

void LCD4bits_Data(unsigned char data)
{
    LCD_Write4bits(data & 0xF0 , RS);   // Upper nibble first
    LCD_Write4bits(data << 4 , RS);     // Then lower nibble
    delayUs(40);                        // Delay for LCD (MCU is faster than LCD)
}

void LCDon(void){
    char* str1 = "By: Yolie Reyes";
    char* str2 = "Lios em chaniavu";
    char* str3 = "ket chem allea? ";
    char* str4 = "Joule Juggler V1";

    LCD4bits_Cmd(0x01);               // Clear the display
    LCD4bits_Cmd(0x80);               // Force the cursor to beginning of 1st line
    delayMs(500);                     // Delay 500 ms for LCD (MCU is faster than LCD)
    LCD_WriteString(str2);            // Write the string  "Lios em chaniavu" on LCD
    delayMs(500);
    LCD4bits_Cmd(0xC0);               // Force the cursor to beginning of 1st line

    LCD_WriteString(str3);            // Write the string "ket chem allea? " on LCD
    delayMs(3000);

    LCD4bits_Cmd(0x01);               // Clear the display
    LCD4bits_Cmd(0x80);               // Force the cursor to beginning of 1st line
    delayMs(500);                     // Delay 500 ms for LCD (MCU is faster than LCD)
    LCD_WriteString(str4);            // Write the string  "Joule Juggler V1" on LCD
    delayMs(500);
    LCD4bits_Cmd(0xC0);               //Force the cursor to beginning of 1st line

    LCD_WriteString(str1);            //Write the string "By: Yolie Reyes" on LCD
    delayMs(500);
    //LCD4bits_Cmd(0x01);               // Clear the display                                      //Delay 500 ms to let the LCD diplays the data

}

void LCDoff(void){
    char* str  = "Thhheeee Ennnnd!";
    char* str2 = " By: Yolie Reyes";

    LCD4bits_Cmd(0x01);           // Clear the display
    delayMs(500);
    LCD4bits_Cmd(0x80);           // Force the cursor to beginning of 1st line
    delayMs(500);

    LCD_WriteString(str);         // Write the string on LCD
    delayMs(500);                 // Delay 500 ms to let the LCD displays the data

    LCD4bits_Cmd(0xC0);           // Force the cursor to beginning of 2nd line
    delayMs(500);                 // Delay 500 ms to let the LCD displays the data
    LCD_WriteString(str2);        // Write the string on LCD
    delayMs(500);                 // Delay 500 ms to let the LCD displays the data
}

void LCDbattHealth(int p1, int p2){
    char* str  = " Battery Life:";
    char* str2 = "  % Charged:  ";
    char* str4 = "  # of Cycles: ";

    char* str3 = LCDintConversion(p1);      // p1 is used to determine the % of charge battery is currently at
    char* str5 = LCDintConversion(p2);      // p2 is used to determine the # of charging/discharging current experiment is at
    double c20 = 2.76 * 0.2;                 // c20 is the threshold to trigger battery charging
    double c10 = 2.76 - (2.76 * 0.1);        // c10 is the threshold to trigger battery discharging

        LCD4bits_Cmd(0x01);     // Clear the display
        delayMs(500);           // Delay 500 ms to let the LCD diplays the data
        LCD4bits_Cmd(0x80);     // Force the cursor to beginning of 1st line
        delayMs(500);           // Delay 500 ms for LCD (MCU is faster than LCD)

        LCD_WriteString(str);   // Write the string " Battery Life:" on LCD
        delayMs(500);           // Delay 500 ms for LCD (MCU is faster than LCD)
        LCD4bits_Cmd(0xC0);     // Force the cursor to beginning of 2nd line
        LCD_WriteString(str2);  // Write the string "  % Charged:  " on LCD
        delayMs(500);           // Delay 500 ms for LCD (MCU is faster than LCD)
        LCD_WriteString(str3);  // Write the string returned by LCDintConversion(p1) for % battery left
        delayMs(500);           // Delay 500 ms for LCD (MCU is faster than LCD)
        LCD_WriteString(str4);  // Write the string on LCD
        delayMs(500);           // Delay 500 ms for LCD (MCU is faster than LCD)
        LCD_WriteString(str5);  // Write the string returned by LCDintConversion(p2) for # of cycles run
        delayMs(3000);          // Delay 500 ms to let the LCD diplay the data

        if(p1 < c20 && p1 > c10){     // This line is executed if we are in the battery discharging state
            char* drain  = "Draining Joules";
            char* drain2  = "  One by one...  ";
            toggle = 1;               // Toggle is used to determine the state of the system 1 = drain

            LCD4bits_Cmd(0x01);       // Clear the display
            delayMs(500);             // Delay 500 ms to let the LCD diplay a blank screen
            LCD4bits_Cmd(0x80);       // Force the cursor to beginning of 1st line
            delayMs(500);             // Delay 500 ms to let the LCD diplay a blank screen

            LCD_WriteString(drain);   // Write the string "Draining Joules" on LCD
            delayMs(500);             // Delay 500 ms to let the LCD diplay data
            LCD4bits_Cmd(0xC0);       // Force the cursor to beginning of 2nd line
            LCD_WriteString(drain2);  // Write the string  "  One by one...  " on LCD
            delayMs(500);             // Delay 500 ms for LCD (MCU is faster than LCD)
        }
        if(p1 < c10){                 // This line is execute if we are in the battery charge state
            char* gain = "Gaining Joules ";
            char* gain2 = "  Some by some!";
            toggle = 0;                // Toggle is used to determine the state of the system 0 = gain

            LCD4bits_Cmd(0x01);        // Clear the display
            delayMs(500);              // Delay 500 ms to let the LCD diplay the data
            LCD4bits_Cmd(0x80);        // Force the cursor to beginning of 1st line
            delayMs(500);              // Delay 500 ms to let the LCD catch up
            LCD_WriteString(gain);     // Write the string "Gaining Joules "
            delayMs(500);              // Delay 500 ms to let the LCD diplay the data

            LCD4bits_Cmd(0xC0);        // Force the cursor to beginning of 2nd line
            LCD_WriteString(gain2);    // Write the string "  Some by some!"
            delayMs(500);              // delay 500 ms for LCD (MCU is faster than LCD)
        }
}

char* LCDintConversion(int integer){    // Returns 0 - 100 in ASCII, default "Error > 100!"
    switch(integer){
        case 0:
            return "0";
        case 1:
            return "1";
        case 2:
            return "2";
        case 3:
            return "3";
        case 4:
            return "4";
        case 5:
            return "5";
        case 6:
            return "6";
        case 7:
            return "7";
        case 8:
            return "8";
        case 9:
            return "9";
        case 10:
            return "10";
        case 11:
            return "11";
        case 12:
            return "12";
        case 13:
            return "13";
        case 14:
            return "14";
        case 15:
            return "15";
        case 16:
            return "16";
        case 17:
            return "17";
        case 18:
            return "18";
        case 19:
            return "19";
        case 20:
            return "20";
        case 21:
            return "21";
        case 22:
            return "22";
        case 23:
            return "23";
        case 24:
            return "24";
        case 25:
            return "25";
        case 26:
            return "26";
        case 27:
            return "27";
        case 28:
            return "28";
        case 29:
            return "29";
        case 30:
            return "30";
        case 31:
            return "31";
        case 32:
            return "32";
        case 33:
            return "33";
        case 34:
            return "34";
        case 35:
            return "35";
        case 36:
            return "36";
        case 37:
            return "37";
        case 38:
            return "38";
        case 39:
            return "39";
        case 40:
            return "40";
        case 41:
            return "41";
        case 42:
            return "42";
        case 43:
            return "43";
        case 44:
            return "44";
        case 45:
            return "45";
        case 46:
            return "46";
        case 47:
            return "47";
        case 48:
            return "48";
        case 49:
            return "49";
        case 50:
            return "50";
        case 51:
            return "51";
        case 52:
            return "52";
        case 53:
            return "53";
        case 54:
            return "54";
        case 55:
            return "55";
        case 56:
            return "56";
        case 57:
            return "57";
        case 58:
            return "58";
        case 59:
            return "59";
        case 60:
            return "60";
        case 61:
            return "61";
        case 62:
            return "62";
        case 63:
            return "63";
        case 64:
            return "64";
        case 65:
            return "65";
        case 66:
            return "66";
        case 67:
            return "67";
        case 68:
            return "68";
        case 69:
            return "69";
        case 70:
            return "70";
        case 71:
            return "71";
        case 72:
            return "72";
        case 73:
            return "73";
        case 74:
            return "74";
        case 75:
            return "75";
        case 76:
            return "76";
        case 77:
            return "77";
        case 78:
            return "78";
        case 79:
            return "79";
        case 80:
            return "80";
        case 81:
            return "81";
        case 82:
            return "82";
        case 83:
            return "83";
        case 84:
            return "84";
        case 85:
            return "85";
        case 86:
            return "86";
        case 87:
            return "87";
        case 88:
            return "88";
        case 89:
            return "89";
        case 90:
            return "90";
        case 91:
            return "91";
        case 92:
            return "92";
        case 93:
            return "93";
        case 94:
            return "94";
        case 95:
            return "95";
        case 96:
            return "96";
        case 97:
            return "97";
        case 98:
            return "98";
        case 99:
            return "99";
        case 100:
            return "100";
        default:
            return "  Error > 100!";
    }
}


// See Page 722 in the TM4C123 Manual
/*
 *  Joule Timer Values and Functions
 */
void Timer0A_Init() {

    SYSCTL_RCGCTIMER_R |= 0x01;     // Enable the clock for Timer0A (1<<0)
    TIMER0_CTL_R = 0x00000000;      // Disable Timer0 during configuration (1<<0) Page 737
    TIMER0_CFG_R = 0x00000000;      // Enable 32-bit mode, Page 727
    TIMER0_TAMR_R = 0x2;            // Configure Timer0A as a 32-bit periodic timer (0x2<<0) Page 732
    TIMER0_TAILR_R = 0x2FAF07F;     // Set the start value for a 1 second period ( 50,000,000 - 1) = 0x2FAF07F.
                                           // 50 MHz clock (one tick every 20 ns == 1/50,000,000 seconds
                                           // !!!!! THE PROJECT IS 50 MHZ!!!!! Assuming a 16 MHz clock Page 756
                                           // # ticks/second = 1 second/20 * 10 ^ -9 seconds/ tick

    NVIC_EN0_R |= 1 << 19;                                      // Enable the interrupt for Timer0 in the NVIC, IRQ 19 Page 723
    TIMER0_TAPR_R = 0;                                          // Set bus clock resolution Page 760
    TIMER0_ICR_R = 0x00000001;                                  // Clear the Timer0 interrupt flag Page 754
    TIMER0_IMR_R = 0x00000001;                                  // Enable Timer0A interrupt
    NVIC_PRI4_R =  (NVIC_PRI4_R & 0x00ffffff) | 0x80000000;     // Enables interrupts with a priority of 4 page354
    TIMER0_CTL_R |= 0x00000001;                                 // Enable Timer0A
    //__enable_irq();
}

void Timer0A_Handler() {
    timer0ACounter++;

    //Check the voltage on charge pin
    if(timer0ACounter == TV*1){
//        SysTick_Wait50ms(3);        // Short Delay 0.5 seconds
        delayMs(500);
        LCDbattHealth(3,4);
    }

    if(timer0ACounter == TV*2){
//        SysTick_Wait50ms(3);        // Short Delay 0.5 seconds
        delayMs(500);
        LCDoff();

    }

    //
    if( timer0ACounter == TV*3){
//        SysTick_Wait50ms(2);        // Short Delay VALUES = {1,20,100,80
        delayMs(500);
        LCDon();
    }
    if(timer0ACounter == TV*4){
//        SysTick_Wait50ms(3);        // Short Delay 0.5 seconds
        delayMs(500);
        LCDbattHealth(3,4);
    }

    //
    if(timer0ACounter == TV*5){
//        SysTick_Wait50ms(3);        // Short Delay 0.5 seconds
        delayMs(500);
        LCDoff();
    }

    //
    if(timer0ACounter == TV*6){
//        SysTick_Wait50ms(2);        // Short Delay VALUES = {1,20,100,80
        delayMs(500);
        LCDon();
    }

    TIMER0_ICR_R = 0x00000001;     // Clear the Timer0 interrupt flag, acknowledge Timer0A timeout
}
