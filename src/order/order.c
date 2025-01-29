#include "order.h"
#include <stdlib.h>
#include <time.h>

Order generate_order(int id, int max_A, int max_B, int max_C)
{
    Order order;
    order.mtype = 1;
    order.order_id = id;
    order.A = rand() % (max_A + 1);
    order.B = rand() % (max_B + 1);
    order.C = rand() % (max_C + 1);
    return order;
}