/************************************
 * Exercise: calculator (FSM)
 * Date: 15/08/25
 * Developer: Baruch Haimson
 * Reviewer: Tal
 * Status: In Progress
 ************************************/

#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

typedef enum status
{
    SUCCESS,
    MATH_ERROR,
    SYNTAX_ERROR,
    ALLOC_ERROR
} calculator_status_e;

typedef enum state
{
    STATE_NUMBER,
    STATE_OPERATOR,
    NUM_STATES
} state_t;

typedef enum event
{
    EVENT_DIGIT,
    EVENT_OPERATOR,
    EVENT_LPAREN,
    EVENT_RPAREN,
    EVENT_SPACE,
    EVENT_END,
    EVENT_ERROR,
    NUM_EVENTS
} event_t;


calculator_status_e Calculator(const char* expression, double* res);

#endif /* __CALCULATOR_H__ */