#include "renderer.hpp"

using namespace std;
//renderer functions...

string renderer::init_screen(){

	//sets up the initial screen of the game
	int i, selected_option=-1;
	string msg[N_options]={"Enter your own seed","Gosper's Glider Gun with eater","Spaceships","Oscillators","LOGIC GATES"};

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
