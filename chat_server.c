#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 1024

void* handle_client(void* arg);

int server_fd;
int client_fds[2];
int client_count = 0;
int okay = 0;

int main() {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);
    pthread_t threads[2];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr*)&addr, addr_size) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    system("clear");
    printf("Server started working!\n");
    while(1){
        for (int i = 0; i < 2; i++) {
            int* client_id = malloc(sizeof(int));
            *client_id = i;

            client_fds[i] = accept(server_fd, (struct sockaddr*)&addr, &addr_size);
            if (client_fds[i] < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("Client %d has connected!\n", i + 1);

            pthread_create(&threads[i], NULL, handle_client, client_id);
        }
        okay = 1;

        for (int i = 0; i < 2; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    close(server_fd);
    return 0;
}

void* handle_client(void* arg) {
    int client_id = *(int*)arg;
    free(arg);
    char buffer[SIZE];
    int other_client_id = (client_id == 0) ? 1 : 0;
    while(okay == 0){
        sleep(1);
    }
    while (1) {
        int recv_size = recv(client_fds[client_id], buffer, SIZE, 0);
        if (recv_size <= 0) {
            if (recv_size == 0) {
                printf("Client %d has disconnected!\n", client_id + 1);
            } else {
                perror("recv");
            }
            close(client_fds[client_id]);
            pthread_exit(NULL);
        } else {
            buffer[recv_size] = '\0';
            send(client_fds[other_client_id], buffer, recv_size, 0);
            printf("Message from: %d to %d sent\n", client_id + 1, other_client_id + 1);
        }
    }
}
