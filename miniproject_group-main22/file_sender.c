#include "protocol.h"
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int file_sender(const char *filename, const char *host, int port, int window_size) {
    // Validação do tamanho da janela
    if (window_size <= 0 || window_size > MAX_WINDOW_SIZE) {
        fprintf(stderr, "Erro: Tamanho da janela (%d) inválido. Deve ser entre 1 e %d.\n", 
                window_size, MAX_WINDOW_SIZE);
        return -1;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return -1;
    }

    int socket_fd = setup_udp_socket(0, 0);  // Cliente
    if (socket_fd < 0) {
        fclose(file);
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        perror("Endereço inválido");
        fclose(file);
        close(socket_fd);
        return -1;
    }

    set_socket_timeout(socket_fd, TIMEOUT_SEC);

    uint32_t next_seq = 1;  // Sequência inicial
    int retries = 0;
    data_packet_t packet;
    ack_packet_t ack;

    while (1) {
        size_t bytes_read = fread(packet.data, 1, MAX_CHUNK_SIZE, file);
        if (bytes_read == 0) break;  // Fim do arquivo

        packet.seq_num = htonl(next_seq);

        ssize_t sent = sendto(socket_fd, &packet, sizeof(packet), 0, 
                              (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (sent < 0) {
            perror("Erro no envio do pacote");
            break;
        }

        ssize_t received = recvfrom(socket_fd, &ack, sizeof(ack), 0, NULL, NULL);
        if (received < 0) {
            if (++retries >= MAX_RETRIES) {
                perror("Máximo de retries alcançado");
                break;
            }
            continue;  // Retransmitir
        }

        if (ntohl(ack.seq_num) == next_seq) {
            retries = 0;
            next_seq++;
        }
    }

    fclose(file);
    close(socket_fd);
    return 0;
}
