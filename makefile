CC = gcc
CFLAGS = -Wall -I./validators


SRC_DISTRIBUTION_HUB = src/distribution_hub/distribution_hub.c src/validators/distribution_hub.c src/validators/utils.c src/order/order.c src/logger/logger.c
OBJ_DISTRIBUTION_HUB = $(SRC_DISTRIBUTION_HUB:.c=.o)
EXEC_DISTRIBUTION_HUB = dystrybutornia

SRC_STORAGE = src/storage/storage.c src/validators/storage.c src/validators/utils.c src/validators/read_config_file.c src/logger/logger.c
OBJ_STORAGE = $(SRC_STORAGE:.c=.o)
EXEC_STORAGE = m1

all: $(EXEC_DISTRIBUTION_HUB) $(EXEC_STORAGE)

$(EXEC_DISTRIBUTION_HUB): $(OBJ_DISTRIBUTION_HUB)
	$(CC) -o $(EXEC_DISTRIBUTION_HUB) $(OBJ_DISTRIBUTION_HUB)

$(EXEC_STORAGE): $(OBJ_STORAGE)
	$(CC) -o $(EXEC_STORAGE) $(OBJ_STORAGE)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ_DISTRIBUTION_HUB) $(OBJ_STORAGE) $(EXEC_DISTRIBUTION_HUB) $(EXEC_STORAGE)
