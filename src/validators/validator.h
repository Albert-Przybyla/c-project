#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdbool.h>

int validate_distribution_hub(int argc, char *argv[], int *order_count, int *max_a, int *max_b, int *max_c);
int validate_storage(int argc, char *argv[]);
int read_config_file(const char *file_path, const char *key, int *a, int *b, int *c);

#endif