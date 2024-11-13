#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "stack.h"

#define EXPRESSION_MAX_LENGTH 1024

void destroy(STACK *s1, STACK *s2)
{
    stack_destroy(s1);
    stack_destroy(s2);
}

static int getpri(char ch)
{
    switch (ch)
    {
    case '(':
        return 0;
    case '+':
    case '-':
        return 10;
    case '*':
    case '/':
        return 100;
    default:
        return -1;
    }
}

double compute(STACK *stack_num, char op)
{
    double num1, num2;
    if (stack_pop(stack_num, &num2) != 0)
    {
        printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
        exit(1);
    }
    if (stack_pop(stack_num, &num1) != 0)
    {
        printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
        exit(1);
    }
    switch (op)
    {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        if (num2 == 0)
        {
            printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
            return __LONG_MAX__;
        }
        return num1 / num2;
    default:
        printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
        return __LONG_MAX__;
    }
}

bool isoperator(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

bool isnumber(char ch)
{
    return ch >= '0' && ch <='9';
}

int main()
{
    int i = 0, flag = 0, xiaoshustart = 0, t = 0;
    char ch, tch;

    double num = 0.0;
    double tmp = 0.0;
    char str[EXPRESSION_MAX_LENGTH];
    STACK *stack_op = stack_create(sizeof(char));
    if (stack_op == NULL)
    {
        printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
        exit(1);
    }
    STACK *stack_num = stack_create(sizeof(double));
    if (stack_num == NULL)
    {
        printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
        exit(1);
    }

    printf("Please enter for str [Q/q for quit]:\n");
    while (1)
    {
        xiaoshustart = 0;
        flag = 0;
        tmp = 0.0;
        i = 0;
        // scanf("%[^\n]", str) 将读取输入字符串中的所有字符，直到遇到换行符为止。
        // scanf("%[^\n]", str);
        // getchar();

        // fgets(str, EXPRESSION_MAX_LENGTH, stdin); // add '\0'

        while ((t = getchar()) != '\n' && t != EOF)
        {
            if(i == 0 && t == 'Q' || t == 'q')
            {
                break;
            }
            if(t == '.' || isnumber(t) || isoperator(t) || t == '(' || t==')')
                str[i++] = t;
        }

        t = 0;

        str[i] = '\0';
        if(i == 0)
        {
            break;
        }
        printf("Input:%s\n", str);

        for (i = 0; str[i] != '\0'; i++)
        {
            ch = str[i];
            if (isnumber(ch))
            {
                flag = 1;
                if (xiaoshustart)
                {
                    tmp = tmp + (ch - '0') * pow(10, 0 - xiaoshustart);
                    xiaoshustart++;
                }
                else
                    tmp = tmp * 10 + (ch - '0');
            }
            else if (ch == '.')
            {
                if (xiaoshustart)
                {
                    printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
                    exit(1);
                }
                flag = 1;
                xiaoshustart = 1;
            }
            else
            {
                if (flag == 1)
                {
                    if (stack_push(stack_num, &tmp) != 0)
                    {
                        printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
                        exit(1);
                    }
                }
                if (ch == '(')
                {
                    tch = '*';
                    if (flag && stack_push(stack_op, &tch) != 0)
                    {
                        printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
                        exit(1);
                    }
                    if (stack_push(stack_op, &ch) != 0)
                    {
                        printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
                        exit(1);
                    }
                }
                else if (ch == ')')
                {
                    if (stack_isempty(stack_op))
                    {
                        printf("[%s:%d]: error () mismatch!!\n", __FUNCTION__, __LINE__);
                        //exit(1);
                    }
                    while (!stack_isempty(stack_op))
                    {
                        stack_pop(stack_op, &tch);
                        if (tch == '(')
                        {
                            t = 1;
                            break;
                        }

                        num = compute(stack_num, tch);
                        if (num == __LONG_MAX__)
                        {
                            printf("[%s:%d]: The result is likely to be wrong !\n", __FUNCTION__, __LINE__);
                        }
                        stack_push(stack_num, &num);
                    }
                    if( t != 1)
                    {
                        printf("[%s:%d]: error () mismatch!!\n", __FUNCTION__, __LINE__);
                    }
                    t = 0;
                }
                else if (isoperator(ch))
                {
                    while (!stack_isempty(stack_op))
                    {
                        stack_top(stack_op, &tch) != 0;

                        if (getpri(ch) > getpri(tch))
                        {
                            if (stack_push(stack_op, &ch) != 0)
                            {
                                printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
                                exit(1);
                            }
                            break;
                        }
                        else
                        {
                            if (tch == '(')
                            {
                                break;
                            }
                            stack_pop(stack_op, &tch);
                            num = compute(stack_num, tch);
                            if (num == __LONG_MAX__)
                            {
                                printf("[%s:%d]: The result is likely to be wrong !\n", __FUNCTION__, __LINE__);
                            }
                            stack_push(stack_num, &num);
                        }
                    }
                    if (stack_isempty(stack_op))
                    {
                        stack_push(stack_op, &ch);
                    }
                }
                else
                {
                    // do nothing
                }
                flag = 0;
                tmp = 0;
                xiaoshustart = 0;
            }
        }

        if (flag == 1)
        {
            if (stack_push(stack_num, &tmp) != 0)
            {
                printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
                exit(1);
            }
        }

        while (!stack_isempty(stack_op))
        {

            stack_pop(stack_op, &tch);
            num = compute(stack_num, tch);
            if (num == __LONG_MAX__)
            {
                printf("[%s:%d]: The result is likely to be wrong !\n", __FUNCTION__, __LINE__);
            }
            stack_push(stack_num, &num);
        }
        stack_pop(stack_num, &tmp);
        if (!stack_isempty(stack_num))
        {
            printf("[%s:%d]: error!\n", __FUNCTION__, __LINE__);
            exit(1);
        }
        printf("The Result is: %.2lf\n", tmp);
        printf("Please enter for str [Q/q for quit]:\n");
    }

    destroy(stack_num, stack_op);

    exit(0);
}