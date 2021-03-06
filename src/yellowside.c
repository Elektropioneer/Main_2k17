
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
const struct goto_fields yellow_tactic_side_positions[TACTIC_SIDE_POSITION_COUNT] =
{
		//950+282
	{{1050,896},60,BACKWARD,NULL},
	{{1250,596},60,FORWARD,NULL},
	{{1300,256},60,FORWARD,NULL}

};

// CENTER TACTIC
const struct goto_fields yellow_tactic_center_positions[TACTIC_CENTER_POSITION_COUNT] =
{
	{{100,0},LOW_SPEED,FORWARD,NULL}
};


void yellowside(void)
{
	struct odometry_position starting_position;
	uint8_t current_position 	= 0;
	uint8_t next_position 		= 0;
	uint8_t odometry_status;
	int8_t active_state 		= ROBOT_STATE_TACTIC_SIDE;

	// ^ y
	// |
	// |	x
	// |----->
	//////////

	starting_position.x			= 282; // mm
	starting_position.y			= 896; // mm
	starting_position.angle 	= -180;

	odometry_set_position(&starting_position);

	while(1)
	{
		switch(active_state)
		{
			case ROBOT_STATE_TACTIC_SIDE:
				for(current_position = next_position;current_position < TACTIC_SIDE_POSITION_COUNT; current_position++)
				{
					odometry_status = odometry_move_to_position(&yellow_tactic_side_positions[current_position].point, yellow_tactic_side_positions[current_position].speed,yellow_tactic_side_positions[current_position].direction, yellow_tactic_side_positions[current_position].callback);
					if(odometry_status == ODOMETRY_FAIL)
					{
						break;
					}
					if(current_position == TACTIC_SIDE_POSITION_COUNT - 1){		// when at the last position
						while(1);
					}
				}//end for
				/* no break */
			case ROBOT_STATE_TACTIC_CENTER:
				for(current_position = next_position;current_position < TACTIC_CENTER_POSITION_COUNT; current_position++)
				{
					odometry_status = odometry_move_to_position(&(yellow_tactic_center_positions[current_position].point), yellow_tactic_center_positions[current_position].speed,
					yellow_tactic_center_positions[current_position].direction,yellow_tactic_center_positions[current_position].callback);

					if(odometry_status == ODOMETRY_FAIL)
					{
						break;
					}
					if(current_position == TACTIC_CENTER_POSITION_COUNT - 1){	// when at the last position
						while(1);
					}
				}//end for
		}//end switch
	}//end while
}//end void

