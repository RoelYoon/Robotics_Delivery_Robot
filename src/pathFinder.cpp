#include "deliveryRobot.hpp"
// Node struct definitions
Node::Node(Position p):pos(p),parent(nullptr){}
Node::Node(Position p, std::shared_ptr<Node> n):pos(p),parent(n){}
bool Node::operator==(const Node &other){
	return (other.pos.row==pos.row && other.pos.col==pos.col);
}
bool Node::operator==(const Position &other){
	return (other.row==pos.row && other.col==pos.col);
}
/*
  calculates path with BFS, returns pointer to the head of a linked list
  of Nodes that represents the delivery path (the head is the node at the
  destination, and the tail is the current robot's position).
*/
std::shared_ptr<Node> getDeliveryPath(){
	bool visited[MAX_R][MAX_C]={};
	std::queue<std::shared_ptr<Node>> bfsQ;
	bfsQ.push(std::make_shared<Node>(Node(Position(robotPos.row,robotPos.col))));
	visited[robotPos.row][robotPos.col]=1;
	while(!bfsQ.empty()){
		std::shared_ptr<Node> cur = bfsQ.front();
		visited[cur->pos.row][cur->pos.col]=1;
		if(virtualMap[cur->pos.row][cur->pos.col]==destination){
			return cur;
		}
		for(int i = 0; i < 4; i++){
			int nextRow = cur->pos.row + dirY[i];
			int nextCol = cur->pos.col + dirX[i];
			//bounds check, visited check, obstacle check
			if(nextRow>=0 && nextRow<MAX_R && nextCol>=0 && nextCol<MAX_C && visited[nextRow][nextCol]==0 && virtualMap[nextRow][nextCol]!=1){
				visited[nextRow][nextCol]=1;
				bfsQ.push(std::make_shared<Node>(Node(Position(nextRow,nextCol),cur)));
			}
		}
		bfsQ.pop();
		fprintf(stderr, std::to_string(errno).c_str());
		pros::delay(2);
	}
	throw NoPathFound();
}
/*
  makes the robot follow the delivery path of the linked list of Nodes
  given by getDeliveryPath().
*/
void followPath(std::shared_ptr<Node> pathHead){
	if(*pathHead==robotPos){return;}
	followPath(pathHead->parent);
	int targetDirection=0;
	for(int i = 0; i < 4; i++)
		if(robotPos.row+dirY[i]==pathHead->pos.row && robotPos.col+dirX[i]==pathHead->pos.col)
			targetDirection=i; 
	turn(targetDirection);
	//distance sensor check
	for(int i = 0; i < 10; i++){
		if(distSensor.get() <= UNIT_DIST*10){
			virtualMap.mark(pathHead->pos.row,pathHead->pos.col,1);
			fprintf(stderr, (std::to_string(pathHead->pos.row)+","+std::to_string(pathHead->pos.col)+"\n").c_str());
			throw ObstacleFound();
		}
		pros::delay(2);
	}
	//moves a unit
	leftMotors.tare_position();
	rightMotors.tare_position();
	leftMotors.move_relative(UNIT_ROTATION,40);
	rightMotors.move_relative(-UNIT_ROTATION,40);
	while(!((leftMotors.get_positions()[0] < UNIT_ROTATION + 5) && (leftMotors.get_positions()[0] > UNIT_ROTATION-5))){
		pros::delay(2);
	}
	robotPos.row+=dirY[robotDir];
	robotPos.col+=dirX[robotDir];
	/*
	while(!((allMotors.get_positions()[0] < UNIT_ROTATION + ANGLE_ERR) && (allMotors.get_positions()[0] > UNIT_ROTATION-ANGLE_ERR))){
		if(leftBumper.get_new_press() || rightBumper.get_new_press()){
			allMotors.move_absolute(0,-100);
			while(!((allMotors.get_positions()[0] < ANGLE_ERR) && (allMotors.get_positions()[0] > -ANGLE_ERR))){
				pros::delay(2); //blocks until position is back to 0
			}
			allMotors.brake();
			virtualMap.mark(pathHead->pos.row,pathHead->pos.col,1);
			throw ObstacleFound();
		}
		pros::delay(2);
	}*/
	pros::delay(2);
}