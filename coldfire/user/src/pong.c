#include "rtx.h"
#include "k_primitives.h"
#include "dbug.h"
#include "k_globals.h"

#define PADDLE_LENGTH 5
#define BOARD_ROWS 20
#define BOARD_COLS 80

char board[20][80];
int left_paddle_pos; // 0 to 15
int right_paddle_pos; // 0 to 15
int ball_r, ball c;
int r_speed, c_speed;

void init_pong ()
{
    int r, c;
    for (r = 0; r < BOARD_ROWS; r++)
    {
        for (c = 0; c < BOARD_COLS; c++)
        {
            board[r][c] = ' ';
        }
    }
    left_paddle_pos = 7;
    right_paddle_pos = 8;
    
    for (r = 0; r < PADDLE_LENGTH; r++)
    {
        board[left_paddle_pos + r][0] = 'M';
        board[right_paddle_pos + r][BOARD_COLS-1] = 'M';
    }
    
    ball_r = 10;
    ball_c = 40;
    board[ball_r][ball_c] = 'X';
    
    r_speed = 1;
    c_speed = 1;
}

void print_board ()
{
    int r, c;
    for (r = 0; r < BOARD_ROWS; r++)
    {
        MsgEnv* message = request_msg_env();
        for (c = 0; c < BOARD_COLS; c++)
        {
            message->msg[c] = board[r][c];
        }
        send_console_chars(message);
    }
}

void update_board ()
{
    while (1)
    {
    
    }
}