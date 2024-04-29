#include <stdio.h>
#include <string.h>



int main(){
    FILE *file;
    char line[200] = {0};
    int total = 0;
    int result;

    //open a file to read our data
    file = fopen("small_input", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    //loop through the file line by line
    while (fgets(line, sizeof(line), file)) {
        int line_len = strlen(line);
        ///printf("===========\n");
        printf("working on line       : %s \n", line);

        for (int i = 0; i < line_len; i++){

            if (line[i] > '0' && line[i] < '9'){
                //printf("found digit: %c \n", line[i]);
                char *pstart = &line[i];
                char *pend = pstart;

                for (int j = i; j < line_len; j++){
                    //printf("searching ahead %c \n", line[j]);

                    if ( !(line[j] > '0' && line[i] < '9')){
                        //printf(" found end. \n");
                        pend = &line[j];
                        i = j;
                        break;
                    }
                }
                int found_num_length = pend-pstart;
                //printf("pend - pstart: %d\n", found_num_length); 
                printf("full number: %.*s\n",found_num_length, pstart);








            }
        }

    }

    fclose(file);
    return 0;
}