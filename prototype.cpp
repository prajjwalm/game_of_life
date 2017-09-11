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


enum state {dead,live}; 			// dead <=> 0, live <=> 1


const int SZ=10;

class cell {
	state curr,prev;
	int n;						// no of live neighbours
public:
	cell();
	state getstate();
	int set_n(int);				// assigning of n
	state init_state(state);	// forced assigning of state
	state setstate();			// sets the current state based on prev and returns it
};

class map {
	cell matrix[SZ][SZ];
	int gen;
public:
	map();
	void rand_init();
	void next_gen();
	void display();
	int set_active(int,int);
};

// Cell functions...

cell::cell(){
	//called only when board is created, sets all at 0
	curr=dead;
	prev=dead;

	n=0;
}


state cell::getstate(){
		return prev;
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
	// sets and returns the current cell state in terms of the prev and living neighbours
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

	//creating the 2D array which will later be assigned to the matrix's cells...
	int n_val[SZ][SZ];

	for (x=0; x<SZ; x++){
		for (y=0; y<SZ; y++){
			n_val[x][y]=0;
		}
	}

	// randomizing the initial states...
	srand(time(0));
	for (x=0; x<SZ; x++){
		for (y=0; y<SZ; y++){
			state s1;
			s1=rand()%2;
			matrix[x][y].init_state(s1);
			if (s1) {
				//increase n_val[x+-1][y+-1] by one, if they exist... (think of adding padding cells at edges)
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

