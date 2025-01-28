CC = gcc
CFLAGS = -Wall -I./validators


SRC_DISTRIBUTION = src/distribution/distribution.c src/validators/distribution_hub.c src/validators/utils.c
OBJ_DISTRIBUTION = $(SRC_DISTRIBUTION:.c=.o)
EXEC_DISTRIBUTION = dystrybutornia

SRC_STORAGE = src/storage/storage.c src/validators/storage.c src/validators/read_config_file.c
OBJ_STORAGE = $(SRC_STORAGE:.c=.o)
EXEC_STORAGE = m1

all: $(EXEC_DISTRIBUTION) $(EXEC_STORAGE)

$(EXEC_DISTRIBUTION): $(OBJ_DISTRIBUTION)
	$(CC) -o $(EXEC_DISTRIBUTION) $(OBJ_DISTRIBUTION)

$(EXEC_STORAGE): $(OBJ_STORAGE)
	$(CC) -o $(EXEC_STORAGE) $(OBJ_STORAGE)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ_DISTRIBUTION) $(OBJ_STORAGE) $(EXEC_DISTRIBUTION) $(EXEC_STORAGE)
