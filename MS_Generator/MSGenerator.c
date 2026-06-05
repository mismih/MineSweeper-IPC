#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <zmq.h>
#include "MSGenerator.h"

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

	puts("Minesweeper server started succesfully and listening on port 5555....\n");

	while (1) {
		userClick primljeniClick;

		zmq_recv(responder, &primljeniClick, sizeof(userClick), 0);
		printf("Click recieved on: (%d, %d) type: %c\n", primljeniClick.x, primljeniClick.y, primljeniClick.type);

		if (primljeniClick.type == 'f') {
			InitMatrix();
			zmq_send(responder, displayMatrix, sizeof(displayMatrix), 0);
			continue;
		}

		if (primljeniClick.type == 'r') {
			RevealCell(primljeniClick.x, primljeniClick.y);
		}

		DisplayMatrix();
		if (matrix[primljeniClick.x][primljeniClick.y] == 1) {
			zmq_send(responder, matrix, sizeof(displayMatrix), 0);
		}
		else {
			zmq_send(responder, displayMatrix, sizeof(displayMatrix), 0);
		}
	}


	return 0;
}