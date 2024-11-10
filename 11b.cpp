#include <iostream>
#include <pthread.h>

// Function that will be executed by each thread
void* printHello(void* threadNumber) {
    int* num = static_cast<int*>(threadNumber);  // Cast the argument to int*
    std::cout << "Hello World from Thread " << *num << std::endl;
    pthread_exit(NULL);
}

int main() {
    const int numThreads = 5;

    // Loop to create threads
    for (int i = 1; i <= numThreads; ++i) {
        pthread_t thread;
        int threadNumber = i;  // Assign the thread number
        
        int threadCreateStatus = pthread_create(&thread, NULL, printHello, &threadNumber);
        
        if (threadCreateStatus) {
            std::cerr << "Error creating thread: " << threadCreateStatus << std::endl;
            return -1;
        }

        // Wait for the thread to finish
        pthread_join(thread, NULL);
    }

    return 0;
}
