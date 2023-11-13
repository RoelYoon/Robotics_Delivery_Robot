#include "deliveryRobot.hpp"
const char* ObstacleFound::what(){
    return "Sensor detected an obstacle";
}
const char* NoPathFound::what(){
    return "No possible path to the destination was found";
}