#ifndef CONNECT4_H_
#define CONNECT4_H_

//------------------------------- Constants ----------------------------//
#define BOARD_MAX_SIDE  25

#define MSG_WELCOME_MESSAGE  0
#define MSG_GET_BOARD_ROWS   1
#define MSG_GET_BOARD_COLS   2
#define MSG_GET_COLOR        3
#define MSG_PLAYER_MOVE      4
#define MSG_WINNER           5
#define MSG_GET_COLUMN       6
#define MSG_COLUMN_FULL      7
#define MSG_NO_WINNER        8
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


#endif /* CONNECT4_H_ */
