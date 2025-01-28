
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
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
    int shmid = shmget(shm_key, sizeof(Order) * order_count, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        return 1;
    }

    Order *orders = (Order *)shmat(shmid, NULL, 0);
    if (orders == (Order *)-1)
    {
        perror("shmat");
        return 1;
    }

    for (int i = 0; i < order_count; i++)
    {
        orders[i] = generate_order(max_a, max_b, max_c);
        printf("Zamówienie %d: A = %d, B = %d, C = %d\n", i + 1, orders[i].A, orders[i].B, orders[i].C);
        usleep(500000);
    }

    if (shmdt(orders) == -1)
    {
        perror("shmdt");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return 1;
    }

    printf("Zakończono generowanie zamówień.\n");

    return 0;
}