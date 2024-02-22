#include <stdio.h>
#include <stdbool.h>
#include <math.h>


int EMPTY = 0;
int X_PLAYER = 1;
int O_PLAYER = 2;


// Game state:
int board[][3] = {};
int filled_choice_count = 0;


char get_player_as_char(int choice){
    if (choice == EMPTY){
        return ' ';
    }else if (choice == X_PLAYER)
    {
        return 'X';
    }else if (choice == O_PLAYER)
    {
        return 'O';
    }
    return '?';
}

void print_board(){
    int i, j;
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            printf(" %c ", get_player_as_char(board[i][j]));
            if (j != 2){
                printf("|");
            }
        }
        if (i != 2){
            printf("\n-----------");
        }
        printf("\n");
    }
}

bool apply_choice(int choice, int player){
    if (choice < 0 || choice > 8){
        printf("Choose between 0-8\n");
        return false;
    }

    int i = floor(choice / 3);
    int j = choice % 3;

    if (board[i][j] != EMPTY){
        printf("Choice already taken\n");
        return false;
    }

    board[i][j] = player;

    return true;

}

void run_turn(int player){
    printf("%c turn:", get_player_as_char(player));
    char buffer[1];
    int choice;
    scanf("%d", &choice);
    if (!apply_choice(choice, player)){
        run_turn(player);
    }
}

int get_winner(){
    if (filled_choice_count < 5){
        return 0;
    }

    bool diag_eq = true;
    bool diag_neg_eq = true;

    int i, j = 0;
    for (i = 0; i < 3; i++){
        bool vertical_eq = true;
        bool horizontal_eq = (i != 0);

        for (j = 0; j < 3; j++){
            if (j != 0){
                vertical_eq = vertical_eq && (board[j][i] == board[j-1][i]);
            }
            if (i != 0){
                horizontal_eq = horizontal_eq && (board[i][j] == board[i-1][j]);
            }
        }

        if (vertical_eq){
            return board[0][i];
        }

        if (horizontal_eq){
            return board[i][0];
        }

        if (i != 0){
            diag_eq = diag_eq && (board[i][i] == board[i-1][i-1]);
            int neg_row = 2 - i;
            int neg_col = 2 - neg_row;
            diag_neg_eq = diag_neg_eq && (board[neg_row][neg_col] == board[neg_row+1][neg_col-1]);
        }

    }

    if (diag_eq){
        return board[0][0];
    }
    if (diag_neg_eq){
        return board[0][2];
    }

    return 0;
}


int main(int argc, char* args[]){

    int player = X_PLAYER;
    print_board();

    while (filled_choice_count < 9){

        run_turn(player);
        filled_choice_count++;
        print_board();

        int winner = get_winner();
        if (winner != 0){
            printf("Winner is: %c", get_player_as_char(winner));
            break;
        }


        if (player == X_PLAYER){
            player = O_PLAYER;
        }else{
            player = X_PLAYER;
        }

    }

    return 0;
}


