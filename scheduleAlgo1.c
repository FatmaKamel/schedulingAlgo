#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function prototypes
void generateRequests(int requests[], int numRequests);
int fcfs(int requests[], int initialHead, int numRequests);
int scan(int requests[], int initialHead, int maxCylinder, int numRequests, int direction);
int cscan(int requests[], int initialHead, int maxCylinder, int numRequests, int direction);
int compareInt(const void *a, const void *b);

int main() {
    int initialHead, numCylinders, numRequests, direction, choice;

    while (1) {
        // Get simulation parameters
        printf("\nEnter the initial head position: ");
        scanf("%d", &initialHead);

        printf("Enter the number of cylinders: ");
        scanf("%d", &numCylinders);

        printf("Enter the number of requests: ");
        scanf("%d", &numRequests);

        // Validate inputs
        if (initialHead < 0 || initialHead >= numCylinders) {
            printf("Initial head position must be between 0 and %d.\n", numCylinders - 1);
            continue;
        }
        if (numCylinders <= 0) {
            printf("Number of cylinders must be positive.\n");
            continue;
        }
        if (numRequests <= 0) {
            printf("Number of requests must be positive.\n");
            continue;
        }

        // Prompt for direction
        printf("Enter the initial direction (1 for outer to inner, 2 for inner to outer): ");
        scanf("%d", &direction);

        if (direction != 1 && direction != 2) {
            printf("Invalid direction. Please enter 1 (outer to inner) or 2 (inner to outer).\n");
            continue;
        }

        // Allocate memory for requests
        int *requests = (int *)malloc(numRequests * sizeof(int));
        if (requests == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }

        // Generate random requests
        generateRequests(requests, numRequests);

        // FCFS
        int fcfsMovement = fcfs(requests, initialHead, numRequests);
        printf("Total head movement (FCFS): %d\n", fcfsMovement);

        // SCAN
        int scanMovement = scan(requests, initialHead, numCylinders - 1, numRequests, direction);
        printf("Total head movement (SCAN): %d\n", scanMovement);

        // C-SCAN
        int cscanMovement = cscan(requests, initialHead, numCylinders - 1, numRequests, direction);
        printf("Total head movement (C-SCAN): %d\n", cscanMovement);

        // Free allocated memory
        free(requests);

        // Ask the user if they want to exit
        printf("\nEnter 0 to exit or any other number to continue: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Exiting the program. Goodbye!\n");
            break;
        }
    }

    return 0;
}

// Function to generate random cylinder requests
void generateRequests(int requests[], int numRequests) {
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < numRequests; i++) {
        requests[i] = rand() % numRequests; // Random number between 0 and numRequests-1
    }
}

// Function to calculate total head movement for FCFS
int fcfs(int requests[], int initialHead, int numRequests) {
    int totalMovement = 0;
    int currentHead = initialHead;

    for (int i = 0; i < numRequests; i++) {
        totalMovement += abs(requests[i] - currentHead);
        currentHead = requests[i];
    }

    return totalMovement;
}

// Function to calculate total head movement for SCAN
int scan(int requests[], int initialHead, int maxCylinder, int numRequests, int direction) {
    int totalMovement = 0;
    int currentHead = initialHead;
    int minCylinder = 0;

    // Create a copy of requests and sort it
    int sortedRequests[numRequests];
    for (int i = 0; i < numRequests; i++) {
        sortedRequests[i] = requests[i];
    }
    qsort(sortedRequests, numRequests, sizeof(int), compareInt);

    // Find the starting index based on initialHead
    int startIndex = 0;
    while (startIndex < numRequests && sortedRequests[startIndex] < initialHead) {
        startIndex++;
    }

    // Move in the initial direction
    if (direction == 2) { // Inner to outer
        for (int i = startIndex; i < numRequests; i++) {
            totalMovement += abs(sortedRequests[i] - currentHead);
            currentHead = sortedRequests[i];
        }
        // Reverse direction at the end
        totalMovement += abs(maxCylinder - currentHead);
        currentHead = maxCylinder;
        for (int i = startIndex - 1; i >= 0; i--) {
            totalMovement += abs(sortedRequests[i] - currentHead);
            currentHead = sortedRequests[i];
        }
    } else { // Outer to inner
        for (int i = startIndex - 1; i >= 0; i--) {
            totalMovement += abs(sortedRequests[i] - currentHead);
            currentHead = sortedRequests[i];
        }
        // Reverse direction at the beginning
        totalMovement += abs(minCylinder - currentHead);
        currentHead = minCylinder;
        for (int i = startIndex; i < numRequests; i++) {
            totalMovement += abs(sortedRequests[i] - currentHead);
            currentHead = sortedRequests[i];
        }
    }

    return totalMovement;
}

// Function to calculate total head movement for C-SCAN
int cscan(int requests[], int initialHead, int maxCylinder, int numRequests, int direction) {
    int totalMovement = 0;
    int currentHead = initialHead;
    int minCylinder = 0;

    // Create a copy of requests and sort it
    int sortedRequests[numRequests];
    for (int i = 0; i < numRequests; i++) {
        sortedRequests[i] = requests[i];
    }
    qsort(sortedRequests, numRequests, sizeof(int), compareInt);

    // Find the starting index based on initialHead
    int startIndex = 0;
    while (startIndex < numRequests && sortedRequests[startIndex] < initialHead) {
        startIndex++;
    }

    // Move in the initial direction
    if (direction == 2) { // Inner to outer
        for (int i = startIndex; i < numRequests; i++) {
            totalMovement += abs(sortedRequests[i] - currentHead);
            currentHead = sortedRequests[i];
        }
        // Jump to the beginning and continue servicing
        totalMovement += abs(maxCylinder - currentHead); // Jump to the end
        currentHead = minCylinder;
        for (int i = 0; i < startIndex; i++) {
            totalMovement += abs(sortedRequests[i] - currentHead);
            currentHead = sortedRequests[i];
        }
    } else { // Outer to inner
        for (int i = startIndex - 1; i >= 0; i--) {
            totalMovement += abs(sortedRequests[i] - currentHead);
            currentHead = sortedRequests[i];
        }
        // Jump to the end and continue servicing
        totalMovement += abs(currentHead - minCylinder); // Jump to the beginning
        currentHead = maxCylinder;
        for (int i = numRequests - 1; i >= startIndex; i--) {
            totalMovement += abs(sortedRequests[i] - currentHead);
            currentHead = sortedRequests[i];
        }
    }

    return totalMovement;
}

// Comparison function for qsort
int compareInt(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}
