#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STRING_WIDTH 142 // X
#define FILE_LENGTH 140 // Y

struct coords {
    int x;
    int y;
};

struct gridnumber {
    int number;
    int digits;
    struct coords numbercoords;
};


int get_line_numbers(char source[FILE_LENGTH][STRING_WIDTH], int line_num, struct gridnumber *destination);



int main(){
    FILE *file;
    char lines[FILE_LENGTH][STRING_WIDTH];

    // open a file
    file = fopen("input2", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    //read all the lines into a 2D array
    for (int i = 0; i < FILE_LENGTH; i++) {
        if (fgets(lines[i], sizeof(lines[i]), file) == NULL) {
            break;
        }
    }
    fclose(file);


    // print out all the data we collected:
    // struct coords readdata;
    // printf("data:\n");
    // for ( readdata.y = 0; readdata.y < FILE_LENGTH; readdata.y++) {
    //     printf("%s", lines[readdata.y]);
    // }
    // printf("\n");



    struct coords loop;
    int total_gear_ratios = 0;;
    
    //go through every character in the data and search for a * symbol
    for (loop.y = 0; loop.y < FILE_LENGTH; loop.y++){
        for (loop.x = 0; loop.x < FILE_LENGTH; loop.x++){

            if (lines[loop.y][loop.x] == '*'){
                printf("\nfound a * at coords [%d][%d]\n", loop.x, loop.y);


                //if we find a *, check the lines above, on, and below it for numbers
                // get their xy coords so we can check if they are touching *
                struct gridnumber line1[20];
                struct gridnumber line2[20];
                struct gridnumber line3[20];
                int line1_length = get_line_numbers(lines, loop.y-1, line1);
                int line2_length = get_line_numbers(lines, loop.y, line2);
                int line3_length = get_line_numbers(lines, loop.y+1, line3);
                int touching_numbers[20];
                int total_touching_numbers = 0;



                //loop through all the data we got per line
                // 3 loops, one for each set of line data 
                for (int i = 0; i < line1_length; i++){
                    // check if the numbers XY coords are touching the *
                    // because numbers can be up to 3 digits and our X/Y coords only points to first digit we gotta do some mathing
                    if (( loop.x - line1[i].numbercoords.x <= line1[i].digits) && ( loop.x - line1[i].numbercoords.x >= -1)){ 
                        printf("    Touching Number: %d  at location X: %d, Y: %d and they have %d digits\n", line1[i].number, line1[i].numbercoords.x, line1[i].numbercoords.y, line1[i].digits);
                        touching_numbers[total_touching_numbers] = line1[i].number;
                        total_touching_numbers += 1;
                    }
                }

                for (int i = 0; i < line2_length; i++){
                    if (( loop.x - line2[i].numbercoords.x <= line2[i].digits) && ( loop.x - line2[i].numbercoords.x >= -1)){
                        printf("    Touching Number: %d at location X: %d, Y: %d and they have %d digits\n", line2[i].number, line2[i].numbercoords.x, line2[i].numbercoords.y, line2[i].digits);
                        touching_numbers[total_touching_numbers] = line2[i].number;
                        total_touching_numbers += 1;
                    }
                }
                for (int i = 0; i < line3_length; i++){
                    if (( loop.x - line3[i].numbercoords.x <= line3[i].digits) && ( loop.x - line3[i].numbercoords.x >= -1)){
                        printf("    Touching Number: %d at location X: %d, Y: %d and they have %d digits\n", line3[i].number, line3[i].numbercoords.x, line3[i].numbercoords.y, line3[i].digits);
                        touching_numbers[total_touching_numbers] = line3[i].number;
                        total_touching_numbers += 1;

                    }
                }

                //printf("    total touching numbers: %d \n", total_touching_numbers);
                
                if (total_touching_numbers == 2){
                    int gear_ratio = touching_numbers[0] * touching_numbers[1];
                    printf("\nFOUND A GEAR! ratio %d made from %d * %d\n", gear_ratio, touching_numbers[0], touching_numbers[1]);
                    total_gear_ratios += gear_ratio;
                }



            }
        }
        
    }




    printf("\nTOTAL GEAR RATIOS: %d\n", total_gear_ratios);
    return 0;
}




//go through the specified line from the data
//Get every number on the line and it's X Y coordinates
// X Y coordinates are for the left most digit in a found number
// data is stored in a gridnumber struct
// returns how many numbers it found.
int get_line_numbers(char source[FILE_LENGTH][STRING_WIDTH], int line_num, struct gridnumber *destination){
    struct coords loop;
    int total_nums = 0;
    loop.x = 0;
    loop.y = line_num;

    for (loop.x = 0; loop.x < STRING_WIDTH-1; loop.x++){

        if (source[loop.y][loop.x] >= '0' && source[loop.y][loop.x] <= '9'){
            bool full_num_found = false;
            int search_breadth = 4;
            int found_digits = 0;
            int loop_length = (loop.x + search_breadth);

            destination->numbercoords.x = loop.x; //mark the coords of the number on the first digit we found.
            destination->numbercoords.y = loop.y;

            //we found a digit, now we need to search forward until we reach the end of the number.
            for (int i = loop.x; i < loop_length; i++){

                // if it's a digit, track it and keep going. If it's the first digit we've found, track
                if (source[loop.y][i] >= '0' && source[loop.y][i] <= '9'){
                    found_digits += 1;
                    continue;
                }

                // if it's not a digit, we've reached the end of the number. Store it's info
                if ((i >= STRING_WIDTH-2) || (source[loop.y][i] < '0' || source[loop.y][i] > '9')){
                    char tmp[10];
                    strncpy(tmp, &source[loop.y][loop.x], found_digits);
                    tmp[found_digits] = '\0';

                    destination->number = (atoi(tmp));
                    destination->digits = found_digits;
                    //printf("we found %d at location X: %d, Y: %d\n", destination->number, destination->numbercoords.x, destination->numbercoords.y);

                    loop.x = i;
                    found_digits = 0;
                    total_nums += 1;
                    destination += 1;
                    break;

                }
            }

        }

    }
    return total_nums;

}
