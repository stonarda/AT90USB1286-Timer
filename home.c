/* To Display The Menu and Allow User To Set Up */

#include "digits.h"
#include "menu.h"
#include "time.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#define CENTER_BUTTON PE7
#define UP PC4
#define DOWN PC2

#define LED PINB7
#define LED_TOGGLE PINB |= _BV(PINB7)
#define LED_OFF PINB &= ~_BV(PINB7);

// To tell if a second has past
int msCount = 0;

// Holds current time
// time[0]=seconds
// time[1]=minutes
// time[2]=hours
uint8_t time[3];

// Direction of change in time
int direction = 0;

// <3 if setting the time
uint8_t timeSet = 3;

uint16_t centerPushed = 0, updateTime = 0, countdownEnd = 0;

/* Millisecond timer */
ISR(TIMER0_COMPA_vect)
{
	// Called
	msCount+= 1;

	// Checkes if second has gone past
	if(msCount > 999)
	{
		// Indicates gui needs updating
		updateTime++;

		// Increments time as per direction
		if(direction > 0)
		{
			incrementTime(time, 0);
		}
		else if(direction < 0)
		{
			decrementTime(time, 0);

			// if countiing down and reaches 0, stops and indicates end
			if(isZero(time))
			{
				countdownEnd = 50;
				direction = 0;
			}
		}
		
		msCount = 0;
	}
}

void init(void)
{
	// Enable center button interrupt
	DDRE &= ~_BV(CENTER_BUTTON);
	PORTE |= _BV(CENTER_BUTTON);
	
	// Up and Down arrows
	DDRC &= ~_BV(UP);
	DDRC &= ~_BV(DOWN);
	PORTC |= _BV(UP);
	PORTC |= _BV(DOWN);

	// LED output
	DDRB  |=  _BV(PB7); 
	PORTB &= ~_BV(PB7); 
	PINB &= ~_BV(PINB7);

	// Timer interrupt
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);
	TIMSK0 |= _BV(OCIE0A);

	// Interrupts to happen every 1ms
	OCR0A = 124;
}

void main(void)
{			
	// 8MHz clock with no prescaling
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;

	cli();
	
	init_lcd();
	init();

	// Time is initially zero
	time[0] = 0;
	time[1] = 0;
	time[2] = 0;

	uint16_t updateGui = 0;

	drawMenu(&timeSet);
	showClock(time);

	sei();

	// Constant loop
	while(1)
	{
		if(countdownEnd)
		{
			// At end of countdown so toggles LED
			LED_TOGGLE;
			countdownEnd--;	
		}
		else
		{
			// LED needs to be off
			LED_OFF;
		}

		// Checks if center button was pushed
		if(!(PINE & _BV(CENTER_BUTTON)))
		{
			selectPush(&direction, &timeSet, time);
			centerPushed = 0;
			updateGui++;
		}

		// Checks if up button is being pushed
		if(!(PINC & _BV(UP)))
		{
			updateGui++;

			if(timeSet < 3)
			{
				// Setting the time
				decrementTime(time, timeSet);
				updateTime++;
			}
			else
			{
				adjustMenu(1);
			}
		}

		// Checks if down button is being pushed
		if(!(PINC & _BV(DOWN)))
		{
			updateGui++;

			if(timeSet < 3)
			{
				// Setting the time
				incrementTime(time, timeSet);
				updateTime++;
			}
			else
			{
				adjustMenu(-1);
			}
		}

		// Checks if menus need updating
		if(updateGui > 0)
		{
			updateGui = 0;
			drawMenu(&timeSet);
		}

		// Checks if time needs updating
		if(updateTime > 0)
		{
			updateTime = 0;
			showClock(time);
		}
	
		_delay_ms(75);
	}
}
