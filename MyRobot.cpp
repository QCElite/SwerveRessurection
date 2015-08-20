#include "WPILib.h"
#include <stdlib.h>
#include <math.h>

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */
class RobotDemo : public SimpleRobot
{
	RobotDrive		myRobot;		// robot drive system
	Joystick		stick_l;		// driving (left) joystick
	Joystick		stick_r;		// fire control joystick ( right )

	Jaguar rearSteer;
	Jaguar frontRight;
	Jaguar rearRight;
	Jaguar frontSteer;
	Jaguar frontLeft;
	Jaguar rearLeft;

	AnalogChannel rearSteerAngle;
	AnalogChannel frontTrim;
	AnalogChannel frontSteerAngle;
	AnalogChannel rearTrim;
	
	float frontTarget, rearTarget;
	bool rearReversed;

public:
	RobotDemo():
		myRobot(7,8,9,10),		// these must be initialized in the same order
		stick_l(1),				// as they are declared above.
		stick_r(2),				// right joystick
		rearSteer(1),
		frontRight(2),
		rearRight(3),
		frontSteer(4),
		frontLeft(6),
		rearLeft(5),
		rearSteerAngle(1),
		frontTrim(2),
		frontSteerAngle(3),
		rearTrim(4)
	{
		myRobot.SetExpiration(0.1);
	}
	
	bool equals(float a, float b, float thresh){
		float diff = a-b;
		if((-thresh < diff && thresh > diff)){
			return true;
		}
		return false;
	}
	
	float rationalize(float angle){
		if(angle > 360){
			return rationalize(angle - 360);
		}
		
		if(angle < 0){
			return rationalize(angle + 360);
		}
		
		return angle;
	}
	
	void adjustAngles(void){
		float frontReal = 360.0/5*frontSteerAngle.GetVoltage() + 170.0/5*frontTrim.GetVoltage();
		float rearReal = 360.0/5*rearSteerAngle.GetAverageVoltage() + 170.0/5*rearTrim.GetAverageVoltage();
		
		rearReversed = false;
		
		frontReal = rationalize(frontReal);
		rearReal = rationalize(rearReal);
		frontTarget = rationalize(frontTarget);
		rearTarget = rationalize(rearTarget);
		
		float tmpTarget = rearTarget;
		if(tmpTarget < 85 && tmpTarget > 40){
			tmpTarget = tmpTarget+180;
			rearReversed = true;
		}
		
		
		if(equals(frontReal, frontTarget, 2.0)){
			frontSteer.Set(0.0);
		}else if(frontReal < frontTarget){
			frontSteer.Set(0.5);
		}else{
			frontSteer.Set(-0.5);
		}
		
		if(equals(rearReal, tmpTarget, 2.0)){
			rearSteer.Set(0.0);
		}else if(rearReal < tmpTarget){
			rearSteer.Set(-0.5);
		}else{
			rearSteer.Set(0.5);
		}
		
		DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();

		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "QC Elite Team # 648");
		dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "%f" , frontTarget);
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "%f",  frontReal);
		dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "%f" , rearTarget);
		dsLCD->Printf(DriverStationLCD::kUser_Line5, 1, "%f",  rearReal);
		dsLCD->UpdateLCD();
	}

	void RobotInit(void)
	{
	
		frontTarget = rearTarget = 180;
		printf("RobotInit Started\n");

				DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();

				dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "QC Elite Team # 648");

				dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "%f" , rearSteerAngle.GetVoltage());
				dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "%f",  frontTrim.GetVoltage());
				dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "%f" , frontSteerAngle.GetVoltage());
				dsLCD->Printf(DriverStationLCD::kUser_Line5, 1, "%f",  rearTrim.GetVoltage());
				dsLCD->UpdateLCD();


		myRobot.SetInvertedMotor(RobotDrive::kFrontLeftMotor, false);
		myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, false);
		myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, false);
		myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, false);
		myRobot.SetSafetyEnabled(false);

	}


	/**
	 * Drive Autonomous mode
	 */
	void Autonomous(void)
	{
		float frontReal = 360.0/5*frontSteerAngle.GetVoltage() + 170.0/5*frontTrim.GetVoltage();
		float rearReal = 360.0/5*rearSteerAngle.GetVoltage() + 170.0/5*rearTrim.GetVoltage();
		
		
		
		if(frontReal > 360){
			frontReal -= 360;
		}
		
		if(rearReal > 360){
			rearReal -= 360;
		}
	
		DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();

		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "QC Elite Team # 648");

		dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "%f" , frontTarget);
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "%f",  frontReal);
		dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "%f" , rearTarget);
		dsLCD->Printf(DriverStationLCD::kUser_Line5, 1, "%f",  rearReal);
		dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "%f",  rearSteer.Get());
		dsLCD->UpdateLCD();
		
		Wait(5);
		
		frontSteer.Set(0.5);
		Wait(2);
		frontSteer.Set(0.0);
		
		rearSteer.Set(0.5);
		Wait(2);
		rearSteer.Set(0);

		
		
		frontReal = 360.0/5*frontSteerAngle.GetVoltage() + 170.0/5*frontTrim.GetVoltage();
		rearReal = 360.0/5*rearSteerAngle.GetVoltage() + 170.0/5*rearTrim.GetVoltage();
		
		
		
		if(frontReal > 360){
			frontReal -= 360;
		}
		
		if(rearReal > 360){
			rearReal -= 360;
		}
		
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "QC Elite Team # 648");

		dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "%f" , frontTarget);
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "%f",  frontReal);
		dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "%f" , rearTarget);
		dsLCD->Printf(DriverStationLCD::kUser_Line5, 1, "%f",  rearReal);
		dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "%f",  rearSteer.Get());
		dsLCD->UpdateLCD();
	
	}

	void OperatorControl(void)
	{
		printf("Operator Control 28-Feb-14\n");

		myRobot.SetSafetyEnabled(true);


		Wait(0.1);


		while (IsOperatorControl())
		{
			myRobot.SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);
			myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
			myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
			myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
			
			adjustAngles();
			
			float stickx = stick_l.GetX();
			float sticky= -stick_l.GetY();
			
			if(stickx < 0.2 && stickx > -0.2){
				stickx = 0;
			}
			
			if(sticky < 0.2 && sticky > -0.2){
				sticky = 0;
			}
			float magnitude = sqrt(pow(stickx, 2) + pow(sticky, 2));
			
			if(magnitude > 0){
				float theta = asin(stickx/magnitude) * 180 / 3.14159;
				if(sticky<0){
					theta = 180-theta;
				}
				
				rearTarget = -theta;
				frontTarget = theta;
				
			}
			
			
			float throttle = magnitude / sqrt(2);
			int dir = -1;
			if(rearReversed){
				dir = 1;
			}
		
			/*
			frontLeft.Set(throttle);
			frontRight.Set(throttle);
			rearRight.Set(throttle*dir);
			rearLeft.Set(throttle*dir);
			*/
			
			Wait(0.005);				// wait for a motor update time

		}

	}
};

START_ROBOT_CLASS(RobotDemo);
