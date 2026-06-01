#ifndef MSGENERATOR_H
#define MSGENERATOR_H

#define SIZE 10
#define HIDDEN -1
#define FLAG -2

const int nOfSpaces = SIZE;
const int nOfBombs = 13;

int matrix[SIZE][SIZE];       
int displayMatrix[SIZE][SIZE]; 

void InitMatrix();
void RevealCell(int x, int y);
void DisplayMatrix();
int IsNeighborWithBomb(int x, int y);

typedef struct {
	int x;
	int y;
	char type; // o - open  f - flag
} userClick;

#endif