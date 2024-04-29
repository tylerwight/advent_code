#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the node structure
typedef struct Node {
    char *data;          // Pointer to the string data
    struct Node *next;   // Pointer to the next node
} Node;

// Function to create a new node
Node* createNode(const char *str) {
    Node *newNode = (Node*) malloc(sizeof(Node));  // Allocate memory for the node
    if (newNode == NULL) {
        fprintf(stderr, "Unable to allocate memory for new node\n");
        exit(1);
    }
    newNode->data = strdup(str);  // Copy the string into the node
    newNode->next = NULL;
    return newNode;
}

// Function to print the data of a node at a given index
void printNodeAt(Node *head, int index) {
    int currentIndex = 0;
    while (head != NULL && currentIndex < index) {
        head = head->next;
        currentIndex++;
    }
    if (head != NULL) {
        printf("Node at index %d: %s\n", index, head->data);
    } else {
        printf("No node found at index %d\n", index);
    }
}


char* getNodeData(Node *head, int index) {
    int currentIndex = 0;
    while (head != NULL && currentIndex < index) {
        head = head->next;
        currentIndex++;
    }
    if (head != NULL) {
        return head->data;
    } else {
        return NULL; // or handle the error accordingly
    }
}


// Function to add a node to the beginning of the list
void addNode(Node **head, const char *str) {
    Node *newNode = createNode(str); // Create a new node
    newNode->next = *head;           // Point it to the current head
    *head = newNode;                 // Update head to the new node
}

// Function to print the linked list
void printList(Node *head) {
    while (head != NULL) {
        printf("%s -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

int countNodes(Node *head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

// Function to free the linked list
void freeList(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->data); // Free the string data
        free(temp);       // Free the node
    }
}

int main() {
    Node *head = NULL; // Start with an empty list
    FILE *file;
    char line[200] = {0};

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
        //printf("working on line       : %s \n", line);

        addNode(&head, line);


    }
    fclose(file);

    int nodeCount = countNodes(head);
    printf("The list has %d nodes.\n", nodeCount);

    for(int i = nodeCount; i >= 0; i--){
        char *data = getNodeData(head, i);

        printf("node %d : %s \n", i, data);
    }


    //printList(head); // Print the list
    freeList(head);  // Free all memory used by the list

    return 0;
}
