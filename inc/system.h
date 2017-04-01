#ifndef SYSTEM_H_
#define SYSTEM_H_

#define MAX_INPUTS	    		20
#define USE_TIMER_HOOK  		1

#define ROBOT_SIDE_FRONT		1
#define ROBOT_SIDE_BACK			2

#define ALL						1

#define GPIOA_BASE 0x22
#define GPIOB_BASE 0x25
#define GPIOC_BASE 0x28
#define GPIOD_BASE 0x2B
#define GPIOE_BASE 0x2E
#define GPIOG_BASE 0x34
#define GPIOF_BASE 0x31

#define PURPLE					1
#define GREEN					0

#define ACTIVATE				1
#define DEACTIVE			 	0

#define TRUE					0
#define FALSE					1

#define ON						1
#define OFF						0

#define JUMPER_PIN				40		//portf 0
#define SIDE_PIN				41		//portf 1
#define TACTIC_PIN				42		//portf 2
#define VALJAK_PIN				43		//portf 3

#define SENSOR_F_L_PIN			8		//portb 0	
#define SENSOR_F_R_PIN			10		//portb 2 
#define SENSOR_B_L_PIN			12		//portb 4
#define SENSOR_B_R_PIN			14		//portb 6

#define DIST_TRIG_PIN			0
#define DIST_ECHO_PIN			0
//Function to enable the timer with argument of the frequency
void timer_init(unsigned int freq);

//Variables
unsigned char restartCheck;
unsigned char stateRobot;
int active_state;

//Function that returns the state of the robot
uint8_t return_active_state(void);

//Checking the side switch function
signed char sides_switch_check(void);

//Checking the tactic switch function
signed char tactic_switch_check(void);

//Initing the system functions
void system_init(void);

//Sensor function
signed char check_sensor(signed char robot_side, signed char sensor_side);

//Functions for setting and getting match started
void system_set_match_started(void);
uint8_t system_get_match_started(void);

uint32_t system_get_system_time(void);

void leds(unsigned char state);

#endif
