#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
    int items[10000];
    int size;
} stack;

bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool symbol_validation(char c) {
    return is_operator(c) || is_digit(c) || c == '\n';
}

void print_syntax_error_and_terminate(void) {
    puts("syntax error");
    exit(EXIT_SUCCESS);  // but actually FAILURE
}

int get_precedence(char c) {
    if (c == '(' || c == ')') {
        return 1;
    }
    if (c == '+' || c == '-') {
        return 2;
    }
    if (c == '*' || c == '/') {
        return 3;
    }
    return EXIT_SUCCESS;  // but actually FAILURE
}

void stack_init(struct stack *st) {
    st->size = 0;
}

int stack_pop(stack *st) {
    if (st->size == 0) {
        print_syntax_error_and_terminate();
    }
    st->size--;
    return st->items[st->size];
}

void stack_push(stack *st, const int val) {
    st->items[st->size] = val;
    st->size++;
}

int stack_peek(stack *st) {
    if (st->size == 0) {
        print_syntax_error_and_terminate();
    }
    return st->items[st->size - 1];
}

void convert_to_RPN(char *input_string, char *RPN_string) {
    stack st_operators;
    stack_init(&st_operators);
    int i = 0;
    int RPN_str_len = 0;
    while (input_string[i] != '\0') {
        if (is_digit(input_string[i])) {
            while (!is_operator(input_string[i])) {
                RPN_string[RPN_str_len] = input_string[i];
                RPN_str_len++;
                i++;
                if (input_string[i] == '\0')
                    break;
            }
            RPN_string[RPN_str_len] = ' ';
            RPN_str_len++;
        }

        if (is_operator(input_string[i])) {
            if (input_string[i] == '(') {
                stack_push(&st_operators, (int)input_string[i]);
                i++;
            } else if (input_string[i] == ')') {
                if (i == 0 || input_string[i - 1] == '(') {
                    print_syntax_error_and_terminate();
                }
                char tmp = (char)stack_pop(&st_operators);
                while (tmp != '(') {
                    RPN_string[RPN_str_len] = tmp;
                    RPN_string[RPN_str_len + 1] = ' ';
                    RPN_str_len += 2;
                    tmp = (char)stack_pop(&st_operators);
                }
                i++;
            } else {
                if (st_operators.size > 0 &&
                    get_precedence((char)stack_peek(&st_operators)) >=
                        get_precedence(input_string[i])) {
                    RPN_string[RPN_str_len] = (char)stack_pop(&st_operators);
                    RPN_string[RPN_str_len + 1] = ' ';
                    RPN_str_len += 2;
                }
                stack_push(&st_operators, (int)input_string[i]);
                i++;
            }
        }
    }

    while (st_operators.size > 0) {
        RPN_string[RPN_str_len] = (char)stack_pop(&st_operators);
        RPN_string[RPN_str_len + 1] = ' ';
        RPN_str_len += 2;
    }
    RPN_string[RPN_str_len] = '\0';
}

int apply_op(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                puts("division by zero");
                exit(EXIT_SUCCESS);  // but actually FAILURE
            }
            return a / b;
    }
}

int eval_expr(char *RPN_string) {
    stack operands;
    stack_init(&operands);
    int i = 0;
    while (RPN_string[i] != '\0') {
        if (RPN_string[i] == ' ') {
            i++;
            continue;
        }
        if (is_digit(RPN_string[i])) {
            int p = 0;
            while (!is_operator(RPN_string[i]) && RPN_string[i] != ' ') {
                p *= 10;
                p += (int)(RPN_string[i] - '0');
                i++;
                if (RPN_string[i] == '\0')
                    break;
            }
            stack_push(&operands, p);
            i++;
        } else if (is_operator(RPN_string[i])) {
            char op = RPN_string[i];
            int b = (int)stack_pop(&operands);
            int a = (int)stack_pop(&operands);
            int res = apply_op(a, b, op);

            stack_push(&operands, res);
            i++;
        }
    }
    return stack_pop(&operands);
}

void input_validation(char string[]) {
    for (int i = 0; string[i] != '\0'; ++i) {
        if (!symbol_validation(string[i])) {
            print_syntax_error_and_terminate();
        }
    }
}

int main(void) {
    char input_string[2500];
    if (!scanf("%1001[^\n]s", input_string)) {
        print_syntax_error_and_terminate();
    }
    input_validation(input_string);

    char RPN_string[2500];
    convert_to_RPN(input_string, RPN_string);
    printf("%d", eval_expr(RPN_string));
    return EXIT_SUCCESS;
}
