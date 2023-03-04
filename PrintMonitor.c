// Printer management scenario
// James Miller
// 2/26/2023
// Assignment 2 - Monitors and Semaphores

#define _BSD_SOURCE
#define MAX_PRINTER_QUEUE 10

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function prototypes
struct Monitor *init_monitor(int);
void add_job(struct Monitor *, int);
int remove_job(struct Monitor *);
void createPrint();

// Monitor definition
struct Monitor
{
    struct Printer *printer;
    int printer_queue_size;
    int print_wait;
    int cancel_wait;
};

// Structure used to store the printer queue
struct Printer
{
    int job_id;
    int queue_size;
};

// Initialize the monitor
struct Monitor *init_monitor(int printer_queue_size)
{
    struct Monitor *monitor = (struct Monitor *)malloc(sizeof(struct Monitor));
    monitor->printer_queue_size = printer_queue_size;
    monitor->printer = (struct Printer *)malloc(printer_queue_size * sizeof(struct Printer));
    monitor->print_wait = 0;
    monitor->cancel_wait = 0;
    return monitor;
}

// Add job to printer queue
void add_job(struct Monitor *monitor, int job_id)
{
    if (monitor->printer_queue_size == MAX_PRINTER_QUEUE)
    {
        // Printer queue is full
        printf("Printer queue is full, print job %d is waiting...\n", job_id);
        monitor->print_wait = 1;
    }
    else
    {
        // Add job to printer queue
        monitor->printer[monitor->printer_queue_size].job_id = job_id;
        printf("Print Job %d - added to printer queue\n", job_id);
        monitor->printer_queue_size++;
        // If cancel job was waiting, notify it
        if (monitor->cancel_wait)
        {
            monitor->cancel_wait = 0;
            printf("Job cancellation - notified\n");
        }
    }
}

// Remove job from printer queue
int remove_job(struct Monitor *monitor)
{
    if (monitor->printer_queue_size == 0)
    {
        // Printer queue is empty
        printf("Printer queue is empty, job is waiting...\n");
        monitor->cancel_wait = 1;
        return -1;
    }
    else
    {
        // Remove job from printer queue
        int job_id = monitor->printer[monitor->printer_queue_size - 1].job_id;
        printf("Job %d - removed from printer queue\n", job_id);
        monitor->printer_queue_size--;
        // If print job was waiting, notify it
        if (monitor->print_wait)
        {
            monitor->print_wait = 0;
            printf("Print job - notified\n");
        }
        return job_id;
    }
}

void createPrint()
{
    printf("Job Producer ID 1\n");
    printf("Job Consumer ID 2\n");
}

int main()
{
    int printer_queue_size = 0;
    struct Monitor *monitor = init_monitor(printer_queue_size);

    printf("Creating print jobs (Producer Consumer Scenario).\n");
    createPrint();
    printf("\n");

    int i = 5;

    while (i > 0)
    {
        // Add 3 jobs to printer queue
        for (int i = 1; i <= 3; i++)
        {
            add_job(monitor, i);
            usleep(500000);
        }

        // Remove 3 jobs from printer queue
        for (int i = 1; i <= 3; i++)
        {
            int job_id = remove_job(monitor);
            if (job_id != -1)
            {
                // printf("Job %d printed.\n", job_id);
            }
            usleep(500000);
        }
        i--;
    }

    return 0;
}