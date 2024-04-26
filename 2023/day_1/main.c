#include <stdio.h>
#include <string.h>

void print_int_array(int input[], int length);
int get_advent_int(char input[], int length);
char *replace_substring(char *source, const char *target, const char *replacement);

int main(){
    FILE *file;
    char line[100] = {0};
    int total = 0;
    int result;

    file = fopen("small_input", "r"); // Open the file for reading
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        int line_len = strlen(line);
        printf("===========\n");
        printf("working on line: %s \n", line);

        replace_substring(line, "one", "1");
        replace_substring(line, "two", "2");
        replace_substring(line, "three", "3");
        replace_substring(line, "four", "4");
        replace_substring(line, "five", "5");
        replace_substring(line, "six", "6");
        replace_substring(line, "seven", "7");
        replace_substring(line, "eight", "8");
        replace_substring(line, "nine", "9");

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

        printf("source: %s \n", source);
        printf("source addr: %p \n", source);
        printf("strfound addr: %p \n", p);

        //because we are in the loop we know we found the substr(target) in here somewhere (source)
        size_t target_len = strlen(target);
        size_t replacement_len = strlen(replacement);

        //if the target and what we want to replace it are different lengths, we need to move memory to close the gap. Otherwise 1two3two1 could turn into 12wo32wo1 (moving from "two" to "2" leaves 3 leftover spaces, so we memmove them to bridge the gap)
        if (target_len != replacement_len){
            memmove(p + replacement_len, p + target_len, strlen(p+target_len) +1);
        }

        //using the length of the replacement string (in our example "two"s length is 3), start at memory address p (the start of the matching target screen found by strstr) and put the contnets of replacement there, only moving the length of replacement (3)
        memcpy(p, replacement, replacement_len);



        printf("target: %s \n", target);
        //printf("target len: %ld \n", strlen(target));
        printf("replacement: %s \n - \n", replacement);
    }

}