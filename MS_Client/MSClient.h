#ifndef MSCLIENT_H
#define MSCLIENT_H

#define SIZE 10
#define HIDDEN -1
#define FLAG -2

const int nOfSpaces = SIZE;
const int nOfBombs = 13;
const int screenWidth = 1920;
const int screenHeight = 1080;

int fieldsX[100];
int fieldsY[100];
int matrix[SIZE][SIZE];
int displayMatrix[SIZE][SIZE];
int primljenaMatrica[SIZE][SIZE];
int flagged[SIZE][SIZE];
int clickX;
int clickY;
int flags = 13;
float vremeAkumulator = 0.0f;
int protekleSekunde = 0;

typedef struct {
	int x;
	int y;
	char type; // o - open  f - flag
} userClick;

bool over = false;
bool retry = false;
bool isMenu = true;
bool menuIs = true;
bool predatHighScore = false;
bool edgeScore = false;

void drawGrid() {
	int rows = 10;
	int cols = 10;
	int cellSize = 40;
	int padding = 4;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int pos = i * 10 + j;
			int x = fieldsX[pos];
			int y = fieldsY[pos];

			if (primljenaMatrica[i][j] == HIDDEN && flagged[i][j] != 1) {
				DrawRectangle(x, y, cellSize, cellSize, LIGHTGRAY);
			}
			else if (primljenaMatrica[i][j] == 1 && over == true) {
				DrawRectangle(x, y, cellSize, cellSize, RED);
			}
			else if (primljenaMatrica[i][j] == HIDDEN && flagged[i][j] == 1) {
				DrawRectangle(x, y, cellSize, cellSize, RED);
			}
			else if (primljenaMatrica[i][j] == 1) {
				DrawText(TextFormat("%d", primljenaMatrica[i][j]), x + 13, y + 2, cellSize, BLACK);
			}
			else if (primljenaMatrica[i][j] > 1) {
				DrawText(TextFormat("%d", primljenaMatrica[i][j]), x + 10, y + 2, cellSize, BLACK);
			}
			DrawRectangleLines(x, y, cellSize, cellSize, GRAY);
		}
	}
}

void testDrawMatrix() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (primljenaMatrica[i][j] == HIDDEN) {
				printf("x ");
			}
			else if (primljenaMatrica[i][j] == -1) {
				printf("* ");
			}
			else {
				printf("%d ", primljenaMatrica[i][j]);
			}
		}
		putchar('\n');
	}
}

void initGrids() {
	int rows = 10;
	int cols = 10;
	int cellSize = 40;
	int padding = 4;

	int gridWidth = cols * (cellSize + padding) - padding;
	int gridHeight = rows * (cellSize + padding) - padding;

	int startX = (screenWidth - gridWidth) / 2;
	int startY = (screenHeight - gridHeight) / 2;
	fieldsX[0] = startX;
	fieldsY[0] = startY;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int pos = i * 10 + j;
			fieldsX[pos] = startX + j * (cellSize + padding);
			fieldsY[pos] = startY + i * (cellSize + padding);
		}
	}
}

bool whichFieldWasClicked(Vector2 mousePoint) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			int pos = i * 10 + j;
			Rectangle buttonRect = { fieldsX[pos], fieldsY[pos], 40, 40 };
			if (CheckCollisionPointRec(mousePoint, buttonRect)) {
				clickX = i;
				clickY = j;
				printf("User clicked on x: %d, y: %d", clickX, clickY);
				return true;
			}
		}
	}
	return false;
}

void initMatrix() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			primljenaMatrica[i][j] = -1;
		}
	}
}

bool isItTheEnd() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (primljenaMatrica[i][j] != 0 && primljenaMatrica[i][j] != 1) {
				return false;
			}
		}
	}
	return true;
}

void retryLogic() {
	Rectangle rec1 = { 860, 880, 200, 80 };
	DrawRectangleLines(860, 880, 200, 80, BLACK);
	DrawText("RETRY", 870, 895, 50, BLACK);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 mousePoint = GetMousePosition();
		if (CheckCollisionPointRec(mousePoint, rec1)) {
			retry = true;
		}
	}
}

void clearFields() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			flagged[i][j] = 0;
		}
	}
}

void drawMenu() {
	DrawText("IPC MINESWEEPER", 510, 250, 100, BLACK);

	Vector2 mousePos = GetMousePosition();

	Rectangle startBtn = { 810, 500, 300, 70 };
	Rectangle exitBtn = { 810, 600, 300, 70 };

	bool hoverStart = CheckCollisionPointRec(mousePos, startBtn);
	DrawRectangleRec(startBtn, hoverStart ? LIGHTGRAY : GRAY);
	DrawText("START", startBtn.x + 95, startBtn.y + 18, 35, BLACK);

	if (hoverStart && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		menuIs = false;
	}

	bool hoverExit = CheckCollisionPointRec(mousePos, exitBtn);
	DrawRectangleRec(exitBtn, hoverExit ? LIGHTGRAY : GRAY);
	DrawText("EXIT", exitBtn.x + 105, exitBtn.y + 18, 35, BLACK);

	if (hoverExit && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		exit(0);
	}
}

bool didPlayerWin() {
	int skrivenaPolja = 0;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (primljenaMatrica[i][j] == HIDDEN) {
				skrivenaPolja++;
			}
		}
	}

	if (skrivenaPolja == 13) {
		return true;
	}

	return false;
}

int highScore() {
	int best = INT_MAX;
	FILE* file = fopen("highscore.txt", "r");

	if (file != NULL) {
		fscanf(file, "%d", &best);
		fclose(file);
	}

	return best;
}

void checkScore(int cur) {
	int old = highScore();

	if (cur < old) {
		FILE* fajl = fopen("highscore.txt", "w");
		if (fajl != NULL) {
			fprintf(fajl, "%d", cur);
			fclose(fajl);
		}
	}
}

#endif