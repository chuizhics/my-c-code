#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

double calculate(char *expr);

// 解析数字
double parse_number(char *expr, int *pos) {
    double num = 0.0;
    int decimal = 0;
    while (isdigit(expr[*pos]) || expr[*pos] == '.') {
        if (expr[*pos] == '.') {
            decimal = 1;
        } else if (decimal) {
            num += (expr[*pos] - '0') / (decimal *= 10.0);
        } else {
            num = num * 10.0 + (expr[*pos] - '0');
        }
        (*pos)++;
    }
    return num;
}

// 解析表达式
double parse_expr(char *expr, int *pos) {
    double lhs = parse_number(expr, pos);
    while (expr[*pos] == '*' || expr[*pos] == '/') {
        char op = expr[(*pos)++];
        double rhs = parse_number(expr, pos);
        lhs = (op == '*') ? lhs * rhs : lhs / rhs;
    }
    return lhs;
}

// 解析带括号的表达式
double parse_parentheses_expr(char *expr, int *pos) {
    double lhs = parse_expr(expr, pos);
    while (expr[*pos] == '+' || expr[*pos] == '-') {
        char op = expr[(*pos)++];
        double rhs = parse_expr(expr, pos);
        lhs = (op == '+') ? lhs + rhs : lhs - rhs;
    }
    return lhs;
}

// 计算表达式
double calculate(char *expr) {
    int pos = 0;
    return parse_parentheses_expr(expr, &pos);
}

int main() {
    char expr[] = "2 * ((7+51*2)/3 -5)";
    printf("%s = %g\n", expr, calculate(expr));
    return 0;
}
