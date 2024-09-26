#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 1024

void* receive_messages(void* arg);
void* send_messages(void* arg);

int client_fd;
pthread_t thread_recv, thread_send;
pthread_mutex_t io_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    struct sockaddr_in addr;
    struct in_addr ip_addr;

    inet_pton(AF_INET, "127.0.0.1", &ip_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = ip_addr.s_addr;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    system("clear");
    printf("Connection has been successfully established\n");

    pthread_create(&thread_recv, NULL, receive_messages, NULL);
    pthread_create(&thread_send, NULL, send_messages, NULL);

    pthread_join(thread_recv, NULL);
    pthread_join(thread_send, NULL);

    close(client_fd);
    return 0;
}

void* receive_messages(void* arg) {
    char buffer[SIZE];
    while (1) {
        int recv_size = recv(client_fd, buffer, SIZE, 0);
        if (recv_size <= 0) {
            if (recv_size == 0) {
                pthread_mutex_lock(&io_mutex);
                printf("\rServer has disconnected!\n");
                pthread_cancel(thread_send);
                pthread_mutex_unlock(&io_mutex);
                exit(0);
            } else {
                perror("recv");
            }
            close(client_fd);
            exit(0);
        } else {
            buffer[recv_size] = '\0';
            pthread_mutex_lock(&io_mutex);
            printf("\r\033[K> %s> ", buffer);  
            fflush(stdout);
            pthread_mutex_unlock(&io_mutex);
        }
    }
}

void* send_messages(void* arg) {
    char buffer[SIZE];
    while (1) {
        pthread_mutex_lock(&io_mutex);
        printf("> ");
        fflush(stdout);
        pthread_mutex_unlock(&io_mutex);

        if (fgets(buffer, SIZE, stdin) != NULL) {
            pthread_mutex_lock(&io_mutex);
            if(buffer[0] != '\n'){
                if (send(client_fd, buffer, strlen(buffer), 0) <= 0) {
                    perror("send");
                    close(client_fd);
                    exit(1);
                }
            }
            pthread_mutex_unlock(&io_mutex);
        }
    }
}
