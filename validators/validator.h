#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdbool.h>

bool validate_arguments(int argc, char *argv[], int *order_count, int *max_a, int *max_b, int *max_c);

#endif