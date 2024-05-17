#include <stdio.h>
#include <string.h>

void print_int_array(int input[], int length);
int get_advent_int(char input[], int length);
char *replace_substring(char *source, const char *target, const char *replacement);
char *replace_numstring_with_digitchar(char *source);

int main(){
    FILE *file;
    char line[200] = {0};
    int total = 0;
    int result;

    //open a file to read our data
    file = fopen("input", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    //loop through the file line by line
    while (fgets(line, sizeof(line), file)) {
        int line_len = strlen(line);
        printf("===========\n");
        printf("working on line       : %s \n", line);
        //comment out replace_numstring... if you want to solution to part one
        //replaces all words that spell a number for part 2
        replace_numstring_with_digitchar(line);
        printf("line after str replace: %s \n", line);

        //takes the string and gets the left most and right most digits and concats them into a 2 digit integer
        result = get_advent_int(line, line_len);

        // add them all up
        total += result;
        printf("result: %d \n", result);
        printf("running total: %d\n", total);

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
    const char *digits[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    // I went through this with a string replacement solution. Based on the examples I thought I had to replace the strings in order and if they shared a letter, it broke the world and thus wasn't used:
    // this is in the example: 'eightwothree' and they say it represents 83. My code would turn 'eightwothree" into "8wo3". WHich does work in this case. But this isn't how they want it done
    // No test case really shows it well, so it's hard to pick out but for example "eighthree" should still be 83. My solution would turn 'eighthree' into '8hee' which turns into 88, but that is wrong
    // A test case like that should have been included imo
    // So, I wrote all of this going down the string substituion path when I shouldn't have.
    // This digits_replace var is a quick hacky way of turning my string subsitution into a working solution. 
    //Really I should rewrite the whole solution to just count and keep track of the digits made from words, rather than replace the string. But right now I'd rather move forward
    const char *digits_replace[] = {"1e", "2o", "3e", "4r", "5e", "6x", "7n", "8t", "9e"};
    char *pfound = NULL;
    int found_digit;

    //Keep searching string until we found nothing matching the digits[] string
    do{ 
        char *ptmp = NULL;
        pfound = NULL;

        //for each digit[i] search the line for that string
        for (int i = 0; i < 9; i++){

            ptmp = strstr(source, digits[i]);

            //if we found something, and our found variable hasn't been populated, add that one to found
            if (pfound == NULL && ptmp != NULL){
                pfound = ptmp;
                found_digit = i;
                printf("found string %s\n", digits[i]);
            }

            //if we found something, and it's earlier in the string that what we previously found, update to the earlier one
            if (ptmp < pfound && ptmp != NULL){
                pfound = ptmp;
                found_digit = i;
                printf("found string %s and it appears earlier\n ", digits[i]);
            }
            
        }
        //if we found something (should be earlier occurence), replace it
        if (pfound != NULL){
            printf("replacing '%s' with '%s' \n", digits[found_digit], digits_replace[found_digit]);
            replace_substring(source, digits[found_digit], digits_replace[found_digit] );
            printf("source looks like: '%s' \n", source);
        }
        printf("----\n");

    //if we did end up finding something, do it all over again, until we find nothing.
    }while(pfound != NULL);

}