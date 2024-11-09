#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

struct client_data {
    int client_socket;
};

int authenticate(const char *username, const char *password) {
    FILE *file = fopen("user.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    char line[50];
    while (fgets(line, sizeof(line), file)) {
        char *file_uname = strtok(line, ",");
        char *file_pass = strtok(NULL, ";");

        if (file_uname && file_pass) {
            file_pass[strcspn(file_pass, "\n")] = '\0';

            if (strcmp(username, file_uname) == 0 && strcmp(password, file_pass) == 0) {
                fclose(file);
                return 1; 
            }
        }
    }

    fclose(file);
    return 0; 
}

void *handle_client(void *arg) {
    struct client_data *data = (struct client_data *)arg;
    int client_socket = data->client_socket;
    free(data);

    char buffer[50];
    recv(client_socket, buffer, sizeof(buffer), 0);

    char *username = strtok(buffer, ",");
    char *password = strtok(NULL, ";");

    if (authenticate(username, password)) {
        send(client_socket, "Successful login\n", 17, 0);
    } else {
        send(client_socket, "Login Denied\n", 13, 0);
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listening failed");
        close(server_socket);
        exit(1);
    }
    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket < 0) {
            perror("Client connection failed");
            continue;
        }

        printf("Connected to client\n");

        struct client_data *data = malloc(sizeof(struct client_data));
        data->client_socket = client_socket;
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, data);
        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
}