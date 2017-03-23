#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "can.h"
#include "actuators.h"

/*
 *	Function: 		void servo_init(unsigned int f_pwm)
 *	Parameters: 	unsigned int f_pwm
 *	Description: 	servo init
 */
void servo_init(unsigned int f_pwm)
{
	DDRE |= ((1 << PINE3) | (1 << PINE4) | (1 << PINE5));
	DDRB |= (1 << PINB7);//OCR0A
	
	TCNT3 = 0;
	TCNT0 = 0;

	OCR3A = 0;
	OCR3B = 0;
	OCR3C = 0;

	TCCR3A = (1 << COM3A1)  | (1 << COM3B1) | (1 << COM3B0) | (1 << COM3C1) | (1 << COM3C0) | (1 << WGM31);
	TCCR3B = (1<< CS31) | (1 << WGM32) | (1 << WGM33) ; // PRESKALER = 8
	
	//8bit timer for pwm shit
	TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << CS01) | (1 << CS00);

	ICR3   = ((double)F_CPU) / (8.0 * f_pwm) + 0.5;
}

/*
 *	Function: 		void servo_set_duty_cycle_one-two-three(int16_t value)
 *	Parameters: 	int16_t value - the pwm value
 *	Description: 	duty cycle for servos
 */
void servo_set_duty_cycle_one(int16_t value)
{
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3AH = temp >> 8;
	OCR3AL = temp & 0xFF;
}
void servo_set_duty_cycle_two(int16_t value)
{
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3BH = temp >> 8;
	OCR3BL = temp & 0xFF;
}
void servo_set_duty_cycle_three(int16_t value)
{
	uint16_t temp = ((double)ICR3 / 255.0) * value + 0.5;
	OCR3CH = temp >> 8;
	OCR3CL = temp & 0xFF;
}

/*
 *	Function: 		void actuator_setup(void)
 *	Description: 	csetup the actuators
 */
void actuator_setup(void)
{

}

/*********************************************************************/
/*
 *	Main actuator functions
 */

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
	buffer[0] = 'A';		// indicating that I'm using
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
	buffer[0] = 'B';		// indicating that I'm using
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

/*********************************************************************/


