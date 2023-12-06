#include "deliveryRobot.hpp"
// EXTERNAL variable definitions. These are global to all linked files.
const int dirX[4] = {0,1,0,-1};
const int dirY[4] = {1,0,-1,0};
Map virtualMap;
Position robotPos=Position(5,0);
int robotDir=0;
int destination=-1;
pros::Controller master=pros::Controller(pros::E_CONTROLLER_MASTER);
pros::Imu gyro=pros::Imu(IMU_PORT);
pros::Distance distSensor=pros::Distance(DIST_PORT);
//pros::ADIDigitalIn leftBumper=pros::ADIDigitalIn(BUMPER_L_PORT);
//pros::ADIDigitalIn rightBumper=pros::ADIDigitalIn(BUMPER_R_PORT);
pros::Motor_Group leftMotors=pros::Motor_Group({-MOTOR_L1_PORT,-MOTOR_L2_PORT});
pros::Motor_Group rightMotors=pros::Motor_Group({-MOTOR_R1_PORT,-MOTOR_R2_PORT});
pros::Motor_Group allMotors=pros::Motor_Group({-MOTOR_L1_PORT,-MOTOR_L2_PORT,-MOTOR_R1_PORT,-MOTOR_R2_PORT});
// This function is called as soon as the program is started.
void initialize(){
	pros::lcd::initialize();
	gyro.reset(true);
	initializeMap(virtualMap);
}
// This function will run immediately following initialize().
void opcontrol(){
	while(true){
		setDestination();
		if(destination!=-1){
			while(true){
				pros::lcd::set_text(1, "Heading to destination "+std::to_string(destination-1));
				pros::lcd::set_text(2, "Delivery on the way!");
				try{
					followPath(getDeliveryPath());
					pros::lcd::set_text(1, "Delivery Successful!");
					pros::lcd::set_text(2, "Choose another destination");
					break;
				}catch(NoPathFound &e){
					pros::lcd::set_text(1, e.what());
					pros::lcd::set_text(2, "Choose another destination");
					break;
				}catch(ObstacleFound &e){
					pros::lcd::set_text(1, e.what());
					pros::lcd::set_text(2, "Recalculating path...");
				}catch(...){
					//prints to PROS terminal when connected to computer
					fprintf(stderr, std::to_string(errno).c_str());
				}
				pros::delay(2);
			}
		}
		destination=-1; 
		pros::delay(2);
	}
}
/*
- does not exit or crash
- no infinite loops
- no errors
- no exceptions thrown
*/