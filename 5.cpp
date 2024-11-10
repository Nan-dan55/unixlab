#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>

#define MAX_BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

std::vector<int> buffer; // Shared buffer
sem_t mutex;             // Mutex semaphore for mutual exclusion
sem_t empty;             // Semaphore for tracking empty slots in the buffer
sem_t full;              // Semaphore for tracking filled slots in the buffer

// Producer function
void* producer(void* arg) {
    int item = *((int*)arg); // Get the item (producer ID)
    while (true) {
        sleep(1); // Simulate production time

        sem_wait(&empty); // Wait until there’s an empty slot in the buffer
        sem_wait(&mutex); // Lock the critical section

        buffer.push_back(item); // Add the item to the buffer
        std::cout << "Produced: " << item << ", Buffer size: " << buffer.size() << std::endl;

        sem_post(&mutex); // Unlock the critical section
        sem_post(&full);  // Signal that a slot in the buffer is now filled
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    while (true) {
        sleep(1); // Simulate time to consume an item

        sem_wait(&full); // Wait for a filled slot in the buffer
        sem_wait(&mutex); // Lock the critical section

        int item = buffer.back(); // Consume an item from the buffer
        buffer.pop_back();
        std::cout << "Consumed: " << item << ", Buffer size: " << buffer.size() << std::endl;

        sem_post(&mutex); // Unlock the critical section
        sem_post(&empty); // Signal that a slot in the buffer is now empty
    }
    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&mutex, 0, 1); // Mutex semaphore for mutual exclusion
    sem_init(&empty, 0, MAX_BUFFER_SIZE); // Empty semaphore (buffer slots available)
    sem_init(&full, 0, 0);  // Full semaphore (buffer slots filled)

    // Create producer and consumer threads
    pthread_t producer_threads[NUM_PRODUCERS]; // Producer thread array
    pthread_t consumer_threads[NUM_CONSUMERS]; // Consumer thread array

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        int* item = new int(i);  // Assign item number to producer
        pthread_create(&producer_threads[i], NULL, producer, (void*)item);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);
    }

    // Join producer threads
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_join(producer_threads[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
