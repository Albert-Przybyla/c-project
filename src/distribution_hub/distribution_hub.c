
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <signal.h>

#include "../logger/logger.h"
#include "../validators/validator.h"
#include "../order/order.h"
#include "../validators/utils.h"

void listen_for_payments(int msgid, int shmid)
{
    int *total_cost = shmat(shmid, NULL, 0);
    if (total_cost == (int *)-1)
    {
        perror("shmat");
        exit(1);
    }
    while (1)
    {
        PaymentResponse payment_response;
        payment_response.mtype = 2;
        if (msgrcv(msgid, &payment_response, sizeof(PaymentResponse) - sizeof(long), 2, 0) < 0)
        {
            perror("msgrcv 2");
            exit(1);
        }
        *total_cost += payment_response.total_cost;
        state("Received payment response: Order ID = %d, cost = %dGLD\n", payment_response.order_id, payment_response.total_cost);
    }
}

void listen_for_stop(int msgid, pid_t parent_pid, int shmid)
{
    int *total_cost = shmat(shmid, NULL, 0);
    if (total_cost == (int *)-1)
    {
        perror("shmat");
        exit(1);
    }
    int i = 0;
    while (1)
    {
        StopCounter stop_counter;
        stop_counter.mtype = 3;
        if (msgrcv(msgid, &stop_counter, sizeof(StopCounter) - sizeof(long), 3, 0) < 0)
        {
            perror("msgrcv 3");
            exit(1);
        }
        i++;
        state("Received stop counter signal %d\n", i);
        if (i >= 3)
        {
            warn("Received 3 stop signals, terminating...\n");
            state("Total cost: %dGLD\n", *total_cost);
            kill(parent_pid, SIGTERM);
            shmdt(total_cost);
            shmctl(shmid, IPC_RMID, NULL);
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{

    int order_count, max_a, max_b, max_c;

    if (!validate_distribution_hub(argc, argv, &order_count, &max_a, &max_b, &max_c))
    {
        return 1;
    }
    int key = get_key(argv[1]);

    init("Key: %d\n", key);
    init("Order Count: %d\n", order_count);
    init("Max A: %d, Max B: %d, Max C: %d\n", max_a, max_b, max_c);

    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        return 1;
    }

    srand(time(NULL));

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid < 0)
    {
        perror("msgget");
        return 1;
    }

    pid_t pid_payment = fork();
    if (pid_payment < 0)
    {
        perror("fork for payments");
        return 1;
    }
    else if (pid_payment == 0)
    {
        listen_for_payments(msgid, shmid);
        exit(0);
    }

    pid_t pid_stop = fork();
    if (pid_stop < 0)
    {
        perror("fork for stop signals");
        return 1;
    }
    else if (pid_stop == 0)
    {
        listen_for_stop(msgid, getppid(), shmid);
        exit(0);
    }

    for (int i = 0; i < order_count; i++)
    {
        Order order = generate_order(i, max_a, max_b, max_c);
        if (msgsnd(msgid, &order, sizeof(Order) - sizeof(long), 0) < 0)
        {
            perror("msgsnd");
            return 1;
        }
        else
        {
            printf("Order %d sent\n", i + 1);
        }
        info("Sent Order: A=%d, B=%d, C=%d\n", order.A, order.B, order.C);
        usleep(500000);
    }

    warn("Order generation completed.\n");
    wait(NULL);
    wait(NULL);

    return 0;
}