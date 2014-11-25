#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

/*
Carolyn Saund
SARSA and Dyna-SARSA-RTDP implementation
5 October 2014
*/

/* SARSA
epsilon = 0.1
alpha = 0.1 
on policy TD control, reset to bottom left every time goal reached.
evaluate performance after every 100 steps.
-initialize the Q-values table, Q(s, a).
Repeat
		-Observe the current state, s.
		-Choose an action, a from s using policy derived from Q.
		Repeat	
				-Take the action, and observe the reward, r and the new state, s'.
				-Update the Q-value and (s,a) derived above
				-repeat the process until a terminal state is reached.
*/

/* Dyna-SARSA-RTDP
extends SARSA for simulated updates (drawing traces from a model
according to E-greedy policy and making single state Bellman updates.
m = 10 simulated episodes per step
evaluate performance after every 100 steps.
*/
const float ALPHA = 0.1;
const float EPSILON = 0.1;
const int GRID_SIZE = 15;
const int GS = GRID_SIZE;
const float GAMMA = 0.9;

struct cell{
	string action;
	float actval;
	float qright;
	float pright_states[GS][GS];
	float qleft;
	float pleft_states[GS][GS];
	float qup;
	float pup_states[GS][GS];
	float qdown;
	float pdown_states[GS][GS];
	float reward;
	string mark;
};

void print_array(cell[][GS]);
float up_val(cell[][GS], int, int);
float down_val(cell[][GS], int, int);
float left_val(cell[][GS], int, int);
float right_val(cell[][GS], int, int);
void move(cell grid[][GS], int* i, int* j, string action);
float policy_evaluation(cell grid[][GS]);
float quality(cell grid[][GS]);
//act_val_pair Qval(cell grid[][GS], int i, int j, string act);
string rand_act();
void init_pi(cell grid[][GS]);
float total_prob(float array[][GS]);
void dyna_sarsa(cell grid[][GS]);
void choose_state_probabalistically(cell grid[][GS], int* i, int* j, float pstates[][GS]);

void print_array(cell grid[][GS]){
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			printf("%.2f ", grid[i][j].qright);
			printf("%s ", grid[i][j].action.c_str());
		}
		printf("\n");
	}
	return;
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
		if(grid[i][j].reward == 10){
			return (pow(GAMMA, c)*10);
		}
		else{
			c++;
			move(grid, &i, &j, grid[i][j].action);
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


//returns truly randomly
string rand_act(){
	string acts[4] = {"^","d","<",">"};
	return acts[rand()%4];
}

void init_pi(cell grid[][GS]){
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			grid[i][j].action = rand_act();
		}
	}
}

//truly randomly decides
void update_policy(cell grid[][GS]){
	//cout << "updating policy" << endl;
	float max = 0;
	float vals[4]; 
	int up = 0;
	int down = 0;
	int right = 0;
	int left = 0;
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			
			max = 0;
			vals[0] = grid[i][j].qup;
			vals[1] = grid[i][j].qdown;
			vals[2] = grid[i][j].qleft;
			vals[3] = grid[i][j].qright;
			//max of these
			//need to nondeterministically choose
			//if all qvalues equal
			
			vector<int> indeces;
			for(int k = 0; k < 4; k++){
				if(max < vals[k]){
					max = vals[k];
				}
			}
			for(int k = 0; k < 4; k++){
				if(vals[k] == max){
					indeces.push_back(k);
				}
			}
			int rand_vec_ind = rand() % indeces.size();
			int ind_move = indeces[rand_vec_ind];
				if(ind_move == 0){
					grid[i][j].action = "^";
					grid[i][j].actval = grid[i][j].qup;
					up++;
				}
				else if(ind_move == 1){
					grid[i][j].action = "d";
					grid[i][j].actval = grid[i][j].qdown;
					down++;
				}
				else if(ind_move == 2){
					grid[i][j].action = "<";
					grid[i][j].actval = grid[i][j].qleft;
					left++;
				}
				else{
					grid[i][j].action = ">";
					grid[i][j].actval = grid[i][j].qright;
					right++;
				}

		}
	}
	//cout << up << " " << down << " " << left << " " << right << endl;
}

//move in SARSA
	//update Q
	//update p for action
//10 steps in dyna
void dyna_sarsa(cell grid[][GS]){
	//randomly initialize policy
	int i = 0; 
	int j = 0;
	int old_i = 0; 
	int old_j = 0;
	int real_i = 0;
	int real_j = 0;
	print_array(grid);
	cout << endl;
	int r = 0;
	string act = "";
	//srand(time(NULL));
	for(int k = 0; k < 601; k++){
		//initialize s to bottom left
		init_pi(grid);
		update_policy(grid);
		i = GS-1;
		j = 0;
		//cout << endl;
		//remake policy by choosing best action
		//pretty sure this means for all states and actions
		int l = 0;
		while(l < 500000 && (i != 0 || j != GS-1)){
			old_i = i;
			old_j = j;
			act = grid[i][j].action;
			r = rand() % 10;
			if(r < 1){	//10% chance performing random action
				act = rand_act();
			}
			if(act == "^"){
				move(grid, &i, &j, "^");
				grid[old_i][old_j].pup_states[i][j] += 200;	//arbitrary big jump in probability
				grid[old_i][old_j].qup = grid[old_i][old_j].qup + 
								 ALPHA * (grid[i][j].reward + GAMMA * 
								 grid[i][j].actval - grid[old_i][old_j].qup);
			}
			else if(act == "d"){
				move(grid, &i, &j, "d");
				grid[old_i][old_j].pdown_states[i][j] += 200;	//arbitrary big jump in probability
				grid[old_i][old_j].qdown = grid[old_i][old_j].qdown + 
								 ALPHA * (grid[i][j].reward + GAMMA * 
								 grid[i][j].actval - grid[old_i][old_j].qdown);
			}
			else if(act == ">"){
				move(grid, &i, &j, ">");
				grid[old_i][old_j].pright_states[i][j] += 200;	//arbitrary big jump in probability
				grid[old_i][old_j].qright = grid[old_i][old_j].qright + 
								 ALPHA * (grid[i][j].reward + GAMMA * 
								 grid[i][j].actval - grid[old_i][old_j].qright);
			}
			else if(act == "<"){
				move(grid, &i, &j, "<");
				grid[old_i][old_j].pleft_states[i][j] += 200;	//arbitrary big jump in probability
				grid[old_i][old_j].qleft = grid[old_i][old_j].qleft + 
								 ALPHA * (grid[i][j].reward + GAMMA * 
								 grid[i][j].actval - grid[old_i][old_j].qleft); 
			}
			//enter dream sequence
			for(int m = 0; m < 10; m++){
				//given i, j, use action given by Q (grid[i][j].action) to 
				//probabalistically go somewhere, according to probability
				//of that action going to any state.
				//update qvalues
				old_i = i;
				old_j = j;
				if(grid[i][j].action == "^"){
					choose_state_probabalistically(grid, &i, &j, grid[i][j].pup_states);
					grid[old_i][old_j].qup = grid[old_i][old_j].qup + 
								 ALPHA * (grid[i][j].reward + GAMMA * 
								 grid[i][j].actval - grid[old_i][old_j].qup); 
				}
				else if(grid[i][j].action == "d"){
					choose_state_probabalistically(grid, &i, &j, grid[i][j].pdown_states);
					grid[old_i][old_j].qdown = grid[old_i][old_j].qdown + 
								 ALPHA * (grid[i][j].reward + GAMMA * 
								 grid[i][j].actval - grid[old_i][old_j].qdown); 
				}
				else if(grid[i][j].action == "<"){
					choose_state_probabalistically(grid, &i, &j, grid[i][j].pleft_states);
					grid[old_i][old_j].qleft = grid[old_i][old_j].qleft + 
								 ALPHA * (grid[i][j].reward + GAMMA * 
								 grid[i][j].actval - grid[old_i][old_j].qleft); 
				}
				else if(grid[i][j].action == ">"){
					choose_state_probabalistically(grid, &i, &j, grid[i][j].pright_states);
					grid[old_i][old_j].qright = grid[old_i][old_j].qright + 
								 ALPHA * (grid[i][j].reward + GAMMA * 
								 grid[i][j].actval - grid[old_i][old_j].qright); 
				}

			}
			i = real_i;
			j = real_j;
			l++;
		}
		real_i = i;
		real_j = j;
		if(k % 50 == 0){
			cout << k << " " << quality(grid) << endl;
			//print_array(grid);
			cout << endl;
		}
	}
}

void choose_state_probabalistically(cell grid[][GS], int* x, int* y, float pstates[][GS]){
	float r = (rand() % 100) / 100 ;	//fix r to be between 0 and 1
	float total = total_prob(pstates);
	float sum = 0;
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			sum += pstates[i][j];		//slowly add probability until greater
			if(r < (sum / total)){		//so when sum/total sig changed, liklihood
				*x = i;					//r is between sig prob differences high
				*y = j;
				return;
			}
		}
	}
}

float total_prob(float array[][GS]){
	float sum = 0;
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			sum += array[i][j];
		}
	}
	return sum;
}


//state has action associeted, value for each action as well
//


/*dyna-SARSA
	for each state
		for each action
			probability of going to every other state
*/

void init_array(float array[][GS]){
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			array[i][j] = 1;
		}
	}
}

int main(){
	//cell grid[][GS] = init_grid();
	cell grid[GS][GS];
	for(int i = 0; i < GS; i++){
		for(int j = 0; j < GS; j++){
			grid[i][j].reward = 0;
			grid[i][j].actval = 0;
			grid[i][j].qup = 0;
			init_array(grid[i][j].pup_states);
			grid[i][j].qdown = 0;
			init_array(grid[i][j].pdown_states);
			grid[i][j].qleft = 0;
			init_array(grid[i][j].pleft_states);
			grid[i][j].qright = 0;
			init_array(grid[i][j].pright_states);
			grid[i][j].action = "";
			grid[i][j].mark = "O";
		}
	}
	grid[0][GS-1].reward = 10;
	srand(time(NULL));
	dyna_sarsa(grid);
	return 0;
}
