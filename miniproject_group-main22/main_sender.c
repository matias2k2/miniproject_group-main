#include "protocol.h"  // Declaração de file_sender
#include <stdlib.h>    // Declaração de atoi

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <arquivo> <host> <porta> <tamanho_janela>\n", argv[0]);
        return -1;
    }

    int window_size = atoi(argv[4]);
    return file_sender(argv[1], argv[2], atoi(argv[3]), window_size);
}
