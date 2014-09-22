#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

const int GRID_SIZE = 15;
const int GS = GRID_SIZE;

int init_grid(){
	int grid = [GS][GS] = {0};
	grid[GS-1][GS-1] = 10;
	return grid;
}

//initialize board
//print board
//loop through bellman backup
	//perform bellman backup
	//check bounds
	
int main(){
	printf("gs: %d \n", GS);
	int grid = init_grid();
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			printf("%d", grid[i][j]);
		}
	}
	return 0;
}

