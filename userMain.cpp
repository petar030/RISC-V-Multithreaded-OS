#include "h/printing.hpp"
#include "h/syscall_cpp.hpp"

#define BUFFER_SIZE 10
#define NUM_CONSUMERS 3
#define NUM_ITEMS 20
#define NUM_THREADS (NUM_CONSUMERS + 1)  // Producer + Consumers

struct ProducerConsumerContext {
    Semaphore emptySlots;
    Semaphore fullSlots;
    Semaphore mutex;
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    Semaphore done;  // Semaphore to track thread completion

    ProducerConsumerContext()
            : emptySlots(BUFFER_SIZE), fullSlots(0), mutex(1), in(0), out(0), done(0) {}
};

void producer(void* arg) {
    auto* context = static_cast<ProducerConsumerContext*>(arg);
    for (int i = 0; i < NUM_ITEMS; ++i) {
        context->emptySlots.wait();
        context->mutex.wait();

        // Produce an item
        context->buffer[context->in] = i;
        context->in = (context->in + 1) % BUFFER_SIZE;

        printString("Produced item: ");
        printInt(i);
        printString("\n");

        context->mutex.signal();
        context->fullSlots.signal();
    }

    context->done.signal();  // Signal that the producer has finished
}

void consumer(void* arg) {
    auto* context = static_cast<ProducerConsumerContext*>(arg);
    int id = *static_cast<int*>(arg);
    for (int i = 0; i < NUM_ITEMS / NUM_CONSUMERS; ++i) {
        context->fullSlots.wait();
        context->mutex.wait();

        // Consume an item
        int item = context->buffer[context->out];
        context->out = (context->out + 1) % BUFFER_SIZE;

        printString("Consumer ");
        printInt(id);
        printString(" consumed item: ");
        printInt(item);
        printString("\n");

        context->mutex.signal();
        context->emptySlots.signal();
    }

    context->done.signal();  // Signal that the consumer has finished
}

void testProducerConsumer() {
    printString("Testing producer-consumer problem:\n");
    ProducerConsumerContext context;
    Thread* producerThread = new Thread(producer, &context);
    Thread* consumerThreads[NUM_CONSUMERS];

    producerThread->start();

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumerThreads[i] = new Thread(consumer, &context);
        consumerThreads[i]->start();
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        context.done.wait();  // Wait for producer + consumer threads to signal
    }

    // Clean up threads
    delete producerThread;
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        delete consumerThreads[i];
    }
}

void userMain() {
    testProducerConsumer();
    Thread::sleep(10000); // Allow time for producer-consumer test to complete
}
