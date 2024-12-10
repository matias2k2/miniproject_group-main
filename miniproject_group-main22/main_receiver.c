#include "protocol.h"  // Declaração de file_receiver
#include <stdlib.h>    // Declaração de atoi

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <arquivo_saida> <porta> <tamanho_janela>\n", argv[0]);
        return -1;
    }

    int window_size = atoi(argv[3]);
    return file_receiver(argv[1], atoi(argv[2]), window_size);
}
