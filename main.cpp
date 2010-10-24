#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#define _USE_MATH_DEFINES
#include "math.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int board[6][7] = {0};

//X, Y co-ordinates of the chip to be placed
int xchip = 210;
int ychip = 650;

int turn = 1;

//Vars used to draw circles
int x, y, radius, xcent, ycent;

//int used to count the number of encountered pieces of same type
int win = 0;

//Y co-ordinates of the right and left pile of chips
int piley1 ,piley2, piley3, piley4;

//Number of chips with each player
int numberOfChips1 = 21;
int numberOfChips2 = 21;

//Y co-ordinate of the text of the number indicating the number of left chips
int numberOfLeftPileY = 340;
int numberOfRightPileY = 340;

//Array which contains the winning combination that led to winning the game
int winningComboX[42]= {0};
int winningComboY[42] = {0};

//Pointer to indicate the last occupied element of array
int winningPointer = 0;

//Indicates if the game ended and if it ended in a draw or not
bool gameEnd = false;
bool gameDraw = false;

void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0); // set white background color
	glColor3f(0.0f, 0.0f, 0.0f); // set the drawing color
	glPointSize(4.0); // a ‘dot’ is 4 by 4 pixels
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1000.0, 0.0, 700);
}
/*
 Method used to print text on-screen.
 */
void print(int x, int y, char *string, int choice)
{
    int len, i;
    
    glRasterPos2i(x, y);
    
    len = (int) strlen(string);//loop to display character by character
    
    for (i = 0; i < len; i++) 
    {
		switch (choice) {
			case 0:
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
				break;
			case 1:
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);				
				break;
			case 2:
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15,string[i]);				
				break;		
				
			default:
				break;
		}
    }
    
}
/* Method responsible for drawing a circle
 */
void drawChips(int r, int style)
{
	radius = r;
	switch (style) {
		case 1:
			glBegin(GL_POLYGON);
			break;
		case 2:
			glBegin(GL_LINE_STRIP);
			break;			
		default:
			break;
	}	
	x = radius * cos(359 * M_PI/180.0f) + xcent;
	y = radius * sin(359 * M_PI/180.0f) + ycent;
	
	for(int j = 0; j < 360; j++)
	{
		glVertex2f(x,y);
		x = radius * cos(j * M_PI/180.0f) + xcent;
		y = radius * sin(j * M_PI/180.0f) + ycent;
		glVertex2f(x,y);
	}
	glEnd();
}

/* Method responsible for converting an int into a string
 */
string convertInt(int number)
{
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}


void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen
	
	//Check for draw
	if (numberOfChips1 == 0 && numberOfChips2 == 0 && !gameEnd ) {
		gameDraw = true;
		glColor3ub(0, 0, 0);
		//This will remove the chip on the board
		xchip = -50;
		ychip = -50;
		print(420, 650, "Draw! Nobody wins today.", 0);
	}
	
	//Winner text
	if (gameEnd && turn == 1) {
		glColor3ub(237, 28, 36);
		print(420, 650, "Player 1 Won", 0);
	} else if (gameEnd && turn == 2) {
		glColor3ub(0, 128, 0);
		print(420, 650, "Player 2 Won", 0);
	}
	
	//Left pile of 21 chips
	int piley1 = 20;
	int piley2 = 30;
	glColor3ub(237, 28, 36);
	
	std::string str = convertInt(numberOfChips1);
	char *result = strdup(str.c_str());	
	
	print(45, numberOfLeftPileY, result, 1);
	for (int i = 0; i < numberOfChips1; i++) {
		
		glBegin(GL_POLYGON);		
			glVertex2i(20, piley1);
			glVertex2i(90, piley1);
			glVertex2i(90, piley2);
			glVertex2i(20, piley2);		
		glEnd();
		
		piley1 += 15;
		piley2 += 15;
	}
	
	//Right pile of 21 chips
	int piley3 = 20;
	int piley4 = 30;
	glColor3ub(0, 128, 0);
	
	std::string str2 = convertInt(numberOfChips2);
	char *result2 = strdup(str2.c_str());	
	
	print(930, numberOfRightPileY, result2, 1);
	for (int i = 0; i < numberOfChips2; i++) {
		
		glBegin(GL_POLYGON);		
			glVertex2i(910, piley3);
			glVertex2i(980, piley3);
			glVertex2i(980, piley4);
			glVertex2i(910, piley4);		
		glEnd();
		
		piley3 += 15;
		piley4 += 15;
	}
	
	
	//Left rectangle of the board
	glColor3ub(127, 127, 127);

	glBegin(GL_POLYGON);
		glVertex2i(120, 20);
		glVertex2i(120, 600);
		glVertex2i(150, 600);
		glVertex2i(150, 20);
	glEnd();
	
	//Right 3amood
	glBegin(GL_POLYGON);	
		glVertex2i(850, 20);
		glVertex2i(850, 600);
		glVertex2i(880, 600);
		glVertex2i(880, 20);	
	glEnd();
	
	//Board
	glColor3ub(195, 195, 195);
	
	glBegin(GL_POLYGON);	 
		glVertex2i(150, 600);
		glVertex2i(150, 20);
		glVertex2i(850, 20);
		glVertex2i(850, 600);	
	glEnd();
	
	//Chips drawing
	xcent = 115;
	ycent = 550;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			if (board[i][j] == 11) {
				//Chip is present and belongs to player 1				
				xcent += 95;
				glColor3ub(237, 28, 36);
				drawChips(40, 1);
			} else if (board[i][j] == 12) {
				//Chip is present and belongs to player 2
				xcent += 95;
				glColor3ub(0, 128, 0);
				drawChips(40, 1);
			}else {
				//Empty slot
				xcent += 95;
				glColor3ub(255, 255, 255);
				drawChips(40, 1);
			}

		}
		ycent -= 95;
		xcent = 115;
	}
	
	//The chip to be placed into the board, the one placed above the board
	if (turn == 1) {
		//Player 1
		glColor3ub(237, 28, 36);
	}else if (turn == 2) {
		//Player 2
		glColor3ub(0, 128, 0);
	}
	glBegin(GL_POLYGON);
	
	x = radius * cos(359 * M_PI/180.0f) + xchip;
	y = radius * sin(359 * M_PI/180.0f) + ychip;
	
	for(int j = 0; j < 360; j++)
	{
		glVertex2f(x,y);
		x = radius * cos(j * M_PI/180.0f) + xchip;
		y = radius * sin(j * M_PI/180.0f) + ychip;
		glVertex2f(x,y);
	}
	glEnd();
	
	
	//Circling the winning chips
	if (gameEnd) {		
		//glLineWidth(200.0);
		for (int k = 1; k < win; k++) {
			//Chips drawing
			xcent = 115;
			ycent = 550;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 7; j++) {
					xcent += 95;
					if (i == winningComboX[winningPointer-k] && j == winningComboY[winningPointer -k]) {
						//cout << "Gowa" <<endl;
						switch (turn) {
							case 1:
								glColor3ub(237, 28, 36);
								break;
							case 2:
								glColor3ub(0, 128, 0);
								break;
							default:
								break;
						}
						drawChips(45, 2);
					}
				}
				ycent -= 95;
				xcent = 115;
			}
		}	
	}
	
	//Connecting a line through the winning chips
	if (gameEnd) {		
		glLineWidth(5.0);
		glBegin(GL_LINE_STRIP);
		for (int k = 1; k < win; k++) {
			//Chips drawing
			xcent = 115;
			ycent = 550;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 7; j++) {
					xcent += 95;
					if (i == winningComboX[winningPointer-k] && j == winningComboY[winningPointer -k]) {
						cout << "Gowa" <<endl;
						switch (turn) {
							case 1:
								glColor3ub(237, 28, 36);
								break;
							case 2:
								glColor3ub(0, 128, 0);
								break;

							default:
								break;
						}
						glVertex2i(xcent, ycent);
					}
				}
				ycent -= 95;
				xcent = 115;
			}
		}
		glEnd();
	}
	
	glFlush();
}

/* Method to check if there is a winner, it's a recursive function
 that calls itself to go through all the directions to check for winning
 chips. Dir: indicates the direction to check. Pl: indicates which player's 
 chips to check.
 */
int checkWinner(int x, int y, int dir, int pl)
{	
	//Base case
	if (board[x][y] != 11 && board[x][y] != 12 || x > 5 || y > 6 || x < 0 || y < 0) {
		return 0;
	}else {
		cout << "X : " << x << " Y: " << y << " = " << board[x][y] << endl;
		switch (dir) {
			case 1:
				if (pl == 1 && board[x][y] == 11) {
					//cout << "Case 1" << endl;
					
					//Adding the current chip to array that has the *probable* winning chips
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					//Recursive call
					return 1 + checkWinner(x, y+1, 1, 1);
				}else if (pl == 2 && board[x][y] == 12) {
				
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x, y+1, 1, 2);
				}
				break;				
			case 2:
				if (pl == 1 && board[x][y] == 11) {
					//cout << "Case 2" << endl;
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x, y-1, 2, 1);
				}else if (pl == 2 && board[x][y] == 12) {
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x, y-1, 2, 2);
				}				
				break;
			case 3:
				if (pl == 1 && board[x][y] == 11) {
					//cout << "Case 2" << endl;
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x+1, y, 3, 1);
				}else if (pl == 2 && board[x][y] == 12) {
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x+1, y, 3, 2);
				}				
				break;
			case 4:
				if (pl == 1 && board[x][y] == 11 ) {
					//cout << "Case 4" << endl;
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x-1, y, 4, 1);
				}else if (pl == 2 && board[x][y] == 12 ) {
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x-1, y, 4, 2);
				}				
				break;
			case 5:
				if (pl == 1 && board[x][y] == 11 ) {
					//cout << "Case 5" << endl;
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x+1, y+1, 5, 1);
				}else if (pl == 2 && board[x][y] == 12 ) {
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x+1, y+1, 5, 2);
				}				
				break;
			case 6:
				if (pl == 1 && board[x][y] == 11 ) {
					//cout << "Case 6" << endl;
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x-1, y-1, 8, 1);
				}else if (pl == 2 && board[x][y] == 12 ) {
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x-1, y-1, 8, 2);
				}
				break;
			case 7:
				if (pl == 1 && board[x][y] == 11 ) {
					//cout << "Case 7" << endl;
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x+1, y-1, 7, 1);
				}else if (pl == 2 && board[x][y] == 12 ) {
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x+1, y-1, 7, 2);
				}
				break;
			case 8:
				if (pl == 1 && board[x][y] == 11 ) {
					//cout << "Case 8" << endl;
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x-1, y+1, 6, 1);
				}else if (pl == 2 && board[x][y] == 12 ) {
					
					winningComboX[winningPointer] = x;
					winningComboY[winningPointer] = y;
					winningPointer++;
					
					return 1 + checkWinner(x-1, y+1, 6, 2);
				}				
				break;		
			default:
				break;
		}
	}
	
	return 0;
}

/* Method to handle keyboard events
 */
void keyboard (int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT && xchip < 780 && !gameEnd && !gameDraw) {
		//Moving the on board chip to the right
		xchip += 95;
		myDisplay();
	}else if (key == GLUT_KEY_LEFT && xchip > 210 && !gameEnd && !gameDraw) {
		//Moving the on board chip to the left
		xchip -= 95;
		myDisplay();
	}else if (key == GLUT_KEY_DOWN && !gameEnd && !gameDraw) {
		//Game logic
		
		//To set which coloumn the chip is inserted in
		int j = 0;
		switch (xchip) {
			case 210:
				j = 0;
				break;
			case 305:
				j = 1;
				break;
			case 400:
				j = 2;
				break;
			case 495:
				j = 3;
				break;
			case 590:
				j = 4;
				break;
			case 685:
				j = 5;
				break;
			case 780:
				j = 6;
				break;
			default:
				break;
		}	
		//Traverse the rows as we already have the colomn
		for (int i = 5; i >= 0; i--) {
			if (board[i][j] != 11 && board[i][j] != 12) {
				if (turn == 1) {
					board[i][j] = 11;
					cout << "New chip player 1 was placed in " << i << " " << j << " Value is: " << board[i][j] << endl;
					for (int k = 1; k < 9; k++) {
						
						//This will count the number of pieces in both directions. for an example
						//--> and <-- starting from the piece just inserted to both directions.
						//The value of win will indicate if there is a winning situation or not.						
						win = checkWinner(i, j, k, 1);
						k++;
						win += checkWinner(i, j, k, 1);
						
						cout << "Value of Win is: " << win << endl;
						if (win > 4) {
							
							cout << "Value of X is: " << winningComboX[winningPointer -1] << " Y is: " << winningComboY[winningPointer -1] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -2] << " Y is: " << winningComboY[winningPointer -2] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -3] << " Y is: " << winningComboY[winningPointer -3] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -4] << " Y is: " << winningComboY[winningPointer -4] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -5] << " Y is: " << winningComboY[winningPointer -5] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -6] << " Y is: " << winningComboY[winningPointer -6] << endl;
							 
							gameEnd = true;
							
							//Remove the on-board chip
							xchip = -50;
							ychip = -50;
							
							//To negate the effect of the upcoming increment
							turn--;
							
							cout << "Winner!! Player 1" << endl;
							break;
						}
					}
					turn++;
					numberOfChips1--;
					numberOfLeftPileY -= 10;
					myDisplay();
					break;
				}else {
					board[i][j] = 12;
					cout << "New chip player 2 was placed in " << i << " " << j << " Value is: " << board[i][j] << endl;
					for (int k = 1; k < 9; k++) {
						//Same as above
						win = checkWinner(i, j, k, 2);
						k++;
						win += checkWinner(i, j, k, 2);
						
						cout << "Value of Win is: " << win << endl;
						if (win > 4) {
							
							cout << "Value of X is: " << winningComboX[winningPointer -1] << " Y is: " << winningComboY[winningPointer -1] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -2] << " Y is: " << winningComboY[winningPointer -2] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -3] << " Y is: " << winningComboY[winningPointer -3] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -4] << " Y is: " << winningComboY[winningPointer -4] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -5] << " Y is: " << winningComboY[winningPointer -5] << endl;
							cout << "Value of X is: " << winningComboX[winningPointer -6] << " Y is: " << winningComboY[winningPointer -6] << endl;
							 
							
							gameEnd = true;
							xchip = -50;
							ychip = -50;
							
							//To negate the effect of the upcoming decrement
							turn++;
							cout << "Winner!! Player 2" << endl;
							break;
						}
					}
					turn--;
					numberOfChips2--;
					numberOfRightPileY -= 10;
					myDisplay();
					break;
				}
			}
		}
	}
}

int main(int argc, char** argv)
{
    //GLUT & OpenGL
	glutInit(&argc, argv); // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowSize(1000, 700); // set window size
	glutInitWindowPosition(100, 150); // set window position on	screen
	glutCreateWindow("Connect-4"); // open the screen window
	glutDisplayFunc(myDisplay); // register redraw function
	glutSpecialFunc(keyboard);
	myInit();
	glutMainLoop(); // go into a perpetual loop
	
}