
#include "StepMotor.h"
#include "API.h"
#include "main.h"

StepMotorType FunctionalMotorStatus;

void StepMotor_Config(void)
{
	FunctionalMotorStatus.StepMotor.CurrentCoil		= START_UP_STAGE;
	FunctionalMotorStatus.StepMotor.OperationStage		= START_UP;
	FunctionalMotorStatus.StepMotor.MaxMinLimit		= DISABLE;
	FunctionalMotorStatus.StepMotor.Position		= 0;
	FunctionalMotorStatus.StepMotor.TotalSteps		= DEFAULT_TOTAL_NUM_OF_STEPS;

	FunctionalMotorStatus.StepMotor.Step1			= FLAP_STATE_POS_1_PERCENT;
	FunctionalMotorStatus.StepMotor.Step2			= FLAP_STATE_POS_2_PERCENT;
	FunctionalMotorStatus.StepMotor.Step3			= FLAP_STATE_POS_3_PERCENT;
	FunctionalMotorStatus.StepMotor.Step4			= FLAP_STATE_POS_4_PERCENT;
	FunctionalMotorStatus.StepMotor.Step5			= FLAP_STATE_POS_5_PERCENT;
}
void Step_Motor_States(bool coil_1_state,bool coil_2_state,bool coil_3_state,bool coil_4_state)
{
	STEP_MOTOR_4(coil_4_state);
	STEP_MOTOR_3(coil_3_state);
	STEP_MOTOR_2(coil_2_state);
	STEP_MOTOR_1(coil_1_state);
}
void StepMotor_Coils(uint8_t coil)
{
	switch (coil)
	{
		case COIL1_EXCITED:
		case START_UP_STAGE:
		case 20:
			Step_Motor_States(SET, SET, SET, RESET);
			FunctionalMotorStatus.StepMotor.CurrentCoil = COIL1_EXCITED;
			break;

		case COIL1_2_EXCITED:
			Step_Motor_States(SET, SET, RESET, RESET);
			FunctionalMotorStatus.StepMotor.CurrentCoil = COIL1_2_EXCITED;
			break;

		case COIL2_EXCITED:
			Step_Motor_States(SET, SET, RESET, SET);
			FunctionalMotorStatus.StepMotor.CurrentCoil = COIL2_EXCITED;
			break;

		case COIL2_3_EXCITED:
			Step_Motor_States(SET, RESET, RESET, SET);
			FunctionalMotorStatus.StepMotor.CurrentCoil = COIL2_3_EXCITED;
			break;

		case COIL3_EXCITED:
			Step_Motor_States(SET, RESET, SET, SET);
			FunctionalMotorStatus.StepMotor.CurrentCoil = COIL3_EXCITED;
			break;

		case COIL3_4_EXCITED:
			Step_Motor_States(RESET, RESET, SET, SET);
			FunctionalMotorStatus.StepMotor.CurrentCoil = COIL3_4_EXCITED;
			break;

		case COIL4_EXCITED:
			Step_Motor_States(RESET, SET, SET, SET);
			FunctionalMotorStatus.StepMotor.CurrentCoil = COIL4_EXCITED;
			break;

		case 10:
		case COIL4_1_EXCITED:
			Step_Motor_States(RESET, SET, SET, RESET);
			FunctionalMotorStatus.StepMotor.CurrentCoil = COIL4_1_EXCITED;
			break;
	}
}
void StepMotor_Operation(int16_t numofsteps)
{
	if (numofsteps > 0)
	{
		if(FunctionalMotorStatus.StepMotor.MaxMinLimit == SET)
		{
			/*Dont pass the limit*/
			if(FunctionalMotorStatus.StepMotor.Position == FunctionalMotorStatus.StepMotor.TotalSteps)
			{
				Step_Motor_States(RESET, RESET, RESET, RESET);
			}
			else
			{
				StepMotor_Coils(FunctionalMotorStatus.StepMotor.CurrentCoil + 1);
				FunctionalMotorStatus.StepMotor.Position++;
			}
		}
		else
		{
			StepMotor_Coils(FunctionalMotorStatus.StepMotor.CurrentCoil + 1);
			FunctionalMotorStatus.StepMotor.Position++;
		}
		numofsteps--;
		FunctionalMotorStatus.StepMotor.NumOfSteps--;
		if (numofsteps == 0)
		{
			Step_Motor_States(RESET, RESET, RESET, RESET);
			if(FunctionalMotorStatus.StepMotor.OperationStage   == START_UP)
			{
				FunctionalMotorStatus.StepMotor.OperationStage  = FIND_REFERENCE;
			}
			else if(FunctionalMotorStatus.StepMotor.OperationStage   == NORMAL_OPERATION)
			{
				if(OperatingStatus.SwingMode)/*ENABLE*/
					FunctionalMotorStatus.StepMotor.OpenVent = DISABLE;
			}
		}
	}
	else if(numofsteps < 0)/* numofsteps < 0 */
	{
		if(FunctionalMotorStatus.StepMotor.MaxMinLimit == SET)
		{
			/*Dont pass the limit*/
			if(FunctionalMotorStatus.StepMotor.Position == STEP_MOTOR_MIN_POS)
			{
				Step_Motor_States(RESET, RESET, RESET, RESET);
			}
			else
			{
				StepMotor_Coils(FunctionalMotorStatus.StepMotor.CurrentCoil - 1);
				FunctionalMotorStatus.StepMotor.Position--;
			}
		}
		else
		{
			StepMotor_Coils(FunctionalMotorStatus.StepMotor.CurrentCoil - 1);
			FunctionalMotorStatus.StepMotor.Position--;
			if (FunctionalMotorStatus.StepMotor.Position > 65000)/*Reset it*/
				FunctionalMotorStatus.StepMotor.Position = STEP_MOTOR_MIN_POS;
		}
		numofsteps++;
		FunctionalMotorStatus.StepMotor.NumOfSteps++;
		if (numofsteps == 0)
		{
			Step_Motor_States(RESET, RESET, RESET, RESET);
			if(FunctionalMotorStatus.StepMotor.OperationStage   == FIND_REFERENCE)
			{
				FunctionalMotorStatus.StepMotor.OperationStage  = NORMAL_OPERATION;
				FunctionalMotorStatus.StepMotor.Position		= STEP_MOTOR_MIN_POS;
				FunctionalMotorStatus.StepMotor.PositionPercent = STEP_MOTOR_MIN_POS;
				FunctionalMotorStatus.StepMotor.OpenVent		= ENABLE;
				FunctionalMotorStatus.StepMotor.MaxMinLimit	    = SET;
			}
			else if(FunctionalMotorStatus.StepMotor.OperationStage   == NORMAL_OPERATION)
			{
				if(OperatingStatus.SwingMode)/*ENABLE*/
					FunctionalMotorStatus.StepMotor.OpenVent = ENABLE;
			}

		}
	}
}
void StepMotor_Goto_Position(int16_t position_percent)
{
	FunctionalMotorStatus.StepMotor.NumOfSteps = (position_percent*FunctionalMotorStatus.StepMotor.TotalSteps)/100;
	StepMotor_Operation(FunctionalMotorStatus.StepMotor.NumOfSteps - FunctionalMotorStatus.StepMotor.Position);
	FunctionalMotorStatus.StepMotor.PositionPercent	= ((FunctionalMotorStatus.StepMotor.Position*100)/FunctionalMotorStatus.StepMotor.TotalSteps);
}
void StepMotor_Controller(void)
{
	switch(FunctionalMotorStatus.StepMotor.OperationStage)
	{
		case START_UP:
			StepMotor_Goto_Position(STEP_MOTOR_MAX_POS);
			break;

		case FIND_REFERENCE:
			StepMotor_Goto_Position(CLOSE_160_PERCENT);
			break;

		case NORMAL_OPERATION:
			if(OperatingStatus.PowerStatus == SYSTEM_ON)
			{
				if(OperatingStatus.SwingMode)/*ENABLE*/
				{
					if(FunctionalMotorStatus.StepMotor.OpenVent)
						StepMotor_Goto_Position(SWING_MAX_POS_PERCENT);
					else/*DISABLE*/
						StepMotor_Goto_Position(SWING_MIN_POS_PERCENT);
				}
				else/*DISABLE*/
				{
					switch(FunctionalMotorStatus.StepMotor.FixedPos)
					{
						case FLAP_STATE_STEP_1:
							StepMotor_Goto_Position(FunctionalMotorStatus.StepMotor.Step1);
							break;

						case FLAP_STATE_STEP_2:
							StepMotor_Goto_Position(FunctionalMotorStatus.StepMotor.Step2);
							break;

						case FLAP_STATE_STEP_3:
							StepMotor_Goto_Position(FunctionalMotorStatus.StepMotor.Step3);
							break;

						case FLAP_STATE_STEP_4:
							StepMotor_Goto_Position(FunctionalMotorStatus.StepMotor.Step4);
							break;

						case FLAP_STATE_STEP_5:
							StepMotor_Goto_Position(FunctionalMotorStatus.StepMotor.Step5);
							break;
					}
				}
			}
			else if(OperatingStatus.PowerStatus == SYSTEM_OFF)
			{
				if(!HAL_GPIO_ReadPin(FAN_S1_GPIO_Port, FAN_S1_Pin))
					if(!HAL_GPIO_ReadPin(FAN_S2_GPIO_Port, FAN_S2_Pin))
						if(!HAL_GPIO_ReadPin(FAN_S3_GPIO_Port, FAN_S3_Pin))
							StepMotor_Goto_Position(STEP_MOTOR_MIN_POS);
			}
			break;

		case SHUT_DOWN_OPERATION:
			break;

	}
}
