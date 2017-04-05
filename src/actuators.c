#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "can.h"
#include "actuators.h"
#include "gpio.h"

volatile uint32_t dist_overflow_counter = 0;
volatile uint32_t dist_trig_counter		= 0;
volatile uint32_t dist_num_of_ticks		= 0;

ISR(TIMER2_OVF_vect)
{
	dist_overflow_counter++;
	TCNT2 = 0;
}

/*
 *	Function: 		void servo_init(unsigned int f_pwm)
 *	Parameters: 	unsigned int f_pwm
 *	Description: 	servo init
 */
void servo_init(unsigned int f_pwm)
{
	DDRE |= ((1 << PINE3) | (1 << PINE4) | (1 << PINE5));
	DDRB |= (1 << PINB7);										//OCR0A 8bit
	
	TCNT3 = 0;
	TCNT0 = 0;

	OCR3A = 0;
	OCR3B = 0;
	OCR3C = 0;

	TCCR3A = (1 << COM3A1)  | (1 << COM3B1) | (1 << COM3B0) | (1 << COM3C1) | (1 << COM3C0) | (1 << WGM31);
	TCCR3B = (1<< CS31) | (1 << WGM32) | (1 << WGM33) ; 		// PRESKALER = 8
	
	//8bit timer
	TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << CS01) | (1 << CS00);

	ICR3   = ((double)F_CPU) / (8.0 * f_pwm) + 0.5;
}

/*
 *	Function: 		static void servo_set_duty_cycle_one-two-three(int16_t value)
 *	Parameters: 	int16_t value - the pwm value
 *	Description: 	duty cycle for servos
 *	Pin:			PE3, PE4, PE5
 */
static void servo_set_duty_cycle_one(int16_t value)
{
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3AH = temp >> 8;
	OCR3AL = temp & 0xFF;
}
static void servo_set_duty_cycle_two(int16_t value)
{
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3BH = temp >> 8;
	OCR3BL = temp & 0xFF;
}
static void servo_set_duty_cycle_three(int16_t value)
{
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3CH = temp >> 8;
	OCR3CL = temp & 0xFF;
}

/*
 *	Function: 		void klip_servo(int8_t angle)
 *	Parameters: 	int8_t angle - the angle
 *	Description: 	angle of klip servo, using PIN E3
 */
void klip_servo(int8_t angle)
{
	servo_set_duty_cycle_one( 255-(236.0 - ((double)angle / 90.0) * 11.4));
}

/*
 *	Function: 		void actuator_setup(void)
 *	Description: 	setup the actuators
 */
void actuator_setup(void)
{
	klip(OFF);
	pumps(OFF);
}

/*
 *	Function: 		void ball_grabbers(unsigned char side, unsigned char state)
 *	Parameters: 	unsigned char side 	- which side to control
 *					unsigned char state	- UP or DOWN or MID state
 *	Description: 	controls the ball grabber servos through CAN communication with actuator board
 */
void ball_grabbers(unsigned char side, unsigned char state)
{
	// Side 	- LEFT or RIGHT
	// State 	- UP or DOWN or MID

	uint8_t buffer[8];
	buffer[0] = 'A';		// indicating what I'm using
	buffer[1] = side;		// indicating which side
	buffer[2] = state;		// indicating what state

	// send it off
	while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
		_delay_ms(50);
}

/*
 *	Function: 		void dispenser(unsigned char side, unsigned char state)
 *	Parameters: 	unsigned char side 	- which side to control
 *					unsigned char state	- UP or DOWN state
 *	Description: 	controls the dispenser servos through CAN communication with actuator board
 */
void dispenser(unsigned char side, unsigned char state)
{
	// Side 	- LEFT or RIGHT
	// State 	- UP or DOWN

	uint8_t buffer[8];
	buffer[0] = 'B';		// indicating what I'm using
	buffer[1] = side;		// indicating which side
	buffer[2] = state;		// indicating what state

	// send it off
	while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
		_delay_ms(50);
}

/*
 *	Function: 		void mosfet(unsigned char side, unsigned char state)
 *	Parameters: 	unsigned char side 	- which side to control
 *					unsigned char state	- ON or OFF state
 *	Description: 	controls the diaframs through CAN communication with actuator board
 */
void mosfet(unsigned char side, unsigned char state)
{
	// Side 	- LEFT or RIGHT
	// State 	- ON or OFF

	uint8_t buffer[8];
	buffer[0] = 'C';		// indicating that I'm using diafram
	buffer[1] = side;		// indicating which side
	buffer[2] = state;		// indicating what state

	// send it off
	while(CAN_Write(buffer, DRIVER_LIFT_TX_IDENTIFICATOR))
		_delay_ms(50);
}
/*
 *	Function: 		void pumps(unsigned char state)
 *	Parameters: 	unsigned char state	- ON or OFF state
 *	Description: 	controls the pumps from the relay board
 */
void pumps(unsigned char state)
{

	//2 -> PA0
	//3 -> PA1
	//1 -> PA2

	if(!state)					// !state because the relay turns ON when pulled to GND
	{
		PORTA |= (1 << PA2) | (1 << PA0);
	} else {

		PORTA &= ~(1 << PA2) &  ~(1 << PA0);
	}
}

/*
 *	Function: 		void klip(unsigned char state)
 *	Parameters: 	unsigned char state	- ON or OFF state
 *	Description: 	controls the klip from the relay board
 */
void klip(unsigned char state)
{

	//2 -> PA0
	//3 -> PA1
	//1 -> PA2

	if(!state)					// !state because the relay turns ON when pulled to GND
	{
		PORTA |= (1 << PA1);
	} else {
		PORTA &= ~(1 << PA1);
	}
}

/*
 *	Function: 		static void trig_pulse()
 *	Description: 	specific trigure pulses for the distance sensor
 */
static void trig_pulse()
{
	dist_trig_off()				// Clear pin before setting it high
	_delay_us(1);				// Clear to zero and give time for electronics to set
	dist_trig_on()				// Set pin high
	_delay_us(12);				// Send high pulse for minimum 10us
	dist_trig_off()				// Clear pin
	_delay_us(1);				// Delay not required, but just in case...
}

/*
 *	Function: 		unsigned int distance_sensor()
 *	Description: 	returns the distance sensor read value
 */
unsigned int distance_sensor()
{

	trig_pulse();														// Sending the pulse

	while(!(gpio_read_pin(DIST_ECHO_PIN)))								// While echo pin is low (so no feedback)
	{
		dist_trig_counter++;
		uint32_t max_response_time = DIST_SENSOR_TIMOUT;				// getting timeout
		if(dist_trig_counter > max_response_time)						// timeoued
			return TRIG_ERROR;
	}

	TCNT2 = 0;															// reseting the timer
	TCCR2A |= (1 << CS20);												// no prescaler
	TIMSK2 |= (1 << TOIE2);												// interrupt overflow
	dist_overflow_counter = 0;
	sei();																// enable global interupts

	while(gpio_read_pin(DIST_ECHO_PIN))									// getting echo
	{
		if(((dist_overflow_counter*TIMER_MAX) + TCNT2) > DIST_SENSOR_TIMOUT)
			return ECHO_ERROR;
	}

	TCCR2A = 0x00;														// stop the timer
	cli();																// disable global int
	dist_num_of_ticks = ((dist_overflow_counter * TIMER_MAX) + TCNT2);	// number of ticks

	return (dist_num_of_ticks / (CONVERT_TO_CM * CYCLES_PER_US));

}

