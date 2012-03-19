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
  // we'll count down from the number of microseconds we are told to wait
 
  while (microsecs > 0) 
	{
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   sbi(PORTB,PB4);
	 //_delay_us(3);
	 //digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   _delay_us(11);
	 //delayMicroseconds(10);         // hang out for 10 microseconds
   cbi(PORTB,PB4);
	 //digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   _delay_us(11);
   //delayMicroseconds(10);         // hang out for 10 microseconds
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
}

void SendPentaxCode() 
{
  //cli();  // this turns off any background interrupts
  
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
  
	//sei();  // this turns them back on
}


int main(void)
{
    // Set up Port B pin 4 mode to output
    DDRB = 1<<DDB4;
 
    // Set up Port B data to be all low
    PORTB = 0;  

		//GIMSK = _BV (INT0); // int - Enable external interrupts int0
		GIMSK = _BV (PCIE);
		PCMSK = _BV (PCINT0);            // int - Enable external interrupts PCINT1
		
		//on rising edge
		MCUCR = _BV (ISC01); // int - INT0 is falling edge
		//MCUCR = _BV (ISC00); // int - INT0 is on any edge
		
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sei();

    while (1) 
		{
			sleep_mode();
			cli();
			SendPentaxCode();
      _delay_ms (200);
			sei();
			//SendPentaxCode(); 
      // Pause a little while
    }
}

ISR (PCINT0_vect) // Interrupt on Int0 vector
{
}
