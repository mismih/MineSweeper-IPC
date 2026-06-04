#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include <raylib.h>
#include "MSClient.h"


int main() {
	puts("Starting client...\n");

	void* context = zmq_ctx_new(); // opening connection
	void* requester = zmq_socket(context, ZMQ_REQ);

	int rc = zmq_connect(requester, "tcp://localhost:5555"); // connecting on port 5555
	if (rc != 0) {
		puts("Couldnt connect to the server\n");
		zmq_close(requester);
		zmq_ctx_destroy(context);
		return -1;
	}

	userClick probniKlik; // user click
	probniKlik.x = 0;
	probniKlik.y = 0;
	probniKlik.type = 'r';

	InitWindow(screenWidth, screenHeight, "minesweeper"); // raylib window initialize
	SetTargetFPS(60);
	
	initMatrix();
	testDrawMatrix(); // draws the matrix in cmd to see if everythings ok
	initGrids();

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(RAYWHITE);
		drawGrid(); // draws the placeholder for the mines


		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && over == false) {
			Vector2 mousePoint = GetMousePosition();

			if (whichFieldWasClicked(mousePoint)) {
				probniKlik.x = clickX;
				probniKlik.y = clickY;
				testDrawMatrix();

				zmq_send(requester, &probniKlik, sizeof(userClick), 0); // sending user click to server
				zmq_recv(requester, primljenaMatrica, sizeof(primljenaMatrica), 0); // recieving the new matrix
				if (isItTheEnd()) {
					over = true;
				}
			}
		}	
		else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && over == false) {
			Vector2 mousePoint = GetMousePosition();
			whichFieldWasClicked(mousePoint);
			probniKlik.x = clickX;
			probniKlik.y = clickY;
			if (flagged[clickX][clickY] == 1 && flags > 0) {
				flagged[clickX][clickY] = 0;
				flags++;
			}
			else if (flags > 0) {
				flagged[clickX][clickY] = 1;
				flags--;
			}
		}

		if (over) {
			retryLogic();
		}
		else {
			vremeAkumulator += GetFrameTime();
			protekleSekunde = (int)vremeAkumulator;
		}
		if (retry) {
			return 0;
		}

		DrawText(TextFormat("Bombs found: %d", 13 - flags), 30, 30, 30, BLACK);
		DrawText(TextFormat("Time: %d", protekleSekunde), 30, 70, 30, BLACK);

		EndDrawing();
	}


	CloseWindow();
	zmq_close(requester);
	zmq_ctx_destroy(context);

	return 0;
}