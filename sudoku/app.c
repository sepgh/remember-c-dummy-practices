#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <sys/time.h>


typedef struct Sudoku {
    unsigned int board[9][9];
} Sudoku;



void print_board(Sudoku* sudoku){
    int i, j;
    for (i = 0; i < 9; i++){
        for (j = 0; j < 9; j++){
            int value = sudoku->board[i][j];
            if (value == 0)
                printf(" %c ", ' ');    
            else
                printf(" %d ", value);
            if (j != 8){
                if ((j + 1) % 3 == 0)
                    printf(" || ");
                else
                    printf("|");
            }
        }
        if (i != 8){
            if ((i + 1) % 3 == 0)
                    printf("\n=========================================");
                else
                    printf("\n-----------------------------------------");

        }
        printf("\n");
    }
    printf("\n");
}


bool read_board(char* path, Sudoku* sudoko){
    FILE* file = fopen(path, "r");
    if (file == NULL){
        return false;
    }

    size_t buffer_size = 24;
    char buffer_in[buffer_size];

    int row = 0;
    while(fgets(buffer_in, buffer_size, file) != NULL){
        sscanf(
            buffer_in,
            "%d %d %d  %d %d %d  %d %d %d",
            &sudoko->board[row][0],
            &sudoko->board[row][1],
            &sudoko->board[row][2],

            &sudoko->board[row][3],
            &sudoko->board[row][4],
            &sudoko->board[row][5],

            &sudoko->board[row][6],
            &sudoko->board[row][7],
            &sudoko->board[row][8]
        );
        row++;
    }
    fclose(file);
}


void generate_board(Sudoku* sudoko){
    sudoko->board[0][0] = 4;
    sudoko->board[0][1] = 8;
    sudoko->board[0][2] = 9;
    sudoko->board[0][3] = 5;
    sudoko->board[0][5] = 1;
    sudoko->board[0][7] = 2;
    
    sudoko->board[1][0] = 7;
    sudoko->board[1][1] = 5;
    sudoko->board[1][6] = 8;
    sudoko->board[1][7] = 1;
    

    sudoko->board[2][4] = 2;
    sudoko->board[2][6] = 5;
    sudoko->board[2][7] = 9;
    sudoko->board[2][8] = 4;

    sudoko->board[3][2] = 8;
    sudoko->board[3][4] = 9;
    sudoko->board[3][7] = 7;
    sudoko->board[3][8] = 5;

    sudoko->board[4][0] = 5;
    sudoko->board[4][5] = 8;
    
    sudoko->board[5][2] = 1;
    sudoko->board[5][5] = 3;

    sudoko->board[6][0] = 1;
    sudoko->board[6][1] = 6;
    sudoko->board[6][3] = 3;
    sudoko->board[6][4] = 7;
    sudoko->board[6][5] = 4;
    sudoko->board[6][7] = 8;
    sudoko->board[6][8] = 2;
    
    sudoko->board[7][5] = 5;
    sudoko->board[7][6] = 7;
    sudoko->board[7][7] = 3;
    sudoko->board[7][8] = 6;

    sudoko->board[8][2] = 3;
    sudoko->board[8][4] = 6;
    sudoko->board[8][5] = 2;
    sudoko->board[8][6] = 4;
    sudoko->board[8][7] = 5;
}


bool solve(Sudoku* sudoku, int index){
    int row = floor(index / 9);
    int col = floor(index % 9);


    if (index == (9*9))
        return true;
    if (sudoku->board[row][col] != 0){
        return solve(sudoku, index + 1);
    }

    
    int possible_numbers[] = {1,2,3,4,5,6,7,8,9};
    
    // Checking horizontal
    int z;
    for(z = 0; z < 9; z++){
        int val = sudoku->board[row][z];
        if (val == 0){
            continue;
        }
        possible_numbers[val-1] = 0;
    }

    // Checking vertical
    for(z = 0; z < 9; z++){
        int val = sudoku->board[z][col];
        if (val == 0){
            continue;
        }
        possible_numbers[val-1] = 0;
    }


    // Checking inner box
    int box_first_row = floor(row / 3) * 3;
    int box_first_col = floor(col / 3) * 3;

    // printf("Box > ");

    int m,n;
    for (m = 0; m < 3; m++){
        for (n = 0; n < 3; n++){
            int val = sudoku->board[box_first_row+m][box_first_col+n];
            if (val == 0){
                continue;
            }
            possible_numbers[val-1] = 0;
        }            
    }

    bool solved_index = false;
    for (int si = 0; si < 9; si++){
        if (possible_numbers[si] != 0){
            sudoku->board[row][col] = possible_numbers[si];
            if (solve(sudoku, index+1)){
                solved_index = true;
                break;
            }
        }
    }

    if (!solved_index){
        sudoku->board[row][col] = 0;
    }
    return solved_index;
}


int main(int argc, char* args[]){
    Sudoku sudoku = {};
    read_board("board.txt", &sudoku);
    print_board(&sudoku);
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    bool solved = solve(&sudoku, 0);
    gettimeofday(&stop, NULL);
    if (solved){
        printf("Solved! \n");
        printf("took %lu microseconds\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); 
    }
    print_board(&sudoku);
    return 0;
}







