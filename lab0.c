#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define INACCURACY 1e-10
#define LEN_INIT_NUMBERS 13
#define WHOLE 1
#define OTHER 0
#define TRUE 1
#define FALSE 0

int is_letter(char c) {
    c = toupper(c);
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return FALSE;
}

char number_to_char(int num){
    if (num > 9) {
        return (char)(num + 'a' - 10);
    } else {
        return (char)(num + '0');
    }
}

int char_to_number(char c, int b) {
    if (is_letter(c) != 0) {
        b = is_letter(c);
    } else {
        b = c - '0';
    }
    return b;
}

int check_digits_of_number(char *init_number, int b1, int b2, int counter_points, int len_whole_part, int len_fractional_part) {
    int a;
    if ((b1 < 2 || b1 > 16) || (b2 < 2 || b2 > 16)) {
        return 1;
    }
    for (int i = 0; i < LEN_INIT_NUMBERS; i++) { // Проверка на b1-ричность цифр числа
        if (init_number[i] == '\0'){
            break;
        } else if (init_number[i] == '.') {
            counter_points += 1;
        } else if ((is_letter(init_number[i]) != 0) && (is_letter(init_number[i]) >= b1)) {
            return 1;
        } else if (is_letter(init_number[i]) == 0) {
            a = init_number[i] - '0';
            if ((a >= b1) || (a > 9) || (a < 0)){
                return 1;
            }
        }
    }
    if (((len_whole_part == 0 || len_fractional_part == 0) && (counter_points >= 1)) || (counter_points > 1)) {
        return 1;
    }
    return 0;
}

int conversion_from_decimal_to_b2(char *remainder_of_decimal_division, long whole_part, double fractional_part, int b2) {
    int count = 0, n;
    char c;
    if ((whole_part == 0) && (fabs(fractional_part - 0.0) < INACCURACY)) {
        printf("0");
        return 0;
    } else if(whole_part == 0) {
        remainder_of_decimal_division[0] = '0';
        remainder_of_decimal_division[1] = '.';
    } else {
        while (whole_part > 0) { //помещаем остатки в массив
            n = whole_part % b2;
            remainder_of_decimal_division[count] = number_to_char(n);
            whole_part /= b2;
            count += 1;
        }
        count -= 1;
        for (int i = 0; i <= count/2; i++) { //переворот целой части в массиве
            c = remainder_of_decimal_division[i];
            remainder_of_decimal_division[i] = remainder_of_decimal_division[count-i];
            remainder_of_decimal_division[count-i] = c;
        }
        if (fabs(fractional_part - 0.0) < INACCURACY) { //выводим строку, если дробная часть равна 0
            remainder_of_decimal_division[count+1] = '\0';
            printf("%s", remainder_of_decimal_division);
            return 0;
        }
        remainder_of_decimal_division[count+1] = '.';
    }
    for (int i = 0; i < 12; i++) { //перевод дробной части числа и запись остатков в массиве
        fractional_part *= b2;
        double a = floor(fractional_part);
        remainder_of_decimal_division[count+2+i] = number_to_char(a);
        if (fabs(fractional_part - a) < INACCURACY) {
            remainder_of_decimal_division[count+3+i] = '\0';
            break;
        }
        fractional_part -= a;
    }
    
    printf("%s", remainder_of_decimal_division);
    return 0;
}

void conversion_to_decimal(char *init_number, int len_whole_part, int len_fractional_part, int b1, long *whole_part, double *fractional_part) {
    int counter_digits_fract_part = 1, flag, b = 0;
    flag = WHOLE;
    for (int j = 0; j < LEN_INIT_NUMBERS; j++) {
        if (init_number[j] == '\0'){
            break;
        } else if (init_number[j] == '.') {
            flag = OTHER;
        } else {
            if (flag == WHOLE) {
                b = char_to_number(init_number[j], b);
                *whole_part += pow(b1, (len_whole_part-1))*b;
                len_whole_part -= 1;
            } else if (flag == OTHER) {
                b = char_to_number(init_number[j], b);
                *fractional_part += (1.0/pow(b1, counter_digits_fract_part))*b;
                if (counter_digits_fract_part < len_fractional_part) {
                    counter_digits_fract_part += 1;
                }
            }
        }
    }
}

void digit_counter(char *init_number, int *len_whole_part, int *len_fractional_part, int flag) {
    for (int j = 0; j < LEN_INIT_NUMBERS; j++){
        if (init_number[j] == '\0') {
            break;
        } else if (init_number[j] == '.') {
            flag = OTHER;
        }
        if (flag == WHOLE) {
            *len_whole_part += 1;
        } else if (flag == OTHER) {
            *len_fractional_part += 1;
        }
    }
}

int main(void) {
    int b1, b2, len_whole_part = 0, len_fractional_part = -1, counter_points = 0;
    char init_number[100], remainder_of_decimal_division[100];
    long whole_part = 0;
    double fractional_part = 0.0;
    
    if (scanf("%2d %2d", &b1, &b2) != 2){
        printf("bad input");
        return 0;
    }
    if (scanf("%13s", init_number) > 13){
        printf("bad input");
        return 0;
    }
        
    digit_counter(init_number, &len_whole_part, &len_fractional_part, WHOLE);

    if (check_digits_of_number(init_number, b1, b2, counter_points, len_whole_part, len_fractional_part) != 0) {
        printf("bad input");
        return 0;
    }
    
    conversion_to_decimal(init_number, len_whole_part, len_fractional_part, b1, &whole_part, &fractional_part);
    conversion_from_decimal_to_b2(remainder_of_decimal_division, whole_part, fractional_part, b2);
    
    return 0;
}
