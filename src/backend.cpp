#include "backend.hpp"

// cell funtions...

backend::cell::cell(){
	//called only when board is created, sets all at 0
	curr=dead;
	prev=dead;
	n=0;
}

backend::cell::cell(state _p, int _n){
	n=_n;
	prev=_p;
}

backend::state backend::cell::getstate(){
		return curr;
	}

void backend::cell::set_prev(){
	prev=curr;
}

int backend::cell::set_n(int i){
	n=i;
	return i;
}



backend::state backend::cell::init_state(state s){
	curr=s;
	return curr;
}

backend::state backend::cell::setstate(){
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

int backend::cell::getn(){
	return n;
}

// universe functions...

backend::universe::universe(){
	gen=0;
}

void backend::universe::initialize(bool X[size][size]){
	gen=0;
	for (int i=0;i<size;i++){
		for (int j=0;j<size;j++){
			if (X[j][i])
				matrix[j][i].init_state(live);
			else
				matrix[j][i].init_state(dead);
		}
	}
}

int backend::universe::getnxy(int x, int y){
	return matrix[x][y].getn();
}

backend::state backend::universe::getstatexy(int x, int y){
	return matrix[x][y].getstate();
}

void backend::universe::update_n(){
	int x,y;
	state s1;
	//creating the 2D array which will later be assigned to the matrix's cells...
	int n_val[size][size];

	for (x=0; x<size; x++){
		for (y=0; y<size; y++){
			n_val[x][y]=0;
		}
	}
	//////////////////////////////////////////////////////////////////////
	// modifying universe, taking										//
	// (x,y) as															//
	//																	//
	// (0,0), (0,1), (0,2), ... , (0,size-1)							//
	// (1,0), (1,1), (1,2), ... , (1,size-1)							//
	// ...																//
	// (size-1,0), (size-1,1), (size-1,2), ... , (size-1,size-1)		//
	//																	//
	//////////////////////////////////////////////////////////////////////

	for (x=0; x<size; x++){
			for (y=0; y<size; y++){
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
								case (size-1):
									n_val[0][size-2]++;
									n_val[1][size-2]++;
									n_val[1][size-1]++;
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
						case (size-1):
							switch (y){
								case 0:
									n_val[size-1][1]++;
									n_val[size-2][0]++;
									n_val[size-2][1]++;
									break;
								case (size-1):
									n_val[size-1][size-2]++;
									n_val[size-2][size-2]++;
									n_val[size-2][size-1]++;
									break;
								default:
									for (int i=0;i<2;i++){
										for (int j=-1;j<2;j++){
											if (i || j) n_val[size-1-i][y+j]++;
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
									case (size-1):
										for (int i=-1;i<2;i++){
											for (int j=0;j<2;j++){
												if (i || j) n_val[x+i][size-1-j]++;
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
	for (x=0; x<size; x++){
		for (y=0; y<size; y++){
			matrix[x][y].set_n(n_val[x][y]);
		}
	}
}

void backend::universe::next_gen(){

	update_n();
	int x,y;
	for (x=0; x<size; x++){
		for (y=0; y<size; y++){
			matrix[x][y].set_prev();
			matrix[x][y].setstate();		//note: given n, setstate is independant of the prev values of
											// 		neighbouring cells
		}
	}
	gen++;
}

void backend::universe::setstatexy(int x, int y, state S){
	matrix[x][y].init_state(S);
}
