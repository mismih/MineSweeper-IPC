#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include "MSClient.h"

int main() {
	printf("Starting client...\n");

	void* context = zmq_ctx_new();
	void* requester = zmq_socket(context, ZMQ_REQ);

	int rc = zmq_connect(requester, "tcp://localhost:5555");
	if (rc != 0) {
		printf("Couldnt connect to the server\n");
		zmq_close(requester);
		zmq_ctx_destroy(context);
		return -1;
	}

	userClick probniKlik;
	probniKlik.x = 0;
	probniKlik.y = 0;
	probniKlik.type = 'r';

	zmq_send(requester, &probniKlik, sizeof(userClick), 0);

	int primljenaMatrica[SIZE][SIZE];
	zmq_recv(requester, primljenaMatrica, sizeof(primljenaMatrica), 0);

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

	zmq_close(requester);
	zmq_ctx_destroy(context);

	int ran = 3;
	while (1) {
		ran++;
		ran--;
	}

	return 0;
}