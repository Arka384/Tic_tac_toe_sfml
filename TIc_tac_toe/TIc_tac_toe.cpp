
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

using namespace sf;

//Global variables
RectangleShape a, b, c, d;	//for the frame
RectangleShape button[4][4];
Text text[4][4];
char ar[3][3] = {0};	//actual matrix
int timeLimit_excedeed = 0;
int win_flag = -1;
	// -1 -> invalid
	// 0 -> player 0 won 
	// 1 -> player 1 won
int TURN = 0;
	//turn = 0 player 0
	//turn = 1 player 1
int game_state = 1;
int LastState;
	//	0 -> Invalid state
	//	1 -> start screen state
	//	2 -> play state 1v1
	//	3 -> against comp
	//	4 -> round finished state

//start state
void start_state(RenderWindow &);
void clear_array(void);

//play state 1v1
void play_state(RenderWindow &);
void init_border(void);
void player_0_update(RenderWindow &);
void player_1_update(RenderWindow &);
void check_win(char );
void won(char);

//play state 1vComp
void play_state_computer(RenderWindow &);
void computer_update(char, char);

//round finished state
void reset(void);
void round_finished(RenderWindow & );


int main()
{
	RenderWindow window(VideoMode(800, 600), "Title", Style::Close);

	while (window.isOpen())
	{
		Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case Event::Closed:
				window.close();
			default:
				break;
			}
		}
		// functions according to the game state
		// no rendering will take place from main function 
		switch (game_state)
		{
		case 1:
			start_state(window);
			break;
		case 2:
			play_state(window);
			break;
		case 3:
			play_state_computer(window);
			break;
		case 4:
			round_finished(window);
			break;
		default:
			break;
		}
		

	}
	return 0;
}

////////////////////////////
////game state functions////
////////////////////////////

// start screen state fucntions
void start_state(RenderWindow &thisWindow)
{
	//in start state will have some simple buttons describing the basic functions.
	//one will be start game and another will be exit with some other buttons too.

	int width[3], height[3];
	RectangleShape button[4];
	Text t[4];
	Font font;

	font.loadFromFile("font.ttf");

	t[0].setString("PLAY 1_V_1");
	t[1].setString("Play Against Computer");
	t[2].setString("EXIT");

	for (int i = 0; i < 3; i++)
	{
		button[i].setFillColor(Color::Black);
		t[i].setFont(font);
		t[i].setCharacterSize(30);
		t[i].setFillColor(Color(218, 134, 11, 255));

		// dimensions of text and buttons will be same 
		width[i] = t[i].getLocalBounds().width;
		height[i] = t[i].getLocalBounds().height;

		button[i].setSize(Vector2f(width[i], height[i]));
		button[i].setPosition(800 / 2 - width[i] / 2, 170 + i * 70);
		t[i].setPosition(800 / 2 - width[i] / 2, 170 + i * 70);

	}

	float mx = Mouse::getPosition(thisWindow).x;
	float my = Mouse::getPosition(thisWindow).y;

	//updating buttons
	for (int i = 0; i < 3; i++)
	{
		int hot = mx > button[i].getPosition().x && mx < button[i].getPosition().x + width[i] &&
			my > button[i].getPosition().y && my < button[i].getPosition().y + height[i];

		if (hot)	//if mouse is hovering over the button
			t[i].setFillColor(Color(245, 222, 179, 255));

		//if button is hot and mouse is clicked
		if (hot && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (i == 0)
			{
				clear_array();
				game_state = 2;	//turning into play state 1v1
			}
			if (i == 1)
			{
				clear_array();
				game_state = 3;	//turning into play state 1vcomp
			}
			if (i == 2)
				thisWindow.close();
		}
	}

	thisWindow.clear();
	for (int i = 0; i < 3; i++)
	{
		thisWindow.draw(button[i]);
		thisWindow.draw(t[i]);
	}
	thisWindow.display();
}


//play state function for 1v1
void play_state(RenderWindow &thisWindow)
{
	int ypos = 50;
	Font font;
	RectangleShape bt[2];
	Text t[2];
	int width[2], height[2];

	font.loadFromFile("font.ttf");

	//this is main game loop in play state
	sleep(milliseconds(100));	// this is extreamly important 
		// this makes the function wait until the value of TURN is changed completely
		// without this the result will be unpredictable
	switch (TURN)
	{
	case 0:	//turn a
		player_0_update(thisWindow);
		check_win('X');
		break;
	case 1:	//turn b
		player_1_update(thisWindow);
		check_win('O');
		break;
	default:
		break;
	}


	// updating buttons and texts
	init_border();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			button[i][j].setFillColor(Color::Black);
			button[i][j].setPosition(110 + j * 70, ypos);
			button[i][j].setSize(Vector2f(40, 40));

			text[i][j].setFont(font);
			text[i][j].setCharacterSize(30);

			if (ar[i][j] == '0')
				text[i][j].setString(" ");
			if (ar[i][j] == 'O')
				text[i][j].setString("O");
			if (ar[i][j] == 'X')
				text[i][j].setString("X");

			text[i][j].setPosition(Vector2f(120 + j * 70, ypos));
		}
		ypos += 70;
	}

	// utility buttons
	// this buttons are for reset in the middle of the game and 
	// for going to main menu
	t[0].setString("Reset");
	t[1].setString("Main Menu");

	for (int i = 0; i < 2; i++)
	{
		width[i] = t[i].getLocalBounds().width;
		height[i] = t[i].getLocalBounds().height;

		t[i].setFont(font);
		t[i].setCharacterSize(28);
		t[i].setFillColor(Color::Black);

		bt[i].setSize(Vector2f(200, 40));
		bt[i].setPosition(500, 100 + i * 60);
		bt[i].setFillColor(Color(255, 140, 0, 255));
	}
	t[0].setPosition(555, 102);
	t[1].setPosition(520, 162);

	// utility buttons update

	float mx = Mouse::getPosition(thisWindow).x;
	float my = Mouse::getPosition(thisWindow).y;

	for (int i = 0; i < 2; i++)
	{
		int hot = mx > bt[i].getPosition().x && mx < bt[i].getPosition().x + 200 &&
			my > bt[i].getPosition().y && my < bt[i].getPosition().y + 40;

		if (hot)
			bt[i].setFillColor(Color(220, 20, 60, 255));

		if (hot && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			bt[i].setFillColor(Color::Red);
			if (i == 0)
				clear_array();
			if (i == 1)
			{
				reset();
				game_state = 1;
			}
		}
	}


	thisWindow.clear();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			thisWindow.draw(button[i][j]);
			thisWindow.draw(text[i][j]);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		thisWindow.draw(bt[i]);
		thisWindow.draw(t[i]);
	}

	thisWindow.draw(a);
	thisWindow.draw(b);
	thisWindow.draw(c);
	thisWindow.draw(d);

	thisWindow.display();

	if (win_flag != -1)
	{
		sleep(seconds(2));
		LastState = game_state;
		game_state = 4;	//round finished state
	}
}

void player_0_update(RenderWindow &thisWindow)
{
	float mx = Mouse::getPosition(thisWindow).x;
	float my = Mouse::getPosition(thisWindow).y;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int hot = mx > button[i][j].getPosition().x && mx < button[i][j].getPosition().x + 40 &&
				my > button[i][j].getPosition().y && my < button[i][j].getPosition().y + 40;

			if (hot && Mouse::isButtonPressed(Mouse::Button::Left))
			{
				if (ar[i][j] == 'O' || ar[i][j] == 'X')
					continue;	// invalid movement
				ar[i][j] = 'X';	// for player 0 he gives X
				text[i][j].setFillColor(Color::Yellow);
				TURN = 1;
				return;
			}
		}
	}
}

void player_1_update(RenderWindow &thisWindow)
{
	float mx = Mouse::getPosition(thisWindow).x;
	float my = Mouse::getPosition(thisWindow).y;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int hot = mx > button[i][j].getPosition().x && mx < button[i][j].getPosition().x + 40 &&
				my > button[i][j].getPosition().y && my < button[i][j].getPosition().y + 40;

			if (hot && Mouse::isButtonPressed(Mouse::Button::Left))
			{
				if (ar[i][j] == 'O' || ar[i][j] == 'X')
					continue;	// invalid movement
				ar[i][j] = 'O';	// for player 1 he gives O
				text[i][j].setFillColor(Color::Yellow);
				TURN = 0;
				return;
			}
		}
	}
}


// play state function for 1vComp
void play_state_computer(RenderWindow &thisWindow)
{
	int ypos = 50;
	Font font;
	RectangleShape bt[2];
	Text t[2];
	int width[2], height[2];

	font.loadFromFile("font.ttf");

	//this is main game loop in play state
	sleep(milliseconds(100));	// this is extreamly important 
		// this makes the function wait until the value of TURN is changed completely
		// without this the result will be unpredictable

	// if TLE occurs then restart the game with the same game_state
	if (timeLimit_excedeed == 1)
	{
		clear_array();
		sleep(milliseconds(200));
		timeLimit_excedeed = 0;
	}

	switch (TURN)
	{
	case 0:	//turn a
		player_0_update(thisWindow); //plays for X
		check_win('X');
		break;
	case 1:	//turn b
		computer_update('O', 'X');	//turn for O and against X
		check_win('O');
		break;
	default:
		break;
	}

	// updating buttons and texts
	init_border();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			button[i][j].setFillColor(Color::Black);
			button[i][j].setPosition(110 + j * 70, ypos);
			button[i][j].setSize(Vector2f(40, 40));

			text[i][j].setFont(font);
			text[i][j].setCharacterSize(30);

			if (ar[i][j] == '0')
				text[i][j].setString(" ");
			if (ar[i][j] == 'O')
				text[i][j].setString("O");
			if (ar[i][j] == 'X')
				text[i][j].setString("X");

			text[i][j].setPosition(Vector2f(120 + j * 70, ypos));
		}
		ypos += 70;
	}

	// utility buttons
	// this buttons are for reset in the middle of the game and 
	// for going to main menu
	t[0].setString("Reset");
	t[1].setString("Main Menu");

	for (int i = 0; i < 2; i++)
	{
		width[i] = t[i].getLocalBounds().width;
		height[i] = t[i].getLocalBounds().height;

		t[i].setFont(font);
		t[i].setCharacterSize(28);
		t[i].setFillColor(Color::Black);

		bt[i].setSize(Vector2f(200, 40));
		bt[i].setPosition(500, 100 + i * 60);
		bt[i].setFillColor(Color(255, 140, 0, 255));
	}
	t[0].setPosition(555, 102);
	t[1].setPosition(520, 162);

	// utility buttons update

	float mx = Mouse::getPosition(thisWindow).x;
	float my = Mouse::getPosition(thisWindow).y;

	for (int i = 0; i < 2; i++)
	{
		int hot = mx > bt[i].getPosition().x && mx < bt[i].getPosition().x + 200 &&
			my > bt[i].getPosition().y && my < bt[i].getPosition().y + 40;

		if (hot)
			bt[i].setFillColor(Color(220, 20, 60, 255));

		if (hot && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			bt[i].setFillColor(Color::Red);
			if (i == 0)
				clear_array();
			if (i == 1)
			{
				reset();
				game_state = 1;
			}
		}
	}

	thisWindow.clear();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			thisWindow.draw(button[i][j]);
			thisWindow.draw(text[i][j]);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		thisWindow.draw(bt[i]);
		thisWindow.draw(t[i]);
	}

	thisWindow.draw(a);
	thisWindow.draw(b);
	thisWindow.draw(c);
	thisWindow.draw(d);

	thisWindow.display();

	if (win_flag != -1)
	{
		sleep(seconds(2));
		LastState = game_state;
		game_state = 4;	//round finished state
	}

}

void computer_update(char SF, char SA)
{
	time_t start, end;
	int i, j, ei, ej;	// ei = emptyi and ej = emptyj indexes
	int ei2, ej2;
	int f = 0;	//denotes if theres an empty place or not
	int x = 0;	//counter for X or O

	//first checking for 2 SF in all cases

		// for the row
	for (i = 0; i < 3; i++)
	{
		x = f = 0;	// x=f=0 for each new row
		for (j = 0; j < 3; j++)
		{
			if (ar[i][j] == '0')
			{	//finding an empty index
				ei = i;
				ej = j;
				f = 1;
			}
			if (ar[i][j] == SF)
				x++;
			if (x == 2 && f == 1)
			{
				ar[ei][ej] = SF;
				TURN = 0;
				return;
			}
		}
	}

	//for column
	for (j = 0; j < 3; j++)
	{
		x = f = 0;	// x=f=0 for each new column
		for (i = 0; i < 3; i++)
		{
			if (ar[i][j] == '0')
			{	//finding an empty index
				ei = i;
				ej = j;
				f = 1;
			}
			if (ar[i][j] == SF)
				x++;
			if (x == 2 && f == 1)
			{
				ar[ei][ej] = SF;
				TURN = 0;
				return;
			}
		}
	}

	//for diagonal 1
	x = f = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (i == j)
			{
				if (ar[i][j] == '0')
				{	//finding an empty index
					ei = i;
					ej = j;
					f = 1;
				}
				if (ar[i][j] == SF)
					x++;
				if (x == 2 && f == 1)
				{
					ar[ei][ej] = SF;
					TURN = 0;
					return;
				}
			}
		}
	}

	//for diagonal 2
	x = f = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (i + j == 2)
			{
				if (ar[i][j] == '0')
				{	//finding an empty index
					ei = i;
					ej = j;
					f = 1;
				}
				if (ar[i][j] == SF)
					x++;
				if (x == 2 && f == 1)
				{
					ar[ei][ej] = SF;
					TURN = 0;
					return;
				}
			}
		}
	}
	// checking for all SF is done

	////////////////////////////////////////

	// checking for 2 SA

	// for the row
	for (i = 0; i < 3; i++)
	{
		x = f = 0;	// x=f=0 for each new row
		for (j = 0; j < 3; j++)
		{
			if (ar[i][j] == '0')
			{	//finding an empty index
				ei = i;
				ej = j;
				f = 1;
			}
			if (ar[i][j] == SA)
				x++;
			if (x == 2 && f == 1)
			{
				ar[ei][ej] = SF;
				TURN = 0;
				return;
			}
		}
	}

	//for column
	for (j = 0; j < 3; j++)
	{
		x = f = 0;	// x=f=0 for each new column
		for (i = 0; i < 3; i++)
		{
			if (ar[i][j] == '0')
			{	//finding an empty index
				ei = i;
				ej = j;
				f = 1;
			}
			if (ar[i][j] == SA)
				x++;
			if (x == 2 && f == 1)
			{
				ar[ei][ej] = SF;
				TURN = 0;
				return;
			}
		}
	}

	//for diagonal 1
	x = f = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (i == j)
			{
				if (ar[i][j] == '0')
				{	//finding an empty index
					ei = i;
					ej = j;
					f = 1;
				}
				if (ar[i][j] == SA)
					x++;
				if (x == 2 && f == 1)
				{
					ar[ei][ej] = SF;
					TURN = 0;
					return;
				}
			}
		}
	}

	//for diagonal 2
	x = f = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (i + j == 2)
			{
				if (ar[i][j] == '0')
				{	//finding an empty index
					ei = i;
					ej = j;
					f = 1;
				}
				if (ar[i][j] == SA)
					x++;
				if (x == 2 && f == 1)
				{
					ar[ei][ej] = SF;
					TURN = 0;
					return;
				}
			}
		}
	}
	//	checking for all SA is done

	// now if all the above case fails then we choose a random place
	// which is empty

	start = time(NULL);
	again:
	
	ei2 = rand() % 3;
	ej2 = rand() % 3;

	end = time(NULL);	
	// this is to check if the code is stuck in infinite while loop
	// in that case set TLE var and return controle
	if (difftime(end, start) > 2)
	{
		timeLimit_excedeed = 1;
		return;
	}
		
	if (ar[ei2][ej2] == '0')
	{
		ar[ei2][ej2] = SF;
		TURN = 0;
	}
	else
		goto again;

}


// round finished state functions
void round_finished(RenderWindow &thisWindow)
{
	int width[4], height[4];
	Font font;
	Text t[3];
	int fontSize = 40;
	font.loadFromFile("font.ttf");

	if (win_flag == 0)
		t[0].setString("PLAYER 1(X) WON");
	if (win_flag == 1)
	{
		if(LastState == 3)
			t[0].setString("COMPUTER WON");
		else
			t[0].setString("PLAYER 2(O) WON");
	}
		
	t[1].setString("Play Again");
	t[2].setString("Main Menu");

	t[0].setFillColor(Color::White);
	t[1].setFillColor(Color::Cyan);
	t[2].setFillColor(Color::Cyan);

	for (int i = 0; i < 3; i++)
	{
		t[i].setFont(font);
		t[i].setCharacterSize(fontSize);
		width[i] = t[i].getLocalBounds().width;
		height[i] = t[i].getLocalBounds().height;
		t[i].setPosition(400 - width[i] / 2, 150 + i * 70);
		fontSize = 30;
	}	

	float mx = Mouse::getPosition(thisWindow).x;
	float my = Mouse::getPosition(thisWindow).y;

	//updating buttons
	for (int i = 1; i < 3; i++)
	{
		int hot = mx > t[i].getPosition().x && mx < t[i].getPosition().x + width[i] &&
			my > t[i].getPosition().y && my < t[i].getPosition().y + height[i];

		if (hot)	//if mouse is hovering over the button
			t[i].setFillColor(Color(204, 102, 0, 255));

		//if button is hot and mouse is clicked
		if (hot && Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (i == 1)
				reset();
			if (i == 2)
			{
				reset();
				game_state = 1;
			}
		}
	}

	thisWindow.clear();
	for (int i = 0; i < 3; i++)
		thisWindow.draw(t[i]);
	thisWindow.display();

}



///////////////////////////////
////  utility functions	  /////
///////////////////////////////

void init_border()
{
	a.setSize(Vector2f(200, 10));
	a.setPosition(100, 100);
	b.setSize(Vector2f(200, 10));
	b.setPosition(100, 170);

	c.setSize(Vector2f(10, 200));
	c.setPosition(160, 40);
	d.setSize(Vector2f(10, 200));
	d.setPosition(230, 40);
}

void check_win(char s)
{
	int i, j, c, d1 = 0, d2 = 0;
	for (i = 0; i < 3; i++)
	{
		c = 0;
		for (j = 0; j < 3; j++)
		{
			if (ar[i][j] == s)
				c++;
			if (c == 3)
				won(s);
		}
	}
	for (j = 0; j < 3; j++)
	{
		c = 0;
		for (i = 0; i < 3; i++)
		{
			if (ar[i][j] == s)
				c++;
			if (c == 3)
				won(s);
		}
	}
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (i == j)
			{
				if (ar[i][j] == s)
					d1++;
				if (d1 == 3)
					won(s);
			}
			if (i + j == 2)
			{
				if (ar[i][j] == s)
					d2++;
				if (d2 == 3)
					won(s);
			}
		}
	}
}

void won(char s)
{
	if (s == 'X')
		win_flag = 0;
	if (s == 'O')
		win_flag = 1;
}

void clear_array()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ar[i][j] = '0';
			text[i][j].setFillColor(Color::White);
		}
			
	}
	win_flag = -1;
}

void reset()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ar[i][j] = '0';
			text[i][j].setFillColor(Color::White);
		}	
	}
	win_flag = -1;
	game_state = LastState;
}