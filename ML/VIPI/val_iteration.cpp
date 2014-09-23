#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;

const int GRID_SIZE = 15;
const int GS = GRID_SIZE;
const float GAMMA = 0.9;
const float EPSILON = 0.001;
//initialize board
//print board
//loop through bellman backup
	//perform bellman backup
	//check bounds

struct cell{
	float old_val;
	float new_val;
};

void print_array(cell grid[][GS]){
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			printf("%.2f ", grid[i][j].new_val);
		}
		printf("\n");
	}
	return;
}
	
/*
int init_grid(){
	int grid[GS][GS];
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			grid[i][j] = 0;
		}
	}
	grid[GS-1][GS-1] = 10;
	return grid;
}
*/

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

float bellman_backup(cell grid[][GS], int i, int j){
	//max of sum of probs * values for each action
	float up,down,left,right = 0;
	//bounds check
	if(i == 0 && j == GS-1){ return 10; }
	down  = GAMMA * (1 	  *  down_val(grid, i, j));
	left  = GAMMA * (1 	  *  left_val(grid, i, j));
	up 	  = GAMMA * (0.5  *    up_val(grid, i, j) +
				  	 0.25 *  left_val(grid, i, j) +
				  	 0.25 * right_val(grid, i, j));
	right = GAMMA * (0.9  * right_val(grid, i, j) + 
					 0.05 *  left_val(grid, i, j) + 
					 0.05 *  down_val(grid, i, j));

	float max = 0;
	if (max < down)  {max = down;}
	if (max < left)  {max = left;}
	if (max < up)    {max = up;}
	if (max < right) {max = right;}

	return max;
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

int main(){
	cell grid[GS][GS];
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			grid[i][j].old_val = 0;
			grid[i][j].new_val = 0;
		}
	}
	grid[0][GS-1].new_val = 10;

	print_array(grid);
	cout << endl;
	int k = 0;
	while(true){
		for(int i = 0; i < GS; i++){
			for(int j = 0; j < GS; j++){
				grid[i][j].new_val = bellman_backup(grid, i, j);
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
		
		k++;
	}
	cout << k << endl;
	print_array(grid);
	//iterate through doing bellman backup, updating new_val,
	//at end, go through once more, updating old val to be new val.
	return 0;
}

