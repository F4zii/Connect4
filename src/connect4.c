#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "connect4.h"



char const* const game_messages[];
//---------------------------- Predeclarations -------------------------//

/*
 * Outputs game board.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 */
void print_board(char board[][BOARD_MAX_SIDE], int board_side[]);
/* makes the board[25][25] ,
  and every cell == ' ' */
void create_space_board(char board[][BOARD_MAX_SIDE]);

 /* starts the game by calling the functions:
* set blocks size
* get colors
* create the starting board
this function was made to
make the lines shorter and look better.
*/
void start_game(int boardSizes[], char colors[]);
/*
 * Outputs winner or tie message.
 *
 * player_id - the id to be output. Pass <0 for tie.
 */
void print_winner(int player_id);
/*prints starting message,
gets the starting player color ,
and update the array.
* arr[0] == the first player color */
void get_starting_color(char colors[]);
/*sets the color array ,
colors[0] = player 1 color
colors[1] = player 2 color*/
int set_colors_array(char colors[], char curr);
// sets the block Sizes.
// blockSize[0] = Rows
// blockSize[1] = Columns
void set_board_sizes(int blockZises[]);
/*
important function;
calling functions to
get moves from the players
updates the game board ,
and sets the playing status*/
void update_game(char board[][BOARD_MAX_SIDE],int boardSizes[], char colors[], int *player_id, int *game_over);
/* moves the player ,
asks him where to draw his R or Y
and than calls the draw function
after the player moves we toggle the player id*/
void player_move(char board[][BOARD_MAX_SIDE], int boardSizes[], char colors[], int* player_id, int *game_over);
/* toggles the player id from 1 to 2 or from 2 to 1 */
void toggle_player_id(int *player_id);
/* draws moves on the board
Y or R.
checks the board with help functions and changes the int variable
called game_over if theres a winner.
when we call the function
we know that the column we need to draw in
is not full
*/
void draw_move_and_check_board(char board[][BOARD_MAX_SIDE], int boardSizes[2], int column, int *player_id, char colors[], int *game_over);

/* checks if a column is in the array bound*/
int column_in_bound(int boardSizes[2], int column);

/* checks if theres 4 in a row in column */
int check_column(char board[][BOARD_MAX_SIDE], int boardSizes[2], int i , int j);

/* checks if theres 4 in a row in a row (parameter) */
int check_row(char board[][BOARD_MAX_SIDE], int boardSizes[2], int i , int j);

/* checks all the near diagonal slopes
that are positive and returns 1
if theres a match(4 in a row)
and 0 if not */
int check_positive_slope_diagonal(char board[][BOARD_MAX_SIDE], int boardSizes[2], int row , int column);
/* checks all the near diagonal slopes
that are negative and returns 1
if theres a match(4 in a row)
and 0 if not */
int check_negative_slope_diagonal(char board[][BOARD_MAX_SIDE], int boardSizes[2], int row , int column);

/* checks if the board is full */
int is_board_full(char board[][BOARD_MAX_SIDE],int boardSizes[2]);

/* prints the winner and the final board */
void finally_print_winner(int *player_id, char board[][BOARD_MAX_SIDE], int boardSizes[]);

//--------------------------- Main Program -----------------------------//

int main()
{
    int boardSizes[2] = {0}, game_over = 0, player_id = 1; // final result is the winner player id or 0
    char colors[2] = {'\0'},board[25][25];
    int *ptr = &game_over;
    create_space_board(board);
    start_game(boardSizes, colors);
    while (!game_over)
        update_game(board, boardSizes, colors, &player_id, ptr);
    print_board(board, boardSizes);
    finally_print_winner(&player_id, board, boardSizes);
    return 0;
}



//---------------------- Printing Functions ----------------------------//

// Inclusive check in range
#define IS_IN_RANGEI(VAL, MIN, MAX) (((VAL)>=(MIN)) && ((VAL)<=(MAX)))

char const* const game_messages[] = {
        "Welcome to 4-in-a-row game! \n",
        "Please enter number of rows: ",
        "Please enter number of columns: ",
        "Please choose starting color (Y)ellow or (R)ed: ",
        "Your move, player %d. ",
        "Player %d won! \n",
        "Please enter column: ",
        "Column full. ",
        "That's a tie. \n"
};

enum {BOARD_VERT_SEP=(int)'|',
    BOARD_LEFT_ANG=(int)'\\',
    BOARD_RIGHT_ANG=(int)'/',
    BOARD_BOTTOM=(int)'-',
    BOARD_BOTTOM_SEP=(int)'-'};

void print_board(char board[][BOARD_MAX_SIDE], int board_side[])
{
    assert(IS_IN_RANGEI(board_side[0], 0, BOARD_MAX_SIDE)
           && IS_IN_RANGEI(board_side[1], 0, BOARD_MAX_SIDE));

    for (int row=0; row<board_side[0]; ++row)
    {
        printf("\n%c", (char)BOARD_VERT_SEP);
        for (int col = 0; col < board_side[1]; ++col)
            printf("%c%c", board[row][col], (char)BOARD_VERT_SEP);
    }
    printf("\n%c", (char)BOARD_LEFT_ANG);
    for (int ii=0; ii<board_side[1]-1; ++ii)
        printf("%c%c", (char)BOARD_BOTTOM, (char)BOARD_BOTTOM_SEP);
    printf("%c%c\n", (char)BOARD_BOTTOM, (char)BOARD_RIGHT_ANG);
}

void print_winner(int player_id)
{
    if (player_id > 0)
        printf(game_messages[MSG_WINNER], player_id);
    else
        printf(game_messages[MSG_NO_WINNER]);
}



int set_colors_array(char colors[], char curr)
{
   if (curr == 'Y')
    {
        colors[0] = 'Y';
        colors[1] = 'R';
        return 0;
    }
    else if (curr == 'R')
    {
        colors[0] = 'R';
        colors[1] = 'Y';
        return 0;
    }
    else return 1;
}


void get_starting_color(char colors[])
{
    /* i made it \n because
     when the program is running
     curr is turning into \n mid code */
    char curr = '\n';
    int invalid_input = 1; // input is invalid until proved its valid
    printf(game_messages[MSG_WELCOME_MESSAGE]);
    do
    {
        printf(game_messages[MSG_GET_COLOR]);
        /* did some debugging and i saw that curr seems to
        * turn into \n so i added a check */
        scanf(" %c", &curr);
         // debug , if curr did not change, i ask again.
        if (curr == '\n') if (scanf("%c", &curr)!=1) continue; // another debugging here
        invalid_input = set_colors_array(colors, curr);
    } while (invalid_input);
}




void set_board_sizes(int boardSizes[])
{
    int curr = 0;
    while (1)
    {
        printf(game_messages[MSG_GET_BOARD_ROWS]);
        scanf(" %d", &curr);
        if (curr > 25 || curr < 4) continue;
        break;
    }
    boardSizes[0] = curr;
    while (1)
    {
        printf(game_messages[MSG_GET_BOARD_COLS]);
        scanf(" %d", &curr);
        if (curr > 25) continue;
        break;
    }
    boardSizes[1] = curr;
}



void start_game(int boardSizes[], char colors[])
{
    get_starting_color(colors);
    set_board_sizes(boardSizes);
}



void update_game(char board[][BOARD_MAX_SIDE],int boardSizes[], char colors[], int *player_id, int *game_over)
{
    print_board(board, boardSizes);
    player_move(board,boardSizes,colors, player_id, game_over);
    if (is_board_full(board, boardSizes))
    {
        *game_over = 1;
        *player_id = -1;
    }

}



void player_move(char board[][BOARD_MAX_SIDE], int boardSizes[2], char colors[], int* player_id, int *game_over)
{
    int column = 0;
    printf(game_messages[MSG_PLAYER_MOVE], *player_id);
    while (1)
    {
        printf(game_messages[MSG_GET_COLUMN]);
        if (scanf(" %d", &column)!=1) continue;
        // column number is not given by index
        //so i pass column-1
        if (column_in_bound(boardSizes, column))
            if (board[0][column-1] == ' ')
            {
                draw_move_and_check_board(board, boardSizes, column, player_id, colors, game_over);
                break;
            }
        printf(game_messages[MSG_COLUMN_FULL]);
        // check for is column is full
    }
    toggle_player_id(player_id);
}


int column_in_bound(int boardSizes[], int column)
{
    // in  this function
    //i cant do && so
    //i did booleans in other way
    int tmp = (column > -1);
    tmp += (column <= boardSizes[1]);
    return (tmp==2);
}



void toggle_player_id(int *player_id)
 {
    if (*player_id == 1)
    {
        *player_id = 2;
    }

    else if (*player_id == 2)
    {
        *player_id = 1;
    }
}

void draw_move_and_check_board(char board[][BOARD_MAX_SIDE], int boardSizes[2], int column, int *player_id, char colors[], int *game_over)
{
    int i, p = *player_id;
    for (i = boardSizes[0]-1; i > -1;)
    {
        if (board[i][column-1] != ' ')
            i--;
        else
        {
            board[i][column-1] = colors[p-1];
            break;
        }
    }
    int boolean = check_row(board, boardSizes, i, column-1) + check_column(board, boardSizes, i , column-1);
    boolean += check_positive_slope_diagonal(board, boardSizes, i, column-1);
    boolean += check_negative_slope_diagonal(board, boardSizes, i, column-1);
    *game_over = boolean;
}


int check_row(char board[][BOARD_MAX_SIDE], int boardSizes[2], int row , int column)
{
    char color = board[row][column];
    int start_left = column-3, end_right = column+3, max_right = boardSizes[1],color_seq = 0;
    if (start_left < 0) start_left = 0;
    if (end_right > max_right) end_right = max_right - 1;

    for (int current = start_left; current <= end_right; current++)
    {
        if (board[row][current] != color)
        {
            color_seq=0;
            continue;
        }
        if (++color_seq == 4) return 1;
    }
    return 0;
}

int check_positive_slope_diagonal(char board[][BOARD_MAX_SIDE], int boardSizes[2], int row , int column)
{
    int c, r, boolean;
    char color = board[row][column];
    for (c=0; c < boardSizes[1]-3; c++)
    {
        for (r = boardSizes[0]-1; r > boardSizes[0]-3; r--)
        {
            boolean = ((board[r][c] == color) && (board[r-1][c+1] == color));
            boolean += ((board[r-2][c+2] == color) && (board[r-3][c+3] == color));
            if (boolean == 2)
                return 1;
        }
    }
    return 0;
}



int check_negative_slope_diagonal(char board[][BOARD_MAX_SIDE], int boardSizes[2], int row , int column)
{
    int c, r, count_pairs;
    char color = board[row][column];
    for (c=boardSizes[1]-1; c > 2; c--)
    {
        for (r = boardSizes[0]-1; r > 2; r--)
        {
            count_pairs = ((board[r][c] == color) && (board[r-1][c-1] == color));
            count_pairs += ((board[r-2][c-2] == color) && (board[r-3][c-3] == color));
            if (count_pairs == 2)
                return 1;
        }
    }
    return 0;
}




int check_column(char board[][BOARD_MAX_SIDE], int boardSizes[2], int row , int column)
{
    char color = board[row][column];
    int start_top = row-3,end_bottom = row+3, max_bottom = boardSizes[0],color_seq = 0;
    if (start_top < 0) start_top = 0;
    if (end_bottom > max_bottom) end_bottom = max_bottom - 1;

    for (int current = start_top; current <= end_bottom; current++)
    {
        if (board[current][column] != color)
        {
            color_seq=0;
            continue;
        }
        if (++color_seq == 4)
            return 1;
    }
    return 0;
}


void create_space_board(char board[][BOARD_MAX_SIDE])
{
    for (int i = 0; i < 25; i++)
    {
        for (int j=0; j<25; j++)
        {
            board[i][j] = ' ';
        }
    }
}
int is_board_full(char board[][BOARD_MAX_SIDE],int boardSizes[2])
{
    int counter = 0;
    for (int i = 0; i < boardSizes[1]; i++)
    {
        counter += (board[0][i] != ' ');
    }
    return (counter == boardSizes[1]);
}



void finally_print_winner(int *player_id, char board[][BOARD_MAX_SIDE], int boardSizes[])
{
     if(is_board_full(board, boardSizes))
        *player_id = -1;
    toggle_player_id(player_id);
    print_winner(*player_id);
}
