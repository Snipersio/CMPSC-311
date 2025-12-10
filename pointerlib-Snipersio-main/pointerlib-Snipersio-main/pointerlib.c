#include "pointerlib.h"


/*
This is the implementation of the functions in pointerlib.h. 
Please read the comments in pointerlib.h before you start working on this file.
*/

// Memory preallocated for 100 nodes; DO NOT MODIFY THIS ARRAY
list_node_t NODE_LIST[MAX_NODE_LIST_SIZE] = {0};


int swap_int_values(int* a, int* b) { //swaps the value of two integers
    // TODO
    if (a == NULL || b == NULL){ //Invalid input pointers
	    return -1;
    }
    int temp = *a; //uses a temp variable to change pointer values
    *a = *b;
    *b = temp;
    return 0;
}


list_node_t* create_node(int data) { //Creates a new node pointer
    // TODO
    for (int i = 0; i < MAX_NODE_LIST_SIZE; i++){ //itterates through all nodes in list
	   if (NODE_LIST[i].used == false){ //If node is not being used
		 NODE_LIST[i].data = data; //Changing data to make node useable
		 NODE_LIST[i].used = true;
		 NODE_LIST[i].next = NULL;
		 return &NODE_LIST[i]; //Returns the address of the new node
	   }
    }
   return NULL; 
}

int destroy_node(list_node_t* node) { //Removes a node
    // TODO
    if(node == NULL){ //node is invalid
	    return -1;
    }
    node->data = 0; //Changing node to not be part of the linked list anymore
    node->used = false;
    node->next = NULL;
    return 0;
}

int link_nodes(list_node_t* node1, list_node_t* node2) { //Linkes 2 nodes togeather
    // TODO
    if (node1 == NULL){ //head node can't be NULL
	    return -1;
    }
    node1 ->next = node2; //Links both of the nodes
    return 0;
}

int list_length(list_node_t* head) { //Gets the length of a linked list
    // TODO
    if (head == NULL){ //no valid nodes
	    return 0;
    }
    int i = 0;
    list_node_t* currentN = head;
    while (currentN != NULL){ //Itterates through the linked list until reaching the end
	    i++;
	    currentN = currentN->next;
    }
    return i; //returns int of length of the list
}

int append_new_node(list_node_t* head, int data) { //Appends a new node to the list
    // TODO
    if (head == NULL){ //Header node can't be NULL
	    return -1;
    }
    list_node_t* current = head;
    while (current->next != NULL){ //Itterates through all elements in the list
	    current = current->next;
    }
    list_node_t* newNode = create_node(data); //Creates a new node with inputed data value
    if (newNode == NULL){ //If new node couldn't be properly created
	    return -1;
    }
    current->next = newNode;
    return 0;
}

int list_statistics(list_node_t* head, double* avg, int* min, int* max, int* sum) { //Finds the average, minimum, maximum, and sum of a list
    // TODO
    if (head == NULL){ //Header node can't be 0
	    return -1;
    }
    int Min = head->data; //Setting values to first element in list
    int Max = head->data;
    int Sum = 0;
    list_node_t* current = head;
    int length = list_length(current);
    if (length == 0){ //If list has no nodes
	    return -1;
    }
    while (current != NULL){ //Itterates through nodes in linked list
	    Sum += current->data;
	    if (current->data < Min){ //updates Minimum if lower minimum is found
		    Min = current->data;
	    }
	    if (current->data > Max){ //Updates Maximum if higher maximum is found
		    Max = current->data;
	    }
	    current = current->next; //Itterates current to next node
    }
    *sum = Sum; //Applies new values to the inputed data pointers
    *min = Min;
    *max = Max;
    *avg = (double)Sum / length; //Calculates average
    return 0;
}

list_node_t* list_find(list_node_t* head, int target) { //Finds if any node in a linked list has a specific data value
    // TODO
    if (head == NULL){ //Head node can't be NULL
	    return NULL;
    }
    list_node_t* current = head;
    while(current != NULL){ //Itterates through nodes in linked list
	    if (current->data == target){ //If target data value is found then returns the node
		    return current;
	    }
	    current = current->next;
    }
    return NULL;
}

list_node_t* list_get_index(list_node_t* head, int index) {//Returns the node at a index within a linked list
    // TODO
    if (head == NULL || list_length(head) <= index || index < 0){ //Cases that would prehibit this function from working properly
	    return NULL;
    }
    list_node_t* current = head;
    for (int i = 0; i < index; i++){ //Itterates through linked list nodes until at inputed index
	    current = current->next;
    }
    return current; //returns node
}

list_node_t* list_remove_index(list_node_t* head, int index) {//Removes a node in a list at a inputted index
    // TODO
    if (head == NULL || list_length(head) <= index || index<0){ //Cases that would prehibit this function from working properly
		    return head;
    }
    if (index == 0){ //If head is being removed
	    list_node_t* nHead = head->next;
	    destroy_node(head);
	    return nHead;
    }
    list_node_t* Pnode = list_get_index(head,index -1); //need the node before the inputted index to reassign correctly
    if ( Pnode == NULL || Pnode->next == NULL){
	    return head;
    }

    list_node_t* node = Pnode->next;
    Pnode->next = node->next;
    destroy_node(node);
    return head;
}


list_node_t* list_insert_index(list_node_t* head, int index, int data) { //Inserts a node in a list at a specific index
    // TODO
    if (index<0){ //index can't be negative
        return NULL;
    }
    if (head==NULL) { //If inserting a node into a empty list
        if (index==0) {
            return create_node(data);
        } else {
            return NULL; //index out of bounds
        }
    }
    if (index== 0) { //Inserting at the front of a list, making it the new head
        list_node_t* Nhead = create_node(data);
        if (Nhead == NULL) return NULL;
        Nhead->next = head;
        return Nhead;
    }
    list_node_t* Pnode = list_get_index(head, index - 1);//Insertting at the middle or the end of a list
    if (Pnode == NULL) {
        return NULL; // invalid index
    }
    list_node_t* newNode = create_node(data);
    if (newNode == NULL) {
        return NULL; //no available nodes
    }
    newNode->next = Pnode->next;
    Pnode->next = newNode;
    return head;
}

int list_insert_index_hard(list_node_t** head, int index, int data) { //Simular to list_insert_index though the head is updates with a double pointer, and output ints are different
    // TODO
    if (head==NULL || index < 0) { //Not valid inputs
        return -1;
    }
    if (*head== NULL) { //If inserting into a empty list
        if (index ==0) {
            *head =create_node(data);
            if (*head != NULL){
		    return 0;
	    }
	    else
		    return -1;
        }
        return -1; //invalid index for empty list
    }
    //Inserting at the front of a list, making it the new head
    if (index == 0) {
        list_node_t* Nhead = create_node(data);
        if (Nhead == NULL) return -1;
        Nhead->next = *head;//Linkes old head to new head
        *head = Nhead;//Updes the head pointer
        return 0;
    }
    list_node_t* Pnode = list_get_index(*head, index - 1);//Insertting into the middle or the end of a list
    if (Pnode == NULL) return -1;//Invalid inputted index
    list_node_t* newNode = create_node(data);
    if (newNode == NULL) return -1;
    newNode->next = Pnode->next;//Connects new node to the next node
    Pnode->next = newNode;//Connects the previous node to the new node
    return 0;
}
