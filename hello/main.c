#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#define BAUD 9600
#include <util/setbaud.h>

#define LED PORTB5
#define BVV(bit, val) ((val)?_BV(bit):0)

static void usart_init(void)
{
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~_BV(U2X0);
#endif
  UCSR0B = BVV(TXEN0, 1) | BVV(RXEN0, 1); /* Only TX/RX */
}

static void usart_tx(char c) // send
{
  //while(!(UCSR0A & _BV(UDRE0)));
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
}

static int usart_getchar(FILE *stream)
{

  while (!(UCSR0A & (1 << UDRE0)));
  (void)*stream;
  return UDR0;
}

static void usart_puts(const char *s)
{
  while(*s != '\0')
  {
    usart_tx(*s++);
  }
}

int main()
{
  usart_init();
  DDRB |= (1 << LED);
  for/*ever*/(;;)
  {
    usart_getchar(stdin);
    PORTB |= (1 << LED);
    _delay_ms(500);
    PORTB &= (0 << LED);
    _delay_ms(500);
    usart_puts("ping\n");
  }
}


