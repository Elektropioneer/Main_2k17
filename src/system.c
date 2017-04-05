#include <stdint.h>
#include "system.h"
#include "can.h"
#include "gpio.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "actuators.h"
#include "list_generic.h"
#include "odometry.h"
#include "uart.h"

static volatile unsigned long sys_time;
static uint8_t match_started;
static void (*timer_callback)(void) = NULL;

unsigned int received = 0;

/*
 *	Function: 		void timer_register_callback(void (*callback)(void))
 *	Parameters: 	void (*callback)(void))
 *	Description: 	timer callbacks
 */
void timer_register_callback(void (*callback)(void))
{
    timer_callback = callback;
}

/*
 *	Function: 		void timer_init(unsigned int freq)
 *	Parameters: 	unsigned int freq - frequency of timer
 *	Description: 	setup timer
 */
void timer_init(unsigned int freq)
{
    TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS10);
	OCR1A = (double)F_CPU / (double)freq + 0.5;
	TIMSK1 = 1 << OCIE1A;

	SREG |= 0x80;
}

/*
 *	Function: 		ISR(TIMER1_COMPA_vect)
 *	Description: 	ISR interrupt for timer
 */
ISR(TIMER1_COMPA_vect)
{
    if(timer_callback != NULL)
        timer_callback();
	if(sys_time >= 90000)					// if match end
	{
		if(system_get_match_started())		// if the match has started before
		{
			odometry_stop(HARD_STOP);
			while(1);
		}
	}
	sys_time++;
}

/*
 *	Function: 		signed char sides_switch_check(void)
 *	Parameters: 	void
 *	Description: 	checking side switch
 */
signed char sides_switch_check(void)
{
	if(gpio_read_pin(SIDE_PIN) == 1)
	{
		return 1;
	}
	return 0;
}

/*
 *	Function: 		signed char tactic_switch_check(void)
 *	Parameters: 	void
 *	Description:	reading tactic switch
 */
signed char tactic_switch_check(void)
{
	if(gpio_read_pin(TACTIC_PIN) == 1)
	{
		return 1;
	}
	return 0;
}

/*
 *	Function: 		signed char klip_switch_check(void)
 *	Parameters: 	void
 *	Description:	reading klip switch
 */
signed char klip_switch_check(void)
{
	if(gpio_read_pin(VALJAK_PIN) == 1)
	{
		return 1;
	}
	return 0;
}

/*
 *	Function: 		void system_reset_system_time(void)
 *	Parameters: 	void
 *	Description:	reset system time
 */
void system_reset_system_time(void)
{
	sys_time = 0;
}

/*
 *	Function: 		void system_set_match_started(void)
 *	Parameters: 	void
 *	Description:	let the match begiiiiiiiiiin
 */
void system_set_match_started(void)
{
	match_started = 1;
}

/*
 *	Function: 		uint32_t system_get_system_time(void)
 *	Parameters: 	void
 *	Description:	returns current system time
 */
uint32_t system_get_system_time(void)
{
	return sys_time;
}

/*
 *	Function: 		uint8_t system_get_match_started(void)
 *	Parameters: 	void
 *	Description:	return if match has started
 */
uint8_t system_get_match_started(void)
{
	return match_started;
}

/*
 *	Function: 		uint8_t return_active_state(void)
 *	Parameters: 	void
 *	Description:	return active state
 */
uint8_t return_active_state(void)
{
	return active_state;
}

/*
 *	Function: 		void leds(unsigned char state)
 *	Parameters: 	unsigned char state - the state
 *	Description:	setting the debug led's states
 */
void leds(unsigned char state)
{
	if(state) { PORTG = 0xff; } else { PORTG = 0x00; }
}

/*
 *	Function: 		void system_init(void)
 *	Parameters: 	void
 *	Description:	initing all the things
 */
void system_init(void)
{	
	// sets debouncer
	timer_register_callback(gpio_debouncer);
	
	// small delay
	_delay_ms(100);
	
	/*
	 *	gpio setups
	 */
	gpio_register_pin(JUMPER_PIN,GPIO_DIRECTION_INPUT,TRUE);							//jumper
	gpio_register_pin(SIDE_PIN,GPIO_DIRECTION_INPUT,TRUE);								//prekidac za stranu
	gpio_register_pin(TACTIC_PIN,GPIO_DIRECTION_INPUT,TRUE);							//prekidac za stranu
	gpio_register_pin(VALJAK_PIN,GPIO_DIRECTION_INPUT,TRUE);							//prekidac za valjak
	/*
	gpio_register_pin(SENSOR_F_L_PIN,GPIO_DIRECTION_INPUT,TRUE);						//sensor front left
	gpio_register_pin(SENSOR_F_R_PIN,GPIO_DIRECTION_INPUT,TRUE);						//sensor front right
	gpio_register_pin(SENSOR_B_L_PIN,GPIO_DIRECTION_INPUT,TRUE);						//sensor back left
	gpio_register_pin(SENSOR_B_R_PIN,GPIO_DIRECTION_INPUT,TRUE);						//sensor back right
	*/

	// led debugs
	DDRG = 0xff;
	DDRA = 0xff;
	leds(ON);

	// inits
	servo_init(50);
	timer_init(1000);
	CAN_Init(1);
	initUart1(UART1_BAUD,UART_ISR_ON);
	actuator_setup();

	// waiting for the jumper
	while(!(gpio_read_pin(JUMPER_PIN)));
		_delay_ms(10);
		
	// indicating the led
	leds(OFF);
	
	system_reset_system_time();															// reset system time
	system_set_match_started();															// match has started!

}

/*
 *	Function: 		signed char check_sensor(signed char robot_side, signed char sensor_side)
 *	Parameters: 	signed char robot_side, signed char sensor_side
 *	Description:	checking all the sensors
 */
signed char check_sensor(signed char robot_side, signed char sensor_side)
{
	if(robot_side == ROBOT_SIDE_FRONT)
	{
		if(sensor_side == ALL)
		{
			// check if ! or normal
			if(!(gpio_read_pin(SENSOR_F_L_PIN)) || !(gpio_read_pin(SENSOR_F_R_PIN)))
			{
				return DETECTED;
			}
			return NOT_DETECTED;
		}
		else if(sensor_side == LEFT)
		{
			if(!(gpio_read_pin(SENSOR_F_L_PIN)))
			{
				return DETECTED;
			}
			return NOT_DETECTED;
		}
		else if(sensor_side == RIGHT)
		{
			if(!(gpio_read_pin(SENSOR_F_R_PIN)))
			{
				return DETECTED;
			}
			return NOT_DETECTED;
		}
	}
	else if(robot_side == ROBOT_SIDE_BACK)
	{
		if(sensor_side == ALL)
		{
			// check if ! or normal
			if(!(gpio_read_pin(SENSOR_B_L_PIN)) || !(gpio_read_pin(SENSOR_B_R_PIN)))
			{
				return DETECTED;
			}
			return NOT_DETECTED;
		}
		else if(sensor_side == LEFT)
		{
			if(!(gpio_read_pin(SENSOR_B_L_PIN)))
			{
				return DETECTED;
			}
			return NOT_DETECTED;
		}
		else if(sensor_side == RIGHT)
		{
			if(!(gpio_read_pin(SENSOR_B_R_PIN)))
			{
				return DETECTED;
			}
			return NOT_DETECTED;
		}
	}
	return NOT_DETECTED;
}
