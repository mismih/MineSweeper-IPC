#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include <raylib.h>
#include "MSClient.h"

int main() {
	puts("Starting client...\n");

	void* context = zmq_ctx_new();
	void* requester = zmq_socket(context, ZMQ_REQ);

	int rc = zmq_connect(requester, "tcp://localhost:5555");
	if (rc != 0) {
		puts("Couldnt connect to the server\n");
		zmq_close(requester);
		zmq_ctx_destroy(context);
		return -1;
	}

	userClick probniKlik;
	probniKlik.x = 0;
	probniKlik.y = 0;
	probniKlik.type = 'r';

	InitWindow(screenWidth, screenHeight, "minesweeper");
	SetTargetFPS(60);

	initMatrix();
	initGrids();

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(RAYWHITE);
		if (!isMenu) drawGrid();
		else drawMenu();

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && over == false && !isMenu) {
			Vector2 mousePoint = GetMousePosition();

			if (whichFieldWasClicked(mousePoint)) {
				probniKlik.x = clickX;
				probniKlik.y = clickY;

				zmq_send(requester, &probniKlik, sizeof(userClick), 0);
				zmq_recv(requester, primljenaMatrica, sizeof(primljenaMatrica), 0);
				if (isItTheEnd()) {
					over = true;
				}
			}
		}
		else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && over == false && !isMenu) {
			Vector2 mousePoint = GetMousePosition();
			if (whichFieldWasClicked(mousePoint)) {
				probniKlik.x = clickX;
				probniKlik.y = clickY;
				if (primljenaMatrica[probniKlik.x][probniKlik.y] == HIDDEN) {
					if (flagged[clickX][clickY] == 1 && flags > 0) {
						flagged[clickX][clickY] = 0;
						flags++;
					}
					else if (flags > 0) {
						flagged[clickX][clickY] = 1;
						flags--;
					}
				}
			}
		}
		if (over) {
			retryLogic();
		}
		else if (!isMenu && !didPlayerWin()) {
			vremeAkumulator += GetFrameTime();
			protekleSekunde = (int)vremeAkumulator;
		}

		if (retry) {
			probniKlik.type = 'f';
			zmq_send(requester, &probniKlik, sizeof(userClick), 0);
			zmq_recv(requester, primljenaMatrica, sizeof(primljenaMatrica), 0);
			probniKlik.type = 'r';
			protekleSekunde = 0;
			vremeAkumulator = 0;
			flags = 13;
			clearFields();
			printf("Desilo se");
			retry = false;
			over = false;
			predatHighScore = false; // Resetujemo zastavicu za sledeću partiju
		}

		if (!isMenu) {
			DrawText(TextFormat("Bombs found: %d", 13 - flags), 30, 30, 30, BLACK);
			DrawText(TextFormat("Time: %d", protekleSekunde), 30, 70, 30, BLACK);

			int trenutniRekord = highScore();
			if (trenutniRekord == INT_MAX && edgeScore == false) DrawText("Best score: N/a", 30, 110, 30, BLACK);
			else {
				DrawText(TextFormat("Best score: %d", trenutniRekord), 30, 110, 30, BLACK);
				edgeScore == true;  // if u really get max int
			}
		}

		if (didPlayerWin() && !isMenu) {
			DrawText("YOU WIN!", 830, 200, 50, RED);
			retryLogic();

			if (!predatHighScore) {
				checkScore(protekleSekunde);
				predatHighScore = true;
			}
		}

		if (!menuIs) isMenu = false;

		EndDrawing();
	}

	CloseWindow();
	zmq_close(requester);
	zmq_ctx_destroy(context);

	return 0;
}