
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include "../validators/validator.h"
#include "../order/order.h"
#include "../validators/utils.h"

int main(int argc, char *argv[])
{

    int order_count, max_a, max_b, max_c;

    if (!validate_distribution_hub(argc, argv, &order_count, &max_a, &max_b, &max_c))
    {
        return 1;
    }
    int key = get_key(argv[1]);

    printf("Key: %d\n", key);
    printf("Order Count: %d\n", order_count);
    printf("Max A: %d, Max B: %d, Max C: %d\n", max_a, max_b, max_c);

    srand(time(NULL));

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid < 0)
    {
        perror("msgget");
        return 1;
    }
    for (int i = 0; i < order_count; i++)
    {
        Order order = generate_order(max_a, max_b, max_c);
        if (msgsnd(msgid, &order, sizeof(Order) - sizeof(long), 0) < 0)
        {
            perror("msgsnd");
            return 1;
        }
        else
        {
            printf("Order %d sent\n", i + 1);
        }
        printf("Sent Order: A=%d, B=%d, C=%d\n", order.A, order.B, order.C);
        usleep(500000);
    }

    printf("Order generation completed. Orders sent to storage.\n");

    return 0;
}