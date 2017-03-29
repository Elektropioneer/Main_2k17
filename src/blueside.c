#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "system.h"
#include "odometry.h"
#include "can.h"
#include "sides.h"
#include "uart.h"
#include "actuators.h"
#include "gpio.h"

//////////////////////////////////////////////////////////////////////////
//																		//
//					   DETEKCIJA/CALLBACK FUNKCIJE						//
//																		//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

// SIDE TACTIC
const struct goto_fields blue_tactic_side_positions[TACTIC_SIDE_POSITION_COUNT] =
{
	{{100,0},LOW_SPEED,FORWARD,NULL}
};

// CENTER TACTIC
const struct goto_fields blue_tactic_center_positions[TACTIC_CENTER_POSITION_COUNT] =
{
	{{100,0},LOW_SPEED,FORWARD,NULL}
};


void blueside(void)
{
	struct odometry_position starting_position;
	uint8_t current_position = 0;
	uint8_t next_position = 0;
	uint8_t odometry_status;
	int8_t active_state = ROBOT_STATE_TACTIC_SIDE;

	starting_position.x		= 0;
	starting_position.y		= 0;
	starting_position.angle = 0;

	odometry_set_position(&starting_position);

	while(1)
	{
		switch(active_state)
		{
			case ROBOT_STATE_TACTIC_SIDE:
				for(current_position = next_position;current_position < TACTIC_SIDE_POSITION_COUNT; current_position++)
				{
					odometry_status = odometry_move_to_position(&(blue_tactic_side_positions[current_position].point), blue_tactic_side_positions[current_position].speed,
					blue_tactic_side_positions[current_position].direction,blue_tactic_side_positions[current_position].callback);

					if(odometry_status == ODOMETRY_FAIL)
					{
						break;
					}
					if(current_position == 1){
						while(1);
					}
				}//end for
				/* no break */
			case ROBOT_STATE_TACTIC_CENTER:
				for(current_position = next_position;current_position < TACTIC_CENTER_POSITION_COUNT; current_position++)
				{
					odometry_status = odometry_move_to_position(&(blue_tactic_center_positions[current_position].point), blue_tactic_center_positions[current_position].speed,
					blue_tactic_center_positions[current_position].direction,blue_tactic_center_positions[current_position].callback);

					if(odometry_status == ODOMETRY_FAIL)
					{
						break;
					}
					if(current_position == 1){
						while(1);
					}
				}//end for
		}//end switch
	}//end while
}//end void
