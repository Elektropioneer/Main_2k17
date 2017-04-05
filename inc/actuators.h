#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#define DETECTED			1
#define NOT_DETECTED		0

#define DOWN				0
#define UP					1
#define MID					2

#define LEFT				1
#define RIGHT				0

#define FIRST				1
#define SECOND				2

#define ON					1
#define OFF 				0

#define CW					0
#define CCW					1

#define klip_servo_right	70
#define klip_servo_center	-15
#define klip_servo_left		-90

////////////////////////// distance sensor stuff //////////////////////////

#define dist_trig_on()	gpio_write_pin(DIST_TRIG_PIN, TRUE);
#define dist_trig_off()	gpio_write_pin(DIST_TRIG_PIN, FALSE);

// Speed of sound
#define SPEED_OF_SOUND				343
#define MAX_SENSOR_RANGE			8											//2*4M
#define TIMER_MAX					65535										//16 bit timer

#define TRIG_ERROR					-1
#define ECHO_ERROR					-2

#define CYCLES_PER_US				(F_CPU/1000000)								//in us
#define CYCLES_PER_MS				(F_CPU/1000)								//in ms

#define DIST_SENSOR_TIMOUT			((F_CPU*MAX_SENSOR_RANGE)/SPEED_OF_SOUND)

#define CONVERT_TO_CM				((10000*2)/SPEED_OF_SOUND)

#define DETECTING_DISTANCE_MIN		180
#define DETECTING_DISTANCE_MAX		200

/////////////////////////////////////////////////////////////////
void servo_init(unsigned int f_pwm);
void klip_servo(int8_t angle);
/*void servo_set_duty_cycle_one(int16_t value);
void servo_set_duty_cycle_two(int16_t value);
void servo_set_duty_cycle_three(int16_t value);*/

void ball_grabbers(unsigned char side, unsigned char state);
void dispenser(unsigned char side, unsigned char state);
void mosfet(unsigned char side, unsigned char state);

void pumps(unsigned char state);
void klip(unsigned char state);

void actuator_setup(void);

unsigned int distance_sensor();

#endif
