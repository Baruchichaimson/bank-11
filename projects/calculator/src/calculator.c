/************************************
 * Exercise: calculator (FSM)
 * Date: 15/08/25
 * Developer: Baruch Haimson
 * Reviewer: Tal
 * Status: In Progress
 ************************************/

#include <stdlib.h> /* malloc, free */
#include <string.h> /* strlen */
#include <assert.h> /* assert */
#include <math.h> /* pow */

#include "calculator.h" /* calculator API */
#include "stack.h" /* stack API */

/* ---- Calculator Data ---- */
typedef struct calc_data
{
    const char* p;
    stack_t* nums;
    stack_t* ops;
    int sign; 
    calculator_status_e status;
} calc_data_t;

/* ---- Operators LUT ---- */
typedef struct operator_lut
{
    int precedence;
    double (*operate)(double, double);
} operator_lut_t;

/* ---- handlers ---- */
typedef calculator_status_e (*event_handler_t)(calc_data_t* calc);

static calculator_status_e HandleNumber(calc_data_t* calc);
static calculator_status_e HandleOperator(calc_data_t* calc);
static calculator_status_e HandleRparen(calc_data_t* calc);
static calculator_status_e HandleLparen(calc_data_t* calc);
static calculator_status_e HandleSpace(calc_data_t* calc);
static calculator_status_e HandleEnd(calc_data_t* calc);

/* ================ Lookup Tables ============================ */

static unsigned char sign_lut[256];
static unsigned char digit_lut[256];
static operator_lut_t op_lut[256];
static event_t char_to_event[256];
static char matching_paren[256];
static unsigned char AS[256];

/* ================= Tables ==================================== */

static event_handler_t state_table[NUM_STATES][NUM_EVENTS];
static state_t next_state[NUM_STATES][NUM_EVENTS];

/* ================== Operator Functions ===================== */

static double Add(double a, double b) { return a + b; }
static double Sub(double a, double b) { return a - b; }
static double Mul(double a, double b) { return a * b; }
static double Divide(double a, double b) { return a / b; }
static double Power(double a, double b) { return pow(a, b); }

/* ================== init luts ================================ */

static void InitOpLut(void)
{
    operator_lut_t op = { 0 };

    op.precedence = 1; 
    op.operate = Add;
    op_lut['+'] = op;

    op.precedence = 1;
    op.operate = Sub;
    op_lut['-'] = op;

    op.precedence = 2;
    op.operate = Mul;
    op_lut['*'] = op;

    op.precedence = 2;
    op.operate = Divide;
    op_lut['/'] = op;

    op.precedence = 3;
    op.operate = Power; 
    op_lut['^'] = op;

}

static void InitSignAndDigitLuts(void)
{
    size_t i = 0;

    for (i = 0; i < 256; i++)
    {
        sign_lut[i] = 0;
        digit_lut[i] = 0;
    }

    sign_lut[(unsigned char)'+'] =  1;
    sign_lut[(unsigned char)'-'] = -1; 

    digit_lut[(unsigned char)'.'] = 1;

    for (i = '0'; i <= '9'; i++)
    {
        digit_lut[(unsigned char)i] = 1;
    }
}

static void InitAS(void)
{
    size_t i = 0;

    for (i = 0; i < 256; i++)
    {
        AS[i] = 0;
    }
    AS['^'] = 1;
}

static void InitMatchingParen(void)
{
    size_t i = 0;

    for (i = 0; i < 256; i++)
    {
        matching_paren[i] = '\0';
    }
    matching_paren[')'] = '(';
    matching_paren[']'] = '[';
    matching_paren['}'] = '{';
    matching_paren['('] = '(';
    matching_paren['['] = '[';
    matching_paren['{'] = '{';
}

/* ================== init tables ========================= */

static void InitTables(void)
{
    size_t i = 0;

    for (i = 0; i < 256; i++)
    {
        char_to_event[i] = EVENT_ERROR;
    }
    for (i = '0'; i <= '9'; i++) 
    {
        char_to_event[i] = EVENT_DIGIT;
    }
    char_to_event['.'] = EVENT_DIGIT;
    char_to_event['+'] = char_to_event['-'] = char_to_event['*'] = char_to_event['/'] = char_to_event['^'] = EVENT_OPERATOR;
    char_to_event['('] = EVENT_LPAREN;
    char_to_event['['] = EVENT_LPAREN;
    char_to_event['{'] = EVENT_LPAREN;
    char_to_event[')'] = EVENT_RPAREN;
    char_to_event[']'] = EVENT_RPAREN;
    char_to_event['}'] = EVENT_RPAREN;
    char_to_event[' '] = EVENT_SPACE;
    char_to_event['\t'] = EVENT_SPACE;
    char_to_event['\0'] = EVENT_END;

    /* STATE_NUMBER */
    state_table[STATE_NUMBER][EVENT_DIGIT]    = HandleNumber;  
    next_state[STATE_NUMBER][EVENT_DIGIT]    = STATE_OPERATOR;

    state_table[STATE_NUMBER][EVENT_OPERATOR]    = HandleNumber; 
    next_state[STATE_NUMBER][EVENT_OPERATOR] = STATE_NUMBER;

    state_table[STATE_NUMBER][EVENT_LPAREN]   = HandleLparen;   
    next_state[STATE_NUMBER][EVENT_LPAREN]   = STATE_NUMBER;

    state_table[STATE_NUMBER][EVENT_SPACE]    = HandleSpace;    
    next_state[STATE_NUMBER][EVENT_SPACE]    = STATE_NUMBER;

    state_table[STATE_NUMBER][EVENT_END]      = HandleEnd;      
    next_state[STATE_NUMBER][EVENT_END]      = STATE_NUMBER;

    state_table[STATE_NUMBER][EVENT_RPAREN]   = HandleRparen;   
    next_state[STATE_NUMBER][EVENT_RPAREN]   = STATE_OPERATOR;

    /* STATE_OPERATOR */
    state_table[STATE_OPERATOR][EVENT_OPERATOR] = HandleOperator; 
    next_state[STATE_OPERATOR][EVENT_OPERATOR] = STATE_NUMBER;

    state_table[STATE_OPERATOR][EVENT_RPAREN]   = HandleRparen;  
    next_state[STATE_OPERATOR][EVENT_LPAREN] = STATE_NUMBER; 

    state_table[STATE_OPERATOR][EVENT_LPAREN]   = HandleLparen;
    next_state[STATE_OPERATOR][EVENT_RPAREN]   = STATE_OPERATOR;

    state_table[STATE_OPERATOR][EVENT_SPACE]    = HandleSpace;    
    next_state[STATE_OPERATOR][EVENT_SPACE]    = STATE_OPERATOR;

    state_table[STATE_OPERATOR][EVENT_END]      = HandleEnd;      
    next_state[STATE_OPERATOR][EVENT_END]      = STATE_OPERATOR;
}

static void InitALL(void)
{
    InitOpLut();             /* Operator table with precedence and function pointers */
    InitSignAndDigitLuts();  /* Lookup tables for signs (+/-) and digits (0-9, '.') */
    InitAS();                /* Associativity table */
    InitMatchingParen();     /* Matching pairs for parentheses/brackets/braces */
    InitTables();            /* Map characters to events and initialize FSM state tables */
}

/* ================ Stack Function Helpers ======================= */

static void NumPush(stack_t *s, double v)
{
    assert(s);
    assert(StackSize(s) != StackCapacity(s));

    StackPush(s, &v);
}

static double NumPop(stack_t *s)
{
    double val = 0;

    assert(s);
    assert(StackSize(s) != 0);

    val = *(double *)StackPeek(s);

    StackPop(s);
    return val;
}

static void OpPush(stack_t *s, char c)
{
    assert(s);
    assert(StackSize(s) != StackCapacity(s));

    StackPush(s, &c);
}

static char OpPop(stack_t *s)
{
    char val = 0;

    assert(s);
    assert(StackSize(s) != 0);

    val = *(char *)StackPeek(s);
    
    StackPop(s);
    return val;
}

static char OpPeek(stack_t *s)
{
    return *(char *)StackPeek(s);
}

static void ApplyTop(calc_data_t* calc)
{
    double b = 0.0;
    double a = 0.0;
    char op = 0;

    if (StackSize(calc->ops) < 1)
    {
        calc->status = SYNTAX_ERROR;
        return;
    }

    b = NumPop(calc->nums);
    a = NumPop(calc->nums);
    op = OpPop(calc->ops);

    if ((op == '/' && b == 0.0) || (op == '^' && a == 0.0 && b <= 0.0))
    {
        calc->status = MATH_ERROR;
        return; 
    }
    NumPush(calc->nums, op_lut[(unsigned char)op].operate(a, b));
}

/* ========================== Handlers ======================================= */

static calculator_status_e HandleNumber(calc_data_t* calc)
{
    int sign = 1;
    double val = 0.0;

    while (sign_lut[(unsigned char)*calc->p])
    {
        sign *= (signed char)sign_lut[(unsigned char)*calc->p];
        calc->p++;
    }

    calc->status = (digit_lut[(unsigned char)*calc->p] ? SUCCESS : SYNTAX_ERROR);

    val = strtod(calc->p, (char**)&calc->p);

    NumPush(calc->nums, val * sign);
    return calc->status;
}

static calculator_status_e HandleOperator(calc_data_t* calc)
{
    char op = *calc->p;

    while (StackSize(calc->ops) > 0 && (op_lut[(unsigned char)OpPeek(calc->ops)].precedence >= op_lut[(unsigned char)op].precedence + AS[(unsigned char)OpPeek(calc->ops)]))
    {
        ApplyTop(calc);
    }

    if (calc->status != SUCCESS)
    {
        return calc->status;
    }

    OpPush(calc->ops, op);
    calc->p++;
    return SUCCESS;
}




static calculator_status_e HandleLparen(calc_data_t* calc)
{
    char lparen = 0;

    lparen = matching_paren[(unsigned char)*calc->p];

    OpPush(calc->ops, lparen);
    calc->p++;
    return calc->status;
}


static calculator_status_e HandleRparen(calc_data_t* calc)
{
    char lparen = 0;

    lparen = matching_paren[(unsigned char)*calc->p];

    while (calc->status == SUCCESS && StackSize(calc->ops) > 0 && OpPeek(calc->ops) != lparen)
    {
        if (matching_paren[(unsigned char)OpPeek(calc->ops)] == OpPeek(calc->ops))
        {
            return SYNTAX_ERROR;
        }
        ApplyTop(calc);    
    }
    
    if (StackSize(calc->ops) == 0)
    {
        return SYNTAX_ERROR;
    }

    OpPop(calc->ops); 
    calc->p++;
    return SUCCESS;
}

static calculator_status_e HandleSpace(calc_data_t* calc)
{
    calc->p++;
    return SUCCESS;
}

static calculator_status_e HandleEnd(calc_data_t* calc)
{
    while (StackSize(calc->ops) > 0)
    {
        if(calc->status != SUCCESS)
        {
            return calc->status;
        }
        if (OpPeek(calc->ops) == '(' || OpPeek(calc->ops) == '[' || OpPeek(calc->ops) == '{') 
        {
            calc->status = SYNTAX_ERROR;
            return calc->status;  
        }
       
        ApplyTop(calc);
    }
    return calc->status;
}

/* ---- Public API ---- */
calculator_status_e Calculator(const char *expression, double *res)
{
    calc_data_t* calc = NULL;
    state_t state = STATE_NUMBER;
    event_t ev = EVENT_END;
    event_handler_t handler = NULL;
    calculator_status_e status = SUCCESS;
    size_t len = 0;

    assert(expression);

    InitALL();

    len = strlen(expression);
    
    calc = malloc(sizeof(calc_data_t));
    if (!calc)
    {
        return ALLOC_ERROR;
    }
    
    calc->nums = NULL;
    calc->ops = NULL;
    calc->status = SUCCESS;
    calc->p = expression;
    
    calc->nums = StackCreate(len, sizeof(double));
    if (!calc->nums)
    {
        free(calc);
        return ALLOC_ERROR;
    }
    calc->ops  = StackCreate(len, sizeof(char));
    if (!calc->ops)
    {
        StackDestroy(calc->nums);
        free(calc);
        return ALLOC_ERROR;
    }
    
    while (calc->status == SUCCESS)
    {
        ev = char_to_event[(unsigned char)*calc->p];
        handler = state_table[state][ev];
        if (!handler)
        {
            calc->status = SYNTAX_ERROR;
            break;
        }

        calc->status = handler(calc);

        state = next_state[state][ev];

        if (ev == EVENT_END) 
        {
            break;
        }
    }

    if (calc->status == SUCCESS && StackSize(calc->nums) == 1)
    {
        *res = NumPop(calc->nums);
    }
    else if (calc->status == SUCCESS)
    {
        calc->status = SYNTAX_ERROR;
    }

    status = calc->status;

    StackDestroy(calc->nums);
    StackDestroy(calc->ops);
    free(calc);

    return status;
}
