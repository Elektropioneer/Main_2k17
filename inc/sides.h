#ifndef SIDES_H_
#define SIDES_H_

#define TACTIC_SIDE_POSITION_COUNT 1
#define TACTIC_CENTER_POSITION_COUNT 1

#define PURPLE_SIDE 0
#define GREEN_SIDE 1

struct goto_fields
{
	struct odometry_position point;
	uint8_t					 speed;
	uint8_t					 direction;
	uint8_t					 (*callback)(uint32_t start_time);
};




void purpleside(void);
void greenside(void);
#endif /* SIDES_H_ */
