
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CARD_INTS 101

struct card{
    int winning_nums[MAX_CARD_INTS];
    int winning_len;
    int my_nums[MAX_CARD_INTS];
    int my_len;
    int matches;
    int id;
    int copies;
    struct card *next;
    struct card *previous;
};

void parse_card_line(char *source, struct card *dest_card);//takes in a card line and populates a struct with the correct data
void print_card(struct card *input);//prints an individual card struct
struct card* create_card( struct card *input);//creates a card with data from a card struct, but does not link the card (don't call this directly)
void add_card_node(struct card **head, struct card *input);//uses the above create card function to make a card and add it to the list
void print_card_list(struct card *head);//prints an entire linked list of cards
void card_list_pop(struct card **head);// delete the head of the linked list and set new head;
void split_card(char *source, int *left_nums, int *right_nums, int *left_count, int *right_count); //splits a card line into it's ints. Used in parse_card_line()
int get_total_cards(struct card *head);//gets total number of cards by counting the copies
int get_card_list_len(struct card *head);//get the total number of nodes in the linked list (does not count copies)
int get_card_matches(int *winning_nums, int winning_count, int *my_nums, int my_count);//takes in left and right side of a card line and calculates matches. used in parse_card_line()
void calculate_card_copies(struct card *head);// performs the bulk of the puzzle logic. Feed it a linked list of card structs, and adds copies based on logic


int main(){
    FILE *file;
    char line[200] = {0};
    struct card *head = NULL;

    file = fopen("input", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    //Go through line by line and add each card into a linked list of cards
    while (fgets(line, sizeof(line), file)) {
        struct card input_card;
        parse_card_line(line, &input_card);//parses the line and populates a card struct
        add_card_node(&head, &input_card);//adds that struct to a linked list
    }
    fclose(file); // Close the file

    calculate_card_copies(head); //go through the linked list perform the puzzle logic. Generate copies based 
    print_card_list(head);//Print the whole linked list. Just fun to look at
    printf("Total Cards: %d\n", get_total_cards(head)); // get_total_cards just traverses the list and adds the number of copies

    
    return 0;
}

void parse_card_line(char *source, struct card *dest_card){
    split_card(source, dest_card->winning_nums, dest_card->my_nums, &dest_card->winning_len, &dest_card->my_len);
    int card_id;
    sscanf(source, "Card %d:", &card_id);
    dest_card->id = card_id;
    dest_card->matches = get_card_matches(dest_card->winning_nums, dest_card->winning_len, dest_card->my_nums, dest_card->my_len);
    dest_card->copies = 1;
}


void print_card(struct card *input){
        printf("--------------------------\n");
        printf("Card ID: %d\n", input->id);
        printf("--------------------------\n");
        printf("Winning nums: ");
        for (int i = 0; i < input->winning_len; i++){printf("%d ",input->winning_nums[i]);}
        printf("\nWinning Len: %d", input->winning_len);
        printf("\nMy nums: ");
        for (int i = 0; i < input->my_len; i++){printf("%d ",input->my_nums[i]);}
        printf("\nMy Len: %d", input->my_len);
        printf("\nMatches: %d\n", input->matches);
        printf("Copies: %d\n", input->copies);
        printf("prev: %p\n", input->previous);
        printf("next: %p\n", input->next);
        printf("--------------------------\n");

}

struct card* create_card( struct card *input){
    struct card *newcard = (struct card*) malloc (sizeof(struct card));
    if (newcard == NULL){
        printf("error allocating memory");
        exit(1);
    }
    memcpy(newcard->winning_nums, input->winning_nums, sizeof(input->winning_nums));
    memcpy(newcard->my_nums, input->my_nums, sizeof(input->my_nums));
    newcard->winning_len = input->winning_len;
    newcard->my_len = input->my_len;
    newcard->matches = input->matches;
    newcard->id = input->id;
    newcard->previous = NULL;
    newcard->next = NULL;
    newcard->copies = input->copies;
    return newcard;
}

void add_card_node(struct card **head, struct card *input){
    struct card *newcard = create_card(input);
    newcard->next = *head;
    if (*head != NULL) {
        (*head)->previous = newcard;
    }
    *head = newcard;
    newcard->previous = NULL;
}

void print_card_list(struct card *head){
    struct card *current = head;
    while (current != NULL){
        print_card(current);
        printf("^^^^^^^^^^\nvvvvvvvvvv\n");
        current = current->next;
    }
}

void card_list_pop(struct card **head) {
    if (head == NULL || *head == NULL) {
        printf("List is already empty.\n");
        return;
    }

    struct card *temp = *head;
    *head = temp->next;
    (*head)->previous = NULL;
    free(temp);
}


void split_card(char *source, int *left_nums, int *right_nums, int *left_count, int *right_count){
    char *separator = strchr(source, '|');
    char *left_side = (strchr(source, ':') + 1);
    char *right_side = separator + 1;
    *separator = '\0';
    *left_count = 0;
    *right_count = 0;

    char *token = strtok(left_side, " "); //use <space> as delimiter
    while (token != NULL){
        left_nums[*left_count] = atoi(token);
        (*left_count)++;
        token = strtok(NULL, " ");
    }

    token = strtok(right_side, " ");
    while (token != NULL){
        right_nums[*right_count] = atoi(token);
        (*right_count)++;
        token = strtok(NULL, " ");
    }


}

int get_card_list_len(struct card *head){
    struct card *current = head;
    int length = 0;

    while (current != NULL){
        length++;
        current = current->next;
    }

    return length;
}

int get_total_cards(struct card *head){
    struct card *current = head;
    int total = 0;

    while (current != NULL){
        total += current->copies;
        current = current->next;
    }

    return total;
}

int get_card_matches(int *winning_nums, int winning_count, int *my_nums, int my_count){
    int matches = 0;
    //compare winning numbers to every number in my_nums
    for (int i = 0; i < winning_count; i++){
        for (int j = 0; j < my_count; j++){
            if (winning_nums[i] == my_nums[j]){
                matches += 1;
            }
        }
    }
    return matches;

}


void calculate_card_copies(struct card *head){
    struct card *current = head;
    struct card *acting = head;
    int acting_target_id = 0;
    
    //set current pointer to card id 1
    while (current->id != 1){
        current = current->next;
    }
    
    do{
        acting = current; // reset acting pointer to current
        acting_target_id = current->id + current->matches; // figure out how far forward we need to go and add copies to

        if (current->matches == 0){
            current = current->previous; // skip if there are no matches
            continue;
        }

        // add copies to cards as we traverse from current to target
        // repeat that for the copies of the current card
        for (int i = 0; i < current->copies; i++){
            while (acting->id != acting_target_id){
                acting = acting->previous;
                acting->copies += 1;
            }
            acting = current;
        }

        current = current->previous;// move up one ID and do it all over again, until we are out of cards.
    }while (current->id != head->id);
}
