/* stack-based caclulator program utilizing infix-to-postfix notation conversion */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_LIMIT 1000
#define INPUT_MAX_LEN (INPUT_LIMIT + 1 + 1)  // for "\n\0" because of fgets and strlen use

typedef struct s_stack {
    int items[INPUT_LIMIT];
    int size;
} s_stack;

bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_unary_minus(char *char_p) {
    return *char_p == '-' && is_digit(*++char_p);
}

bool symbol_validate(char c) {
    return is_operator(c) || is_digit(c) || c == '\n';
}

void syntax_error(void) {
    puts("syntax error");
    exit(EXIT_SUCCESS);  // but actually FAILURE
}

void error_terminate(char *message) {
    puts(message);
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

void stack_init(s_stack *st) {
    st->size = 0;
}

bool stack_is_full(s_stack *st) {
    return st->size == INPUT_LIMIT;
}

bool stack_is_empty(s_stack *st) {
    return st->size == 0;
}

int stack_pop(s_stack *st) {
    if (st->size == 0) {
        syntax_error();
    }
    st->size--;
    return st->items[st->size];
}

void stack_push(s_stack *st, const int val) {
    if (stack_is_full(st)) {
        error_terminate("stack overflow");
    }
    st->items[st->size++] = val;
}

int stack_peek(s_stack *st) {
    if (st->size == 0) {
        error_terminate("stack is empty");
    }
    return st->items[st->size - 1];
}

void infix_to_postfix(char *infix_string, char *postfix_string) {
    s_stack st_operators;
    stack_init(&st_operators);

    int postfix_str_ndx = 0;
    char *char_p = infix_string;
    while (*char_p != '\0') {
        if (is_digit(*char_p)) {
            postfix_string[postfix_str_ndx++] = *char_p;
            if (!is_digit(*(char_p + 1))) {
                postfix_string[postfix_str_ndx++] = ' ';
            }
        } else if (*char_p == '(') {
            stack_push(&st_operators, (int)*char_p);
        } else if (*char_p == ')') {
            if (char_p == infix_string || *(char_p - 1) == '(') {
                syntax_error();
            }
            char tmp;
            while ((tmp = stack_pop(&st_operators)) != '(') {
                postfix_string[postfix_str_ndx++] = tmp;
                postfix_string[postfix_str_ndx++] = ' ';
            }
        } else {
            while (!stack_is_empty(&st_operators) &&
                   get_op_precedence(stack_peek(&st_operators)) >= get_op_precedence(*char_p)) {
                postfix_string[postfix_str_ndx++] = stack_pop(&st_operators);
                postfix_string[postfix_str_ndx++] = ' ';
            }
            stack_push(&st_operators, (int)*char_p);
        }
        ++char_p;
    }

    while (!stack_is_empty(&st_operators)) {
        postfix_string[postfix_str_ndx++] = (char)stack_pop(&st_operators);
        postfix_string[postfix_str_ndx++] = ' ';
    }
    postfix_string[postfix_str_ndx] = '\0';
}

int apply_op(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                error_terminate("division by zero");
            }
            return a / b;
        default: return 0;
    }
}

int eval_expr(char *postfix_string) {
    s_stack st_operands;
    stack_init(&st_operands);

    char *char_p = postfix_string;
    while (*char_p != '\0') {
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
        }
        ++char_p;
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
        if (!symbol_validate(string[i])) {
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
