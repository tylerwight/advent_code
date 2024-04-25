#include <stdio.h>
#include <string.h>

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

int main(){
    FILE *file;
    char line[100] = {0}; // Adjust the size as needed for your specific lines
    int total = 0;
    int result;

    file = fopen("input", "r"); // Open the file for reading
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        int line_len = strlen(line);
        result = get_advent_int(line, line_len);

        printf("===========\n");
        printf("working on line: %s", line);
        total += result;
        printf("result: %d \n", result);
        printf("running total: %d\n", total);
        printf("===========\n");
        
    }

    fclose(file); // Close the file
    return 0;
}