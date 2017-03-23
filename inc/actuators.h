#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#define DETECTED		1
#define NOT_DETECTED	0

#define DOWN			0
#define UP				1
#define MID				2

#define LEFT			1
#define RIGHT			0

#define FIRST			1
#define SECOND			2

#define ON				1
#define OFF 			0

#define CW				0
#define CCW				1

void servo_init(unsigned int f_pwm);
void servo_set_duty_cycle_one(int16_t value);
void servo_set_duty_cycle_two(int16_t value);
void servo_set_duty_cycle_three(int16_t value);

void ball_grabbers(unsigned char side, unsigned char state);
void dispenser(unsigned char side, unsigned char state);
void mosfet(unsigned char side, unsigned char state);

void actuator_setup(void);

#endif
