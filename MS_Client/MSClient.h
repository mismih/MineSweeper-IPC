#ifndef MSCLIENT_H
#define MSCLIENT_H

#define SIZE 10
#define HIDDEN -1
#define FLAG -2

const int nOfSpaces = SIZE;
const int nOfBombs = 13;

int matrix[SIZE][SIZE];
int displayMatrix[SIZE][SIZE];

typedef struct {
	int x;
	int y;
	char type; // o - open  f - flag
} userClick;

#endif