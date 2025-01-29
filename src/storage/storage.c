#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <time.h>

#include "../logger/logger.h"
#include "../validators/validator.h"
#include "../validators/utils.h"
#include "../order/order.h"

#define COURIER_COUNT 3

typedef struct
{
    int A, B, C;
    int price_a, price_b, price_c;
    int GLD;
} SharedData;

int createSharedMemory()
{
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }
    return shmid;
}

void initSharedMemory(int shmid, int a, int b, int c, int price_a, int price_b, int price_c)
{
    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    shared_data->A = a;
    shared_data->B = b;
    shared_data->C = c;
    shared_data->price_a = price_a;
    shared_data->price_b = price_b;
    shared_data->price_c = price_c;
    shared_data->GLD = 0;
}

void runCourier(int msgid, int shmid)
{
    init("Courier %d started\n", getpid());

    time_t start_time = time(NULL);

    while (1)
    {
        Order order;
        if (msgrcv(msgid, &order, sizeof(Order) - sizeof(long), 1, IPC_NOWAIT) < 0)
        {
            if (time(NULL) - start_time >= 150)
            {
                info("COURIER %d TIMEOUT: No order received for 150 seconds, exiting.\n", getpid());
                break;
            }
            usleep(100000);
            continue;
        }

        start_time = time(NULL);

        info("COURIER %d RECEIVED ORDER: A=%d, B=%d, C=%d\n", getpid(), order.A, order.B, order.C);

        SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
        if (shared_data == (void *)-1)
        {
            perror("shmat");
            exit(1);
        }

        if (order.A <= shared_data->A && order.B <= shared_data->B && order.C <= shared_data->C)
        {
            shared_data->A -= order.A;
            shared_data->B -= order.B;
            shared_data->C -= order.C;

            state("NEW STOCK: A=%d, B=%d, C=%d\n", shared_data->A, shared_data->B, shared_data->C);

            int total_cost = (order.A * shared_data->price_a) + (order.B * shared_data->price_b) + (order.C * shared_data->price_c);
            shared_data->GLD += total_cost;

            PaymentResponse payment_response;
            payment_response.mtype = 2;
            payment_response.order_id = order.order_id;
            payment_response.total_cost = total_cost;
            if (msgsnd(msgid, &payment_response, sizeof(PaymentResponse) - sizeof(long), 0) < 0)
            {
                perror("msgsnd");
                exit(1);
            }
        }
        else
        {
            warn("COURIER %d OUT OF STOCK: A=%d, B=%d, C=%d\n", getpid(), order.A, order.B, order.C);
            exit(0);
        }

        shmdt(shared_data);
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
    int key = get_key(argv[2]);
    printf("Key: %d\n", key);
    int a = 0, b = 0, c = 0;
    int price_a = 0, price_b = 0, price_c = 0;

    if (!read_config_file(config_file_path, &a, &b, &c, &price_a, &price_b, &price_c))
    {
        return 1;
    }
    int shmid = createSharedMemory();
    initSharedMemory(shmid, a, b, c, price_a, price_b, price_c);

    init("STORAGE STARTED:\n\tSTOCK: A=%d, B=%d, C=%d\n\tPRICES: A=%d GLD, B=%d GLD, C=%d GLD\n", a, b, c, price_a, price_b, price_c);

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid < 0)
    {
        perror("msgget");
        return 1;
    }

    for (int i = 0; i < COURIER_COUNT; i++)
    {
        if (fork() == 0)
        {
            runCourier(msgid, shmid);
            exit(0);
        }
    }

    for (int i = 0; i < COURIER_COUNT; i++)
    {
        wait(NULL);
    }

    info("ALL COURIERS EXITED\n");

    StopCounter stop_counter;
    stop_counter.mtype = 3;
    if (msgsnd(msgid, &stop_counter, sizeof(StopCounter) - sizeof(long), 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }
    info("STOP MESSAGE SENT\n");
    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }
    info("STOCK: A=%d, B=%d, C=%d\n", shared_data->A, shared_data->B, shared_data->C);
    info("TOTAL MONEY: %d GLD\n", shared_data->GLD);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}