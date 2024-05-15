
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CARD_INTS 101


void split_card(char *source, int *left_nums, int *right_nums, int *left_count, int *right_count);
int get_card_score(int *winning_nums, int winning_count, int *my_nums, int my_count);

int main(){
    FILE *file;
    char line[200] = {0};
    int final_score = 0;

    file = fopen("input", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }


    //loop through the file line by line
    while (fgets(line, sizeof(line), file)) {
        int line_len = strlen(line);
        int winning_numbers[MAX_CARD_INTS] = {0};
        int winning_numbers_count = 0;
        int my_numbers[MAX_CARD_INTS] = {0};
        int my_numbers_count = 0;
        
        line[strcspn(line,"\n")] = 0; //strip off the \n for prettier printing
        printf("===========\nWORKING ON : %s \n===========\n", line);

        split_card(line, winning_numbers, my_numbers, &winning_numbers_count, &my_numbers_count); // split line into two int arrays (winning and my), keep track of length
        int score = get_card_score(winning_numbers, winning_numbers_count, my_numbers, my_numbers_count); //Use two int arrays (and their length) to get score

        printf("--------\nCARD SCORE: %d\n--------\n\n", score);
        final_score = final_score + score; //        
    }

    printf("\n\nXXXXXXXXXXX\nFINAL SCORE: %d\nXXXXXXXXXXX\n", final_score);
    fclose(file); // Close the file
    return 0;

}



void split_card(char *source, int *left_nums, int *right_nums, int *left_count, int *right_count){
    char *separator = strchr(source, '|');
    char *left_side = (strchr(source, ':') + 1);
    char *right_side = separator + 1;
    *separator = '\0';

    char *token = strtok(left_side, " "); //use <space> as delimiter
    while (token != NULL){
        left_nums[*left_count] = atoi(token);
        (*left_count)++;
        token = strtok(NULL, " ");
    }

    token = strtok(right_side, " ");
    while (token != NULL){
        right_nums[*right_count] = atoi(token);
        (*right_count)++;
        token = strtok(NULL, " ");
    }


}

int get_card_score(int *winning_nums, int winning_count, int *my_nums, int my_count){
    int score = 0;
    //compare winning numbers to every number in my_nums
    for (int i = 0; i < winning_count; i++){
        for (int j = 0; j < my_count; j++){
            if (winning_nums[i] == my_nums[j]){
                printf("FOUND MATCH:  %d and %d \n", winning_nums[i], my_nums[j]);
                if (score == 0){
                    score = 1;
                } else {
                    score = score * 2;
                }

            }
        }
    }
    return score;

}

/*
        printf("winning_numbers: ");
        for (int i = 0; i < winning_numbers_count; i++){
            printf("%d ",winning_numbers[i]);
        }
        printf("\n");
        printf("my_numbers: ");
        for (int i = 0; i < my_numbers_count; i++){
            printf("%d ", my_numbers[i]);
        }
        printf("\n");

*/