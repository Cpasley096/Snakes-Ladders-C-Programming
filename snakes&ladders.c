/**
 * @author Conor Pasley
 * @date 2022-05-01
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//global variable of the game board
struct game_board *board;
struct game_board{
    struct square *start; //Head of the linked list 
    struct square *current; //Players current position 
    struct square *path[500]; //Path the player took
    int moves; // amount of moves;
    int rows; // amount of rows in the board
    int cols; // amount of columns in the board
};

struct square{
    int cell_id; 
    struct square *next_square;
    struct square *snake_head_pointer; // points to the head of the snake or null
    struct square *snake_tail_pointer; // points to the tail of the snake or null
    struct square *ladder_start_pointer; // points to the foot of the ladder or null
    struct square *ladder_end_pointer; // points to the top of the ladder or null
};

/**
 * @brief Create a new square. Allocates memory and initialises a new square.
 * @param id The position of the square on the board
 * @return the square that was created
 */
struct square *create_new_square(int id){
    struct square *new_square = NULL;
    new_square = malloc(sizeof(struct square));
    new_square->cell_id = id; 
    new_square->next_square = NULL;
    new_square->snake_head_pointer = NULL;
    new_square->snake_tail_pointer = NULL;
    new_square->ladder_start_pointer = NULL;
    new_square->ladder_end_pointer = NULL;
    
    return new_square;
}
/**
 * @brief creating a new board
 * @param rows amount of rows in the board
 * @param cols amount of cols in the board
 * @return returns the created game board
 */
struct game_board *generate_board(int rows, int cols){
    struct game_board *new_game_board = NULL;
    new_game_board = malloc(sizeof(struct game_board)  + 500 * sizeof(struct square));// malloc for gameboard and array for path
    new_game_board->start = NULL;
    new_game_board->current = NULL;
    new_game_board->rows = rows;
    new_game_board->cols = cols;
    new_game_board->moves = 0;

    return new_game_board;
}

/**
 * @brief adding squares to the created game board 
 * @param squareAmount the number of squares in the board 
 * @return returns the created game board
 */
void add_squares_to_board(int squareAmount){
    struct square *next = NULL; 
    for(int i = 1; i <= squareAmount; i ++){
        if(board->start == NULL){
            board->start = create_new_square(i); //create first square
            board->current = board->start; // current pos is the start of the board
            next = board->start;
            board->path[0] = board->start; // add the first square to the path
        } else{
            while(next->next_square != NULL){
                next = next->next_square;
            }
            next->next_square = create_new_square(i);
        }
    }
}

/**
 * @brief Finding a square by id
 * @param cell_id id of sqaure
 * @return returns the square found
 */

struct square *find_square(int cell_id){
    struct square *pointer = NULL;
    int next_cell_id;
    pointer = board->start;
    while(pointer != NULL){
        next_cell_id = pointer->cell_id;
        if(next_cell_id == cell_id) return pointer;
        pointer = pointer->next_square;
    }
}

/**
 * @brief adding snakes to the board 
 * @param amount_of_snakes amount of snakes to be added 
 * @param square_amount amount of squares in the board 
 */
void add_snakes(int amount_of_snakes, int square_amount){
    struct square *head = NULL;
    struct square *tail = NULL;
    int head_position;
    int tail_position;
    int head_cell_id;
    int outer_loop_count;
    int inner_loop_count;
    int snakes_added = 0;

    //place heads
    //If it cannot find a number in a certain amount of tries then it breaks
    for(int i = 0; i < amount_of_snakes; i++){ 
        outer_loop_count = 0;
        while(1){
            if(outer_loop_count >= square_amount) {
                printf("%d/%d snakes added successfully\n", snakes_added,amount_of_snakes);
                return;
            }

            inner_loop_count = 0;
            while(inner_loop_count != square_amount){
                if(inner_loop_count >= square_amount){
                    printf("%d/%d snakes added successfully\n", snakes_added,amount_of_snakes);
                    return;
                }
                head_position = (rand()% ((square_amount - 1) - 3)) + 2; //get position of the next head
                head = find_square(head_position);//find square of the random position

                // if square doesnt point to a tail and is not a tail
                if(head->snake_tail_pointer == NULL && head->snake_head_pointer == NULL){ 
                    break;
                }

                inner_loop_count ++;
            }
            head_cell_id = head->cell_id; 
            if(head_cell_id >= 11) tail_position = (rand()%(10)) + 1; //get a random number between 1 and 10
           
            else if(head_cell_id == 3 || head_cell_id == 2) tail_position = 1;
            //if the position is less than 10 then get random number between 1 and position
            else tail_position = (rand()% ((head_cell_id) - 2)) + 1;
           
            tail = find_square(head_cell_id - tail_position);

            //if the tail doesnt point to a head and isnt a head of another tail
            if(tail->snake_head_pointer == NULL && tail->snake_tail_pointer == NULL){
                head->snake_tail_pointer = tail;
                tail->snake_head_pointer = head;
                snakes_added++;
                break;
            }
                outer_loop_count++;
            } 
        }
    printf("%d/%d snakes added successfully\n", snakes_added,amount_of_snakes);
}

/**
 * @brief adding ladders to the board 
 * @param amount_of_ladders 
 * @param square_amount 
 */
void add_ladders(int amount_of_ladders, int square_amount){
    struct square *top = NULL;
    struct square *foot = NULL;
    int top_position ,foot_position;
    int outer_loop_count, inner_loop_count;
    int top_cell_id;
    int ladders_added = 0;

    //If it cannot find a number in a certain amount of tries then it breaks
    for(int i = 0; i < amount_of_ladders;i++){
        outer_loop_count = 0;
        while(1){
            if(outer_loop_count >= square_amount) {
                printf("%d/%d ladders added successfully\n", ladders_added,amount_of_ladders);
                return;
            }
            inner_loop_count = 0;
            while(1){
                if(inner_loop_count >= square_amount) {
                    printf("%d/%d ladders added successfully\n", ladders_added,amount_of_ladders);
                    return;
                }
                top_position = (rand()% ((square_amount - 1) - 4)) + 3;// top of ladder cant be on square 1 or 2
                top = find_square(top_position);
                if(top->ladder_start_pointer == NULL && top->ladder_end_pointer == NULL && top->snake_tail_pointer == NULL && top->snake_head_pointer == NULL){
                    break;
                }
                inner_loop_count++;
            }
            top_cell_id = top->cell_id;
            //get foot of ladder
            if(top_cell_id >= 11){ 
                foot_position = (rand()% (10)) + 1;
            }
            else if(top_cell_id == 3) {
                foot_position = 1;

            }//if the position is less than 10 then get random number between 2 and position
            else {
                foot_position = (rand()% ((top_cell_id) - 3)) + 2; // foot of ladder cant be at pos 1
            }
            foot = find_square(top_cell_id - foot_position);
            if(foot->snake_tail_pointer == NULL && foot->snake_head_pointer == NULL && foot->ladder_start_pointer == NULL && foot->ladder_end_pointer == NULL){
                top->ladder_start_pointer = foot;//set foot
                foot->ladder_end_pointer = top; // set top
                ladders_added++;
                break;
            } 
            outer_loop_count++;
        }
    }
    printf("%d/%d ladders added successfully\n", ladders_added,amount_of_ladders);
   
}

/**
 * @brief getting the dimensions of the board
 * 
 * @param square_amount the amount of squares on the board 
 * @param rows row pointer
 * @param cols col pointer
 */
void get_board_rows_and_cols(int square_amount, int *rows, int *cols){
    double square_root = sqrt(square_amount);
    double factor; 

    //if the square root is a whole number
    if((int) square_root == square_root ){
        *rows = (int) square_root; 
        *cols = *rows;
    }  else{
        for(double i = 4;i <= 8;i++){
            factor = square_amount/i;
            //Check if the square amount has factors. factor must be less than 8
            if((int) factor == factor && factor <= 8){
                *rows = i;
                *cols = factor;
                break;
            } else{
                //else if the factor isnt whole (until loop ends)
                *rows = factor + 1;
                *cols = i;
            }
        }
    }
}

/**
 * @brief 
 * 
 * @param start start of the row 
 * @param end  end of the row 
 * @param board_size size of the board
 * @param array 
 * @param off_set blank spaces on last row of board (first row printed)
 */
void print_row(int start,int end,int board_size , int array[board_size], int off_set){
    int player_position = board->current->cell_id;
    //prints offset if there is one
    for(int i = 0; i < off_set; i++){
        printf("     ");
    }    

    if(start > end){
        //numbers print left to right decrementally 
        for(int i = start - 1; i >= end - 1; i--){
            if(array[i] == player_position) { 
                printf("\033[1;31m");
                printf(" P1 "); //player cursor
                printf("\033[0m");
                printf("|");
                // player pos
            } 

            else if(array[i] < 10) printf("  %d |", array[i]); // extra space for single digit numbers
            
            else printf(" %d |", array[i]);
        }
        printf("\n");
    } else{
        //end > start 
        //numbers print left to right incrementally 
        for(int i = start - 1; i <= end - 1; i++){
            if(array[i] == player_position){ 
                printf("\033[1;31m");
                printf(" P1 "); // player cursor
                printf("\033[0m");
                 printf("|");
                // player pos
            } 
            else if(array[i] < 10) printf("  %d |", array[i]); // extra space for single digit numbers
            else printf(" %d |", array[i]);
        }
        printf("\n");
    }
}

/**
 * @brief printing the positions of the laddders 
 */
void print_ladders(FILE *file){
    struct square *pointer = NULL;
    int count = 0;
    pointer = board->start;
     if(file != NULL){ //printing to file
        fprintf(file,"\n====================\n");
        fprintf(file,"|      LADDERS     |\n");
        fprintf(file,"====================\n");
     }else{
        printf("\n====================\n");
        printf("|      LADDERS     |\n");
        printf("====================\n");

     }
    //loop until last square

    while(pointer != NULL){
        // loop until ladder is found
        if(pointer->ladder_end_pointer != NULL){
            //print ladder
            if(file){//printing to file
                fprintf(file, "Ladder from: %d ", pointer->cell_id);
                fprintf(file, "to: %d   ", pointer->ladder_end_pointer->cell_id);
                if(count % 2 == 0){
                    fprintf(file," \n");
                }
            } else{//printing to console
                printf("Ladder from: %d ", pointer->cell_id);
                printf("to: %d   ", pointer->ladder_end_pointer->cell_id);
                if(count % 2 == 0){
                    printf(" \n");
                }
            }
            count++;
        }
        pointer = pointer->next_square;
    }
}

/**
 * @brief printing the positions of the snakes
 */
void print_snakes(FILE *file){
    struct square *pointer = NULL;
    int count = 0;
    pointer = board->start;
     if(file != NULL){
        fprintf(file,"\n====================\n");
        fprintf(file,"|      SNAKES      |\n");
        fprintf(file,"====================\n");
     }else{
        printf("\n====================\n");
        printf("|      SNAKES      |\n");
        printf("====================\n");

     }
    while(pointer != NULL){
        //when snake is found
        if(pointer->snake_head_pointer != NULL){
            //print snake positions
            if(file != NULL){//printing to file
                fprintf(file, "snake head from: %d ", pointer->snake_head_pointer->cell_id);
                fprintf(file, "to tail: %d   ", pointer->cell_id);
                if(count % 2 == 0){
                    fprintf(file," \n");
                }
            } else{//printing to console
                printf("snake head from: %d ", pointer->snake_head_pointer->cell_id);
                printf("to tail: %d   ", pointer->cell_id);
                if(count % 2 == 0){
                    printf(" \n");
                }
            }
                count++;
        }
        pointer = pointer->next_square;
    }
}

/**
 * @brief printing the board
 */
void print_board(){
    int board_size = board->rows * board->cols; //board size can be bigger than square amount in some circumstances(eg. no sqrt and no factors between 4 and 8)
    int rows = board->rows;
    int cols = board->cols;
    int last_cell;
    int array[board_size];
    struct square *pointer = NULL;
    int cell_id;
    pointer = board->start;
    //populate array with square ids 
    while(pointer != NULL){
        cell_id = pointer->cell_id;
        array[cell_id - 1] = cell_id;
        pointer = pointer->next_square;
        last_cell = cell_id ;
    }
    
    
    int off_set = (board_size - last_cell); // off set is the amount of spaces required
    // to fill the last row of the board if squares amount != board size 
    int start; // start of the row to be printed    
    int end; // end of the row to be printed
    for(int i = 1; i <= rows; i++){
        if(i == 1){ // if its the first row (row with the highest cell values)
            if(rows % 2 == 0){ // if the amount of rows is even
                //Start of the row will be bigger than end of the row
                start = board_size - off_set; 
                end = start - cols + 1 + off_set; 
            } else{
                //end of the row will be bigger than start of the row
                end = board_size - off_set;
                start = end - cols + 1 + off_set; 
                off_set = 0;// no offset since smaller numbers start from the left
            }
        }else if(i % 2 == 0){
            off_set = 0;
            if(rows % 2 == 0){
                start = board_size - ((i * cols) - 1); 
                end = start + cols - 1;

            }else {
                start = board_size - (((i - 1) * cols));
                end = start - cols + 1; 
            }
        } else{
            off_set = 0;
            if(rows % 2 == 0){
                start = board_size - (((i - 1) * cols));
                end = start - cols + 1;
            } else{
                end = board_size - (((i - 1) * cols));
                start = end - cols + 1;
            }
        }

        print_row(start, end, board_size , array, off_set);
    }
}

/**
 * @brief rolling the die function
 * @return returns the number rolled
 */
int roll_die(){
    int number = rand()%6 + 1;
    return number;
}

/**
 * @brief function to print the report to a file
 */
void print_to_report_file(){
    FILE *report_file;
    //player positions variables
    int rolled;
    int moved_from;
    int moved_to;
    int top_of_ladder;
    int snake_to;
    int moves = 0;
    int snake_count = 0;
    int ladder_count = 0;
    report_file = fopen("report_file.txt", "w"); // open file
    if (report_file == NULL) { // if the file doesnt exsist
        printf("\n The file could not be opened");
        printf("\n Exiting application!");
        return;
    }

    print_snakes(report_file);
    print_ladders(report_file);
    fprintf(report_file, "=====================\n");
    fprintf(report_file, "Gameplay report\n"); 
    fprintf(report_file, "=====================\n");

    for(int i = 0; i < board->moves; i++){ // 0 to moves made
        moved_from = board->path[i]->cell_id; //get the cell id of the square at path[i]
        moved_to = board->path[i + 1]->cell_id; //cell id of the square at path[i + 1]
        rolled = moved_to - moved_from;
        if(board->path[i + 1]->ladder_end_pointer !=NULL){//if the moved to square has a pointer to the top of a ladder
            //ladder move
            top_of_ladder = board->path[i + 1]->ladder_end_pointer->cell_id;
            //print to file
            fprintf(report_file, "Move: %d\n",moves + 1);
            fprintf(report_file, "Player rolled a: %d\n",rolled);
            fprintf(report_file, "Player moved from %d ",moved_from);
            fprintf(report_file, "to %d\n",moved_to);
            fprintf(report_file, "Player landed on a ladder and moved from %d ", moved_to);
            fprintf(report_file, "to %d\n", top_of_ladder);
            ladder_count++;
            i++;
        } else if(board->path[i + 1]->snake_tail_pointer !=NULL){ //if the moved to square has a pointer to a snake tail
            //snake move
            snake_to = board->path[i + 1]->snake_tail_pointer->cell_id;
            fprintf(report_file, "Move: %d\n",moves + 1);
            fprintf(report_file, "Player rolled a: %d\n",rolled);
            fprintf(report_file, "Player moved from %d ",moved_from);
            fprintf(report_file, "to %d\n",moved_to);
            fprintf(report_file, "Player landed on a snake head and moved from %d ", moved_to);
            fprintf(report_file, "to %d\n", snake_to);
            snake_count++;
            i++;
        } else{ // normal move(no snake or ladder)
            fprintf(report_file, "Move: %d\n",moves + 1);
            fprintf(report_file, "Player rolled a: %d\n",rolled);
            fprintf(report_file, "Player moved from %d ",moved_from);
            fprintf(report_file, "to %d\n",moved_to);
        }
        fprintf(report_file,"==================\n");
        if(board->path[i+1]->next_square == NULL){ // check if last path is the final square.
            fprintf(report_file, "Gameplay stats\n"); 
            fprintf(report_file, "Player reached the final square %d in %d moves\n" ,board->path[i+1]->cell_id, moves + 1);
            fprintf(report_file, "Player landed on %d snakes\n", snake_count);
            fprintf(report_file, "Player landed on %d ladders\n", ladder_count);
            break;
        }
        moves++;
    }

    //print stats
    
    fclose(report_file); // close file
}

void game_over(){
    print_to_report_file();
    printf("\nGAME OVER PLAYER REACHED THE FINAL SQUARE\n");
    printf("\nGame report printed to file\n");
}

void move_player_down_snake(){
    struct square *pointer =board->current;
    printf("Player landed on a snakes head.\n");
    printf("Moves from %d down to %d\n", pointer->cell_id, pointer->snake_tail_pointer->cell_id);
    board->moves++;
    pointer = pointer->snake_tail_pointer;//pointer is now at the tail of the snake
    board->path[board->moves] = pointer; //update player path 
    board->current = pointer;//update current pos
}

void move_player_up_ladder(){
    struct square *pointer =board->current ;
    printf("Player landed at the bottom of a ladder.\n");
    printf("Moves from %d up to %d\n", pointer->cell_id, pointer->ladder_end_pointer->cell_id);
    board->moves++; 
    pointer = pointer->ladder_end_pointer;//pointer is now at the top of the ladder 
    board->path[board->moves] = pointer; //track path
    board->current = pointer; // move 
}

/**
 * @brief function to move the player
 * 
 * @param move amount of spaces to move player 
 */
void move_player(int move){
    struct square *pointer = NULL;
    struct square *current = NULL;
    pointer = board->current;
    for(int i = 0; i < move; i++){
        if(pointer->next_square == NULL){
            board->moves ++;  
            board->path[board->moves] = pointer; //update path
            board->current = pointer; //set player position to new position
            printf("Current position is %d\n", pointer->cell_id);
            printf("\n");
            print_board();
            printf("\n");
            game_over();
            exit(0);
        } else{
            pointer = pointer->next_square;
        }
    }

    board->current = pointer;
    board->moves ++;  
    board->path[board->moves] = pointer; //update path
}

void main(){
    srand(time(NULL));
    int rows = 0; 
    int cols = 0;
    int square_amount = (rand()% (64 - 32 + 1)) + 32; // get random number for board size
    int max_snakes_and_ladders = (square_amount/2) - 5; //a snake and a ladder take 2 squares each.. 
    //-5 squares as randomising might not always find a square (and first and last square)

    get_board_rows_and_cols(square_amount, &rows, &cols);

    printf("\nBoard size %d\n", square_amount);
    printf("\nThe max amount of snakes and ladders combined is %d\n", max_snakes_and_ladders);

    board = generate_board(rows, cols);//init board

    add_squares_to_board(square_amount);//init squares

    //getting user input for the amount of snakes
    char amount_of_snakes_str[3];
    int amount_of_snakes = 0;
    while(1){ 
        printf("Enter the amount of snakes (max %d)", max_snakes_and_ladders - 1);
        printf("\n>>>");
        scanf(" %[^\n]", amount_of_snakes_str);
        amount_of_snakes = strtol(amount_of_snakes_str, NULL, 10);
        if(!(amount_of_snakes > 0 && amount_of_snakes <= max_snakes_and_ladders - 1)){
            printf("Invalid amount\n");
        }else break;
    }

    max_snakes_and_ladders-=amount_of_snakes;//decrease max amount of ladders 

    //getting user input for the amount of ladders
    char amount_of_ladders_str[3];
    int amount_of_ladders = 0;
    while(1){ 
        printf("Enter the amount of ladders (max %d)", max_snakes_and_ladders);
        printf("\n>>>");
        scanf(" %[^\n]", amount_of_ladders_str);
        amount_of_ladders = strtol(amount_of_ladders_str, NULL, 10);        
        if(!(amount_of_ladders > 0 && amount_of_ladders <= max_snakes_and_ladders)){
            printf("Invalid amount\n");;
        }else break;
    }

    //create snakes
    add_snakes(amount_of_snakes,square_amount);//init snakes
    //create ladders
    add_ladders(amount_of_ladders,square_amount);//int ladders

    int choice , prev, number;
    
    while(1){
        printf("\n");
        printf("1) Press 1 to roll die!!\n\n");
        printf("2) Press any other key to quit\n");
        printf(">>>");
        scanf("%d", &choice);//read user input 

        if(choice == 1){
            printf("\n");
            print_ladders(NULL);//display ladders to console
            printf("\n");
            print_snakes(NULL);//display snakes to console
            printf("\n");
            number = roll_die();
            printf("\nPlayer rolled a: %d \n", number);
            printf("Player moved from %d to %d\n", 
                board->current->cell_id,
                board->current->cell_id + number > square_amount ? square_amount : board->current->cell_id + number); //check to make sure player doesnt go past last square
            
            move_player(number);
            printf("\n");
            print_board();
            printf("\n");
            //snake check
            if(board->current->snake_tail_pointer !=NULL){ // player lands on a snake
                move_player_down_snake();
                printf("\n");
                print_board();
                printf("\n");
            } else if(board->current->ladder_end_pointer !=NULL){ //player lands on a ladder
                move_player_up_ladder();
                printf("\n");
                print_board();
                printf("\n");
            }else if(board->current->next_square == NULL){ // player lands on the last square
                game_over();
                exit(0);
            }
            //ladder check
            //game over check
            choice = 0;
        }  else{ // enters key other than 1
            print_to_report_file();
            printf("Exiting the game!");
            exit(0);
        }
    }
}