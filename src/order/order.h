#ifndef ORDER_H
#define ORDER_H

typedef struct
{
    int A;
    int B;
    int C;
} Order;

Order generate_order(int max_A, int max_B, int max_C);

#endif
