
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

int main(int argc, char *argv[])
{
    char *key;
    int order_count, max_a, max_b, max_c;

    if (!validate_distribution_hub(argc, argv, &order_count, &max_a, &max_b, &max_c))
    {
        return 1;
    }

    key = argv[1];

    printf("Key: %s\n", key);
    printf("Order Count: %d\n", order_count);
    printf("Max A: %d, Max B: %d, Max C: %d\n", max_a, max_b, max_c);

    srand(time(NULL));

    key_t shm_key = ftok(key, 'R');
    int msgid = msgget(shm_key, 0666 | IPC_CREAT);
    for (int i = 0; i < order_count; i++)
    {
        Order order = generate_order(max_a, max_b, max_c);
        if (msgsnd(msgid, &order, sizeof(Order) - sizeof(long), 0) < 0)
        {
            perror("msgsnd");
            return 1;
        }
        printf("Sent Order: A=%d, B=%d, C=%d\n", order.A, order.B, order.C);
        usleep(500000);
        usleep(500000);
    }

    printf("Order generation completed. Orders sent to storage.\n");

    return 0;
}