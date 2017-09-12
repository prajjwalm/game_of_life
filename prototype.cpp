/*
 * prototype.cpp
 *
 *  Created on: 11-Sep-2017
 *      Author: Prajjwal Mishra
 *
 *      prototype version 1
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

enum state {dead,live}; 			// dead <=> 0, live <=> 1


const int SZ=10;

class cell {
	state curr,prev;
	int n;						// no of live neighbours
public:
	cell();
	state getstate();			// gives current state
	void set_prev();			// assigns curr to prev
	int set_n(int);				// assigning of n
	state init_state(state);	// forced assigning of state
	state setstate();			// sets the current state based on prev, n and returns it
};

class map {
	cell matrix[SZ][SZ];
	int gen;
	void update_n();		// sets n, based on current states, for future use
public:
	map();
	void rand_init();
	void next_gen();
	void display();
};


/*
 * inititalize::1.rand_init(), ie. set curr of all the cells randomly, and
 * 				2. based on the current state, calculate n for all the cells
 *
 *
 *
 * iterate:: 	0. to prepare for iteration make all current states prev...
 * 				1. setstate() for all cells on the map
 * 				2. based on the current state, calculate n for all the cells
 * 				3. gen++;
 * 				4. display();
 * 				0a. update map, i.e  make all current states prev, and,
 * 				1a. setstate() again...
 *
 */

// Cell functions...

cell::cell(){
	//called only when board is created, sets all at 0
	curr=dead;
	prev=dead;
	n=0;
}


state cell::getstate(){
		return curr;
	}

void cell::set_prev(){
	prev=curr;
}

int cell::set_n(int i){
	n=i;
	return i;
}



state cell::init_state(state s){
	curr=s;
	return curr;
}

state cell::setstate(){
	// sets and returns the current cell state in terms of the previous state and n
	if (prev==dead) {
		if (n==3) {
			curr=live;
		}
		else {
			curr=dead;
		}
	}
	else {
		if (n==2 || n==3){
			curr=live;
		}
		else {
			curr=dead;
		}
	}
	return curr;
}

// Map functions...

map::map(){
	gen=0;
}

void map::rand_init(){
	int x,y;
	int i;
	// randomizing the initial states...
	srand(time(0));
	for (x=0; x<SZ; x++){
		for (y=0; y<SZ; y++){
			state s1;
			i=rand()%2;
			if (i==1) {
				s1=live;
			}
			else {
				s1=dead;
			}
			matrix[x][y].init_state(s1);
		}
	}

	update_n();
}

void map::update_n(){
	int x,y;
	state s1;
	//creating the 2D array which will later be assigned to the matrix's cells...
	int n_val[SZ][SZ];

	for (x=0; x<SZ; x++){
		for (y=0; y<SZ; y++){
			n_val[x][y]=0;
		}
	}

	/*
	 * modifying map, taking
	 * (x,y) as
	 *
	 * (0,0), (0,1), (0,2), ... , (0,SZ-1)
	 * (1,0), (1,1), (1,2), ... , (1,SZ-1)
	 * ...
	 * (SZ-1,0), (SZ-1,1), (SZ-1,2), ... , (SZ-1,SZ-1)
	 *
	 */

	for (x=0; x<SZ; x++){
			for (y=0; y<SZ; y++){
				s1=matrix[x][y].getstate();
				if(s1==live){
					switch (x){
						case 0:
							switch (y){
								case 0:
									n_val[0][1]++;
									n_val[1][0]++;
									n_val[1][1]++;
									break;
								case (SZ-1):
									n_val[0][SZ-2]++;
									n_val[1][SZ-2]++;
									n_val[1][SZ-1]++;
									break;
								default:
									for (int i=0;i<2;i++){
										for (int j=-1;j<2;j++){
											if (i || j) n_val[i][y+j]++;
										}
									}
									break;
							}
							break;
						case (SZ-1):
							switch (y){
								case 0:
									n_val[SZ-1][1]++;
									n_val[SZ-2][0]++;
									n_val[SZ-2][1]++;
									break;
								case (SZ-1):
									n_val[SZ-1][SZ-2]++;
									n_val[SZ-2][SZ-2]++;
									n_val[SZ-2][SZ-1]++;
									break;
								default:
									for (int i=0;i<2;i++){
										for (int j=-1;j<2;j++){
											if (i || j) n_val[SZ-1-i][y+j]++;
										}
									}
								}
							break;
								default:
									switch (y){
									case 0:
										for (int i=-1;i<2;i++){
											for (int j=0;j<2;j++){
												if (i || j) n_val[x+i][j]++;
											}
										}
										break;
									case (SZ-1):
										for (int i=-1;i<2;i++){
											for (int j=0;j<2;j++){
												if (i || j) n_val[x+i][SZ-1-j]++;
											}
										}
									break;
									default:
										for (int i=-1;i<2;i++){
											for (int j=-1;j<2;j++){
												if (i || j) n_val[x+i][y+j]++;
											}
										}
										break;
									}
									break;
						}
					}
			}
	}

	// assigning to the matrix...
	for (x=0; x<SZ; x++){
		for (y=0; y<SZ; y++){
			matrix[x][y].set_n(n_val[x][y]);
		}
	}
}

void map::next_gen(){
	//assumes n is updated
	int x,y;
	for (x=0; x<SZ; x++){
		for (y=0; y<SZ; y++){
			matrix[x][y].set_prev();
			matrix[x][y].setstate();		//note: given n, setstate is independant of the prev values of
											// 		neighbouring cells
		}
	}
	update_n();
	gen++;
	display();
}

void map::display(){
	cout<<"\n\n\nGen "<<gen<<": \n\n";
	state s;
	for (int x=0;x<SZ;x++){
		for (int y=0;y<SZ;y++){
			s=matrix[x][y].getstate();
			if (s==live) {
				cout<<"+ ";
			}
			else {
				cout<<"- ";
			}
		}
		cout<<endl;
	}
}

int main(){
	map M;
	M.rand_init();
	for (int i=0;i<5;i++)
		M.next_gen();
	return 0;
}
