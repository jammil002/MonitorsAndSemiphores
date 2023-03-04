// Printer management scenario
// James Miller
// 2/26/2023
// Assignment 2 - Monitors and Semaphores

#include <semaphore.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_JOBS 10 // Maximum number of jobs that the printer can handle

// Structure for a semaphore
typedef struct
{
    int amount;     // Number of available slots in the buffer
    int accessible; // Whether or not the buffer is accessible
    int mutex;      // Mutual exclusion for accessing the buffer
} semaphore_t;

void startSemaphore(semaphore_t *semaphore, int amount);                            // Function prototype for initializing a semaphore
void enterSempahore(semaphore_t *semaphore);                                        // Function prototype for acquiring a semaphore
void exitSemaphore(semaphore_t *s);                                                 // Function prototype for releasing a semaphore
int generateRandom();                                                               // Function prototype for generating a random number
void start();                                                                       // Function prototype for printing a message indicating that the printer is ready
void consumerProcess(semaphore_t *empty, semaphore_t *full, int pages, int random); // Function prototype for the consumer process
void producerProcess(semaphore_t *empty, semaphore_t *full, int pages, int random); // Function prototype for the producer process

// Function that prints a message indicating that the printer is ready
void start()
{
    printf("Printer - Ready to print!\n");
}

// Function that initializes a semaphore
void startSemaphore(semaphore_t *semaphore, int amount)
{
    semaphore->amount = amount;                   // Set the number of available slots in the buffer
    semaphore->accessible = (amount > 0) ? 1 : 0; // Set whether or not the buffer is accessible
    semaphore->mutex = 1;                         // Set mutual exclusion for accessing the buffer
}

// Function that acquires a semaphore
void enterSempahore(semaphore_t *semaphore)
{
    while (semaphore->mutex == 0) // Wait for the semaphore to become available
        ;
    semaphore->mutex = 0;              // Acquire the semaphore
    while (semaphore->accessible == 0) // Wait for the buffer to become accessible
        ;
    semaphore->amount--; // Decrement the number of available slots in the buffer

    if (semaphore->amount == 0)    // If the buffer is full
        semaphore->accessible = 0; // Set the buffer as not accessible

    semaphore->mutex = 1; // Release the semaphore
}

// Function that releases a semaphore
void exitSemaphore(semaphore_t *semaphore)
{
    while (semaphore->mutex == 0) // Wait for the semaphore to become available
        ;
    semaphore->mutex = 0; // Acquire the semaphore

    semaphore->amount++; // Increment the number of available slots in the buffer

    if (semaphore->amount > 0)     // If the buffer is not full
        semaphore->accessible = 1; // Set the buffer as accessible

    semaphore->mutex = 1; // Release the semaphore
}

// Function that generates a random number between 1 and 10
int generateRandom()
{
    int i, n;
    time_t t;
    srand((unsigned)time(&t)); // Seed the random number generator

    n = rand() % 10 + 1; // Generate a random number between 1 and 10
    return n;
}

void consumerProcess(semaphore_t *empty, semaphore_t *full, int pages, int random)
{
    enterSempahore(full);
    printf("Job %d - Printing, %d pages\n", random, pages);
    usleep(100000);
    printf("Job %d - Completed\n", random);
    usleep(100000);
    exitSemaphore(empty);
}

void producerProcess(semaphore_t *empty, semaphore_t *full, int pages, int random)
{
    enterSempahore(empty);
    printf("Job %d - Queued, %d pages\n", random, pages);
    usleep(100000);
    exitSemaphore(full);
}

int main()
{
    int counter;
    int i = 0;
    int pages[MAX_JOBS];
    semaphore_t empty, full;

    printf("Initializing Printer.\n");
    start();
    printf("\n");

    startSemaphore(&empty, MAX_JOBS);
    startSemaphore(&full, 0);

    while (i != 3)
    {
        for (counter = 0; counter < MAX_JOBS; counter++)
        {
            pages[counter] = generateRandom();
            producerProcess(&empty, &full, pages[counter], counter + 1);
        }
        for (counter = MAX_JOBS - 1; counter > -1; counter--)
        {
            consumerProcess(&empty, &full, pages[counter], counter + 1);
        }
        i++;
    }
    return 0;
}