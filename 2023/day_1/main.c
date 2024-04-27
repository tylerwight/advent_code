#include <stdio.h>
#include <string.h>

void print_int_array(int input[], int length);
int get_advent_int(char input[], int length);
char *replace_substring(char *source, const char *target, const char *replacement);
char *replace_numstring_with_digitchar(char *source);

int main(){
    FILE *file;
    char line[100] = {0};
    const char *digits[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int total = 0;
    int result;

    file = fopen("input", "r"); // Open the file for reading
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        int line_len = strlen(line);
        printf("===========\n");
        printf("working on line: %s \n", line);

        char *pfound = NULL;
        int found_digit;
        do{ 
            char *ptmp = NULL;
            pfound = NULL;
            //printf("line looks like this: %s\n", line);
            for (int i = 0; i < 9; i++){
                // printf("working on index %d\n", i);
                //printf("Working on digit: %s \n", digits[i]);

                ptmp = strstr(line, digits[i]);

                if (pfound == NULL && ptmp != NULL){
                    pfound = ptmp;
                    found_digit = i;
                    printf("found string %s\n", digits[i]);
                }

                if (ptmp < pfound && ptmp != NULL){
                    //printf("tmp %p   <    found %p \n", ptmp, pfound);
                    pfound = ptmp;
                    found_digit = i;
                    printf("found string %s and it appears earlier\n ", digits[i]);
                }
                //printf("found ptr: %p\n ", pfound);
                //printf("found ptr: %d", *pfound);
                
            }
            
            char c[2];
            c[0] = (found_digit + 1 + '0');
            c[1] = '\0';

            if (pfound != NULL){
                printf("replacing '%s' with '%s' \n", digits[found_digit], c);
                replace_substring(line, digits[found_digit], c );
                printf("line looks like: '%s' \n", line);
            }
            printf("----\n");

        }while(pfound != NULL);

        printf("line after str replace: %s \n", line);

        result = get_advent_int(line, line_len);


        total += result;
        printf("result: %d \n", result);
        printf("running total: %d\n", total);
        printf("adress of line: %p \n", line);

        printf("===========\n");
        
    }

    fclose(file); // Close the file
    return 0;
}


void print_int_array(int input[], int length){
    for (int i = 0; i < length; i++){
        printf("%d", input[i]);
    }
    printf("\n");
}

int get_advent_int(char input[], int length){
    int digits[20];
    int current_size = 0;
    
    //loop through the string and pull out all char digits into an array and convert to int
    for (int i = 0; i < length; i++){
        //if char is an ascii digit
        if (input[i] >= '0' && input[i] <= '9'){
            digits[current_size] = input[i] - 48;
            //current_size keeps track of how many digits we put in
            current_size++;
        }
    }

    //if we only found one digit, use that digit for both 
    if (current_size == 1){
        return (digits[0] + 10*digits[0]);
    }

    //return the right most digit (index current_size) in 1s place, and left most digit (index 0) in 10s place
    return (digits[current_size-1] + 10*digits[0]);
}

char *replace_substring(char *source, const  char *target, const  char *replacement){
    //source = pointer to string you want to change
    // target = string you want to find
    // replacement = what to replace target with
    // example: source = '1two3two1', target='two', replacement='2' turns source into '12321'
    
    //keep looping while we can find taret in source (if strstr returns null, it didn't find anything)
    while (strstr(source, target) != NULL){
        //create ptr that points to first char of the found target (inside of source)
        char *p = strstr(source, target);

        //printf("source: %s \n", source);
        //printf("source addr: %p \n", source);
        //printf("strfound addr: %p \n", p);

        //because we are in the loop we know we found the substr(target) in here somewhere (source)
        size_t target_len = strlen(target);
        size_t replacement_len = strlen(replacement);

        //if the target and what we want to replace it are different lengths, we need to move memory to close the gap. Otherwise 1two3two1 could turn into 12wo32wo1 (moving from "two" to "2" leaves 3 leftover spaces, so we memmove them to bridge the gap)
        if (target_len != replacement_len){
            memmove(p + replacement_len, p + target_len, strlen(p+target_len) +1);
        }

        //using the length of the replacement string (in our example "two"s length is 3), start at memory address p (the start of the matching target screen found by strstr) and put the contnets of replacement there, only moving the length of replacement (3)
        memcpy(p, replacement, replacement_len);



        //printf("target: %s \n", target);
        //printf("target len: %ld \n", strlen(target));
        //printf("replacement: %s \n - \n", replacement);
    }

}


char *replace_numstring_with_digitchar(char *source) {
    printf("hey");
}