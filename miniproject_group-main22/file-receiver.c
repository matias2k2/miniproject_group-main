#include "protocol.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int file_receiver(const char *output_filename, int port, int window_size) {
    // Validação do tamanho da janela
    if (window_size <= 0 || window_size > MAX_WINDOW_SIZE) {
        fprintf(stderr, "Erro: Tamanho da janela (%d) inválido. Deve ser entre 1 e %d.\n", 
                window_size, MAX_WINDOW_SIZE);
        return -1;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Erro ao criar arquivo de saída");
        return -1;
    }

    int socket_fd = setup_udp_socket(port, 1);  // Servidor
    if (socket_fd < 0) {
        fclose(output_file);
        return -1;
    }

    uint32_t expected_seq = 1;
    data_packet_t packet;

    while (1) {
        ssize_t received = recvfrom(socket_fd, &packet, sizeof(packet), 0, NULL, NULL);
        if (received < 0) {
            perror("Erro ao receber pacote");
            break;
        }

        uint32_t seq_num = ntohl(packet.seq_num);

        if (seq_num == expected_seq) {
            fwrite(packet.data, 1, received - sizeof(uint32_t), output_file);
            ack_packet_t ack = {htonl(expected_seq), 0};
            sendto(socket_fd, &ack, sizeof(ack), 0, NULL, 0);
            expected_seq++;
        } else {
            ack_packet_t ack = {htonl(expected_seq - 1), 0};
            sendto(socket_fd, &ack, sizeof(ack), 0, NULL, 0);
        }
    }

    fclose(output_file);
    close(socket_fd);
    return 0;
}
