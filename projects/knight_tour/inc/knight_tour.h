/***************************
 Exercise:  knight tour
 Date: 	    02/09/25
 Developer: Baruch Haimson
 Reviewer:  Dana
 Status:    Approved
***************************/

#ifndef __ILRD_KNIGHT_TOUR_H__
#define __ILRD_KNIGHT_TOUR_H__

#include <time.h> /* time_t */

#define CHESS_LEN (8)

typedef enum knight_tour_status
{
    SUCCESS, 
    FAIL,    
    TIMEOUT  
} knight_tour_status_e;

typedef struct chess_square
{
    size_t row; 
    size_t col; 
} chess_square_t;

/* ------------------------------------------------------------------------
 * Receives: 
 *      board        - 8x8 matrix (CHESS_LEN x CHESS_LEN) to fill with moves
 *      start_square - starting position of the knight (row, col)
 *      is_heuristic - flag (0 = simple backtracking, 1 = heuristic/Warnsdorff)
 *
 * Does:
 *      Attempts to perform a full knight’s tour starting from start_square.
 *      Uses either backtracking or heuristic strategy depending on the flag.
 *
 * Returns:
 *      SUCCESS  - if a full tour was found
 *      FAIL     - if no solution was found within constraints
 *      TIMEOUT  - if execution exceeded allowed time (300 sec)
 *
 * Why not defined here:
 *      Only declared in the header for external usage.
 *      Implementation is in the .c file.
 *
 * Complexity:
 *      Worst case: O(8^(N^2)) for backtracking.
 *      With heuristic: drastically reduced, closer to O(N^2).
 * ------------------------------------------------------------------------ */
knight_tour_status_e KnightTour(size_t board[CHESS_LEN][CHESS_LEN], 
                                chess_square_t start_square, 
                                int is_heuristic);

/* ------------------------------------------------------------------------
 * Receives:
 *      board - 8x8 matrix containing knight’s tour path numbers
 *
 * Does:
 *      Prints the chessboard state to stdout.
 *
 * Returns:
 *      void (nothing)
 *
 * Why not defined here:
 *      Only declaration, implementation is in the .c file.
 *
 * Complexity:
 *      O(N^2) = O(64) → constant time for fixed board size.
 * ------------------------------------------------------------------------ */
void PrintBoard(size_t board[CHESS_LEN][CHESS_LEN]);

/* ------------------------------------------------------------------------
 * Receives:
 *      nothing
 *
 * Does:
 *      Records the current time.
 *
 * Returns:
 *      time_t value representing start time.
 *
 * Why not defined here:
 *      Declaration only, implementation is in the .c file.
 *
 * Complexity:
 *      O(1)
 * ------------------------------------------------------------------------ */
time_t StartTimer(void);

/* ------------------------------------------------------------------------
 * Receives:
 *      start_timer - a time_t value returned from StartTimer
 *
 * Does:
 *      Calculates how many seconds passed since start_timer.
 *
 * Returns:
 *      size_t - elapsed seconds.
 *
 * Why not defined here:
 *      Declaration only, implementation is in the .c file.
 *
 * Complexity:
 *      O(1)
 * ------------------------------------------------------------------------ */
size_t EndTimer(time_t start_timer);

#endif /* __ILRD_KNIGHT_TOUR_H__ */
