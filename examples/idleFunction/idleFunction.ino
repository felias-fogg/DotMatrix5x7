// -*- c++ -*-
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DotMatrix5x7.h>
#include <LowPower.h>

char str[]="Hello World!";

void myIdle(long unsigned int period)
{
  unsigned long time_now = millis();
  while (millis() < time_now + period) {
    // go into idle mode until next timer interrupt
#if defined(__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega88__)
    LowPower.idle(SLEEP_FOREVER, ADC_ON, TIMER2_ON, TIMER1_ON, TIMER0_ON, SPI_ON, USART0_ON, TWI_ON);
#elif defined __AVR_ATmega644P__ || defined (__AVR_ATmega1284P__)
    LowPower.idle(SLEEP_FOREVER, ADC_ON, TIMER2_ON, TIMER1_ON, TIMER0_ON, SPI_ON, USART1_ON, USART0_ON, TWI_ON);
#elif defined(__AVR_ATmega2560__)
    LowPower.idle(SLEEP_FOREVER, ADC_ON, TIMER5_ON, TIMER4_ON, TIMER3_ON, TIMER2_ON, TIMER1_ON, TIMER0_ON, SPI_ON, USART3_ON, USART2_ON, USART1_ON, USART0_ON, TWI_ON);
#elif defined(__AVR_ATmega32U4__)
    LowPower.idle(SLEEP_FOREVER, ADC_ON, TIMER4_ON, TIMER3_ON, TIMER2_ON, TIMER1_ON, TIMER0_ON, SPI_ON, USART1_ON, TWI_ON, USB_ON);
#else
    #error "MCU not supported"
#endif
  }
}

void setup(void)
{
  Dot5x7.begin(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
  Dot5x7.setFramesPerSecond(100);
  Dot5x7.setDelayFunction(myIdle);
}

void loop(void)
{
  int i;
  Dot5x7.setUpsideDown(false);
  Dot5x7.showString(str, 600, 100);
  myIdle(3000);
}
