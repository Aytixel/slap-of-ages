#include <stdio.h>
#include <assert.h>
#include "server_socket.h"
#include "client_socket.h"

int main()
{
    printf("Testing server_socket.h client_socket.h\n\n");

    server_t *server = createServer("0.0.0.0", 4539);

    assert(server != NULL);
    printf("Server creation OK\n");

    client_t *client = createClient("localhost", 4539);

    assert(client != NULL);
    printf("Client creation OK\n");

    server_client_t *server_client = acceptServerClient(server);

    assert(server_client != NULL);
    printf("\n\tServer client acceptance OK\n");

    assert(!deleteServerClient(&server_client));
    assert(server_client == NULL);
    printf("\n\tServer client deletion OK\n");

    assert(!deleteClient(&client));
    assert(client == NULL);
    printf("\nClient deletion OK\n");

    assert(!deleteServer(&server));
    assert(server == NULL);
    printf("Server deletion OK\n");

    printf("\nTesting server_socket.h client_socket.h successful\n");

    return 0;
}