
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_MAP_LENGTH 100

struct map{
    long int dest[MAX_MAP_LENGTH];
    long int source[MAX_MAP_LENGTH];
    long int range[MAX_MAP_LENGTH];
    char name[50];
    int length;
};

struct seeds{
    long int seeds[500];
    long int range[500];
    int length;
};


void load_map(char *filename, char *map_name, struct map *destination); //Scans file until it finds the map name, then loads the data into a map struct
void load_seeds(char *filename, struct seeds *destination);//Scans file to find list of seeds, then loads the data into a seed struct
void print_map(struct map *input);//prints a map struct
void print_seeds(struct seeds *input);//prints a seed struct
long int map_seed(long int input_seed, struct map *input_map);// given an input seed, it traverses a map and outputs the mapped seed.


int main(){
    struct map maps[7];
    struct seeds seeds;
    char filename[] = "input";

    //load in all the data
    load_seeds(filename, &seeds);
    load_map(filename, "seed-to-soil", &maps[0]);
    load_map(filename, "soil-to-fertilizer", &maps[1]);
    load_map(filename, "fertilizer-to-water", &maps[2]);
    load_map(filename, "water-to-light", &maps[3]);
    load_map(filename, "light-to-temperature", &maps[4]);
    load_map(filename, "temperature-to-humidity", &maps[5]);
    load_map(filename, "humidity-to-location", &maps[6]);

    //for each seed, run it through all the maps
    long int answer = 0;
    

    //UH OH
    //TAKES 6 MINUTES TO COMPLETE CAUSE I'M BAD
    for (int i = 0; i < seeds.length; i++){
        printf("----\nworking index: %d, seed: %ld range: %ld\n----\n", i, seeds.seeds[i], seeds.range[i]);
        long int lowest_in_range = 0;
        for (int k = 0; k < seeds.range[i]; k++){
            long int working_seed = seeds.seeds[i] + k;
            //printf("\t====\n\tworking on seed %ld\n\t====\n", working_seed);

            for (int j = 0; j < 7; j++){
                long int mapped_seed = map_seed(working_seed, &maps[j]);
                //printf("\tseed: %ld mapped to %ld\n", working_seed, mapped_seed );
                working_seed = mapped_seed;
            }
            if (lowest_in_range == 0 || working_seed < lowest_in_range){
                lowest_in_range = working_seed;
            }

        }
        printf("lowest in range: %ld\n", lowest_in_range);


        if (answer == 0 || lowest_in_range < answer){
            answer = lowest_in_range;
        }
    }



    printf("answer: %ld\n", answer);
    return 0;
}





long int map_seed(long int input_seed, struct map *input_map){
    long int mapped_seed = 0;
    int found_map = 0;

    //loop through all lines of the map to see if seed is in mappable range (if any)
    for (int i = 0; i < input_map->length; i++){
        long int source_min = input_map->source[i];
        long int source_max = input_map->source[i] + input_map->range[i] - 1;
        long int destination_min = input_map->dest[i];

        if (input_seed >= source_min && input_seed <= source_max){
            mapped_seed = input_seed + (destination_min - source_min);//seed is in mappable range, add offset
            found_map = 1;
            break;
        }
    }


    if (found_map){
        return mapped_seed;   
    } else {
        return input_seed;
    }
}

void load_map(char *filename, char *map_name, struct map *destination_map){
    FILE *file;
    
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[200] = {0};
    int index = 0;
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        long int dest = 0, source = 0, range = 0;
        char *psearch = strstr(line, map_name);

        // ifstrstr found the map_name in this line, set our map name and set found, we are ready to read data
        if (psearch != NULL && found == 0){
            found = 1;
            strncpy(destination_map->name, map_name, strlen(map_name));
            destination_map->name[strlen(map_name)] = '\0';
            continue;
        }

        //When found is 1 we know we should be reading the data
        //Read the data until we run out of lines with numbers
        if (found ==1){
            if (!isdigit(line[0])){
                destination_map->length = index;
                break;
            }

            sscanf(line, " %ld %ld %ld", &dest, &source, &range);
            destination_map->dest[index] = dest;
            destination_map->source[index] = source;
            destination_map->range[index] = range;
            index +=1;

        }

    }


    fclose(file);
}

void print_map(struct map *input){
    printf("Map Name: %s\n", input->name);
    for (int i = 0; i < input->length; i++){
        printf("Dest: %ld, Source: %ld, Range: %ld\n", input->dest[i], input->source[i], input->range[i]);
    }
    printf("\n");
}

void load_seeds(char *filename, struct seeds *destination){
    FILE *file; 
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[300] = {0};
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "seeds") == NULL){
            break;
        }

        char *pstart = strstr(line, ":") + 1;
        char *token = strtok(pstart, " ");
        int iteration = 0;
        int length = 0;
        long int seed = 0;
        long int range = 0;

        while (token != NULL){
            printf("TOKEN: %s\n", token);

            if (iteration % 2 == 0){
                sscanf(token, " %ld", &seed);
                printf("loaded seed %ld\n", seed);
                destination->seeds[length] = seed;
            } else {
                sscanf(token, " %ld", &range);
                printf("loaded range %ld\n", range);
                destination->range[length] = range;
                length++;
            }

            iteration++;
            token = strtok(NULL, " ");
        }
        destination->length = length;
    }


    fclose(file); // Close the file

}

void print_seeds(struct seeds *input){
    for (int i = 0; i < input->length; i++){
        printf("Seed: %ld, Range: %ld\n", input->seeds[i], input->range[i]);
    }
    printf("\n");
}









// void load_seeds(char *filename, struct seeds *destination){
//     FILE *file; 
//     file = fopen(filename, "r");
//     if (file == NULL) {
//         perror("Error opening file");
//         return;
//     }

//     char line[200] = {0};
//     while (fgets(line, sizeof(line), file)) {
//         if (strstr(line, "seeds") == NULL){
//             break;
//         }

//         char *pstart = strstr(line, ":") + 1;
//         char *token = strtok(pstart, " ");
//         int iteration = 0;
//         int length = 0;
//         long int seed = 0;
//         long int range = 0;

//         while (token != NULL){
//             printf("TOKEN: %s\n", token);

//             if (iteration % 2 == 0){
//                 sscanf(token, " %ld", &seed);
//                 printf("loaded seed %ld\n", seed);
//             } else {
//                 sscanf(token, " %ld", &range);
//                 printf("loaded range %ld\n", range);
//                 for (int i = 0; i < range; i++){
//                     destination->seeds[length] = seed + i;
//                     //printf("dest->seeds = %ld\n", destination->seeds[length]);
//                     length++;
//                 }
//             }

//             iteration++;
//             token = strtok(NULL, " ");
//         }
//         destination->length = length;
//     }


//     fclose(file); // Close the file

// }