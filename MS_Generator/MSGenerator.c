#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <zmq.h>
#include "MSGenerator.h"

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

void DisplayMatrix() {
	printf("\n");
	for (int i = 0; i < nOfSpaces; i++) {
		for (int j = 0; j < nOfSpaces; j++) {
			if (displayMatrix[i][j] == HIDDEN) {
				printf("x ");
			}
			else if (displayMatrix[i][j] == -1 && matrix[i][j] == 1) {
				printf("* ");
			}
			else {
				printf("%d ", displayMatrix[i][j]);
			}
		}
		putchar('\n');
	}
}

int main() {
	srand((unsigned int)time(NULL));
	InitMatrix();

	void* context = zmq_ctx_new();
	void* responder = zmq_socket(context, ZMQ_REP);

	int rc = zmq_bind(responder, "tcp://*:5555");
	if (rc != 0) {
		printf("Server start error\n");
		return -1;
	}

	printf("Minesweeper server started succesfully and listening on port 5555....\n");

	//printf("\nKlik na: (%d, %d)\n", proba.x, proba.y);
	//RevealCell(9, 9);
	//DisplayMatrix();

	while (1) {
		userClick primljeniClick;

		zmq_recv(responder, &primljeniClick, sizeof(userClick), 0);
		printf("Click recieved on: (%d, %d) type: %c\n", primljeniClick.x, primljeniClick.y, primljeniClick.type);

		if (primljeniClick.type == 'r') {
			RevealCell(primljeniClick.x, primljeniClick.y);
		}

		DisplayMatrix();
		zmq_send(responder, displayMatrix, sizeof(displayMatrix), 0);
	}

	return 0;
}