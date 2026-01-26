#include <stdio.h>
#include <stddef.h>

int main()
{
    char current = 'x';
    int rows = 0;
    int cols = 0;
    size_t i = 0;
    size_t j = 0;
    int win = 0;
    int final_game = 0;
    int count = 0;

    char board[3][3] = 
    {
        {' ',' ',' '},
        {' ',' ',' '},
        {' ',' ',' '}
    };


    while(!final_game) 
    {
        printf("input cell arguments:");

        scanf("%d %d", &rows, &cols);

        if (rows >= 0 && rows <= 2 && cols >= 0 && cols <= 2 && board[rows][cols] == ' ')
        {
            board[rows][cols] = current;
            count++;

            win = 0;

            for(i = 0; i < 3; i++)
            {   
                if(board[i][0] == current && board[i][1] == current && board[i][2] == current)
                {
                    win = 1;
                }
                else if(board[0][i] == current && board[1][i] == current && board[2][i] == current)
                {
                    win = 1;
                }  
            }

            if(!win)
            {
                if(board[0][0] == current && board[1][1] == current && board[2][2] == current)
                {
                    win = 1;
                }
                else if(board[0][2] == current && board[1][1] == current && board[2][0] == current)
                {
                    win = 1;
                }
            }

            if(win)
            {
                printf("player %c is WIN!\n", current);
                final_game = win;
            }
            if(count == 9 && !win)
            {
                printf("the board is full we have draw let's play again...\n");
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 3; j++)
                    {
                        board[i][j] = ' ';
                    }
                }
                count = 0;

            }
            
            current = (current == 'x') ? 'o' : 'x'; 
        }
        else
        {
            printf("invalid cell!\n");
        }
        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++)
            {
                printf("%c", board[i][j]);
            }
            printf("\n");
        }
       
    }
    return 0;
}
