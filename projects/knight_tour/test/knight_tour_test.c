/***************************
 Exercise:  knight tour
 Date: 	    02/09/25
 Developer: Baruch Haimson
 Reviewer:  Dana
 Status:    Approved
***************************/

#include <stdio.h> /* printf */

#include "knight_tour.h" /* chess_square_t */

int main()
{
    size_t board[CHESS_LEN][CHESS_LEN] = { 0 };
    chess_square_t start_square = {0,0};

    KnightTour(board, start_square, 0);
    PrintBoard(board);
    KnightTour(board, start_square, 1);
    PrintBoard(board);

    return 0;
}