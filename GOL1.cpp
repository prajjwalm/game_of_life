/*


 GOL1.cpp
 *
 *  Created on: 30-Sep-2017
 *      Author: Prajjwal Mishra
 *
 *      requires MinGW 6.1.0 (64 bit) and SFML 2.4.2 (64 bit for MinGW 6.1.0)...
 *      also uses arialbd.ttf, a text document containing the seed information and several .png files...



#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <sfml/graphics.hpp>

using namespace std;

const int size=100;

namespace renderer {

	enum command { STOP, RESUME, MODIFY, QUIT};
	enum speed { vslow, slow, med, fast, vfast};

	const sf::Color color_live=sf::Color(0,255,0);
	const sf::Color color_dead=sf::Color(0,0,0);
	const sf::Color color_bkgd=sf::Color(16,16,16);

	const int N_options=5;
	const int side=8;
	const int height=900;
	const int width=1200;
	const int margin=20;

	string init_screen();
	void user_input(bool [size][size],bool);				//boolean value checks if the game has just started
	speed pause_screen(bool [size][size], int, command&);

};

namespace backend {

	enum state {dead, live};

	class cell {
		state curr, prev;
		int n;						// no of live neighbours
	public:
		cell();
		state getstate();			// gives current state, meant for universe::display
		void set_prev();			// assigns curr to prev, meant for universe::next_gen
		int set_n(int);				// assigning of n
		state init_state(state);	// forced assigning of state, meant for universe::initialize
		state setstate();			// sets the current state based on prev, n and returns it,
									// meant for universe::next_gen
	};

	class universe {
		int gen;						//generation  number, increments at call of next_gen
		void update_n();				// sets n for each cell, based on current states, for next_gen
		cell matrix[size][size];
	public:
		universe();
		int getgen() {return gen;}
		state getstatexy(int x, int y);
		void initialize(bool [size][size]);
		void next_gen();					//based on n and the previous states of the cells, generates the
											//current state of all the cells of the matrix, increments gen
		};

}

namespace file {
	struct seed{
		bool matrix[size][size];
		string name;
	};

	void storeseed(seed);
	seed getseed(string str);

}

namespace control {

	class seed_init {
		file::seed S1;
	public:
		void setmatrix(bool [size][size]);
	};


	class controller  {
		renderer::speed pace;
		int tick;							//tick duration, in milliseconds, depends only on pace
		backend::universe life;
	public:
		controller ();
		void update(bool [size][size]);		// assignment of life::matrix during initialization or modification
		void setpace(renderer::speed);				// sets speed and tick, may use enum speed as its argument also
		bool rungame(bool [size][size],int&);			//returns whether paused
														//takes the matrix and gen to pass the paused values
														//of 'life' and life.gen into it
	};
}




int main(){

	control::seed_init S;
	control::controller C;

	int gen=0;
	bool matrix[size][size];
	bool if_paused=false;
	renderer::command pause_cmd;
	renderer::speed s;

	for (int i=0;i<size;i++){
		for (int j=0;j<size;j++){
			matrix[i][j]=0;
		}
	}
	do {
		S.setmatrix(matrix);
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
			case renderer::STOP:								//game restarts with a new matrix
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
	} while(pause_cmd==renderer::STOP);
	return 0;
}

//seed_init functions...

void control::seed_init::setmatrix(bool matrix[size][size]){

	string seedname=renderer::init_screen();
	if (seedname=="Enter your own seed")
		renderer::user_input(matrix, true);
	else{
		S1=file::getseed(seedname);
		for (int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				matrix[i][j]=S1.matrix[i][j];
			}
		}
	}

}

//renderer functions...

string renderer::init_screen(){

	//sets up the initial screen of the game
	int i, selected_option=-1;
	string msg[N_options]={"Enter your own seed","Gosper's Glider Gun with eater","Spaceships","Oscillators"};

	//sfml objects...
	sf::RenderWindow rw(sf::VideoMode(width,height),"GAME OF LIFE");
	sf::Font arial;
	sf::Vector2f point;
	arial.loadFromFile("arialbd.ttf");

	sf::Texture radio0;
	if (!radio0.loadFromFile("unselected_radio.png")){
		cerr<<"Unable to load unselected radio. terminated\n";
		exit(1);
	}
	radio0.setSmooth(true);

	sf::Texture radio1;
	if (!radio1.loadFromFile("selected_radio.png")){
		cerr<<"Unable to load selected radio. terminated\n";
		exit(1);
	}
	radio1.setSmooth(true);

	sf::Texture proceed0;
	if (!proceed0.loadFromFile("Proceed_button.png")){
			cerr<<"Unable to load proceed button terminated\n";
			exit(1);
		}
	proceed0.setSmooth(true);

	sf::Texture proceed1;
	if (!proceed1.loadFromFile("Proceed_pressed.png")){
			cerr<<"Unable to load proceed pressed terminated\n";
			exit(1);
		}
	proceed1.setSmooth(true);

	sf::Sprite radioS[N_options];
	sf::Sprite proceed;
	sf::FloatRect radioSbox[N_options], proceedbox;


	//enter the seed of your choice...
	sf::Text t;
	sf::Text op[N_options];

	t.setFont(arial);
	t.setFillColor(sf::Color(64,64,255));
	t.setCharacterSize(24);
	t.setPosition(margin, margin);
	t.setString("Enter the seed of your choice: ");
	rw.clear(color_bkgd);
	rw.draw(t);



	//draw the option buttons...
	for (i=0; i<N_options; i++){
		op[i].setFont(arial);
		op[i].setFillColor(sf::Color(64,64,255));
		op[i].setCharacterSize(24);
		radioS[i].setTexture(radio0);
		op[i].setString(msg[i]);
		radioS[i].setPosition(margin*2, (4+i*2)*margin);
		radioS[i].setScale(0.3, 0.3);							// sets size of buttons very close to (but <) 10 pixels
		op[i].setPosition(margin*4, (4+i*2)*margin-6);			// emperically set
		if (!msg[i].empty()){
			radioSbox[i]=radioS[i].getGlobalBounds();
			rw.draw(radioS[i]);
			rw.draw(op[i]);
		}
	}

	//draw the proceed option...
	proceed.setTexture(proceed0);
	proceed.setScale(0.5,0.5);
	proceed.setPosition(width-margin-200, height-100);			// emperically set
	proceedbox=proceed.getGlobalBounds();
	rw.draw(proceed);

	rw.display();

	while (rw.isOpen()){
		sf::Event event;

		//while there are pending events
		while (rw.pollEvent(event)){

			// check the type of the event
			switch (event.type) {
			case sf::Event::Closed:
				rw.close();
				exit(0);
				break;
			case sf::Event::MouseButtonPressed:
				// check if the button was pressed in a radio bubble, and take action accordingly
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				for (i=0;i<N_options;i++){
					if (radioSbox[i].contains(point)){
						rw.clear(color_bkgd);
						radioS[i].setTexture(radio1);
						rw.draw(t);
						for (int j=0;j<N_options;j++){
							if (j!=i)
								radioS[j].setTexture(radio0);
							if (!msg[j].empty()){
								rw.draw(radioS[j]);
								rw.draw(op[j]);
							}
						}
						rw.draw(proceed);
						rw.display();
						selected_option=i;
					}

				}
				// check if proceed was clicked, and whether a radio bubble is previously selected, and take action accordingly
				if (proceedbox.contains(point)){
					proceed.setTexture(proceed1);
					proceed.setPosition(width-margin-195, height-95);			// emperically set
					rw.clear(color_bkgd);
					rw.draw(t);
					for (int j=0;j<N_options;j++){
						if (!msg[j].empty()){
							rw.draw(radioS[j]);
							rw.draw(op[j]);
						}
					}
					rw.draw(proceed);
					rw.display();
				}
				break;
				// proceed is called only if the mouse is released there...
			case sf::Event::MouseButtonReleased:
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				proceed.setTexture(proceed0);
				proceed.setPosition(width-margin-200, height-100);
				if (proceedbox.contains(point)){				//proceed clicked...
					if (selected_option>=0){						//with a selected option
						rw.close();
						return msg[selected_option];
					}
					else{										//without a selected option
						rw.clear(color_bkgd);
						rw.draw(t);
						for (int j=0;j<N_options;j++){
							if (!msg[j].empty()){
								rw.draw(radioS[j]);
								rw.draw(op[j]);
							}
						}
						rw.draw(proceed);
						rw.display();
					}
				}
				else {
					rw.clear(color_bkgd);
					rw.draw(t);
					for (int j=0;j<N_options;j++){
						if (!msg[j].empty()){
							rw.draw(radioS[j]);
							rw.draw(op[j]);
						}
					}
					rw.draw(proceed);
					rw.display();
				}
				break;
			}
		}
	}
	cerr<<"NO OPTION WAS SELECTED, TERMINATING";
	exit(1);
}

void renderer::user_input(bool matrix[size][size], bool start) {

	int x1,y1;
	int i,j;

	sf::RenderWindow rw(sf::VideoMode(width,height),"The grid");
	sf::RectangleShape cell(sf::Vector2f(side,side));
	sf::Font arial;
	sf::Sprite submit;
	sf::Vector2f point;

	sf::Texture submit0;
	if (!submit0.loadFromFile("Submit_button.png")){
			cerr<<"Unable to load submit button terminated\n";
			exit(1);
		}
	submit0.setSmooth(true);

	sf::Texture submit1;
	if (!submit1.loadFromFile("Submit_pressed.png")){
			cerr<<"Unable to load submit pressed terminated\n";
			exit(1);
		}
	submit1.setSmooth(true);

	arial.loadFromFile("arial.ttf");
	cell.setOutlineColor(sf::Color(128,128,128));
	cell.setOutlineThickness(1);

	submit.setTexture(submit0);
	submit.setScale(0.5, 0.5);
	submit.setPosition(width-margin-200, height-100);			// emperically set
	sf::FloatRect submitbox=submit.getGlobalBounds();


	rw.clear(color_bkgd);
	if (start){
		for (i=0;i<size;i++)
			for (j=0;j<size;j++) {
				matrix[i][j]=0;
				cell.setFillColor(color_dead);
				cell.setPosition(i*side+margin, j*side+margin);
				rw.draw(cell);
			}
	}
	else {
		for (i=0;i<size;i++){
			for (j=0;j<size;j++) {
				if (matrix[i][j])
					cell.setFillColor(color_live);
				else
					cell.setFillColor(color_dead);
				cell.setPosition(i*side+margin, j*side+margin);
				rw.draw(cell);
			}
		}

	}
	rw.draw(submit);

	rw.display();

	while (rw.isOpen()){
		sf::Event event;

		while (rw.pollEvent(event)){
			switch (event.type) {
			case sf::Event::Closed:
				rw.close();
				cerr<<"unnatural exit";
				exit(1);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.x >margin && event.mouseButton.x < margin+side*size){
					if (event.mouseButton.y >margin && event.mouseButton.y < margin+side*size){
						x1=(event.mouseButton.x-margin)/side;
						y1=(event.mouseButton.y-margin)/side;
						matrix[x1][y1]=!matrix[x1][y1];
						rw.clear(color_bkgd);

						for (i=0;i<size;i++){
							for (j=0;j<size;j++) {
						 		if (matrix[i][j])
						 			cell.setFillColor(color_live);
						 		else
						 			cell.setFillColor(color_dead);
					 			cell.setPosition(i*side+margin, j*side+margin);
					 			rw.draw(cell);
							}
						}
						rw.draw(submit);
						rw.display();
					}
				}
				else {

					point.x=event.mouseButton.x;
					point.y=event.mouseButton.y;
					if (submitbox.contains(point)) {
						submit.setTexture(submit1);
						submit.setPosition(width-margin-195, height-95);			// emperically set
						rw.clear(color_bkgd);
						for (i=0;i<size;i++){
							for (j=0;j<size;j++) {
								if (matrix[i][j])
									cell.setFillColor(color_live);
								else
									cell.setFillColor(color_dead);
								cell.setPosition(i*side+margin, j*side+margin);
								rw.draw(cell);
							}
						}
						rw.draw(submit);
						rw.display();

					}
				}
				break;
			case sf::Event::MouseButtonReleased:
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				submit.setTexture(submit0);
				submit.setPosition(width-margin-200, height-100);
				if (submitbox.contains(point)){
					rw.close();
					return;
				}
				else {
					rw.clear(color_bkgd);
					for (i=0;i<size;i++){
						for (j=0;j<size;j++) {
							if (matrix[i][j])
								cell.setFillColor(color_live);
							else
								cell.setFillColor(color_dead);
							cell.setPosition(i*side+margin, j*side+margin);
							rw.draw(cell);
						}
					}
					submit.setTexture(submit0);
					submit.setPosition(width-margin-200, height-100);			// emperically set
					rw.draw(submit);
					rw.display();
				}
				break;
			}
		}
	}
}

renderer::speed renderer::pause_screen(bool matrix[size][size], int gen, command& cmd){

	int i,j,sval=0;
	const int n_speeds=5;
	string msg[n_speeds]= {"vslow", "slow", "med", "fast", "vfast"};


	sf::RenderWindow rw(sf::VideoMode(width,height),"Game Paused");

	sf::Font arial;
	arial.loadFromFile("arialbd.ttf");


	//cells...
	sf::RectangleShape cell(sf::Vector2f(side,side));
	cell.setOutlineColor(sf::Color(128,128,128));
	cell.setOutlineThickness(1);

	//speed radios and their texts...
	sf::Texture radio0;
	if (!radio0.loadFromFile("unselected_radio.png")){
		cerr<<"Unable to load unselected radio. terminated\n";
		exit(1);
	}
	radio0.setSmooth(true);

	sf::Texture radio1;
	if (!radio1.loadFromFile("selected_radio.png")){
		cerr<<"Unable to load selected radio. terminated\n";
		exit(1);
	}
	radio1.setSmooth(true);
	sf::Text speed_txt[n_speeds], t, gno;
	rw.clear(color_bkgd);
	t.setFont(arial);
	t.setFillColor(sf::Color(64,64,255));
	t.setCharacterSize(24);
	t.setPosition(margin, margin);
	t.setString("Select the pace of the game: ");
	rw.draw(t);


	sf ::Sprite radio[n_speeds];
	sf::FloatRect speedbox[n_speeds];

	//draw the option buttons...
	for (i=0; i<n_speeds; i++){
		speed_txt[i].setFont(arial);
		speed_txt[i].setFillColor(sf::Color(64,64,255));
		speed_txt[i].setCharacterSize(24);
		if (i)
			radio[i].setTexture(radio0);
		else
			radio[i].setTexture(radio1);
		speed_txt[i].setString(msg[i]);
		radio[i].setPosition(margin*2, (4+i*2)*margin);
		radio[i].setScale(0.3, 0.3);						// sets size of buttons very close to (but <) 10 pixels
		speed_txt[i].setPosition(margin*4, (4+i*2)*margin-6);			// emperically set
		speedbox[i]=radio[i].getGlobalBounds();
		rw.draw(radio[i]);
		rw.draw(speed_txt[i]);
	}

	//draw the matrix, aligned right... (MATRIX UNMODIFIABLE)

	for (i=0;i<size;i++)
		for (j=0;j<size;j++) {
			if (matrix[i][j])
				cell.setFillColor(color_live);
			else
				cell.setFillColor(color_dead);
			cell.setPosition(i*side+(width-side*size-margin), j*side+(3*margin));
			rw.draw(cell);
		}

	//write the gen no
	gno.setFont(arial);
	gno.setFillColor(color_live);
	gno.setCharacterSize(24);
	gno.setPosition(width-side*size+margin*3, margin);
	gno.setString("Gen no.: "+to_string(gen));
	rw.draw(gno);

	//draw the modify button
	sf::Texture modify0;
	if (!modify0.loadFromFile("Modify_button.png")){
			cerr<<"Unable to load modify button terminated\n";
			exit(1);
		}
	modify0.setSmooth(true);
	sf::Texture modify1;
	if (!modify1.loadFromFile("Modify_pressed.png")){
			cerr<<"Unable to load modify pressed terminated\n";
			exit(1);
		}
	modify1.setSmooth(true);
	sf::Sprite modify;
	sf::FloatRect modifybox;

	modify.setTexture(modify0);
	modify.setScale(0.5,0.5);
	modify.setPosition(margin, height-margin-50);			// emperically set
	modifybox=modify.getGlobalBounds();
	rw.draw(modify);

	//draw the resume, stop buttons...
	sf::Texture resume0;
	if (!resume0.loadFromFile("Resume_button.png")){
		cerr<<"Unable to load resume button terminated\n";
		exit(1);
	}
	sf::Texture resume1;
	if (!resume1.loadFromFile("Resume_pressed.png")){
		cerr<<"Unable to load resume pressed terminated\n";
		exit(1);
	}
	sf::Sprite resume;
	sf::FloatRect resumebox;
	resume.setTexture(resume0);

	resume.setPosition(width-size*side-100,height/2-25);	//emperical
	resume.setScale(0.6, 0.6);							// sets size of buttons very close to (but <) 20 pixels
	resumebox=resume.getGlobalBounds();
	rw.draw(resume);

	sf::Texture stop0;
	if (!stop0.loadFromFile("Stop_button.png")){
		cerr<<"Unable to load stop button terminated\n";
		exit(1);
	}
	sf::Texture stop1;
	if (!stop1.loadFromFile("Stop_pressed.png")){
		cerr<<"Unable to load stop pressed terminated\n";
		exit(1);
	}
	sf::Sprite stop;
	sf::FloatRect stopbox;
	stop.setTexture(stop0);

	stop.setPosition(width-size*side-100,height/2+25);	//emperical
	stop.setScale(0.6, 0.6);							// sets size of buttons very close to (but <) 20 pixels
	stopbox=stop.getGlobalBounds();
	rw.draw(stop);

// identify the point pressed
	sf::Vector2f point;

	rw.display();
	while (rw.isOpen()){
		sf::Event event;
		while (rw.pollEvent(event)){
			switch (event.type) {
			case sf::Event::Closed:
				rw.close();
				cmd=QUIT;
				break;
			case sf::Event::MouseButtonPressed:
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				for (i=0;i<n_speeds;i++){
					if (speedbox[i].contains(point)){
						sval=i;
						for (j =0;j <n_speeds;j++){
							if (j!=i)
								radio[j].setTexture(radio0);
						}
						radio[i].setTexture(radio1);

					}
				}
				if (modifybox.contains(point)){
					modify.setTexture(modify1);
					modify.setPosition(margin+5, height-margin-45);
				}
				else if (resumebox.contains(point)){
					resume.setTexture(resume1);
				}
				else if (stopbox.contains(point)){
					stop.setTexture(stop1);
				}
				break;
			case sf::Event::MouseButtonReleased:
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				modify.setTexture(modify0);
				resume.setTexture(resume0);
				stop.setTexture(stop0);
				modify.setPosition(margin, height-margin-50);
				if (modifybox.contains(point)){
					rw.close();
					cmd=MODIFY;
				}
				else if (resumebox.contains(point)){
					rw.close();
					cmd=RESUME;
				}
				else if (stopbox.contains(point)){
					rw.close();
					cmd=STOP;
				}
				break;
			}
			if (event.type==sf::Event::MouseButtonPressed || event.type==sf::Event::MouseButtonReleased) {
				//draw all
				rw.clear(color_bkgd);
				rw.draw(t);
				for (i=0;i<n_speeds;i++) {
					rw.draw(radio[i]);
					rw.draw(speed_txt[i]);
				}
				for (i=0;i<size;i++)
					for (j=0;j<size;j++) {
						if (matrix[i][j])
							cell.setFillColor(color_live);
						else
							cell.setFillColor(color_dead);
						cell.setPosition(i*side+(width-side*size-margin), j*side+(3*margin));
						rw.draw(cell);
					}
				rw.draw(gno);
				rw.draw(resume);
				rw.draw(stop);
				rw.draw(modify);
				rw.display();
			}
		}
	}
	switch(sval){
	case 0: return vslow;
	case 1: return slow;
	case 2: return med;
	case 3: return fast;
	case 4: return vfast;
	default:
		cerr<<"invalid speed selected, terminating! ";
		exit(1);
	}
}


// cell funtions...

backend::cell::cell(){
	//called only when board is created, sets all at 0
	curr=dead;
	prev=dead;
	n=0;
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

control::controller::controller (){

	pace=renderer::vslow;
	tick=1;
}

void control::controller::update(bool matrix [size][size]){
	life.initialize(matrix);
	// from here game opens in paused state...
}

void control::controller::setpace(renderer::speed s){
	pace=s;
	switch(pace) {
	case renderer::vslow:
		tick=600/17;	//empirical
		break;
	case renderer::slow:
		tick=400/17;
		break;
	case renderer::med:
		tick=250/17;
		break;
	case renderer::fast:
		tick=100/17;
		break;
	case renderer::vfast:
		tick=1;			// MAX SPEED POSSIBLE
		break;
	}
}

bool control::controller::rungame (bool matrix[size][size], int &final_gen){

	// only non-renderer function that must open a window,
	// made this way so that a new window need not be opened on
	// the derivation of every new generation..
	using renderer::height;
	using renderer::margin;
	using renderer::side;
	using renderer::width;
	using renderer::color_bkgd;
	using renderer::color_dead;
	using renderer::color_live;


	using backend::state;
	using backend::dead;
	using backend::live;

	int i,j;
	bool if_paused;
	sf::RenderWindow rw(sf::VideoMode(width,height),"Game of Life",sf::Style::Titlebar);

	sf::Texture pause0;
	if (!pause0.loadFromFile("Pause_button.png")){
		cerr<<"Unable to load pause button terminated\n";
		exit(1);
	}
	sf::Texture pause1;
	if (!pause1.loadFromFile("Pause_pressed.png")){
		cerr<<"Unable to load pause pressed terminated\n";
		exit(1);
	}
	sf::Sprite pause;
	sf::FloatRect pausebox;
	pause.setTexture(pause0);

	rw.clear(color_bkgd);

	pause.setPosition(margin,height/2-25);
	pause.setScale(0.6, 0.6);							// sets size of buttons very close to (but <) 20 pixels
	pausebox=pause.getGlobalBounds();
	rw.draw(pause);

	//cells...
	sf::RectangleShape cell(sf::Vector2f(side,side));
	cell.setOutlineColor(sf::Color(128,128,128));
	cell.setOutlineThickness(1);
	state s;
	for (i=0;i<size;i++)
		for (j=0;j<size;j++) {
			s=life.getstatexy(i, j);
			if (s==live)
				cell.setFillColor(color_live);
			else
				cell.setFillColor(color_dead);
			cell.setPosition(i*side+(width-side*size-margin), j*side+(3*margin));
			rw.draw(cell);
		}

	//gen no. ...
	sf::Font arial;
	arial.loadFromFile("arialbd.ttf");
	sf::Text gno;
	int gen=life.getgen();
	gno.setFont(arial);
	gno.setFillColor(color_live);
	gno.setCharacterSize(24);
	gno.setPosition(width-side*size+margin*3, margin);
	gno.setString("Gen no.: "+to_string(gen));
	rw.draw(gno);

	rw.display();
	sf::Vector2f point;
//	used for time measurement...
//	sf::Clock clock;
//	sf::Time elapsed1;
	int time_cnt=0;
	while (rw.isOpen()){						//typical iteration takes 17 milliseconds
		time_cnt++;

		sf::Event event;
		while (rw.pollEvent(event)){

			switch (event.type){
			case sf::Event::Closed:
				rw.close();
				cerr<<"unnatural exit";
				exit(1);
			case sf::Event::MouseButtonPressed:
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				if (pausebox.contains(point)){
					pause.setTexture(pause1);
				}
				break;
			case sf::Event::MouseButtonReleased:
				point.x=event.mouseButton.x;
				point.y=event.mouseButton.y;
				pause.setTexture(pause0);
				if (pausebox.contains(point)){
					if_paused=true;
					rw.close();
				}
				break;
			}
		}
		if (rw.isOpen()){
	//		cout<<time_cnt<<"\n";
	//		elapsed1 = clock.getElapsedTime();
	//		cout << elapsed1.asMilliseconds() << endl;

			if (time_cnt%tick==0){
				life.next_gen();								//takes negligible time (ie. <0.5 milliseconds)
				time_cnt=0;
	//			elapsed1 = clock.getElapsedTime();
	//			cout << elapsed1.asMilliseconds() << endl;
	//			clock.restart();
			}

			rw.clear(color_bkgd);
			rw.draw(pause);								//takes neglibible time
			for (i=0;i<size;i++)
				for (j=0;j<size;j++) {
					s=life.getstatexy(i, j);
					if (s==live)
						cell.setFillColor(color_live);
					else
						cell.setFillColor(color_dead);
					cell.setPosition(i*side+(width-side*size-margin), j*side+(3*margin));
					rw.draw(cell);							// executed 10000 times, must be the source of time lag
				}
			gen=life.getgen();
			gno.setString("Gen no.: "+to_string(gen));
			rw.draw(gno);
			rw.display();
		}
	}
	for (i=0;i<size;i++){
		for (j=0;j<size;j++){
			s=life.getstatexy(i, j);
			if (s==live)
				matrix[i][j]=1;
			else
				matrix[i][j]=0;
		}
	}
	final_gen = gen;
	return if_paused;
}

// file functions...
void file::storeseed(seed S){
	ofstream fso("SEEDS.TXT",ios::app);
	fso<<S.name<<"\n";
	for (int i=0;i<size;i++){
		for (int j=0;j<size;j++){
			fso<<S.matrix[i][j];
			if (j!=size-1)
				fso<<" ";
			else
				fso<<"\n";
		}
	}
}

file::seed file::getseed(string str){
	//
	// searches for the seed,
	// returns it if it exists
	// else terminates
	//
	seed s1;
	bool assign=false;
	ifstream fs("SEEDS.TXT",ios::in);
	if (!fs) {
		cerr<<"CAN'T OPEN STREAM. TERMINATING";
		exit(1);
	}
	while(!fs.eof()){
		getline(fs, s1.name,'\n');
		for (int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				s1.matrix[i][j]=fs.get()-'0';
				fs.ignore();
			}
		}
		if (s1.name==str){
			assign=true;
			break;
		}
	}
	if (!assign){
			cerr<<"Didn't find seed terminating";
			exit(1);
		}
	fs.close();
	return s1;
}


*/
