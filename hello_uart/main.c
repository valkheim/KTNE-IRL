#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "main.h"
#include "uart.h"

int main(void)
{
  uart_init();
  stdout = &uart_output;
  stdin  = &uart_input;
  while (1)
  {
    /* Blink led by toggling state of PORTB5 (Arduino digital 13). */
    PORTB ^= _BV(PORTB5);
    puts("Hello im the demo\n");
    _delay_ms(2000);
  }
  return 0;
}
