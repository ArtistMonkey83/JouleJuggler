/*
 * Joule Juggler V1: SysTick.h
 *
 * Latest Revision on: Feb 16, 2025
 *      Author: Yolie Reyes
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);

// Time delay using busy wait.
// The delay parameter is in units of the core clock. 
void SysTick_Wait(uint32_t delay);

// This assumes 50 MHz system clock.
void SysTick_Wait200ms(uint32_t delay);

void SysTick_Wait02ms(uint32_t delay);

void SysTick_Wait08ms(uint32_t delay);

void SysTick_Wait50ms(uint32_t delay);

#endif /* SYSTICK_H__ */
