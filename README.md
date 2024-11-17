
# RISC-V Multithreaded Operating System

This project implements a multithreaded operating system for the RISC-V architecture. It includes support for basic system calls, multithreading, synchronization mechanisms, and basic I/O functionality. Written in both C and C++, it is designed to provide the essential features for user-level programs and kernel operations.

## Features

- **Multithreading**: Supports creating and managing threads for parallel execution.
- **System Calls**: Implements a set of system calls for thread management, synchronization, memory allocation, and basic I/O.
- **Synchronization**: Implements semaphores for synchronizing threads and ensuring safe concurrent access to shared resources.
- **Basic I/O**: Includes system calls for reading from and writing to the console.

## Supported System Calls

### Memory Management
- `mem_alloc(size_t size)`: Allocates memory of the specified size.
- `mem_free(void* ptr)`: Frees allocated memory.

### Thread Management
- `thread_create(thread_t* handle, Body body, void* arg)`: Creates a new thread.
- `thread_exit()`: Exits the current thread.
- `thread_dispatch()`: Dispatches the next thread.

### Semaphore Operations
- `sem_open(sem_t* handle, unsigned init)`: Opens a semaphore with an initial value.
- `sem_close(sem_t handle)`: Closes a semaphore.
- `sem_wait(sem_t id)`: Waits on a semaphore.
- `sem_signal(sem_t id)`: Signals a semaphore.
- `sem_trywait(sem_t id)`: Tries to wait on a semaphore without blocking.
- `sem_timedwait(sem_t id, time_t timeout)`: Waits on a semaphore with a timeout.

### Time Management
- `time_sleep(time_t time)`: Puts the current thread to sleep for a specified time.

### Console I/O
- `getc()`: Reads a character from the console.
- `putc(char c)`: Writes a character to the console.
- `putc_handler()`: Handles character output to the console.

### User Mode
- `user_mode_start()`: Starts execution in user mode.
- `user_mode_end()`: Ends execution in user mode.

## C++ System Calls

The OS also includes support for object-oriented programming features such as C++ system calls:

### Thread Management
- `Thread::start()`: Starts a thread.
- `Thread::dispatch()`: Dispatches threads.
- `Thread::sleep(time_t)`: Puts the current thread to sleep for the specified time.

### Semaphore Operations
- `Semaphore::wait()`: Waits on a semaphore.
- `Semaphore::signal()`: Signals a semaphore.
- `Semaphore::timedWait(time_t)`: Waits on a semaphore with a timeout.
- `Semaphore::tryWait()`: Tries to wait on a semaphore without blocking.

### Periodic Thread
- `PeriodicThread::terminate()`: Terminates a periodic thread.
- `PeriodicThread::periodicActivation()`: Periodically activates the thread.

### Console I/O
- `Console::getc()`: Reads a character from the console.
- `Console::putc(char c)`: Writes a character to the console.

## `userMain.cpp`

The entry point for user-level programs is the `userMain.cpp` file, which you can modify to test or develop your applications. This file allows you to experiment with thread creation, synchronization, and other OS functionalities in user space.

## Compilation Instructions

To compile the operating system, follow these steps:

1. **Install Required Tools**:
   - RISC-V GCC cross-compiler (`riscv64-unknown-elf-gcc`)
   - QEMU (for emulation)
   - Make

2. **Build the OS**:
   In the root of the project, run the following command to compile the project:
   ```bash
   make
   ```

3. **Clean Build**:
   If you need to clean the build artifacts (e.g., object files, binaries), run:
   ```bash
   make clean
   ```

4. **Run the OS in QEMU**:
   After compiling, you can run the OS in a QEMU emulator:
   ```bash
   make qemu
   ```

   This will launch QEMU with the built kernel and run your OS in an emulated RISC-V environment.

## File Structure

- **src/**: Contains the kernel and implementation files for the OS.
- **include/**: Header files for system calls, thread management, synchronization, etc.
- **Makefile**: Makefile for building and managing the project.
- **userMain.cpp**: The main entry point for user-level applications and testing.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests to improve the functionality, add new features, or fix bugs.


