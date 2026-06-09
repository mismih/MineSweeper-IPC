#ifndef MSGENERATOR_H
#define MSGENERATOR_H

#define SIZE 10
#define HIDDEN -1
#define FLAG -2

const int nOfSpaces = SIZE;
const int nOfBombs = 13;
int retry = 0;
int matrix[SIZE][SIZE];
int displayMatrix[SIZE][SIZE];

void InitMatrix();
void RevealCell(int x, int y);
void DisplayMatrix();
int IsNeighborWithBomb(int x, int y);

typedef struct {
	int x;
	int y;
	char type;
} userClick;

void InitMatrix() {
	for (int i = 0; i < nOfSpaces; i++) {
		for (int j = 0; j < nOfSpaces; j++) {
			matrix[i][j] = 0;
			displayMatrix[i][j] = HIDDEN;
		}
	}

	int bombs = nOfBombs;
	while (bombs != 0) {
		int randX = rand() % nOfSpaces;
		int randY = rand() % nOfSpaces;
		if (matrix[randX][randY] != 1) {
			matrix[randX][randY] = 1;
			bombs--;
		}
	}

	printf("\n");
	for (int i = 0; i < nOfSpaces; i++) {
		for (int j = 0; j < nOfSpaces; j++) {
			printf("%d ", matrix[i][j]);
		}
		putchar('\n');
	}
}

int IsNeighborWithBomb(int x, int y) {
	int count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int nx = x + i;
			int ny = y + j;
			if (nx >= 0 && nx < nOfSpaces && ny >= 0 && ny < nOfSpaces) {
				if (matrix[nx][ny] == 1) count++;
			}
		}
	}
	return count;
}

void RevealCell(int x, int y) {
	if (x < 0 || x >= nOfSpaces || y < 0 || y >= nOfSpaces) return;
	if (displayMatrix[x][y] != HIDDEN) return;

	if (matrix[x][y] == 1) {
		displayMatrix[x][y] = -1;
		return;
	}

	int count = IsNeighborWithBomb(x, y);
	displayMatrix[x][y] = count;

	if (count == 0) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0) continue;
				RevealCell(x + i, y + j);
			}
		}
	}
}

#endif