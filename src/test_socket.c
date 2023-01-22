#include <stdio.h>
#include <assert.h>
#include "server_socket.h"
#include "client_socket.h"

int main()
{
    printf("Testing server_socket.h client_socket.h\n\n");

    server_t *server = createServer(4539);

    assert(server != NULL);
    printf("Server creation OK\n");

    server_client_t *client;

    printf("\n\tClient pointer %p\n", client = acceptServerClient(server));

    if (client != NULL)
        deleteServerClient(&client);

    deleteServer(&server);

    assert(server == NULL);
    printf("\nServer deletion OK\n");

    printf("\nTesting server_socket.h client_socket.h successful\n");

    return 0;
}