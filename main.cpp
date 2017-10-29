#include "control.hpp"

using namespace std;

int main(){

	control::seed_init S;
	control::controller C;

	int gen=0;
	bool matrix[size][size];
	bool if_paused=false;
	bool logic_selected;
	renderer::command pause_cmd;
	renderer::speed s;


	for (int i=0;i<size;i++){
		for (int j=0;j<size;j++){
			matrix[i][j]=0;
		}
	}
	do {
		logic_selected=S.setmatrix(matrix);

			// making a pattern of the matrix...
//			for (int i=0;i<logic::eat_rows;i++){
//				for (int j=0;j<size;j++){
//					eatmatrix[i][j]=matrix[j][i];
//				}
//			}
//			file::storepattern(eatmatrix, "gun", logic::eat_rows, logic::eat_cols);
		if (!logic_selected){
			// at this stage, matrix is an initialized 2D array of 0s and 1s
			// now we feed this array to our controller
			C.update(matrix);

		//		to make the matrix a seed...
		//		seed s1;
		//		s1.name="Oscillators";
		//		for (int i=0;i<size;i++){
		//			for(int j=0;j<size;j++){
		//				s1.matrix[i][j]=matrix[i][j];
		//			}
		//		}
		//
		//		file::storeseed(s1);

			// at this stage C.life.matrix is well initialized array of dead and live cells
			// now we open the pause screen...
			do {
				s=renderer::pause_screen(matrix,gen,pause_cmd);
				C.setpace(s);							//controller sets the game speed
				// now we run the game
				switch (pause_cmd) {
				case renderer::RESUME:							//controller runs the game
					if_paused=C.rungame(matrix,gen);
					break;
				case renderer::STOP:							//game restarts with a new matrix
					if_paused=false;
					gen=0;
					break;
				case renderer::MODIFY:							//controller updates the matrix
					renderer::user_input(matrix,false);
					gen=0;
					C.update(matrix);
					if_paused=true;
					break;
				case renderer::QUIT:
					if_paused=false;
					break;
				}
			} while(if_paused==true);
		}
		else {
			//launch logic gates code here...

		}
	} while(pause_cmd==renderer::STOP);
	return 0;
}
