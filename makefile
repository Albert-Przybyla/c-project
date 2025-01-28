CC = gcc
CFLAGS = -Wall -I./validators

SRC = src/distribution/distribution.c src/validators/distribution_hub.c src/validators/utils.c
OBJ = $(SRC:.c=.o)
EXEC = dystrybutornia

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)    

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)    