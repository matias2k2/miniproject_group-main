#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdio.h>

#define MAX_CHUNK_SIZE 1000
#define MAX_WINDOW_SIZE 32
#define MAX_RETRIES 3
#define TIMEOUT_SEC 1

// Estrutura de pacote de dados
typedef struct __attribute__((packed)) {
    uint32_t seq_num;  // Número de sequência
    char data[MAX_CHUNK_SIZE];  // Dados do chunk
} data_packet_t;

// Estrutura de pacote de acknowledgement
typedef struct __attribute__((packed)) {
    uint32_t seq_num;  // Número de sequência base
    uint32_t selective_acks;  // Máscara de ACKs seletivos
} ack_packet_t;

// Assinaturas de funções comuns
int setup_udp_socket(int port, int is_server);
int set_socket_timeout(int socket_fd, int seconds);

// Declarações das funções principais
int file_sender(const char *filename, const char *host, int port, int window_size);
int file_receiver(const char *output_filename, int port, int window_size);

#endif // PROTOCOL_H
