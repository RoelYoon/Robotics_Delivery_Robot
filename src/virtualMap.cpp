#include "deliveryRobot.hpp"
// Map and Position definitions
/*
   virtual mapping:
   0 — unmarked/open spot
   1 - obstacle
   2+ — destination #(n-2)
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
//  selects destination based on controller input
void setDestination(){
	if(master.get_digital_new_press(DIGITAL_UP)){destination=2;}
	else if(master.get_digital_new_press(DIGITAL_LEFT)){destination=3;}
	else if(master.get_digital_new_press(DIGITAL_RIGHT)){destination=4;}
	else if(master.get_digital_new_press(DIGITAL_DOWN)){destination=5;}
	else if(master.get_digital_new_press(DIGITAL_X)){destination=6;}
	else if(master.get_digital_new_press(DIGITAL_Y)){destination=7;}
	else if(master.get_digital_new_press(DIGITAL_A)){destination=8;}
	else if(master.get_digital_new_press(DIGITAL_B)){destination=9;}
}
/* 
   Hard-code destination positions here. 
   Marking any obstacles here is not necessary, as that will 
   be done dynamically during run-time. 
*/
void initializeMap(Map &map){
    map.mark(1,1,0); //destination #1
}