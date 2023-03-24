#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHIFT_CHARS 256
#define SIZE_PATTERN 16
#define SIZE_BUFFER 4096

int max(int a, int b) {
    return (a > b) ? a : b;
}

int stop_char(unsigned char *pattern, int size, unsigned char *chars_shift) {
    int counter_match_with_last_char = 0;
    for (int i = 0; i < SHIFT_CHARS; i++) {
        chars_shift[i] = size;
    }
    for (int i = 0; i < size - 1; i++) {
        if (pattern[i] == pattern[size - 1]) {
            counter_match_with_last_char += 1;
        }
        chars_shift[(int) pattern[i]] = size - i - 1;
    }
    if (counter_match_with_last_char == 0) {
        chars_shift[(int) pattern[size - 1]] = size;
    }
    return 0;
}

int boier_mur(unsigned char *buf, unsigned char *pattern, unsigned char *chars_shift, long *global_shift) {
    FILE *out = fopen("out.txt", "a+");
    int buf_len = (int) strlen((char *) buf);
    int pattern_len = (int) strlen((char *) pattern);
    long shift = 0;
    while (shift <= (buf_len - pattern_len)) {
        int counter_of_mismatched_chars = pattern_len - 1;
        while (counter_of_mismatched_chars >= 0 && pattern[counter_of_mismatched_chars] == buf[shift + counter_of_mismatched_chars]) {
            fprintf(out, "%ld ", *global_shift + shift + counter_of_mismatched_chars + 1);
            counter_of_mismatched_chars--;
        }
        if (counter_of_mismatched_chars < 0) {
            shift += (shift + pattern_len < buf_len) ? pattern_len : 1;
        } else {
            fprintf(out, "%ld ", *global_shift + shift + counter_of_mismatched_chars + 1);
            shift += max(1, chars_shift[(int) buf[shift + pattern_len - 1]]);
        }
    }
    *global_shift += shift;
    fclose(out);
    return 0;
}

int main() {
    unsigned char pattern[SIZE_PATTERN + 2], buf[SIZE_BUFFER + 2], chars_shift[SHIFT_CHARS];
    long global_shift = 0;
    int buffer_len = 0;
    unsigned char letter_of_pattern;
    FILE *input = fopen("in.txt", "r");

    for (int i = 0; i <= SIZE_PATTERN; i++) {
        if (fread(&letter_of_pattern, sizeof(char), 1, input)){
            if (letter_of_pattern == '\n'){
                pattern[i] = '\0';
                break;
            }
            pattern[i] = letter_of_pattern;
        }
    }

    stop_char(pattern, (int) strlen((char *) pattern), chars_shift);
    buffer_len = fread(buf, sizeof(char), SIZE_BUFFER, input);
    if (buffer_len == 0){
        fclose(input);
        return 0;
    }
    while (buffer_len == SIZE_BUFFER) {
        buf[SIZE_BUFFER] = '\0';
        boier_mur(buf, pattern, chars_shift, &global_shift);
        memset(buf, 0, sizeof(buf));
        buffer_len = fread(buf, sizeof(char), SIZE_BUFFER, input);
    };
    buf[buffer_len] = '\0';

    boier_mur(buf, pattern, chars_shift, &global_shift);

    fclose(input);
    return 0;
}
