#include <stdint.h>

// Increments time by either a second, min or hr depending on item
// item=0 -> +1 second
// item=1 -> +1 minute
// item=2 -> +1 hour
void incrementTime(uint8_t* time, uint8_t item)
{
	time[item]++;

	if(time[0] > 59)
	{
		time[1]++;
		time[0] = 0;

		if(time[1] > 59)
		{
			time[2]++;
			time[1] = 0;

			if(time[2] > 99)
			{
				time[2] = 0;
			}
		}
	}
}

// Decrements time by either a second, min or hr depending on item
// item=0 -> -1 second
// item=1 -> -1 minute
// item=2 -> -1 hour
void decrementTime(uint8_t* time, uint8_t item)
{
	time[item]--;

	if(time[0] > 254)
	{
		time[1]--;
		time[0] = 59;
	}

	if(time[1] > 254)
	{
		time[2]--;
		time[1] = 59;
	}

	if(time[2] > 254)
	{
		time[0] = 0;
		time[1] = 0;
		time[2] = 0;
	}
}

// Checks if the time is zero on each sec, min, hr
uint16_t isZero(uint8_t* time)
{
	if(time[0] == 0 && 
		time[1] == 0 && 
		time[2] == 0)
	{
		return 1;
	}
	
	return 0;
}
