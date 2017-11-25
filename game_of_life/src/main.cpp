#include "control.hpp"
#include "test.hpp"

using namespace std;

//*************************************************** source code

int main(){

	renderer::command pause_cmd;
	renderer::speed s;
	file::seed s1;
	control::seed_init S;
	control::controller C;

	int gen = 0;
	bool matrix[size][size];
	bool matrix_cpy[size][size];
	bool if_paused = false;
	bool logic_selected;
	gate G;

	for (int i=0;i<size;i++){
		for (int j=0;j<size;j++){
			matrix[i][j]=0;
		}
	}
	do {
		logic_selected=S.setmatrix(matrix, G);

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

			// at this stage C.life.matrix is well initialized array of dead and live cells
			// now we open the pause screen...
			do {
				s=renderer::pause_screen(matrix,gen,pause_cmd);

				//		to make the matrix a seed...

//				s1.name="edge";
//				for (int i=0;i<size;i++){
//					for(int j=0;j<size;j++){
//						s1.matrix[i][j]=matrix[i][j];
//					}
//				}
//
//				file::storeseed(s1,'\t',"TESTRES.TXT");

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
					//		to make the matrix a seed...

//								s1.name="OR";
//								for (int i=0;i<size;i++){
//									for(int j=0;j<size;j++){
//										s1.matrix[i][j]=matrix[i][j];
//									}
//								}
//
//								file::storeseed(s1,' ',"SEEDS.TXT");

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

			bool A_activated=false,B_activated=false;													//here A_activated and B_activated represent if inputs were just switched on
			bool start=true;

			for (int i=0;i<size;i++){
				for (int j=0;j<size;j++){
					matrix_cpy[i][j]=matrix[i][j];
				}
			}
			C.update(matrix);
			do {
				s=renderer::lpause_screen(matrix,gen,pause_cmd, G, A_activated, B_activated, start);
					//matrix, gen, not_selected and start are inputs
					//s, pause_cmd, A_, B_ are outputs
				start = false;
				C.setpace(s);
				switch (pause_cmd) {
				case renderer::RESUME:
					if (G!=NOT)
						if_paused=C.rungame(matrix, gen, G, A_activated, B_activated);
					else
						if_paused=C.rungame(matrix, gen, NOT, A_activated, false);
					break;
				case renderer::STOP:
					if_paused=false;
					gen=0;
					break;
				case renderer::QUIT:
					if_paused=false;
					break;
				case renderer::L_RESET:
					if_paused=true;
					for (int i=0;i<size;i++){
						for (int j=0;j<size;j++){
							matrix[i][j]=matrix_cpy[i][j];
						}
					}
					gen=0;
					start=true;
					C.update(matrix);
				}
			} while(if_paused==true);
		}
	} while(pause_cmd==renderer::STOP);

//	int m[size][size];
//	file::GetN(m,"glider");
//	for (int i=0;i<size;i++){
//		for(int j=0;j<size;j++){
//			cout<<m[i][j]<<" ";
//		}
//		cout<<"\n";
//	}

	return 0;
}

/****************************************************************************/
