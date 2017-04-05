#include "gpio.h"
#include "system.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "odometry.h"
#include "sides.h"
#include "actuators.h"

/*
	 _______
	< vader >
	 -------
			\    ,-^-.
			 \   !oYo!
			  \ /./=\.\______
				   ##        )\/\
					||-----w||
					||      ||

*/
int main()
{
	system_init();
	_delay_ms(100);
	yellowside();
	/*while(1)
	{
		if(!klip_switch_check())
			{
				//activated
			}
			else
			{
				//off
			}
	}*/
	/*if(sides_switch_check())
	{
		//blue
	}
	else
	{
		//yellow
	}*/
	/*	if(tactic_switch_check())
			//mosfet
			leds(ON);
		else
			leds(OFF);
		_delay_ms(200);
	*/
}
