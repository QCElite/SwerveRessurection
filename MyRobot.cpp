#include "WPILib.h"

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

	DigitalInput	digitalbit1;	// digital bit 1 from option switch
	DigitalInput	digitalbit2;	// digital bit 2 from option switch
	DigitalInput	digitalbit4;	// digital bit 4 from option switch
	DigitalInput	digitalbit8;	// digital bit 8 from option switch
	DigitalInput	catback;		// catapult in back position
	DigitalInput	launchRelease;	// motor attached to dog gears
	Relay			ballpickup;		// control arm motor
	Relay			solonoid2;		// control of firing 2
	Talon			winch;			// winch motor control
	Talon			arm;			// arm motor control
	
	int			imode_select;		// mode selection from mode switch
	int			feed_spin_dir;		// feed spinning direction
	
	bool		feed_but11_press;	//	feed button 11 pressed
	bool		feed_but12_press;	//	feed button 12 pressed	


public:
	RobotDemo():
		myRobot(1,2,3,4),		// these must be initialized in the same order
		stick_l(1),				// as they are declared above.
		stick_r(2),				// right joystick
		digitalbit1(1),			// digital in bit 1
		digitalbit2(2),			// digital in, bit 2
		digitalbit4(3),			// digital in, bit 4
		digitalbit8(4),			// digital in, bit 8
		catback(5),				// catapult pulled back
		launchRelease(6),		// dog gear release
		ballpickup(2),			// ball pickup control
		solonoid2(3),			// solonoid firing control 2
		winch(5),				// winch motor control
		arm(6)					// arm motor control
	{
		myRobot.SetExpiration(0.1);
	}

	void RobotInit(void)
	{
//		int	itmp1 =0;
		printf("RobotInit Started\n");
			
		imode_select = 0;

//		now get the operational mode from mode switch

//		get_mode();			//	get option mode
		
		
		printf("RobotInit Completed, imode_select = %4d", imode_select);
		
	}


	/**
	 * Drive Autonomous mode
	 */
	void Autonomous(void)
	{
	
		myRobot.SetInvertedMotor(RobotDrive::kFrontLeftMotor, false);
		myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, false);
		myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, false);
		myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, false);
		myRobot.SetSafetyEnabled(false);
		
		get_mode();
		
		switch(imode_select){
		case 0:
			myRobot.Drive(0.25, 0.0);
			Wait(4.2);
			myRobot.Drive(0.0, 0.0);
			Wait(1);
			
			arm.Set(0.75);
			Wait(0.5);
			arm.Set(0.0);
			
			winch.Set(-0.75);
			while(catback.Get()){
				Wait(0.05);
			}
			winch.Set(0.0);
			Wait(1.5);
			
			solonoid2.Set(Relay::kForward);		// start trigger
			//Wait until limit switch released
			Wait(2.0);
			while(!launchRelease.Get()){
			
			}
			//Continue until it is triggered again
			while(launchRelease.Get()){ 
			
			}
			solonoid2.Set(Relay::kOff);			//stop trigger
			
			break;
		case 1:
			myRobot.Drive(0.5, 0.0);			// start drive
			Wait(2.0);
			myRobot.Drive(0.0, 0.0);			// stop drive
			break;
		case 2:
			break;
		}
		
		// below is Pekin autonomous
		// CODE ADDED BY BRIAN -------------------------
		/*
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(0.25, 0.0); 			// drive forward .25 speed
		Wait(2.5); 							// for 2.5 second
		myRobot.Drive(0.0, 0.0); 			// stop robot
		arm.Set(0.0);						// move arm forward
		Wait(0.25);							// for .25 second
		arm.Set(0.0);						// stop arm
		winch.Set(-0.75);					// wind winch back at .75 speed
		Wait(0.8);							// for .8 second
		winch.Set(0.0);						// stop winch
		solonoid2.Set(Relay::kForward);		// start launcher release
		Wait (1.0);							// delay 1 second
		solonoid2.Set(Relay::kOff);			// stop launcher release
		myRobot.Drive(0.0, 0.0);  	 		// drive forward 0 speed
		Wait(1.0);							// for 1 second
		myRobot.Drive(0.0, 0.0);			// stop robot
		*/
		// CODE ADDED BY BRIAN -------------------------
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		float	tmp_stickx, tmp_sticky;
		bool pulling = false; // should winch pull
//		bool	loopflg;
		
		printf("Operator Control 28-Feb-14\n");
		
		myRobot.SetSafetyEnabled(true);
		
		DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
		
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "QC Elite Team # 648");

		dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "Time: %4.1f", GetClock());
		dsLCD->UpdateLCD();

		Wait(0.1);
		

		while (IsOperatorControl())
		{
			myRobot.SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);
			myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
			myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
			myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
			
			int driveDirection = 1;
			if(stick_l.GetRawButton(4) or stick_l.GetRawButton(8)){
				driveDirection = -1;
			}
			
			if (!stick_l.GetRawButton(1))
			{

//	trigger button not pressed
				tmp_stickx = stick_l.GetX();
				tmp_sticky = stick_l.GetY();
//				printf("x stick value = %6.3f\n", tmp_stickx);
				tmp_stickx = tmp_stickx * 0.65;
				if (!stick_l.GetRawButton(2))
				{

//	trigger 2 not pressed so use "learner's permit" mode
//	run at half speed mode
				tmp_sticky = tmp_sticky / 2.0;
				}
				myRobot.Drive(tmp_sticky * driveDirection, tmp_stickx * driveDirection);				
			} else
			{
			//  here if button 1 is pressed
			myRobot.ArcadeDrive(stick_l.GetY() * driveDirection, stick_l.GetX(), false); // drive with arcade style (use right stick)				
			}

//	operator code for arm, roller, and winch control
			// CODE ADDED BY BRIAN -------------------------
			if (stick_r.GetRawButton(11)){// roller in
				while(stick_r.GetRawButton(11)){}
				
				if(ballpickup.Get() == Relay::kForward){
					ballpickup.Set(Relay::kOff);
				}else{
					ballpickup.Set(Relay::kForward);		// roller in
				}
			} else if  (stick_r.GetRawButton(12)) {		// roller out
				while(stick_r.GetRawButton(12)){}
				
				if(ballpickup.Get() == Relay::kReverse){
					ballpickup.Set(Relay::kOff);
				}else{
					ballpickup.Set(Relay::kReverse);		// roller out
				}
			}
			if(stick_r.GetRawButton(2)){
				arm.Set(-stick_r.GetY() * 0.75 	);
			}else{
				arm.Set(0);
			}
			
			
			// Manual Trigger
			if(stick_r.GetRawButton(8)){
				solonoid2.Set(Relay::kForward);		// start trigger
				//Wait until limit switch released
				while(!launchRelease.Get()){
				
				}
				//Continue until it is triggered again
				while(launchRelease.Get()){
				
				}
				solonoid2.Set(Relay::kOff);			//stop trigger
			}
			
			if(stick_r.GetRawButton(1) && !catback.Get()){
				solonoid2.Set(Relay::kForward);		// start trigger
				//Wait until limit switch released
				while(!launchRelease.Get()){
				
				}
				//Continue until it is triggered again
				while(launchRelease.Get()){
				
				}
				solonoid2.Set(Relay::kOff);			//stop trigger
			}
			
			//Manual Winch
			if(stick_r.GetRawButton(7)){
				if(catback.Get()){
					winch.Set(-0.75);
				}else{
					winch.Set(0.0);
				}
			}else if(!pulling){
				winch.Set(0.0);
			}
			
			//Auto Winch
			if(stick_r.GetRawButton(4)){
				if(catback.Get()){
					pulling = true;						//set winch to pull back
					winch.Set(-0.75);
				}
			}
			
			if(pulling){
				if(!catback.Get()){
					pulling = false;					//set winch to stop
					winch.Set(0.0);
				}
			}
			
			/*
			if (stick_r.GetRawButton(7)){				// wind winch back to specific position
				winch.Set(-0.0);						// wind winch back to specific position
				Wait(0.0);								// time to get to position
			} else if (stick_r.GetRawButton(8))	{		// manually wind the winch back
				winch.Set(-0.5);						// manually wind the winch back
			} else {									// stop winch
				winch.Set(0.0);							// stop winch
			}
			*/
			for(int x = 0; x < 15; x++){
				if(stick_r.GetRawButton(x)){
					dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "Button pushed: %4.1f", x);
					dsLCD->UpdateLCD();
				}
			}
			
			// CODE ADDED BY BRIAN -------------------------
			Wait(0.005);				// wait for a motor update time

		}
		
//
//		now check for various buttons
//
//		Check for if tigger is pulled to fire
//
			//if ( stick_r.GetRawButton(1) )
			//{
//	fire the catapult
//
				//if ( !catback.Get())
				//{
					// here if fire button pulled and catapult is ready
					//fire_catapult();
				//}
			//}
//
//		Check for pull back of catapult
//
			//if ( stick_r.GetRawButton(4))
			//{
//	pull back winch
				//if ( catback.Get())
				//{
					// pull back winch and winch not already back
					//winch.Set( 0.5);	// start winding the winch back
					//while ( catback.Get())
					//{
						//Wait( 0.05);
					//}
					//winch.Set( 0.0);	// stop the winch
				//}
			//}
//
//		Check for Feeder Roller Control
//
			
			//if (!stick_r.GetRawButton(11))		// roller IN
			//{
				//if ( !feed_but11_press)
				//{
				//  button just pressed 
					//if ( feed_spin_dir == -1)
					//{
					// spinning wrong direction, first stop it
					//ballpickup.Set(Relay::kOff);
					//Wait	(0.5);
					//ballpickup.Set(Relay::kForward);
					//feed_spin_dir = 1;
					//}
					//else if ( feed_spin_dir == 0 )
					//{
					//ballpickup.Set(Relay::kForward);
					//feed_spin_dir = 1;
					//}
					//else if ( feed_spin_dir == 1 )
					//{
					// spinning correct direct, stop
					//ballpickup.Set(Relay::kOff);
					//feed_spin_dir = 0;
					//}

				//feed_but11_press = true;
				//}
			//}
			//else
			//{
		//  button 11 released
				//feed_but11_press = false;
			//}
//
//		now check button 12
//
			//if (!stick_r.GetRawButton(12))		// roller IN
			//{
				//if ( !feed_but12_press)
				//{
				//  button just pressed 
					//if ( feed_spin_dir == -1)
					//{
					// spinning wrong direction, first stop it
					//ballpickup.Set(Relay::kOff);
					//Wait	(0.5);
					//ballpickup.Set(Relay::kReverse);
					//feed_spin_dir = 1;
					//}
					//else if ( feed_spin_dir == 0 )
					//{
					//ballpickup.Set(Relay::kReverse);
					//feed_spin_dir = 1;
					//}
					//else if ( feed_spin_dir == 1 )
					//{
					// spinning correct direct, stop
					//ballpickup.Set(Relay::kOff);
					//feed_spin_dir = 0;
					//}

				//feed_but12_press = true;
				//}
			//}
			//else
			//{
		//  button 12 released
				//feed_but12_press = false;
			//}
//
//
//		now move the loading arm
//
//			loopflg = true;				// enable looping
//			while ( loopflg )
//			{
				//{
//	loading arm activate button  pressed
					//tmp_sticky = stick_l.GetY();
//					printf("y stick value = %6.3f\n", tmp_sticky)
					//if ( tmp_sticky < 0 )
					//{
			// stick is pulled back, pull arm back
						//if ( !armback.Get())
						//{
					// if arm is all the way back, limit switch is hit
							//arm.Set(0.0);
							//loopflg = false;
						//}
						//else
						//{
					// start motor and pull arm in
							//arm.Set(0.25);
							//Wait (0.005);
						//}
					//}
					//else if ( tmp_sticky > 0 )
					//{
				// stick is pushed forward, push arm forward
						//if ( !armfwd.Get())
						//{
						// if arm is all the way forward, limit switch is hit
							//arm.Set(0.0);
							//loopflg = false;
						//}
						//else
						//{
						// start motor and push arm out
							//arm.Set(-0.25);
							//Wait (0.005);
						//}
					//}
				//}
				//else
				//{
//					loopflg = false;		// stop the loop
				//}
//			}
//
//		end of main loop
//
//		}					// end of while loop
//
//
//		end of match fire shooter
//
//
		fire_catapult();
	}
//
//
//		Fire the catapult
//
	void	fire_catapult(void)
	{
		solonoid2.Set(Relay::kForward);
		Wait (1);	// delay 0.25 second
		solonoid2.Set(Relay::kOff);
	}
//
//
//
//	init code
//
//	void	init_settings(void)
//	{
//		//		int	itmp1 =0;
//				printf("Init_settings Started\n");
//					
//				imode_select = 0;
//
//		//		now get the operational mode from mode switch
//
//				get_mode();			//	get option mode
//				
//		printf("Init_settings completed\n");
//						
//	}
//
//	
//
//		Special debug printing
//
//	void	special_debug()
//
//		printout vars
//
//	{
//
//	}
//
//
//		subtract two numbers with fudge factor
//
//	int		subtract_fudge( int itmp1, int itmp2, int itmp3)
//
//		arg 1 & 2 = values to subtract arg2 from arg1
//			fudge factor is arg3
//			returns difference
//
//	{
//		int itmp4;
//		int	itmp5;
//
//		itmp4 = itmp1 - itmp2;
//		itmp5 = itmp4;
//		if ( itmp5 < 0) itmp5 = -itmp5;
//		if ( itmp5 < itmp3) itmp4 = 0;
//
//		printf("subtract, itmp1 %d, itmp2 %d, itmp3 %d\n", itmp1, itmp2, itmp3);
//		printf("subtract itmp4 = %d\n", itmp4);
//
//		return itmp4;
//		
//	}
//
//
//		get the setting of the mode switch
//
	void	get_mode(void)
	{
		
		imode_select = 0;
		if (!digitalbit1.Get()) printf("get_mode = bit1 set\n");
		if (!digitalbit2.Get()) printf("get_mode = bit2 set\n");
		if (!digitalbit4.Get()) printf("get_mode = bit4 set\n");
		if (!digitalbit8.Get()) printf("get_mode = bit8 set\n");

		if (!digitalbit1.Get()) imode_select = imode_select + 1;
		if (!digitalbit2.Get()) imode_select = imode_select + 2;
		if (!digitalbit4.Get()) imode_select = imode_select + 4;
		if (!digitalbit8.Get()) imode_select = imode_select + 8;
		
		printf("Get_mode completed, imode_select = %d \n",imode_select);
	}
	
	/**
	 * Runs during test mode
	 */
//	void Test()
//	{
//
//		float	edirection = 0.0;
//
//		int		ibut = 0, ibut2 = 0;
//		
//		printf("\n test mode entered \n");
//		
//		myRobot.SetSafetyEnabled(false);
//		
//		get_mode();			//   get option mode
//		
//		printf(" test mode, imode_select = %d\n", imode_select);
//		
//		switch (imode_select)
//		{
//		case	0:
//			break;
//		case	1:
//
//
//			break;
//		case	2:
//
//
//			break;
//		case	3:
//
//	
//			break;
//		case	4:
//
//
//			break;
//		case	5:
//
//		calibrate talons
//
//			Wait	(2.0);
//			myRobot.Drive(1.0, 0.0);
//			Wait	(5.0);
//			myRobot.Drive(0.0, 0.0);
//			Wait	(2.0);
//			myRobot.Drive(-1.0, 0.0);
//			Wait	(5.0);
//			myRobot.Drive(0.0, 0.0);
//			printf("calibration complete\n");
//			break;
//		case	6:
//
//
//			break;
//		case	7:
//			ibut = 0;		// init ibut to zero
//			
//			printf("Start of test 7");
//			
//			DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
//			
//			dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "QC Elite Team # 648");
//			dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "Button hit:  %3d", ibut);
//
//			dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "Time: %4.1f", GetClock());
//			dsLCD->UpdateLCD();
//			
//			while ( 1 == 1)
//			{
//				for ( ibut = 1; ibut <=12; ibut++ )
//				{
//					if ( stick_l.GetRawButton(ibut)) 
//						{
//						ibut2 = ibut;
//						printf("button pressed %3d\n", ibut2);
//						}
//					Wait (0.05);
//				}
//				dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "Button hit:  %3d", ibut2);
//				dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "Time: %4.1f", GetClock());
//				dsLCD->UpdateLCD();
//				Wait (3.0);
//				printf("test 7 loop \n");
//			}
//
//			break;
//		case 8:
//		{
//	
//		}
//			
//		default:
//			printf("test mode, option out of range\n");
//		}
//	printf("test mode completed\n");
//	}
};

START_ROBOT_CLASS(RobotDemo);

