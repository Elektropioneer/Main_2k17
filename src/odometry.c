#include <stdint.h>
#include "odometry.h"
#include "system.h"
#include "can.h"
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#include <avr/io.h>

// current position
static uint8_t current_speed = 0;

// global structure for odometry position refrence (position)
volatile struct odometry_position position=
{
	.x     = 0,
	.y     = 0,
	.angle = 0,
	.state = IDLE
};

/*
 *	Function: 		void odometry_set_speed(uint8_t speed)
 *	Parameters: 	uint8_t speed - the speed
 *	Description:	setting the speed
 */
void odometry_set_speed(uint8_t speed)
{
	if(speed == current_speed)
		return;

	uint8_t buffer[8];
	buffer[0] = 'V';
	buffer[1] = speed;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);

	current_speed = speed;
}

/*
 *	Function: 		static uint8_t odometry_wait_until_done(uint8_t (*callback)(uint32_t start_time))
 *	Parameters: 	uint8_t (*callback)(uint32_t start_time)	- function callback
 *	Description:	doing the callback function while odometry moving
 */
static void odometry_query_position(void)
{
	uint8_t buffer[8];
	buffer[0] = 'P';
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);
	CAN_Read(buffer, DRIVER_RX_IDENTIFICATOR);
	position.state = buffer[0];
	position.x	   = (buffer[1] << 8) | buffer[2];
	position.y	   = (buffer[3] << 8) | buffer[4];
	position.angle = (buffer[5] << 8) | buffer[6];
}

/*
 *	Function: 		static uint8_t odometry_wait_until_done(uint8_t (*callback)(uint32_t start_time))
 *	Parameters: 	uint8_t (*callback)(uint32_t start_time)	- function callback
 *	Description:	doing the callback function while odometry moving
 */
static uint8_t odometry_wait_until_done(uint8_t (*callback)(uint32_t start_time))
{
	uint32_t time = system_get_system_time();
	do
	{
		odometry_query_position();
		if(callback != NULL)
		{
			if(callback(time) == 1)
				return ODOMETRY_FAIL;
		}
	}while(position.state == MOVING || position.state == ROTATING);
	
	return ODOMETRY_SUCCESS;
}

/*
 *	Function: 		void odometry_stop(int8_t type)
 *	Parameters: 	int8_t type			- the type of stop
 *	Description:	you shall not pass
 */
void odometry_stop(int8_t type)
{
	uint8_t buffer[8];

	do
	{
		buffer[0] = type;

		while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
			_delay_ms(50);

		odometry_query_position();
	}while(position.state == MOVING || position.state == ROTATING);
}

/*
 *	Function: 		uint8_t odometry_move_straight(int16_t distance, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 *	Parameters: 	int16_t distance							- the distance in mm
 *					uint8_t speed								- speed
 *					uint8_t (*callback)(uint32_t start_time)	- callback
 *	Description:	moving straight
 */
uint8_t odometry_move_straight(int16_t distance, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];
	odometry_set_speed(speed);
	distance *= -1;
	buffer[0] = 'D';
	buffer[1] = distance >> 8;
	buffer[2] = distance & 0xFF;
	
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);
		
	return odometry_wait_until_done(callback);
}

/*
 *	Function: 		uint8_t odometry_move_to_position(struct odometry_position* position, uint8_t speed, int8_t direction, uint8_t (*callback)(uint32_t start_time))
 *	Parameters: 	struct odometry_position* position			- structure for position
 *					uint8_t speed								- speed
 *					int8_t direction							- direction
 *					uint8_t (*callback)(uint32_t start_time)	- callback
 *	Description:	main function for moving
 */
uint8_t odometry_move_to_position(struct odometry_position* position, uint8_t speed, int8_t direction, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];

	odometry_set_speed(speed);

	buffer[0] = 'G';
	buffer[1] = position->x >> 8;
	buffer[2] = position->x & 0xFF;
	buffer[3] = position->y >> 8;
	buffer[4] = position->y & 0xFF;
	buffer[5] = 0;
	buffer[6] = direction * -1;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);
	
	//ovo je bilo na return 0
	//return 0;
	return odometry_wait_until_done(callback);
}

/*
 *	Function: 		void odometry_set_position(struct odometry_position* new_position)
 *	Parameters: 	struct odometry_position* new_position	- struct for positon
 *	Description:	setting the position of the robot
 */
void odometry_set_position(struct odometry_position* new_position)
{
	uint8_t buffer[8];

	buffer[0] = 'I';
	buffer[1] = new_position->x >> 8;
	buffer[2] = new_position->x & 0xFF;
	buffer[3] = new_position->y >> 8;
	buffer[4] = new_position->y & 0xFF;
	
	int8_t temp = new_position->angle * -1;
	buffer[5] = temp >> 8;
	buffer[6] = temp & 0xFF;

	position.x	   = new_position->x;
	position.y	   = new_position->y;
	position.angle = -1 * new_position->angle;

	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);
}

/*
 *	Function: 		uint8_t odometry_rotate(int16_t angle,uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 *	Parameters: 	int16_t angle								- the angle
 *					uint8_t speed								- the speed
 *					uint8_t (*callback)(uint32_t start_time)	- callback function
 *	Description:	rotating robot
 */
uint8_t odometry_rotate(int16_t angle,uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];
	odometry_set_speed(speed);

	angle *= -1;
	buffer[0] = 'T';
	buffer[1] = angle >> 8;
	buffer[2] = angle & 0xFF;

	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);

	return odometry_wait_until_done(callback);

}

/*
 *	Function: 		uint8_t odometry_set_angle(int16_t angle, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
 *	Parameters: 	int16_t angle								- the angle
 *					uint8_t speed								- the speed
 *					uint8_t (*callback)(uint32_t start_time)	- callback function
 *	Description:	setting odometry angle
 */
uint8_t odometry_set_angle(int16_t angle, uint8_t speed, uint8_t (*callback)(uint32_t start_time))
{
	uint8_t buffer[8];
	odometry_set_speed(speed);

	angle *= -1;
	buffer[0] = 'A';
	buffer[1] = angle >> 8;
	buffer[2] = angle & 0xFF;
	while(CAN_Write(buffer, DRIVER_TX_IDENTIFICATOR))
		_delay_ms(50);

	return odometry_wait_until_done(callback);
}

/*
 *	Function: 		uint8_t getState(void)
 *	Parameters: 	void
 *	Description:	return odometry state
 */
uint8_t getState(void)
{
	return position.state;
}

/*
 *	Function: 		int16_t odometry_get_x(void)
 *	Parameters: 	void
 *	Description:	return odometry x position
 */
int16_t odometry_get_x(void)
{
	return position.x;
}

/*
 *	Function: 		int16_t odometry_get_y(void)
 *	Parameters: 	void
 *	Description:	return odometry y position
 */
int16_t odometry_get_y(void)
{
	return position.y;
}

/*
 *	Function: 		int16_t odometry_get_angle(void)
 *	Parameters: 	void
 *	Description:	getting odometry angle
 */
int16_t odometry_get_angle(void)
{
	return position.angle;
}
