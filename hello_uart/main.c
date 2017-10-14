#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <string.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "main.h"
#include "uart.h"

#define BUFSIZE (42)

int main(void)
{
  char buf[BUFSIZE];

  uart_init();
  while (1)
  {
    /* Blink led by toggling state of PORTB5 (Arduino digital 13). */
    PORTB ^= _BV(PORTB5);
    memset(buf, 0, BUFSIZE);
    while (fgets(buf, BUFSIZE, stdin))
    {
      uart_putstr(buf);
      //puts(buf);
      memset(buf, 0, BUFSIZE);
    }
    _delay_ms(2000);
  }
  return 0;
}
