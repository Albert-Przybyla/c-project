#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../validators/validator.h"
#include "../order/order.h"

#define COURIER_COUNT 3

void runCourier(int msgid)
{
    printf("Courier %d started\n", getpid());
    while (1)
    {
        Order order;
        if (msgrcv(msgid, &order, sizeof(Order) - sizeof(long), 1, 0) < 0)
        {
            perror("msgrcv");
            exit(1);
        }
        printf("Courier %d received new order: A=%d, B=%d, C=%d\n", getpid(), order.A, order.B, order.C);

        usleep(1500000);
    }
}

int main(int argc, char *argv[])
{
    if (!validate_storage(argc, argv))
    {
        return 1;
    }

    const char *config_file_path = argv[1];
    const char *key = argv[2];
    int a = 0, b = 0, c = 0;
    int price_a = 0, price_b = 0, price_c = 0;

    if (!read_config_file(config_file_path, &a, &b, &c, &price_a, &price_b, &price_c))
    {
        return 1;
    }

    printf("Storage initialized:\n\tStock: A=%d, B=%d, C=%d\n\tPrices: A=%d GLD, B=%d GLD, C=%d GLD\n", a, b, c, price_a, price_b, price_c);

    key_t msg_key = ftok(key, 'M');
    int msgid = msgget(msg_key, 0666 | IPC_CREAT);
    if (msgid < 0)
    {
        perror("msgget");
        return 1;
    }

    for (int i = 0; i < COURIER_COUNT; i++)
    {
        if (fork() == 0)
        {
            runCourier(msgid);
            exit(0);
        }
    }

    for (int i = 0; i < COURIER_COUNT; i++)
    {
        wait(NULL);
    }

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}