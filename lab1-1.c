#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE_PATTERN 16
#define SIZE_BUFFER 1024 * 32

int rabin_carp(unsigned char *pattern, unsigned char *text, long *global_shift, int pattern_hash, int degree_of_last_elem, int pattern_len) {
    int text_hash = 0;
    int text_len = (int) (strlen((char *) text));

    for (int i = 0; i < pattern_len; i++) {
        text_hash += (((int) text[i]) % 3) * ((int) pow(3, i));
    }

    for (int i = 0; i <= (text_len - pattern_len); i++) {
        if (text_hash == pattern_hash) {
            for (int j = i; j < (i + pattern_len); j++) {
                if (text[j] != pattern[j - i]) {
                    fprintf(stdout, "%ld ", *global_shift + j + 1);
                    break;
                }
                fprintf(stdout, "%ld ", *global_shift + j + 1);
            }
        }
        if (i < (text_len - pattern_len)) {
            text_hash -= ((int) text[i] % 3);
            text_hash -= (text_hash / 3) * 2;
            text_hash += (int) ((text[i + pattern_len] % 3) * degree_of_last_elem);
        }
    }

    *global_shift += SIZE_BUFFER;
    return 0;
}

int main() {
    unsigned char pattern[SIZE_PATTERN + 2], buf[SIZE_BUFFER + 2];
    long global_shift = 0;
    unsigned char letter_of_pattern;
    int buffer_len = 0, pattern_hash = 0, pattern_len = 0, degree_of_last_elem = 0;
    FILE *input = fopen("in.txt", "r");

    for (int i = 0; i <= SIZE_PATTERN; i++) {
        if (fread(&letter_of_pattern, sizeof(char), 1, input)){
            if (letter_of_pattern == '\n'){
                pattern[i] = '\0';
                break;
            }
            pattern_len++;
            pattern[i] = letter_of_pattern;
        }
        pattern_hash += (((int) pattern[i]) % 3) * ((int) pow(3, i));
    }

    buffer_len = fread(buf, sizeof(char), SIZE_BUFFER, input);
    if (buffer_len == 0) {
        fprintf(stdout, "%d", 0);
        fclose(input);
        return 0;
    }

    pattern_len = (int) (strlen((char *) pattern));
    degree_of_last_elem = (int)pow(3, pattern_len - 1);

    fprintf(stdout, "%d ", pattern_hash);

    while (buffer_len == SIZE_BUFFER) {
        buf[SIZE_BUFFER] = '\0';
        rabin_carp(pattern, buf, &global_shift, pattern_hash, degree_of_last_elem, pattern_len);
        buffer_len = fread(buf, sizeof(char), SIZE_BUFFER, input);
    };
    buf[buffer_len] = '\0';

    rabin_carp(pattern, buf, &global_shift, pattern_hash, degree_of_last_elem, pattern_len);

    fclose(input);
    return 0;
}
