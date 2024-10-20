#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // For clock()
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

// Function to get the current CPU time
void get_cpu_times(FILETIME* idleTime, FILETIME* kernelTime, FILETIME* userTime) {
    GetSystemTimes(idleTime, kernelTime, userTime);
}

// Function to calculate CPU usage
double calculate_cpu_usage(FILETIME idleTimeStart, FILETIME idleTimeEnd, 
                           FILETIME kernelTimeStart, FILETIME kernelTimeEnd, 
                           FILETIME userTimeStart, FILETIME userTimeEnd) {
    ULARGE_INTEGER startIdle, endIdle, startKernel, endKernel, startUser, endUser;

    startIdle.LowPart = idleTimeStart.dwLowDateTime;
    startIdle.HighPart = idleTimeStart.dwHighDateTime;
    endIdle.LowPart = idleTimeEnd.dwLowDateTime;
    endIdle.HighPart = idleTimeEnd.dwHighDateTime;

    startKernel.LowPart = kernelTimeStart.dwLowDateTime;
    startKernel.HighPart = kernelTimeStart.dwHighDateTime;
    endKernel.LowPart = kernelTimeEnd.dwLowDateTime;
    endKernel.HighPart = kernelTimeEnd.dwHighDateTime;

    startUser.LowPart = userTimeStart.dwLowDateTime;
    startUser.HighPart = userTimeStart.dwHighDateTime;
    endUser.LowPart = userTimeEnd.dwLowDateTime;
    endUser.HighPart = userTimeEnd.dwHighDateTime;

    ULONGLONG totalSystemTimeStart = (startKernel.QuadPart + startUser.QuadPart) - startIdle.QuadPart;
    ULONGLONG totalSystemTimeEnd = (endKernel.QuadPart + endUser.QuadPart) - endIdle.QuadPart;

    ULONGLONG systemTimeDifference = totalSystemTimeEnd - totalSystemTimeStart;
    ULONGLONG totalTimeDifference = (endKernel.QuadPart + endUser.QuadPart) - (startKernel.QuadPart + startUser.QuadPart);

    return (double)(systemTimeDifference) / (double)(totalTimeDifference) * 100.0;
}

// Function to get memory usage (only for Windows)
void print_memory_usage() {
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter))) {
        printf("Memory used: %ld KB\n", memCounter.WorkingSetSize / 1024);
    }
}
#endif

#define MAX_SIZE 2048  // Maximum matrix size
#define NUM_TESTS 5    // Number of tests per matrix size

double a[MAX_SIZE][MAX_SIZE];
double b[MAX_SIZE][MAX_SIZE];
double c[MAX_SIZE][MAX_SIZE];

// Function to initialize matrices
void initialize_matrices(int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            a[i][j] = (double) rand() / RAND_MAX;
            b[i][j] = (double) rand() / RAND_MAX;
            c[i][j] = 0;
        }
    }
}

// Function to multiply matrices
void multiply_matrices(int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                c[i][j] += a[i][k] * b[k][j];  // Accumulate result
            }
        }
    }
}

// Function to run tests
void run_tests(int size) {
    clock_t start_time, end_time;
    double total_time = 0;

#ifdef _WIN32
    FILETIME idleTimeStart, kernelTimeStart, userTimeStart;
    FILETIME idleTimeEnd, kernelTimeEnd, userTimeEnd;
#endif

    for (int test = 0; test < NUM_TESTS; ++test) {
        initialize_matrices(size);

#ifdef _WIN32
        // Get CPU times before starting
        get_cpu_times(&idleTimeStart, &kernelTimeStart, &userTimeStart);
#endif

        // Measure CPU time
        start_time = clock();  // Start the CPU clock
        multiply_matrices(size);
        end_time = clock();  // End the CPU clock

        // Get CPU times after completion
#ifdef _WIN32
        get_cpu_times(&idleTimeEnd, &kernelTimeEnd, &userTimeEnd);

        // Calculate and print CPU usage
        double cpu_usage = calculate_cpu_usage(idleTimeStart, idleTimeEnd, kernelTimeStart, kernelTimeEnd, userTimeStart, userTimeEnd);
        printf("CPU Usage: %.2f%%\n", cpu_usage);  // Print as percentage with two decimal points
        print_memory_usage();  // Print memory usage on Windows
#endif

        // Calculate total time in seconds
        double diff = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        total_time += diff;

        printf("Test %d for size %d: %0.6f seconds\n", test + 1, size, diff);
    }

    // Calculate average time
    double average_time = total_time / NUM_TESTS;
    printf("Average for size %d: %0.6f seconds\n\n", size, average_time);
}

int main() {
    // Define matrix sizes to test
    int matrix_sizes[] = {256, 512, 1024, 2048};
    int num_sizes = sizeof(matrix_sizes) / sizeof(matrix_sizes[0]);

    for (int i = 0; i < num_sizes; ++i) {
        int size = matrix_sizes[i];
        printf("=== Tests for matrix size: %dx%d ===\n", size, size);
        run_tests(size);
    }

    return 0;
}
