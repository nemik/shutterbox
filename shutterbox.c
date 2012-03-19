/*
 * ATtiny13 LED Flasher
 * File: main.c
 */
#include <avr/io.h> 
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define	sei()   __asm__ __volatile__ ("sei" ::)
#define	cli()   __asm__ __volatile__ ("cli" ::)

#define sbi(port, bit) (port) |= (1 << (bit))
#define cbi(port, bit) (port) &= ~(1 << (bit))

void pulseIR(long microsecs) 
{
	while (microsecs > 0) 
	{
		// 38 kHz is about 13 microseconds high and 13 microseconds low
		sbi(PORTB,PB4);
		_delay_us(11);
		cbi(PORTB,PB4);
		_delay_us(11);

		// so 26 microseconds altogether, pin settings take a little time too
		microsecs -= 26;
	}
}

void SendPentaxCode() 
{
	pulseIR(12740);

	_delay_us(2920);
	pulseIR(1000);

	_delay_us(980);
	pulseIR(980);

	_delay_us(980);
	pulseIR(980);

	_delay_us(980);
	pulseIR(1000);

	_delay_us(980);
	pulseIR(980);

	_delay_us(980);
	pulseIR(980);

	_delay_us(980);
	pulseIR(980);
}


int main(void)
{
	// Set up Port B pin 4 mode to output
	DDRB = 1<<DDB4;

	// Set up Port B data to be all low
	PORTB = 0;  

	GIMSK = _BV (PCIE);
	PCMSK = _BV (PCINT0);            // int - Enable external interrupts PCINT1

	//on falling edge
	MCUCR = _BV (ISC01);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sei();

	while (1) 
	{
		sleep_mode();
		cli();
		SendPentaxCode();
		//ghetto debounce
		_delay_ms (200);
		sei();
	}
}

ISR (PCINT0_vect) // Interrupt on Int0 vector
{
}
