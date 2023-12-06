#include "deliveryRobot.hpp"
// all functions here block
// all units are still in cm or degrees
/*
void moveDist(double dist){
	allMotors.tare_position();
	allMotors.move_absolute(dist/GEAR_DIAMETER*360,100);
	while(!((allMotors.get_positions()[0] < dist/GEAR_DIAMETER*360 + ANGLE_ERR) && (allMotors.get_positions()[0] > dist/GEAR_DIAMETER*360-ANGLE_ERR))){
		if(leftBumper.get_new_press() || rightBumper.get_new_press()){
			allMotors.move_absolute(0,-100);
			while(!((allMotors.get_positions()[0] < ANGLE_ERR) && (allMotors.get_positions()[0] > -ANGLE_ERR))){
				pros::delay(2); //block until position back at 0
			}
			throw ObstacleFound();
		}
		pros::delay(2);
	}
	allMotors.brake();
}*/
void turn90(bool right){
	gyro.reset(true);
	while(!(gyro.get_heading()>90-ANGLE_ERR && gyro.get_heading()<90+ANGLE_ERR)){
		leftMotors.move(right?-40:40);
		rightMotors.move(right?-40:40);
		pros::delay(2);
	}
	leftMotors.brake();
	rightMotors.brake();
}
void turn(int targetDir){
	static const int rotationMap[4] = {0,3,2,1};
	int reps = rotationMap[targetDir>robotDir?4+(robotDir-targetDir)%4:(robotDir-targetDir)%4];
	// 270 degree turn == -90 degree turn. For optimization.
	allMotors.brake();
	if(reps==3){
		turn90(0);
		return;
	}
	for(int i = 0; i < reps; i++)
		turn90(1);
}