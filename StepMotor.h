/*
 * StepMotor.h
 *
 *  Created on: Jul 2, 2024
 *      Author: Ajand Electronics
 */

#ifndef INC_STEPMOTOR_H_
#define INC_STEPMOTOR_H_

#include "stdbool.h"
#include "stdint.h"
typedef enum
{
	DEFAULT_TOTAL_NUM_OF_STEPS	= 900,

	START_UP					= 1,
	FIND_REFERENCE				= 2,
	NORMAL_OPERATION			= 3,
	SHUT_DOWN_OPERATION			= 4,

	COIL1_EXCITED	            = 11,
    COIL1_2_EXCITED             = 12,
    COIL2_EXCITED               = 13,
    COIL2_3_EXCITED             = 14,
    COIL3_EXCITED               = 15,
    COIL3_4_EXCITED             = 16,
    COIL4_EXCITED               = 17,
    COIL4_1_EXCITED             = 18,
	START_UP_STAGE	            = 19,

	STEP_MOTOR_MIN_POS			= 0,
	STEP_MOTOR_MAX_POS			= 100,
	CLOSE_160_PERCENT			= -60,

	SWING_MIN_POS_PERCENT		= 33,
	SWING_MAX_POS_PERCENT		= 83,

	OPEN_VENT					= 1,
	CLOSE_VENT					= 2,

	FLAP_STATE_STEP_1            = 1,
	FLAP_STATE_STEP_2            = 2,
	FLAP_STATE_STEP_3            = 3,
	FLAP_STATE_STEP_4            = 4,
	FLAP_STATE_STEP_5            = 5,

	FLAP_STATE_POS_1_PERCENT    = 33,
	FLAP_STATE_POS_2_PERCENT    = 44,
	FLAP_STATE_POS_3_PERCENT    = 58,
	FLAP_STATE_POS_4_PERCENT    = 72,
	FLAP_STATE_POS_5_PERCENT    = 95,


}StepMotorConstants;

typedef struct
{
	struct
	{
		uint8_t OperationStage;
		int16_t TotalSteps;
		int16_t NumOfSteps;
		uint8_t EEVIntensity;
		uint8_t CurrentCoil;
		uint8_t FixedPos;
		int8_t 	PositionPercent;
		int16_t Position;
		uint8_t Step1;
		uint8_t Step2;
		uint8_t Step3;
		uint8_t Step4;
		uint8_t Step5;

		bool MaxMinLimit;
		bool OpenVent;
	}StepMotor;

}StepMotorType;
extern StepMotorType FunctionalMotorStatus;
void StepMotor_Controller(void);
void StepMotor_Config(void);

#endif /* INC_STEPMOTOR_H_ */
