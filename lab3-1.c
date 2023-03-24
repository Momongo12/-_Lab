#include <stdio.h>
#include <malloc.h>

void swap(int *arr, int index_1, int index_2) {
    int t = arr[index_1];
    arr[index_1] = arr[index_2];
    arr[index_2] = t;
}

void sort(int *arr, int left, int right) {
    int pivot = arr[(left + right) / 2];
    int l = left, r = right;

    while (l <= r) {
        while (arr[l] < pivot) {
            l++;
        }
        while (arr[r] > pivot) {
            r--;
        }
        if (l <= r) {
            swap(arr, l++, r--);
        }
    }
    if (left < r){
        sort(arr, left, r);
    }
    if (right > l){
        sort(arr, l, right);
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

    sort(arr, 0, N - 1);

    for (int i = 0; i < N; i++){
        printf("%d ", arr[i]);
    }

    free(arr);
    return 0;
}
