#ifndef PROS_AND_CUSTOM
#define PROS_AND_CUSTOM
#include "main.h"
// useful macros
#define IMU_PORT 1
#define MOTOR_L1_PORT 3
#define MOTOR_L2_PORT 2
#define MOTOR_R1_PORT 4
#define MOTOR_R2_PORT 5
#define MOTOR_CAP_L_PORT 6
#define MOTOR_CAP_R_PORT 7 
#define DIST_PORT 8
//#define BUMPER_L_PORT 1
//#define BUMPER_R_PORT 2
//units are in cm or degrees
//old unit dist — (6.35 * 4 + 2*2)
#define UNIT_DIST 15
#define GEAR_DIAMETER 5
#define UNIT_ROTATION (UNIT_DIST/GEAR_DIAMETER * 360)
#define ANGLE_ERR 0.5
#define MAX_R 50
#define MAX_C 50
// virtual map stuff
class Map{
private:
    int container[MAX_R][MAX_C]={{}};
    int destinationCount=0;
public:
    void mark(int row, int col, bool isObstacle);
    class Proxy{
    private:
        int* rowArr;
    public:
        Proxy(int* r);
        int& operator[](int c);
    };
    Proxy operator[](int r);
};
struct Position{
    int row;
    int col;
    Position(int r, int c);
};
void tareRobot();
void setDestination();

// custom exceptions
class ObstacleFound : public std::exception{
    public:
    ObstacleFound(){}
    const char* what();
};
class NoPathFound : public std::exception{
    public:
    NoPathFound(){}
    const char* what();
};

// Robot controls
void moveDist(double dist);
void turn90(bool right);
void turn(int targetDir);
void moveCap(bool open);

// Path finding stuff
struct Node{
	Position pos;
	std::shared_ptr<Node> parent; 
	Node(Position p);
	Node(Position p, std::shared_ptr<Node> n);
	bool operator==(const Node &other);
	bool operator==(const Position &other);
};
std::shared_ptr<Node> getDeliveryPath();
void followPath(std::shared_ptr<Node> pathHead);

// external variable declarations
extern const int dirX[4];
extern const int dirY[4];
extern Map virtualMap;
extern Position robotPos;
extern int robotDir;
extern int destination;
extern pros::Controller master;
extern pros::Imu gyro;
extern pros::Distance distSensor;
//extern pros::ADIDigitalIn leftBumper;
//extern pros::ADIDigitalIn rightBumper;
extern pros::Motor_Group leftMotors;
extern pros::Motor_Group rightMotors;
extern pros::Motor_Group capMotors;
#endif