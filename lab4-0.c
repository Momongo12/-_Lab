#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define SYNTAX_ERROR 404
#define STACK_MAX_SIZE 1024
#define STACK_OVERFLOW 100
#define FALSE 0
#define TRUE 1
#define EXPRESSION_SIZE 1002

int calc(char *exp);
int check_syntax(char *exp, int exp_len);
int is_int(char symbol);
int is_operator(char symbol);
int get_int(char *exp, int start_index, int end_index);
int get_priority(char symbol);
int do_math(char operator, int object_1, int object_2);

typedef struct {
    char data[STACK_MAX_SIZE];
    int size;

} Stack;

typedef struct {
    int data[STACK_MAX_SIZE*2];
    int size;
} Stack_int;

void push(Stack *stack, const char value) {
    if (stack->size >= STACK_MAX_SIZE) {
        exit(STACK_OVERFLOW);
    }
    stack->data[stack->size] = value;
    stack->size++;
}

void push_int(Stack_int *stack, const int value){
    if (stack->size >= STACK_MAX_SIZE){
        exit(STACK_OVERFLOW);
    }
    stack->data[stack->size] = value;
    stack->size++;
}

int pop_int(Stack_int *stack){
    if (stack->size == 0){
        return 0;
    }
    stack->size--;
    return stack->data[stack->size];
}

char pop(Stack *stack) {
    if (stack->size == 0) {
        return '0';
    }
    stack->size--;
    return stack->data[stack->size];
}

char peek(Stack *stack) {
    if (stack->size == 0) {
        return '0';
    }
    return stack->data[stack->size - 1];
}

int main() {
    char exp[EXPRESSION_SIZE];
    if(fgets(exp, EXPRESSION_SIZE, stdin)){};
    exp[(int) strlen(exp)-1] = '\0';
    calc(exp);
}

int calc(char *exp) {
    int exp_len = (int) strlen(exp);
    if (check_syntax(exp, exp_len) == SYNTAX_ERROR || exp_len == 0) {
        printf("syntax error");
        return 0;
    }

    int index_of_last_number = -1;
    Stack operators;
    Stack_int nums;
    nums.size = 0;
    operators.size = 0;

    for (int i = 0; i <= exp_len; i++) {
        if (is_int(exp[i]) && i > index_of_last_number) {
            for (int j = i + 1; j <= exp_len; j++) {
                if (!is_int(exp[j])) {
                    int num = get_int(exp, i, j);
                    push_int(&nums, num);
                    index_of_last_number = j - 1;
                    break;
                }
            }
        } else if(is_operator(exp[i])){
            if (get_priority(peek(&operators)) >= get_priority(exp[i])){
                int num1 = pop_int(&nums);
                int num2 = pop_int(&nums);
                push_int(&nums, do_math(pop(&operators), num2, num1));
                push(&operators, exp[i]);
            }else {
                push(&operators, exp[i]);
            }
        } else if (exp[i] == '('){
            push(&operators, exp[i]);
        } else if (exp[i] == ')'){
            while (peek(&operators) != '('){
                int num1 = pop_int(&nums);
                int num2 = pop_int(&nums);
                push_int(&nums, do_math(pop(&operators), num2, num1));
            }
            pop(&operators);
        } else if (exp[i] == '\0'){
            char current_operator = pop(&operators);
            while (current_operator != '0'){
                int num1 = pop_int(&nums);
                int num2 = pop_int(&nums);
                push_int(&nums, do_math(current_operator, num2, num1));
                current_operator = pop(&operators);
            }
            printf("%d", pop_int(&nums));
        }
    }

    return 0;
}

int check_syntax(char *exp, int exp_len) {
    int index_end_of_last_number = -1;
    int open_brackets_count = 0;
    int close_brackets_count = 0;
    int number_count = 0, operator_count = 0;
    for (int i = 0; i < exp_len; i++) {
        if (is_int(exp[i])) {
            if (i > index_end_of_last_number) {
                number_count += 1;
                for (int j = i; j < exp_len && is_int(exp[j]); j++) {
                    index_end_of_last_number = j;
                }
            }
        } else if (is_operator(exp[i])) {
            if (i > 0 && (is_operator(exp[i - 1]) || exp[i - 1] == '(' || exp[i + 1] == ')')) {
                return SYNTAX_ERROR;
            }
            operator_count += 1;
        } else if (exp[i] == ')') {
            close_brackets_count += 1;
            if ((close_brackets_count > open_brackets_count) || (i > 0 && exp[i - 1] == '(')) {
                return SYNTAX_ERROR;
            }
        } else if (exp[i] == '(') {
            open_brackets_count += 1;
        } else if ((is_int(exp[i]) == FALSE) && (is_operator(exp[i]) == FALSE)) {
            return SYNTAX_ERROR;
        }
    }
    if ((open_brackets_count != close_brackets_count) || (operator_count >= number_count)) {
        return SYNTAX_ERROR;
    }
    return 0;
}

int is_int(char symbol) {
    if (symbol >= '0' && symbol <= '9') {
        return TRUE;
    }
    return FALSE;
}

int is_operator(char symbol) {
    if (symbol == '-' || symbol == '*' || symbol == '+' || symbol == '/') {
        return TRUE;
    }
    return FALSE;
}

int get_int(char *exp, int start_index, int end_index) {
    int number = 0, degree = end_index - start_index - 1;
    for (int i = start_index; i < end_index; i++) {
        int x = (int) (exp[i] - '0');
        number += (int) (x * pow(10, degree--));
    }
    return number;
}

int get_priority(char symbol) {
    switch (symbol) {
        case '-':
            return 1;
        case '+':
            return 1;
        case '/':
            return 2;
        case '*':
            return 2;
        default:
            return 0;
    }
}

int do_math(char operator, int obj_1, int obj_2){
    switch (operator) {
        case '-':
            return obj_1 - obj_2;
        case '+':
            return obj_1 + obj_2;
        case '*':
            return obj_1 * obj_2;
        case '/':
            if (obj_2 == 0){
                printf("division by zero");
                exit(0);
            }
            return obj_1 / obj_2;
    }
    return 0;
}
