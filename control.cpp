#include "control.hpp"

using namespace std;
//seed_init functions...

bool control::seed_init::setmatrix(bool matrix[size][size]){

	string seedname=renderer::init_screen();
	if (seedname=="Enter your own seed")
		renderer::user_input(matrix, true);
	else if (seedname == "LOGIC GATES"){
			logic::intro_screen Is1;
			string gatename=Is1.intro();
			//(get logic gate seed)
			return true;
		}
	else{
		S1=file::getseed(seedname);
		for (int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				matrix[i][j]=S1.matrix[i][j];
			}
		}
	}
	return false;
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


void file::storepattern(bool matrix[][size], std::string name, int nrows, int ncols){
	ofstream fso("PATTERN.TXT",ios::app);
	fso<<name<<"\n"<<nrows<<"\n"<<ncols<<"\n";
	for (int i=0;i<nrows;i++){
		for (int j=0;j<ncols;j++){
			fso<<matrix[i][j];
			if (j!=ncols-1)
				fso<<" ";
			else
				fso<<"\n";
		}
	}
}


void file::getpattern(bool matrix[][size],std::string str, int nrows,int  ncols){

	std::string name,rtxt,ctxt;
	int r,c;
	bool assign=false;
	ifstream fs("PATTERN.TXT",ios::in);
	if (!fs) {
		cerr<<"CAN'T OPEN STREAM. TERMINATING";
		exit(1);
	}
	while(!fs.eof()){
		getline(fs,name,'\n');
		getline(fs,rtxt,'\n');
		getline(fs,ctxt,'\n');
		r=stoi(rtxt);
		c=stoi(ctxt);
//		cout<<name<<"\n"<<r<<"\n"<<c<<"\n";
		if (r==nrows && c == ncols && name==str){
//			cout<<"in if"<<endl;
			for (int i=0;i<r;i++){
				for(int j=0;j<c;j++){
					matrix[i][j]=fs.get()-'0';
//					cout<<matrix[i][j]<<" ";
					fs.ignore();
				}
//				cout<<"\n";
			}
			assign=true;
		}
		if (assign)
			break;
		else {
//			cout<<"in else"<<endl;
			for (int i=0;i<r;i++){
				for(int j=0;j<c;j++){
					fs.ignore();
					fs.ignore();
				}
			}
		}
	}
	if (!assign){
			cerr<<"Didn't find pattern terminating";
			exit(1);
		}
//	cout<<"ending for "<<str<<endl;
	fs.close();
}


