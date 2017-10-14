#include <avr/io.h>
#include <stdio.h>

#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

#include "uart.h"

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void uart_init(void)
{
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */

  stdout = &uart_output;
  stdin  = &uart_input;
}

void uart_putstr(char *s)
{
  int i = -1;

  while (s[++i])
    uart_putchar(s[i], stdout);
}

int uart_putchar(char c, FILE *stream)
{
  //if (c == '\n') {
  //  uart_putchar('\r', stream);
  //}
  while (!(UCSR0A & (1 << UDRE0))); //loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
  return 0;
}

int uart_getchar(FILE *stream)
{
  //unsigned char c;

  while (!(UCSR0A & (1 << RXC0))); //loop_until_bit_is_set(UCSR0A, RXC0);
  //c = UDR0;
  //uart_putchar(c, stream);
  return UDR0;
}
