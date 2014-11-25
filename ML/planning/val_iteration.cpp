#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;


/*
AML Project 1
Carolyn Saund
24 Sept 2014
*/

const int GRID_SIZE = 15;
const int GS = GRID_SIZE;
const float GAMMA = 0.9;
const float EPSILON = 0.001;
int bell_count = 0;
//initialize board
//print board
//loop through bellman backup
	//perform bellman backup
	//check bounds

struct cell{
	float old_val;
	float new_val;
	string old_act;
	string new_act;
};

struct act_val_pair{
	float value;
	string action;
};

void print_array(cell grid[][GS], bool vals){
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			if(vals){
				printf("%.2f ", grid[i][j].new_val);
			}
			else{
				printf("%s ", grid[i][j].new_act.c_str());
			}
		}
		printf("\n");
	}
	return;
}
	

float up_val(cell grid[][GS], int i, int j){
	if(i > 0){
		return grid[i-1][j].old_val;
	}
	else{
		return grid[i][j].old_val;
	}
}

float down_val(cell grid[][GS], int i, int j){
	if(i < GS-1){
		return grid[i+1][j].old_val;
	}
	else{
		return grid[i][j].old_val;
	}
}

float left_val(cell grid[][GS], int i, int j){
	if(j > 0){
		return grid[i][j-1].old_val;
	}
	else{
		return grid[i][j].old_val;
	}
}

float right_val(cell grid[][GS], int i, int j){
	if(j < GS-1){
		return grid[i][j+1].old_val;
	}
	else{
		return grid[i][j].old_val;
	}
}

act_val_pair bellman_backup(cell grid[][GS], int i, int j, string act){
	//max of sum of probs * values for each action
	float up,down,left,right = 0;
	act_val_pair act_val; 
	//bounds check
	if(i == 0 && j == GS-1){
		act_val.value = 10; 
		act_val.action = "x";
		return act_val;	   }
	down  = GAMMA * (1 	  *  down_val(grid, i, j));
	left  = GAMMA * (1 	  *  left_val(grid, i, j));
	up 	  = GAMMA * (0.5  *    up_val(grid, i, j) +
					 0.25 *  left_val(grid, i, j) +
					 0.25 * right_val(grid, i, j));
	right = GAMMA * (0.9  * right_val(grid, i, j) + 
					 0.05 *  left_val(grid, i, j) + 
					 0.05 *  down_val(grid, i, j));
	if(act == ""){
			bell_count += 4;
			float max = 0;
			if (max < down){
					max = down;
					act_val.value  = down;
					act_val.action = "d";   }
			if (max < left){
					max = left;
					act_val.value  = left;
					act_val.action = "<"; }
			if (max < up){
					max = up;
					act_val.value  = up;
					act_val.action = "^";   }
			if (max < right){
					max = right;
					act_val.value  = right;
					act_val.action = ">"; }
	}
	else{
		bell_count++;
		act_val.action = act;
		if(act == "d"){
			act_val.value = down;
		}
		else if(act == "<"){
			act_val.value = left;
		}
		else if(act == "^"){
			act_val.value = up;
		}
		else{
			act_val.value = right;
		}
	}
	return act_val;
}


bool infinity_norm_thresh(cell grid[][GS]){
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			if( (grid[i][j].old_val - grid[i][j].new_val) > EPSILON || 
				(grid[i][j].old_val - grid[i][j].new_val) < (0 - EPSILON)){
				return false;
			}
		}
	}
	return true;
		
}


void init_pi(cell grid[][GS], string acts[4]){
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			grid[i][j].old_act = acts[rand() % 4];
			grid[i][j].new_act = acts[rand() % 4];
		}
	}
}


void policy_stabilize(cell grid[][GS]){
	//do bellman, but restrict to policy; this converges --> generates value array	
	while(true){
			for(int i = 0; i < GS; i++){
				for(int j = 0; j < GS; j++){
					grid[i][j].new_val = bellman_backup(grid, i, j, grid[i][j].old_act).value;
				}
			}
			if(infinity_norm_thresh(grid)){
				break;
			}
			for(int i = 0; i < GS; i++){
				for(int j = 0; j < GS; j++){
					grid[i][j].old_val = grid[i][j].new_val;
				}
			}
	}
}


void policy_improvement(cell grid[][GS]){
	//check that each action matches max action
	act_val_pair av;
	av.action = "";
	av.value = 0;
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			//current action does not maximize
			av.action = bellman_backup(grid, i, j, "").action;
			if(av.action != grid[i][j].old_act){
				if(av.action != ""){
					grid[i][j].new_act = av.action;
					//also alter the value of state here?
				}
			}
		}
	}
}


//need to simulate probabalistically not going the right place
void move(cell grid[][GS], int* i, int* j, string action){
	float r = rand() % 100;
	if(action == "^"){
		if(r < 50){
			if(*i > 0){
				*i= *i-1;	
			}
		}
		//0.25 left
		else if(r >= 50 && r < 75){
			move(grid, i, j, "<");
		}
		//0.25 right
		else{
			if(*j < GS-1){
				*j = *j+1;
			}
		}
	}
	else if(action == "d"){
		if(*i < GS-1){
			*i = *i+1;
		}
	}
	else if(action == "<"){
		if(*j > 0){
			*j = *j-1;
		}
	}
	else if(action == ">"){
		//0.9 going right
		if(r < 90){
			if(*j < GS-1){
				*j = *j+1;
			}
		}
		//0.05 left
		else if(r >= 90 && r < 95){
			move(grid, i, j, "<");
		}
		//0.05 down
		else{
			move(grid, i, j, "d");
		}
	}
	else return;
}

float policy_evaluation(cell grid[][GS]){
	//start bottom left, 
	//gamma^#steps * reward
	//only one reward (top right)
	int i = GS-1;
	int j = 0;
	int c = 0;
	for(int k = 0; k < 100; k++){
		if(grid[i][j].old_val == 10){
			return (pow(GAMMA, c)*10);
		}
		else{
			c++;
			move(grid, &i, &j, grid[i][j].new_act);
		}
	}
	return 0;
}

float quality(cell grid[][GS]){
	float avg = 0;
	for(int i = 0; i < 100; i++){
		avg += policy_evaluation(grid);
	}
	return avg/100;
}

//randomly initialize a policy (an action for each state)
//calculate value of policy
//take greedy at every step
//check if same
//if not, reevaluate policy
void policy_iteration(cell grid[][GS]){
	string acts[4] = {"^", "d", ">", "<"};
	init_pi(grid, acts);
	int k = 0;
	float q = 0;
	bool match = false;
	while(!match){
		match = true;
		//update all the values in the grid
		policy_stabilize(grid);
		//update all actions to go towards those values
		policy_improvement(grid);
		//evaluate if any values were changed
		for (int i = 0; i < GS; i++){
			for(int j = 0; j < GS; j++){
				if(grid[i][j].old_act != grid[i][j].new_act){
					match = false;
					grid[i][j].old_act = grid[i][j].new_act;
				}
			}
		}
		q = quality(grid);
		cout << "b: " << bell_count << endl;
		cout << "quality: " << q << endl;
		k++;
	}
	cout << k << endl;
	print_array(grid, false);
	print_array(grid, true);
	cout << endl;
}

void value_iteration(cell grid[][GS]){
	int k = 0;
	float q = 0;
	while(true){
		for(int i = 0; i < GS; i++){
			for(int j = 0; j < GS; j++){
				grid[i][j].new_val = bellman_backup(grid, i, j, "").value;
			}
		}
		if(infinity_norm_thresh(grid)){
			break;
		}
	//iterate through doing bellman backup, updating new_val,
	//at end, go through once more, updating old val to be new val.
		for(int i = 0; i < GS; i++){
			for(int j = 0; j < GS; j++){
				grid[i][j].old_val = grid[i][j].new_val;
			}
		}
		policy_improvement(grid);
		q = quality(grid);
		cout << "b: " << bell_count << endl;
		cout << "quality: " << q << endl;
		k++;
	}
	cout << k << endl;
	print_array(grid, true);
}


int main(){
	//cell grid[][GS] = init_grid();
	cell grid[GS][GS];
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			grid[i][j].old_val = 0;
			grid[i][j].new_val = 0;
			grid[i][j].old_act = "";
			grid[i][j].new_act = "";
		}
	}
	grid[0][GS-1].new_val = 10;
	value_iteration(grid);
	policy_iteration(grid);
	return 0;
}

