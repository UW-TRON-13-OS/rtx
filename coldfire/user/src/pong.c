#include "rtx.h"
#include "dbug.h"
#include "user_processes.h"
#include "pong.h"
#include "utils.h"

#define PADDLE_LENGTH 5
#define BOARD_ROWS 24
#define BOARD_COLS 75
#define DELAY_AMOUNT 5 

char board[BOARD_ROWS][BOARD_COLS];
int left_paddle_pos; // 0 to 15
int right_paddle_pos; // 0 to 15
int ball_r, ball_c;
int r_speed, c_speed;

void init_pong ()
{
    int r, c;
    for (r = 0; r < BOARD_ROWS; r++)
    {
        for (c = 0; c < BOARD_COLS; c++)
        {
            if ((r == 0 && c == 0) || (r == 0 && c == BOARD_COLS-1) || (r == BOARD_ROWS-1 && c == 0) || (r == BOARD_ROWS-1 && c == BOARD_COLS-1))
            {
                board[r][c] = '+';
            }
            else if (r == 0 || r == BOARD_ROWS-1)
            {
                board[r][c] = '-';
            }
            else if (c == 0 || c == BOARD_COLS-1)
            {
                board[r][c] = '|';
            }
            else
            {
                board[r][c] = ' ';
            }
        }
    }
    left_paddle_pos = BOARD_ROWS/2-2;
    right_paddle_pos = BOARD_ROWS/2-2;
    
    for (r = 0; r < PADDLE_LENGTH; r++)
    {
        board[left_paddle_pos + r][1] = 'M';
        board[right_paddle_pos + r][BOARD_COLS-2] = 'M';
    }
    
    ball_r = BOARD_ROWS/2;
    ball_c = BOARD_COLS/2;
    board[ball_r][ball_c] = 'O';
    
    r_speed = 1;
    c_speed = 1;
}

void print_board ()
{
    MsgEnv* msg = request_msg_env();
    rtx_strcpy(msg->msg, "\033[0;0H", 1024);
    send_console_chars(msg, FALSE);
    int r, c;
    for (r = 0; r < BOARD_ROWS; r++)
    {
        MsgEnv* message = request_msg_env();
        for (c = 0; c < BOARD_COLS; c++)
        {
            message->msg[c] = board[r][c];
        }
        message->msg[BOARD_COLS] = '\n';
        message->msg[BOARD_COLS+1] = '\r';
        message->msg[BOARD_COLS+2] = '\0';
        send_console_chars(message, FALSE);
    }
}

void update_board ()
{
    MsgEnv* message = receive_message();
    //release_msg_env(msg);
    init_pong();
    print_board();
    char cmd[100];
    void * params [11];
    request_delay(DELAY_AMOUNT, WAKEUP_10, message);
    while (1)
    {
        /*MsgEnv**/ message = receive_message();
        if (message->msg_type == CONSOLE_INPUT) // people moving their paddles
        {
            // Keys for left are A -> UP, S -> DOWN
            // Keys for right are K -> UP, L -> DOWN
            if ((message->msg[0] == 'a' || message->msg[0] == 'A') && (left_paddle_pos > 0))
            {
                board[left_paddle_pos+4][1] = ' ';
                left_paddle_pos--;
                board[left_paddle_pos][1] = 'M';
            }
            else if ((message->msg[0] == 's' || message->msg[0] == 'S') && (left_paddle_pos < BOARD_COLS-5))
            {
                board[left_paddle_pos+4][1] = ' ';
                left_paddle_pos++;
                board[left_paddle_pos][1] = 'M';
            }
            else if ((message->msg[0] == 'k' || message->msg[0] == 'K') && (right_paddle_pos > BOARD_COLS-5))
            {
                board[right_paddle_pos+4][BOARD_COLS-2] = ' ';
                right_paddle_pos--;
                board[right_paddle_pos][BOARD_COLS-2] = 'M';
            }
            else if ((message->msg[0] == 'l' || message->msg[0] == 'L') && (right_paddle_pos < 15))
            {
                board[right_paddle_pos+4][BOARD_COLS-2] = ' ';
                right_paddle_pos++;
                board[right_paddle_pos][BOARD_COLS-2] = 'M';
            }
            release_msg_env(message);
        }
        else if (message->msg_type == WAKEUP_10) // the timer, so update the ball;
        {
            if (ball_c == 0)
            {
                init_pong();// RIGHT PLAYER SCORED
                print_board();
                request_delay(DELAY_AMOUNT, WAKEUP_10, message);
                continue;
            }
            if (ball_c == BOARD_COLS-1)
            {
                init_pong();// LEFT PLAYER SCORED
                print_board();
                request_delay(DELAY_AMOUNT, WAKEUP_10, message);
                continue;
            }
            
            // bouncing checks
            if (ball_r == 1 || ball_r == BOARD_ROWS-2)
            {
                r_speed = r_speed * -1;
            }
            if (ball_c == 2 && board[ball_r+r_speed][1] == 'M')
            {
                c_speed = c_speed * -1;
            }
            else if (ball_c == BOARD_COLS-3 && board[ball_r+r_speed][BOARD_COLS-2] == 'M')
            {
                c_speed = c_speed * -1;
            }
            
            board[ball_r][ball_c] = ' ';
            ball_r += r_speed;
            ball_c += c_speed;
            board[ball_r][ball_c] = 'O';

            /*int prev_r = ball_r - r_speed;
            int prev_c = ball_c - c_speed;

            params[0] = &prev_r;
            params[1] = &prev_c;
            params[2] = &ball_r;
            params[3] = &ball_c;
            params[4] = NULL;
            rtx_sprintf(cmd, "\033[%d;%dH \033[%d;%dHO", params);
            MsgEnv* env = request_msg_env();
            env->msg = cmd;
            send_console_chars(env, FALSE);*/

            request_delay(DELAY_AMOUNT, WAKEUP_10, message);
        }        
        print_board();
    }
}
