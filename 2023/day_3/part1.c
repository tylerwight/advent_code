#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define STRING_WIDTH 142 // X
#define FILE_LENGTH 140 // Y


int has_adjacent(char source[][STRING_WIDTH],int length, int x, int y);

int main(){
    FILE *file;
    char lines[FILE_LENGTH][STRING_WIDTH];
    int part_numbers[1000];
    int part_numbers_index = 0;
    int total = 0;
    char found_num[10];
    int final_num;
    int result = 0;

    //open a file to read our data
    file = fopen("input2", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    for (int i = 0; i < FILE_LENGTH; i++) {
        if (fgets(lines[i], sizeof(lines[i]), file) == NULL) {
            //printf("sizeof lines: %p\n", fgets(lines[i], sizeof(lines[i]), file));

            break; // Break the loop if there is nothing to read or an error occurs
        }
    }

    fclose(file);


    printf("data:\n");
    for (int i = 0; i < FILE_LENGTH; i++) {
        printf("%s", lines[i]);
    }

    printf("\n\n\n");

    for (int i = 0; i < FILE_LENGTH; i++) {
        printf("%s", lines[i]); // Use %s to print each row as a string


        for (int k = 0; k < STRING_WIDTH; k++){

            //printf("working on lines i[%d]k[%d]\n", i, k);

            if (lines[i][k] >= '0' && lines[i][k] <= '9'){
                //printf("found digit: %c \n", lines[i][k]);
                char *pstart = &lines[i][k];
                char *pend = pstart;

                for (int l = k; l < STRING_WIDTH; l++){
                    //printf("searching ahead %c \n", line[l]);

                    if ( !(lines[i][l] >= '0' && lines[i][k] <= '9')){
                        //printf(" found end. \n");
                        pend = &lines[i][l];
                        k = l;
                        break;
                    }
                }
                int found_num_length = pend-pstart;
                
                //printf("fnum len: %d\n", found_num_length); 
                //printf("full number: %.*s\n",found_num_length, pstart);
                memset(found_num, '\0', sizeof(found_num));
                strncpy(found_num, pstart, found_num_length);
                found_num[found_num_length+1] = '\0';
                final_num = atoi(found_num);
                //printf("number %d found at x[%d]y[%d]\n", final_num,(k - found_num_length), i);
                //printf("final_num: %d \n", final_num);
                

                for (int m = (k - found_num_length); m < k; m++){

                    //printf("sym check: x[%d]y[%d]\n", m, i);

                    if (has_adjacent(lines, FILE_LENGTH, m, i)){
                        printf("sym touching %d at x[%d]y[%d]\n",final_num, m, i);
                        part_numbers[part_numbers_index] = final_num;
                        part_numbers_index++;
                        break;
                    }


                }

            }
        }

    }


    //printf("char at 5, 5: %c\n", lines[5][5]);

    //has_adjacent(lines,FILE_LENGTH,5,5);
    printf("\nvalid part numbers: %d\n", part_numbers_index);

    for (int h = 0; h < part_numbers_index; h++){
        //printf("part number: %d\n", part_numbers[h]);
        result = result + part_numbers[h];
    }


    printf("Answer: %d \n", result);
    return 0;
}

int has_adjacent(char source[][STRING_WIDTH], int length, int x, int y){
    const char acceptable_chars[] = {'.', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}; // don't trigger as an adjacent
    int x_offset = 1;
    int y_offset = 1;
    int found_symbol = 0;

    if (y < 0 || y > length || x < 0 || x > STRING_WIDTH){
        printf("ERROR, OUT OF BOUNDS");
        return -1;
    }

    //printf("scanning: %d - %d: \n", x-x_offset, x+x_offset);
    for (int i = y-y_offset; i <= y+y_offset; i++){ // Y
        for (int j = x-x_offset; j <= x+x_offset; j++){ // X
            //printf("checking if %d < 0 || %d > %d || %d < 0 || %d > %d \n", i, i, length-1, j, j, STRING_WIDTH-3);
            if (i < 0 || i > length -1 || j < 0 || j > STRING_WIDTH -3){
                //printf("Edge detected\n");
                continue;
            }
            if (i == y && j == x){ // skip the center
                continue;
            }
            //printf("searching x[%d]y[%d] = %c \n", j, i, source[i][j]);

            int found_acceptable = 0;
            for (int k = 0; k < sizeof(acceptable_chars); k++){ // check 
                if (source[i][j] == acceptable_chars[k]){
                    found_acceptable = 1;
                    //printf("good symbol: %c\n", source[i][j]);
                    break;
                }
            }

            if (found_acceptable == 0){
                //printf("couldn't find an okay symbol, so it must be bad\n");
                //printf("bad symbol: %c\n", source[i][j]);
                found_symbol = 1;
            }


        }

    }

    //printf("\nin func: %c\n", source[y][x]);
    //printf("found symbol: %d\n", found_symbol);

    return found_symbol;
}