/***************************
 Exercise:  knight tour
 Date: 	    02/09/25
 Developer: Baruch Haimson
 Reviewer:  Dana
 Status:    Approved
***************************/

#include <stdio.h> /* printf */
#include <time.h> /* time_t */

#include "bit_arr.h" /* BitArrSetOn */
#include "knight_tour.h" /* knight_tour_status_e */

typedef struct move_option
{
    chess_square_t pos;
    int degree;
} move_option_t;


int moves[8][2] = 
{
    { 2,1 }, { 1,2 }, { -1,2 }, { -2,1 },
    { -2,-1 }, { -1,-2 }, { 1,-2 }, { 2,-1 }
};

/*================== helper functions ============================*/
static size_t CoordinateToBit(chess_square_t pos);
static int IsValidNext(chess_square_t pos, bit_arr_t visited_lut);
static knight_tour_status_e KnightTourRecursive(size_t board[CHESS_LEN][CHESS_LEN], chess_square_t start_square, size_t counter, bit_arr_t visited_lut, time_t time);
static knight_tour_status_e KnightTourRecursiveHeuristic(size_t board[CHESS_LEN][CHESS_LEN], chess_square_t start_square, size_t counter, bit_arr_t visited_lut, time_t time);
static int CountValidMoves(chess_square_t pos, bit_arr_t visited_lut);

/*================================================================*/

knight_tour_status_e KnightTour(size_t board[CHESS_LEN][CHESS_LEN], chess_square_t start_square, int is_heuristic)
{
    bit_arr_t visited_lut = 0;
    time_t start_time = StartTimer();

    visited_lut = BitArrSetOn(visited_lut, CoordinateToBit(start_square));
    board[start_square.row][start_square.col] = 1;

    if (is_heuristic)
    {
        return KnightTourRecursiveHeuristic(board, start_square, 2, visited_lut,start_time);
    }

    return KnightTourRecursive(board, start_square, 2, visited_lut, start_time);
}

static knight_tour_status_e KnightTourRecursive(size_t board[CHESS_LEN][CHESS_LEN], 
                                                chess_square_t start_square, size_t counter, bit_arr_t visited_lut, time_t time)
{
    size_t i = 0;
    chess_square_t new_pos;
    size_t curr_index = 0;

    if (counter > CHESS_LEN * CHESS_LEN)
    {
        return SUCCESS;
    }

    for (i = 0; i < 8; ++i)
    {
        new_pos.row = start_square.row + moves[i][0];
        new_pos.col = start_square.col + moves[i][1];

        if (!IsValidNext(new_pos, visited_lut))
        {
            continue;
        }
        if(EndTimer(time) > 300)
        {
            return TIMEOUT;
        }

        curr_index = CoordinateToBit(new_pos);
        visited_lut = BitArrSetOn(visited_lut, curr_index);
        board[new_pos.row][new_pos.col] = counter;

        if (KnightTourRecursive(board, new_pos, counter + 1, visited_lut, time) == SUCCESS)
        {
            return SUCCESS;
        }

        board[new_pos.row][new_pos.col] = 0;
        visited_lut = BitArrSetOff(visited_lut, curr_index);
    }

    return FAIL;
}
static knight_tour_status_e KnightTourRecursiveHeuristic(size_t board[CHESS_LEN][CHESS_LEN],
                                                         chess_square_t start_square, size_t counter, bit_arr_t visited_lut,time_t time)
{
    move_option_t options[8];
    chess_square_t new_pos = {0};
    size_t opt_count = 0;
    move_option_t tmp = { 0 };
    size_t curr_index = 0;
    size_t i = 0;
    size_t j = 0;

    if (counter > CHESS_LEN * CHESS_LEN)
    {
        return SUCCESS;
    }

    for (i = 0; i < 8; i++)
    {
        new_pos.row = start_square.row + moves[i][0];
        new_pos.col = start_square.col + moves[i][1];

        if (IsValidNext(new_pos, visited_lut))
        {
            options[opt_count].pos = new_pos;
            options[opt_count].degree = CountValidMoves(new_pos, visited_lut);
            opt_count++;
        }
    }

    for (i = 0; i < opt_count - 1; i++)
    {
        for (j = i + 1; j < opt_count; j++)
        {
            if (options[j].degree < options[i].degree)
            {
                tmp = options[i];
                options[i] = options[j];
                options[j] = tmp;
            }
        }
    }

    for (i = 0; i < opt_count; i++)
    {
        new_pos = options[i].pos;
        curr_index = CoordinateToBit(new_pos);

        if (EndTimer(time) > 300)
        {
            return TIMEOUT;
        }

        visited_lut = BitArrSetOn(visited_lut, curr_index);
        board[new_pos.row][new_pos.col] = counter;

        if (KnightTourRecursiveHeuristic(board, new_pos, counter + 1, visited_lut, time) == SUCCESS)
        {
            return SUCCESS;
        }

        board[new_pos.row][new_pos.col] = 0;
        visited_lut = BitArrSetOff(visited_lut, curr_index);
    }

    return FAIL;
}

void PrintBoard(size_t board[CHESS_LEN][CHESS_LEN])
{
    size_t i = 0, j = 0;

    printf("\n");
    for(i = 0; i < CHESS_LEN; i++)
    {
        for(j = 0; j < CHESS_LEN; j++)
        {
            printf("%2lu ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

time_t StartTimer(void)
{
    return time(NULL);
}

size_t EndTimer(time_t start_timer)
{
    return (size_t)time(NULL) - start_timer;
}

/*================== helper functions ============================*/

static size_t CoordinateToBit(chess_square_t pos)
{
    return pos.row * CHESS_LEN + pos.col;
}

static int IsValidNext(chess_square_t pos, bit_arr_t visited_lut)
{
    size_t bit_index = 0;
    if (pos.row >= CHESS_LEN || pos.col >= CHESS_LEN)
    {
        return 0;
    }

    bit_index = CoordinateToBit(pos);
    return BitArrGetBit(visited_lut, bit_index) == 0;
}

static int CountValidMoves(chess_square_t pos, bit_arr_t visited_lut)
{
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        chess_square_t next = { pos.row + moves[i][0], pos.col + moves[i][1] };
        if (IsValidNext(next, visited_lut))
        {
            count++;
        }
    }
    return count;
}
