#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

//example line: "Game 99: 11 red, 8 green; 16 red, 10 green; 9 red, 6 green; 3 blue, 2 red, 4 green"
int get_game_number(char *source);// Gets game number from line
bool is_game_possible(char *source, char **colors, int *max_colors);// Takes in a full line and outputs if game is possible
void get_colors(char *source, int *red_num, int *green_num, int *blue_num);// takes in line like "3 blue, 2 red, 4 green" and sets the numbers given to the counts of them (0 if nothing found)

int main(){
    //file open stuff
    FILE *file;
    char line[200] = {0};
    file = fopen("input", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    //setup values
    char *colors[] = {"red", "green", "blue"};
    int max_colors[] = {12, 13, 14};
    int total = 0;

    //Main loop of data
    while (fgets(line, sizeof(line), file)) {
        printf("===========\n");
        printf("line: %s \n", line);

        int game_number = get_game_number(line);
        int game_possible = is_game_possible(line, colors, max_colors);

        printf("game #: %d \n", game_number);
        printf("is possible output: %d\n", game_possible);

        if (game_possible){
            total = total + (game_number);
        }
        printf("running total: %d\n", total);
    }


    fclose(file);
    return 0;
}


int get_game_number(char *source){
    int gameNumber;
    if (sscanf(source, "Game %d:", &gameNumber) == 1) {
        return gameNumber;
    } else {
        return -1; // Error: Game number not found
    }

}

bool is_game_possible(char *source, char **colors, int *max_colors){
    char *pstart = strstr(source, ":") + 1;
    char *token = strtok(pstart, ";");

    //token in this loop will look like "3 blue, 2 red, 4 green"
    while (token != NULL){
        int  red_num = 0, green_num = 0, blue_num = 0;
        get_colors(token, &red_num, &green_num, &blue_num );

        if (red_num > max_colors[0] || green_num > max_colors[1] || blue_num > max_colors[2]) {
            return false;
        }

        token = strtok(NULL, ";");
    }

    return true;
}

void get_colors(char *source, int *red_num, int *green_num, int *blue_num ){
    char *saveptr;
    char *token = strtok_r(source, ",", &saveptr);

    //token in this loop will look like "3 blue"
    while (token != NULL){
        int num;
        char color[10];

        sscanf(token, " %d %s", &num, color);

        if (strcmp(color, "red") == 0){
            *red_num = num;
        } else if (strcmp(color, "blue") == 0) {
                *blue_num = num;
        } else if (strcmp(color, "green") == 0) {
                *green_num = num;
        }

        token = strtok_r(NULL, ",", &saveptr);
    }
}
