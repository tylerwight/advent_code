#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>



int get_game_number(char *source);
int reverse_string(char *source);
bool is_game_possible(char *source, char **colors, int *max_colors, int length);


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

        int game_number = get_game_number(line);
        int game_possible = is_game_possible(line, colors, max_colors, 3);
        
        printf("game #: %d \n", game_number);
        printf("is possible output: %d\n", game_possible);

        if (game_possible){
            total = total + (game_number);
        }
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

int get_game_number(char *source){
    char *pfound = NULL;
    pfound = strstr(source, ":"); // search for ":" and work backwards. This symbol only occures once in our data, and is right after the game number
    int length = pfound - source; // get the length of chars it takes to reach :, so we don't search the whole string
    
    char game_number[10] = {'\0'}; //at max we support 10 digits
    int found_digits = 0; // keep track of how many digits we found, to put them in our variable in the right place

    //loop through the source string until we hit the :
    for (int i = 0; i < length; i++){
        
        //if we see a digit add it to to game_numbers
        if (isdigit(source[i])){
            game_number[found_digits] = source[i];
            found_digits++;
        }
    }
    
    if (strlen(game_number) > 0){
        return atoi(game_number); //convert from string to int and return if we found numbers
    }
    return 0;  //could not find any the game number

}


bool is_game_possible(char *source, char **colors, int *max_colors, int length){
    for (int i = 0; i < length; i++){
        printf("Maxium: %s, %d\n", colors[i], max_colors[i]);

    }
    
    
    char *pstart = strstr(source, ":") +1;
    //printf("PSTART 1: %s\n", pstart);
    
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
        printf("working on this iteration: %s\n", working_str);

        

        char *deeper_str_start = working_str;
        int deeper_loop_done = 0;
    
        while (deeper_loop_done != 1){
            char *next_comma_working_str = (strstr(deeper_str_start, ","));
            int segment_len = (next_comma_working_str - deeper_str_start);
            char deeper_working_string[50] = {'\0'};

            //printf("segment_len: %d\n", segment_len);
            ///printf("deeper_str_start: %s\n", deeper_str_start);
            ///printf("next_comma_working_str: %s\n", next_comma_working_str);

            if (strstr(deeper_str_start + 1, ",") == NULL){
                strncpy(deeper_working_string, deeper_str_start, strlen(deeper_str_start));
                deeper_loop_done = 1;

            } else{
                strncpy(deeper_working_string,deeper_str_start, segment_len);
            }

            //printf("found deeper str: %s\n", deeper_working_string);

            //printf("atoi of str: %d\n", atoi(deeper_working_string));

            for (int i = 0; i < length; i++){
                if (strstr(deeper_working_string, colors[i])){
                    //printf("deeperstr contains: %s\n", colors[i]);
                    if ( atoi(deeper_working_string) > max_colors[i]){
                        printf("%s has exceeded it's max of %d, we saw %d\n", colors[i], max_colors[i], atoi(deeper_working_string));
                        return false;
                    }
                }
            }
            if (deeper_loop_done != 1){
                deeper_str_start = next_comma_working_str + 1;
            }
            

        }




        if (loop_done != 1){
            pstart = next_semi + 1;
        }
    }
    return true;
}
