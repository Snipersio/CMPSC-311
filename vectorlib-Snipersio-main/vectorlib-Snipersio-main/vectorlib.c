#include "vectorlib.h"

/*
This is the implementation of the functions in vectorlib.h. 
Please read the comments in vectorlib.h before you start working on this file.
*/


// Feel free to define any helper functions, structs, or typedefs here.


vector_t* vector_create(unsigned int initial_capacity) { //Creates vector
    // TODO: Implement this function
    vector_t* vector = (vector_t*)malloc(sizeof(vector_t)); //Allocating memory for vector
    if (vector == NULL) { //If invalid
        return NULL;
    }
    vector->num_chunks = 0; //Initializing properly
    vector->num_elements = 0;   
    vector->head = NULL;
    vector->tail = NULL;
    if (initial_capacity == 0) { //If empty then finished
        return vector;
    }
    if (vector_extend(vector, initial_capacity, true) != 0) { //If error
        free(vector);
        return NULL;
    }
    return vector;
}

int vector_destroy(vector_t* vector) { //Destroys a vector
    // TODO: Implement this function
    if (vector == NULL) { //Invalid vector
        return -1;
    }
    vector_chunk_t* currentCh = vector->head; //First chunk
    while (currentCh != NULL) { //Iterates through all chunkes
        vector_chunk_t* nextCh = currentCh->next; 
        if (currentCh->elements != NULL) {//If there is a element array then frees its memory
            free(currentCh->elements);
        }
        free(currentCh);
        currentCh = nextCh;
    }
    free(vector);
    return 0;
}    


int vector_extend(vector_t* vector, unsigned int num_elements, bool zero_fill) { //Extends a vector
    // TODO: Implement this function
    if (vector == NULL) { //Vector is invalid
        return -1;
    }
    if (num_elements == 0) { //No elements
        return 0;
    }
    vector_chunk_t* NewChunk = (vector_chunk_t*)malloc(sizeof(vector_chunk_t)); //Alocating memory for new chunk
    if (NewChunk == NULL) { //Invalid new chunk
        return -1;
    }
    size_t bytes = (size_t)num_elements * sizeof(int); //Aloccating memory for elements
    if (num_elements != 0 && bytes / sizeof(int) != num_elements) { //Checking for overflow
        free(NewChunk);
        return -1;
    }
    NewChunk->elements = (int*)malloc(bytes); //Allocating memory for elements
    if (NewChunk->elements == NULL) { //If malloc failed
        free(NewChunk);
        return -1;
    }
    NewChunk->num_elements = num_elements; //Size of new chunk
    NewChunk->next = NULL; //Last chunk in list
    if (zero_fill) {
        memset(NewChunk->elements, 0, bytes);
    }
    if (vector->tail == NULL) { //If list is empty
        vector->head = NewChunk;
        vector->tail = NewChunk;
    } else {
        vector->tail->next = NewChunk; //Appends to the end
        vector->tail = NewChunk;
    }
    vector->num_chunks += 1;
    vector->num_elements += num_elements;
    return 0;
} 

unsigned int vector_size(vector_t* vector) { //Returns size of vector
    // TODO: Implement this function
    if(vector == NULL){ //Vector is invalid
	    return 0;
    }
    return (vector->num_elements); //Returns size
}

int vector_extend_array(vector_t* vector, int* array, unsigned int array_size) {
    // TODO: Implement this function
    if (vector == NULL) { //Invalid Vector
        return -1;
    }
    if (array_size == 0) { //Nothing to add
        return 0;
    }
    if (array == NULL) { //Invalid array
        return -1;
    }
    vector_chunk_t* NewChunk = (vector_chunk_t*)malloc(sizeof(vector_chunk_t));
    if (NewChunk == NULL) {
        return -1;
    }
    size_t bytes = (size_t)array_size * sizeof(int); //The size
    if (array_size != 0 && bytes / sizeof(int) != array_size) {
        free(NewChunk);
        return -1; //prevents overflow
    }
    NewChunk->elements = (int*)malloc(bytes); //Allocating memory for array
    if (NewChunk->elements == NULL) {
        free(NewChunk);
        return -1;
    }
    memcpy(NewChunk->elements, array, bytes); //Copy elements into new chunk
    NewChunk->num_elements = array_size;
    NewChunk->next = NULL;
    if (vector->tail == NULL) { //If empty then set as first chunk
        vector->head = NewChunk;
        vector->tail = NewChunk;
    } else { //Otherwise append
        vector->tail->next = NewChunk;
        vector->tail = NewChunk;
    }
    vector->num_chunks += 1; //Update vector counters
    vector->num_elements += array_size;
    return 0;
}

int* vector_get(vector_t* vector, unsigned int index) {
    // TODO: Implement this function
    if (vector == NULL) { //Invalid vectir
        return NULL;
    }
    if (index >= vector->num_elements) { //Invalid index
        return NULL;
    }
    unsigned int offset = index;
    vector_chunk_t* head = vector->head;
    while (head != NULL) { //Itterate through chunks
        if (offset < head->num_elements) { 
            return &head->elements[offset];
        }
        offset -= head->num_elements;
        head = head->next;
    }
    return NULL;
}

int vector_set(vector_t* vector, unsigned int index, int value) {
    // TODO: Implement this function
    if (vector == NULL) { //Invalid Vector
        return -1;
    }
    if (index >= vector->num_elements) { //Invalid index
        return -1;
    }
    unsigned int offset = index;
    vector_chunk_t* head = vector->head;
    while (head != NULL) { //Itterate through chunks
        if (offset < head->num_elements) {
            head->elements[offset] = value;
            return 0;
        }
        offset -= head->num_elements;
        head = head->next;
    }
    return -1;
}

int vector_coalesce(vector_t* vector) {
    // TODO: Implement this function
    if (vector == NULL) { //Invalid Vector
        return -1;
    }
    if (vector->num_chunks <= 1) { //Nothing to merge
        return 0;
    }
    unsigned int total = vector->num_elements; //Total number of elements
    if (total == 0) { //No elemments to merge
        return 0;
    }
    vector_chunk_t* new_chunk = (vector_chunk_t*)malloc(sizeof(vector_chunk_t));
    if (new_chunk == NULL) {
        return -1;
    }
    size_t bytes = (size_t)total * sizeof(int); //Needed bytes to allocate
    if (bytes / sizeof(int) != total) { //Checking for overflow
        free(new_chunk);
        return -1;
    }
    new_chunk->elements = (int*)malloc(bytes); //Allocating memory for merged
    if (new_chunk->elements == NULL) {
        free(new_chunk);
        return -1;
    }
    int* dst = new_chunk->elements; //Destination for copying
    vector_chunk_t* curr = vector->head; 
    while (curr != NULL) { //Copies elements from all chunks
        memcpy(dst, curr->elements, (size_t)curr->num_elements * sizeof(int));
        dst += curr->num_elements;
        curr = curr->next;
    }
    curr = vector->head;
    while (curr != NULL) { //Itterate through all chunks
        vector_chunk_t* next = curr->next;
        free(curr->elements);
        free(curr);
        curr = next;
    }
    new_chunk->num_elements = total; //Updating counters in the new chunk
    new_chunk->next = NULL;
    vector->head = new_chunk; //Updating counters in the vector
    vector->tail = new_chunk;
    vector->num_chunks = 1;
    return 0;
}
