#include <stdio.h>
#include <malloc.h>

void swap(int *arr, int index_1, int index_2) {
    int elem = arr[index_1];
    arr[index_1] = arr[index_2];
    arr[index_2] = elem;
}

void print_array(int *arr, int len_arr) {
    for (int i = 0; i < len_arr; i++) {
        printf("%d ", arr[i]);
    }
}

void heapify(int *arr, int len_arr, int root) {
    int maximum = root;

    int next_left = root * 2 + 1;
    int next_right = root * 2 + 2;

    if (next_left < len_arr && arr[next_left] > arr[maximum]) {
        maximum = next_left;
    }
    if (next_right < len_arr && arr[next_right] > arr[maximum]) {
        maximum = next_right;
    }
    if (maximum != root) {
        swap(arr, maximum, root);
        heapify(arr, len_arr, maximum);
    }
}

void heapsort(int *arr, int len_arr) {
    for (int i = len_arr / 2; i >= 0; i--) {
        heapify(arr, len_arr, i);
    }
    for (int i = len_arr - 1; i >= 0; i--) {
        swap(arr, 0, i);
        heapify(arr, i, 0);
    }
}

int main() {
    int N;
    int *arr;

    if (scanf("%d", &N)) {};
    if (N == 0) {
        return 0;
    }
    arr = (int *) malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++) {
        if (scanf("%d", &arr[i])) {};
    }

    heapsort(arr, N);
    print_array(arr, N);

    free(arr);
    return 0;
}
