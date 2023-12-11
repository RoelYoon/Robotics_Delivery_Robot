#include "deliveryRobot.hpp"
// Map and Position definitions
/*
   virtual mapping:
   0 — unmarked/open spot
   1 - obstacle
   2 - origin point (since last tare)
   3 - custom set destination
*/
void Map::mark(int row, int col, bool isObstacle){
    int val = isObstacle?1:destinationCount+2;
    container[row][col]=val;
    if(val>1){
        destinationCount++;
    }
}
Map::Proxy::Proxy(int* r):rowArr(r){}
int& Map::Proxy::operator[](int c){
    return rowArr[c];
}
Map::Proxy Map::operator[](int r){
    return Map::Proxy(container[r]);
}
Position::Position(int r, int c):row(r),col(c){}
void tareRobot(){
    robotPos=Position(MAX_R/2,MAX_C/2);
    pros::lcd::set_text(1, "Taring the robot...");
    pros::lcd::set_text(2, "Input robot's heading");
    while(true){
        if(master.get_digital_new_press(DIGITAL_UP)){robotDir=0;break;}
        else if(master.get_digital_new_press(DIGITAL_LEFT)){robotDir=3;break;}
        else if(master.get_digital_new_press(DIGITAL_RIGHT)){robotDir=1;break;}
        else if(master.get_digital_new_press(DIGITAL_DOWN)){robotDir=2;break;}
        pros::delay(2);
    }
    destination=-1;
    pros::lcd::set_text(1, "Robot is tared!");
    pros::lcd::set_text(2, "Select destination");
}
//  selects destination based on controller input
void setDestination(){
    static int destinationR=0;
    static int destinationC=0;
	if(master.get_digital_new_press(DIGITAL_A)){destination=2;}
	else if(master.get_digital_new_press(DIGITAL_X)){
        virtualMap[destinationR][destinationC]=0;
        destination=3;
        int rowCount=0, colCount=0;
        bool absolute = true;
        bool update=true;
        while(true){
            if(master.get_digital_new_press(DIGITAL_X)&&robotPos.row+rowCount>=0&&robotPos.row+rowCount<MAX_R&&robotPos.col+colCount>=0&&robotPos.col+colCount<MAX_C){break;}
        	if(master.get_digital_new_press(DIGITAL_UP)){rowCount++;update=true;}
            else if(master.get_digital_new_press(DIGITAL_LEFT)){colCount--;update=true;}
            else if(master.get_digital_new_press(DIGITAL_RIGHT)){colCount++;update=true;}
            else if(master.get_digital_new_press(DIGITAL_DOWN)){rowCount--;update=true;}
            if(master.get_digital_new_press(DIGITAL_Y)){
                absolute=!absolute;
                update=true;
            }
            if(update){
                pros::lcd::set_text(1, "Current destination:");
                pros::lcd::set_text(2, "("+std::to_string(absolute?robotPos.col+colCount:colCount)+", "+
                                        std::to_string(absolute?robotPos.row+rowCount:rowCount)+")");
                update=false;
            }
            pros::delay(2);
        }
        destinationR=robotPos.row+rowCount;
        destinationC=robotPos.col+colCount;
        virtualMap[robotPos.row+rowCount][robotPos.col+colCount]=3;
    }
    else if(master.get_digital_new_press(DIGITAL_B)){
        tareRobot();
    }
}