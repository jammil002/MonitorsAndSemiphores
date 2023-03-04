// Printer management scenario
// James Miller
// 2/26/2023
// Assignment 2 - Monitors and Semaphores

// Define the _BSD_SOURCE macro and the maximum size of the printer queue
#define _BSD_SOURCE
#define MAX_PRINTER_QUEUE 10

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Declare the struct types for Monitor and Printer
struct Monitor *createMonitor(int);
void addJob(struct Monitor *, int);
int removeJob(struct Monitor *);
void createPrint();

// Define the Monitor struct
struct Monitor
{
    struct Printer *printer; // Pointer to an array of Printer structs

    int printQueueSize; // Current size of the printer queue
    int printWait;      // Flag to indicate whether there is a job waiting to be printed
    int cancelWait;     // Flag to indicate whether there is a job waiting to be cancelled
};

// Define the Printer struct
struct Printer
{
    int jobID;      // Job ID
    int queue_size; // Size of the printer queue
};

// Function to print a message indicating the producer-consumer scenario
void createPrint()
{
    printf("Job -> Producer ID 1\n");
    printf("Job -> Consumer ID 2\n");
}

// Function to create and initialize a monitor struct.
struct Monitor *createMonitor(int printQueueSize)
{
    struct Monitor *monitor = (struct Monitor *)malloc(sizeof(struct Monitor));           // Allocate memory for the Monitor struct
    monitor->printQueueSize = printQueueSize;                                             // Initialize the print queue size
    monitor->printer = (struct Printer *)malloc(printQueueSize * sizeof(struct Printer)); // Allocate memory for the Printer structs

    monitor->printWait = 0;  // Initialize the print wait flag
    monitor->cancelWait = 0; // Initialize the cancel wait flag
    return monitor;          // Return the initialized Monitor struct
}

// Function to add a print job to the printer queue
void addJob(struct Monitor *monitor, int jobID)
{
    // Check if the printer queue is full
    if (monitor->printQueueSize != MAX_PRINTER_QUEUE)
    {
        // Add the print job to the printer queue
        monitor->printer[monitor->printQueueSize].jobID = jobID;
        printf("Print Job %d - added to queue printer \n", jobID);
        monitor->printQueueSize++; // Increment the print queue size
        // Check if there is a job waiting to be cancelled
        if (monitor->cancelWait)
        {
            monitor->cancelWait = 0; // Clear the cancel wait flag
            printf("Print Job cancellation - notified\n");
        }
    }
    else
    {
        printf("Queue for printer is full, print job %d is waiting...\n", jobID);
        monitor->printWait = 1; // Set the print wait flag
    }
}

// Function to remove a print job from the printer queue
int removeJob(struct Monitor *monitor)
{
    // Check if the printer queue is empty
    if (monitor->printQueueSize != 0)
    {
        // Remove a print job from the printer queue
        int jobID = monitor->printer[monitor->printQueueSize - 1].jobID;
        printf("Print Job %d - removed from the printer queue\n", jobID);
        monitor->printQueueSize--; // Decrement the print queue size
        // Check if there is a job waiting to be printed
        if (monitor->printWait)
        {
            monitor->printWait = 0;
            printf("Print job - notified\n");
        }
        return jobID;
    }
    else
    {

        printf("Printer queue is empty, job is waiting...\n");
        monitor->cancelWait = 1; // Set the cancel wait flag
        return -1;
    }
}

int main()
{
    int amount = 0;
    struct Monitor *monitor = createMonitor(amount);

    printf("Creating print jobs - (Producer Consumer Scenario).\n");
    createPrint();
    printf("-----------------------\n");

    int i = 5;

    while (i > 0)
    {
        for (int i = 1; i <= 4; i++)
        {
            addJob(monitor, i);
            usleep(100000);
        }

        for (int i = 1; i <= 4; i++)
        {
            int jobID = removeJob(monitor);
            if (jobID != -1)
            {
            }
            usleep(100000);
        }
        i--;
    }
    printf("-----------------------\n");
    printf("Ending -> Producer Consumer Scenario\n");
    return 0;
}