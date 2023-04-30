// Heap sort of integer array program
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int arr[], int n, int i) {
    // Find largest among root, left child and right child
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // Swap and continue heapifying if root is not largest
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(int arr[], int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);

    // Heap sort
    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);

        // Heapify root element to get highest element at root again
        heapify(arr, i, 0);
    }
}

int main(void) {
    int n = 0;
    if (!scanf("%d", &n)) {
        return EXIT_SUCCESS;  // but actually FAILURE
    }

    int *arr = calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i) {
        if (!scanf("%d", &arr[i])) {
            return EXIT_SUCCESS;  // but actually FAILURE
        }
    }

    heap_sort(arr, n);

    for (int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }

    free(arr);
    return EXIT_SUCCESS;
}
