#include "protocol.h"
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>

// Configuração do socket UDP
int setup_udp_socket(int port, int is_server) {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);  // Cria o socket UDP
    if (sock_fd < 0) {
        perror("Erro ao criar socket");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));  // Zera a estrutura
    addr.sin_family = AF_INET;       // Família do endereço (IPv4)
    addr.sin_port = htons(port);     // Porta em ordem de bytes de rede

    if (is_server) {
        addr.sin_addr.s_addr = INADDR_ANY;  // Endereço para aceitar conexões
        if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("Erro no bind");
            close(sock_fd);
            return -1;
        }
    }

    return sock_fd;
}

// Configuração do timeout no socket
int set_socket_timeout(int socket_fd, int seconds) {
    struct timeval tv;
    tv.tv_sec = seconds;  // Segundos de timeout
    tv.tv_usec = 0;       // Microsegundos (0 neste caso)

    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Erro ao configurar timeout do socket");
        return -1;
    }

    return 0;
}
