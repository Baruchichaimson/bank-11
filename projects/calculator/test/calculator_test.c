#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"

int main(void)
{
    double res;
    /* ========== TEST 1 =======================*/

    printf("test 1: (-3) + (-4) * 20 - (5 + 1) / (5) ^ 2 * (7 - 4 / 2)\n\n");

    if (Calculator("(-3) + (-4) * 20 - (5 + 1) / (5) ^ 2 * (7 - 4 / 2)", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("(-3) + (-4) * 20 - (5 + 1) / (5) ^ 2 * (7 - 4 / 2)", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("(-3) + (-4) * 20 - (5 + 1) / (5) ^ 2 * (7 - 4 / 2)", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 2 =======================*/

    printf("test 2: 3.5 + 4 * 20\n\n");

    if (Calculator("3.5 + 4 * 20 ", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("3.5 + 4 * 20 ", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("3.5 + 4 * 20 ", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 3 =======================*/

    printf("test 3: 3.5 - 4 - 20\n\n");

    if (Calculator("3.5 - 4 - 20 ", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("3.5 - 4 - 20 ", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("3.5 - 4 - 20 ", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 4 =======================*/

    printf("test 4: 3 ^ 4 ^ 2\n\n");

    if (Calculator("3 ^ 4 ^ 2 ", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("3 ^ 4 ^ 2 ", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("3 ^ 4 ^ 2 ", &res) == MATH_ERROR)
    {
        printf("Error in expression\n");
    }

    /* ========== TEST 5 =======================*/

    printf("test 5: 5--7 \n\n");

    if (Calculator("5--7 ", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("5--7 ", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("5--7 ", &res) == MATH_ERROR)
    {
        printf("Error in expression\n");
    }

    /* ========== TEST 6 =======================*/

    printf("test 6: (-5) + 7 \n\n");

    if (Calculator("(-5) + 7 ", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("(-5) + 7 ", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("(-5) + 7 ", &res) == MATH_ERROR)
    {
        printf("Error in expression\n");
    }

    /* ========== TEST 7 =======================*/

    printf("test 7: 4 / 0 + 1 \n\n");

    if (Calculator("4 / 0 + 1", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("4 / 0 + 1", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("4 / 0 + 1", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 8 =======================*/

    printf("test 8: 3 + 5 * 2 \n\n");

    if (Calculator("3 + 5 * 2", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("3 + 5 * 2", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("3 + 5 * 2", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 9 =======================*/

    printf("test 9: (3) + [5] * {2} \n\n");

    if (Calculator("(3) + [5] * {2}", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("(3) + [5] * {2}", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("(3) + [5] * {2}", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 10 =======================*/

    printf("test 10: (3 + 5] * {2} \n\n");

    if (Calculator("(3 + 5] * {2}", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("(3 + 5] * {2}", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("(3 + 5] * {2}", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 11 =======================*/

    printf("test 11: [5 + 2] \n\n");

    if (Calculator("[5 + 2]", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("[5 + 2]", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("[5 + 2]", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 12 =======================*/

    printf("test 12: 2 +-3 \n\n");

    if (Calculator("2 +-3", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("2 +-3", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("2 +-3", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 13 =======================*/

    printf("test 13: +-3 \n\n");

    if (Calculator("+-3", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("+-3", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("+-3", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 14 =======================*/

    printf("test 14: 0 ^ -1 \n\n");

    if (Calculator("0 ^ -1", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("0 ^ -1", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("0 ^ -1", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 15 =======================*/

    printf("test 15: 0 ^ 0 \n\n");

    if (Calculator(" 0 ^ 0", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator(" 0 ^ 0", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator(" 0 ^ 0", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 16 =======================*/

    printf("test 16: (3.5 + 4) - 20 \n\n");

    if (Calculator("(3.5 + 4) - 20 ", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("(3.5 + 4) - 20 ", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("(3.5 + 4) - 20 ", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 17 =======================*/

    printf("test 17: 7 - 4 / 2 \n\n");

    if (Calculator("7 - 4 / 2", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("7 - 4 / 2", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("7 - 4 / 2", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 18 =======================*/

    printf("test 18: 7 c- 4 / 2 \n\n");

    if (Calculator("7 c- 4 / 2", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("7 c- 4 / 2", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("7 c- 4 / 2", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    /* ========== TEST 19 =======================*/

    printf("test 19: [(7 - 4) / 2] \n\n");

    if (Calculator("[(7 - 4) / 2]", &res) == SUCCESS)
    {
        printf("Result: %f\n", res);
    }
    else if(Calculator("[(7 - 4) / 2]", &res) == SYNTAX_ERROR)
    {
        printf("Syntax error in expression\n");
    }
    else if(Calculator("[({7} - 4) / 2]", &res) == MATH_ERROR)
    {
        printf("Arithmetic error in expression\n");
    }

    return 0;
}