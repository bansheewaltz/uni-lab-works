#include <stdio.h>
#include <stdlib.h>
#define true 1

void array_print(int *);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int median_of_three(int arr[], int l, int r) {
    int m = l + (r - l) / 2;

    if (arr[l] > arr[r]) {
        swap(&arr[l], &arr[r]);
    }
    if (arr[l] > arr[m]) {
        swap(&arr[l], &arr[m]);
    }
    if (arr[m] > arr[r]) {
        swap(&arr[m], &arr[r]);
    }

    return arr[m];
}

int partition(int arr[], int l, int r) {
    int i = l - 1;  // 1 to the left of the begining
    int j = r + 1;  // 1 to the right of the ending
    int pivot = median_of_three(arr, l, r);

    while (true) {
        while (arr[++i] < pivot) {
        }
        while (arr[--j] > pivot) {
        }
        if (i >= j) {
            return j;
        }
        swap(&arr[i], &arr[j]);
    }
}

void quicksort(int arr[], int l, int r) {
    if (r - l > 0) {
        int m = partition(arr, l, r);
        quicksort(arr, l, m);
        quicksort(arr, m + 1, r);
    }
}

int array_get_size(int *arr) {
    return arr[-1];
}

void array_print(int arr[]) {
    int array_size = array_get_size(arr);
    for (int i = 0; i < array_size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int array_read_size() {
    int n;
    if (!scanf("%d", &n) || n == 0) {
        exit(EXIT_SUCCESS);  // but actually FAILURE
    }
    return n;
}

int *array_read_elements(int n) {
    int *arr = (int *)calloc(n + 1, sizeof(int));
    if (arr == NULL) {
        exit(EXIT_SUCCESS);  // but actually FAILURE
    }
    arr[0] = n;
    for (int i = 0; i < n; ++i) {
        if (!scanf("%d", &arr[i + 1])) {
            exit(EXIT_SUCCESS);  // but actually FAILURE
        }
    }
    return arr + 1;
}

int *array_read() {
    int n = array_read_size();
    return array_read_elements(n);
}

void array_free(int *arr) {
    free(arr - 1);
}

int main() {
    int *arr = array_read();

    quicksort(arr, 0, array_get_size(arr) - 1);
    array_print(arr);
    array_free(arr);

    return EXIT_SUCCESS;
}
