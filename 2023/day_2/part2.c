#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>



int reverse_string(char *source);
int get_game_power(char *source, char **colors, int *max_colors, int length);

void split_game(char *source, int *left_nums, int *right_nums, int *left_count, int *right_count){
    char *separator = strchr(source, '|');
    char *left_side = (strchr(source, ':') + 1);
    char *right_side = separator + 1;
    *separator = '\0';
    *left_count = 0;
    *right_count = 0;

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


int main(){
    FILE *file;
    char line[200] = {0};

    //open a file to read our data
    file = fopen("input", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }


    char *colors[] = {"red", "green", "blue"};
    int max_colors[] = {12, 13, 14};
    int total = 0;


    //loop through the file line by line
    while (fgets(line, sizeof(line), file)) {
        int line_len = strlen(line);

        printf("===========\n");
        printf("line: %s \n", line);

        int game_power = get_game_power(line, colors, max_colors, 3);
        printf("game power: %d\n", game_power);

        total = total + (game_power);
        printf("running total: %d\n", total);

        
    }

    fclose(file); // Close the file
    return 0;
}


int reverse_string(char *source){
    int length = strlen(source);

    for (int i = 0, j = length - 1; i <= j; i++, j--) {
        char c = source[i];
        source[i] = source[j];
        source[j] = c;
    }

    return 0;
}

//needs refactored, this is ugly and bad
int get_game_power(char *source, char **colors, int *max_colors, int length){    
    char *pstart = strstr(source, ":") +1;
    int redbiggest = 1, bluebiggest = 1, greenbiggest = 1;

    
    int loop_done = 0;
    while(loop_done != 1) {
        char *next_semi = strstr(pstart,";");
        int working_str_len = (next_semi - pstart);
        char working_str[50] = {'\0'};

        if (strstr(pstart, ";") == NULL){
            loop_done = 1;
            strncpy(working_str, (pstart +1), strlen(pstart)-1);
        } else{
            strncpy(working_str, (pstart + 1), (working_str_len - 1));
        }


        
        char *deeper_str_start = working_str;
        int deeper_loop_done = 0;
        
    
        while (deeper_loop_done != 1){
            char *next_comma_working_str = (strstr(deeper_str_start, ","));
            int segment_len = (next_comma_working_str - deeper_str_start);
            char deeper_working_string[50] = {'\0'};


            if (strstr(deeper_str_start + 1, ",") == NULL){
                strncpy(deeper_working_string, deeper_str_start, strlen(deeper_str_start));
                deeper_loop_done = 1;

            } else{
                strncpy(deeper_working_string,deeper_str_start, segment_len);
            }

            //printf("\n-----\nDeep str: %s\n-----\n", deeper_working_string);

            for (int i = 0; i < length; i++){
                if (strstr(deeper_working_string, colors[i])){
                    int colornum = atoi(deeper_working_string);
                    //printf("Color: %s\n", colors[i]);
                    //printf("Number: %d\n", colornum);

                    if (colors[i] == "red"){
                        if (colornum > redbiggest){
                            redbiggest = colornum;
                        }
                    }
                    if (colors[i] == "blue"){
                        if (colornum > bluebiggest){
                            bluebiggest = colornum;
                        }
                    }
                    if (colors[i] == "green"){
                        if (colornum > greenbiggest){
                            greenbiggest = colornum;
                        }
                    }


                }
            }
            if (deeper_loop_done != 1){
                deeper_str_start = next_comma_working_str + 2;
            }
            

        }


        if (loop_done != 1){
            pstart = next_semi + 1;
        }
    }

    printf("Found maximums of %d red, %d blue, %d green\n", redbiggest, bluebiggest, greenbiggest);
    return (redbiggest * greenbiggest * bluebiggest);
}
