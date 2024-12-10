CC = gcc
CFLAGS = -Wall -Wextra -g

# Alvo principal
all: file_sender file_receiver

# Regra para compilar file_sender
file_sender: main_sender.c file_sender.c utils.c
	$(CC) $(CFLAGS) -o $@ main_sender.c file_sender.c utils.c

# Regra para compilar file_receiver (corrigido para file-receiver.c)
file_receiver: main_receiver.c file-receiver.c utils.c
	$(CC) $(CFLAGS) -o $@ main_receiver.c file-receiver.c utils.c

# Limpeza
clean:
	rm -f file_sender file_receiver *.o
