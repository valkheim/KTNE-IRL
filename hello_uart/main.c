#ifndef F_CPU
#define F_CPU 16000000UL
#endif

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
    while (fgets(buf, BUFSIZE, stdin))
    {
      memset(buf, 0, BUFSIZE);
      puts(buf);
    }
    _delay_ms(2000);
  }
  return 0;
}
