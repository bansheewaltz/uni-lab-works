#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_LIMIT 1000
#define INPUT_MAX_LEN (INPUT_LIMIT + 1)  // because of '\n'

typedef struct stack {
    int items[INPUT_LIMIT];
    int size;
} stack;

bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_unary_minus(char *char_p) {
    return *char_p == '-' && is_digit(*++char_p);
}

bool symbol_validation(char c) {
    return is_operator(c) || is_digit(c) || c == '\n';
}

void syntax_error(void) {
    puts("syntax error");
    exit(EXIT_SUCCESS);  // but actually FAILURE
}

int get_op_precedence(char op) {
    switch (op) {
        case '(': return 1;
        case ')': return 1;
        case '-': return 2;
        case '+': return 2;
        case '*': return 3;
        case '/': return 3;
    }

    return EXIT_SUCCESS;  // but actually FAILURE
}

void stack_init(struct stack *st) {
    st->size = 0;
}

int stack_pop(stack *st) {
    if (st->size == 0) {
        syntax_error();
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
        syntax_error();
    }
    return st->items[st->size - 1];
}

void infix_to_postfix(char *input_string, char *postfix_string) {
    stack st_operators;
    stack_init(&st_operators);
    int i = 0;
    int RPN_str_len = 0;

    while (input_string[i] != '\0') {
        if (is_digit(input_string[i])) {
            while (!is_operator(input_string[i])) {
                postfix_string[RPN_str_len] = input_string[i];
                RPN_str_len++;
                i++;
                if (input_string[i] == '\0')
                    break;
            }
            postfix_string[RPN_str_len] = ' ';
            RPN_str_len++;
        }

        if (is_operator(input_string[i])) {
            if (input_string[i] == '(') {
                stack_push(&st_operators, (int)input_string[i]);
                i++;
            } else if (input_string[i] == ')') {
                if (i == 0 || input_string[i - 1] == '(') {
                    syntax_error();
                }
                char tmp = (char)stack_pop(&st_operators);
                while (tmp != '(') {
                    postfix_string[RPN_str_len] = tmp;
                    postfix_string[RPN_str_len + 1] = ' ';
                    RPN_str_len += 2;
                    tmp = (char)stack_pop(&st_operators);
                }
                i++;
            } else {
                if (st_operators.size > 0 &&
                    get_op_precedence((char)stack_peek(&st_operators)) >=
                        get_op_precedence(input_string[i])) {
                    postfix_string[RPN_str_len] =
                        (char)stack_pop(&st_operators);
                    postfix_string[RPN_str_len + 1] = ' ';
                    RPN_str_len += 2;
                }
                stack_push(&st_operators, (int)input_string[i]);
                i++;
            }
        }
    }

    while (st_operators.size > 0) {
        postfix_string[RPN_str_len] = (char)stack_pop(&st_operators);
        postfix_string[RPN_str_len + 1] = ' ';
        RPN_str_len += 2;
    }
    postfix_string[RPN_str_len] = '\0';
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

int eval_expr(char *postfix_string) {
    stack st_operands;
    stack_init(&st_operands);

    char *char_p = postfix_string;
    while (*char_p != '\0') {
        if (*char_p == ' ') {
            ++char_p;
            continue;
        }
        if (is_digit(*char_p) || is_unary_minus(char_p)) {
            int number = strtol(char_p, &char_p, 10);

            stack_push(&st_operands, number);
            continue;
        }
        if (is_operator(*char_p)) {
            char op = *char_p;
            int b = (int)stack_pop(&st_operands);
            int a = (int)stack_pop(&st_operands);
            int res = apply_op(a, b, op);

            stack_push(&st_operands, res);
            ++char_p;
        }
    }

    return stack_pop(&st_operands);
}

void input_read(char *string) {
    if (fgets(string, INPUT_MAX_LEN, stdin) == NULL) {
        syntax_error();
    }
    string[strcspn(string, "\n")] = '\0';
}

void input_validate(char string[]) {
    for (int i = 0; string[i] != '\0'; ++i) {
        if (!symbol_validation(string[i])) {
            syntax_error();
        }
    }
}

int main(void) {
    char input_string[INPUT_MAX_LEN] = "";

    input_read(input_string);
    input_validate(input_string);
    char postfix_string[INPUT_LIMIT] = "";

    infix_to_postfix(input_string, postfix_string);
    printf("%d", eval_expr(postfix_string));

    return EXIT_SUCCESS;
}
