#ifndef ORDER_H
#define ORDER_H

typedef struct
{
    long mtype;
    int order_id;
    int A;
    int B;
    int C;
} Order;

typedef struct
{
    long mtype;
    int order_id;
    int total_cost;
} PaymentResponse;

Order generate_order(int id, int max_A, int max_B, int max_C);

#endif
