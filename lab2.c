#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_INPUT 1
#define LEN_SEQUENCE 10

int compare(char i_0, char i_1) {
    return ((int) i_0 < (int) i_1) ? TRUE : FALSE;
}

int is_digit(char c) {
    if (c < '0' || c > '9') {
        return FALSE;
    }
    return TRUE;
}

int get_data(FILE *input, char *init_permutation, int *N) {
    char c;
    for (int i = 0; i < LEN_SEQUENCE + 1; i++) {
        if (fread(&c, 1, 1, input) == 0) {
            return BAD_INPUT;
        };
        if (c == '\n') {
            init_permutation[i] = '\0';
            break;
        } else if (i == LEN_SEQUENCE && c != '\n') {
            return BAD_INPUT;
        }
        init_permutation[i] = c;
    }

    if (fscanf(input, "%d", N) == 0) {
        return BAD_INPUT;
    };
    return 0;
}

int check_permutation(char *permutation) {
    for (int i = 0; i < (int) strlen(permutation); i++) {
        if (!is_digit(permutation[i])) {
            return BAD_INPUT;
        }
        for (int j = 0; j < i; j++) {
            if (permutation[j] == permutation[i]) {
                return BAD_INPUT;
            }
        }
    }
    return 0;
}

void swap_items(char *sequence, int i_0, int i_1) {
    char item = sequence[i_0];
    sequence[i_0] = sequence[i_1];
    sequence[i_1] = item;
}

int get_next_permutation(char *current_permutation, int cur_permutation_len) {
    int index_1 = cur_permutation_len, index_2 = cur_permutation_len;
    do {
        if (index_1 == 0) {
            return BAD_INPUT;
        }
        index_1--;
    } while (compare(current_permutation[index_1], current_permutation[index_1 + 1]) == FALSE);

    while (index_2 > index_1) {
        if (compare(current_permutation[index_1], current_permutation[index_2])) {
            break;
        }
        index_2--;
    }

    swap_items(current_permutation, index_1, index_2);

    for (int i = index_1 + 1; i < cur_permutation_len; i++) {
        swap_items(current_permutation, i, cur_permutation_len);
        cur_permutation_len--;
    }
    return 0;
}

int main() {
    char init_permutation[LEN_SEQUENCE + 1];
    int N;
    FILE *input = fopen("in.txt", "r");

    if (get_data(input, init_permutation, &N) == BAD_INPUT) {
        printf("bad input");
        return 0;
    };

    if (check_permutation(init_permutation) == BAD_INPUT) {
        printf("bad input");
        return 0;
    }

    for (int i = 0; i < N; i++) {
        if (get_next_permutation(init_permutation, ((int) strlen(init_permutation)) - 1) == BAD_INPUT) {
            return 0;
        }
        printf("%s\n", init_permutation);
    }
    fclose(input);
    return 0;
}
