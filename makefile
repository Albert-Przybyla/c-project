CC = gcc
CFLAGS = -Wall -I./validators

SRC = distribution.c validators/distribution_hub.c validators/utils.c
OBJ = $(SRC:.c=.o)
EXEC = dystrybutornia

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)    

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)    