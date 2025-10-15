#include "genarraylib.h"
#include <stdio.h>
#include <string.h>

/*
This is the implementation of the functions in genarraylib.h. 
Please read the comments in genarraylib.h before you start working on this file.
*/

// You are allowed to define any helper functions or helper structs here.

// hint, a new struct for chunk index, element index may be helpful eventually.
// struct ...;
// typedef ...;


// hint, a new helper function for translating an index to the above struct may also be helpful.
// index -> struct;


generic_element_t create_generic_element(bool usable) { //Creates a generic element as VOID or ERROR
    // TODO: Implement this function
    generic_element_t gen; //Return data

    if (usable){
	    gen.type = GENERIC_TYPE_VOID; //if it's usable space
    }
    else{
	    gen.type = GENERIC_TYPE_ERROR; //If it's not usable space
    }
    gen.value.int_value = 0; //the int value is 0
    return gen;
}


generic_array_chunk_t create_generic_array_chunk(const int capacity) { //Creates a chunk with proper capacity and initilizes elements
    // TODO: Implement this function
    generic_array_chunk_t chun; //Created Chunk
    int cap = capacity; 
    if (cap > MAX_ELEMENTS_PER_CHUNK){ //If capacity is larger than the maximum possible value
	    cap = MAX_ELEMENTS_PER_CHUNK;
    }
    if (cap < 0){ //If capacity is smaller than the smallest possible value
	    cap = 0;
    }
    chun.element_capacity = cap;
    for (int i = 0; i < MAX_ELEMENTS_PER_CHUNK; ++i){ //initilizing all of the slots in the array
	    bool usable = (i < cap); //the current itteration is usable
            chun.elements[i] = create_generic_element(usable); //new element in the chunk
    }
    return chun; //return the created chunk
}



generic_array_t create_generic_array(const int chunk_capacity, const int per_chunk_capacities[], const int num_initial_elements)  {//Builds an array of chunks and put in the number of initial elements
    // TODO: Implement this function
    generic_array_t array; //return array
    int ChunkCapacity = chunk_capacity;
    if (ChunkCapacity > MAX_CHUNKS){ //If chunk capacity is greater than max possible value
            ChunkCapacity = MAX_CHUNKS;
    }
    if (ChunkCapacity < 0){ //If chunk capacity is lower than lowest possible value
            ChunkCapacity = 0;
    }
    array.chunk_capacity = ChunkCapacity; //update created chunks capacity
    array.total_elements = 0;
    for (int i = 0; i < MAX_CHUNKS; ++i){ //Create all slots for all the chuncks by itteration
            int CapIndex = 0;
            if (i < ChunkCapacity && per_chunk_capacities != NULL){ //if this chunk is usable
                    CapIndex = per_chunk_capacities[i];
            }
            array.chunks[i] = create_generic_array_chunk(CapIndex); //creates a chunk with proper capacity
            if (i < ChunkCapacity){ //Only count the usable elements for usable chunks
                    array.total_elements += array.chunks[i].element_capacity;
            }
    }
    int WillInit = num_initial_elements; //number of inital elements to put in
    if (WillInit < 0){ //make sure in possible values
           WillInit = 0;
    }
    if (WillInit > array.total_elements){ //make sure in possible values
            WillInit = array.total_elements;
    }
    int j = 0; //The next element value
    for (int i = 0; i < ChunkCapacity && j < WillInit; ++i){ //Itterate through all active chunks until all elements are put in
            generic_array_chunk_t *ch = &array.chunks[i];
            for (int k = 0; k < ch->element_capacity && j < WillInit; ++k){ //Make sure to fill in only usable slots
                    ch->elements[k].type = GENERIC_TYPE_INT; //making type INT
                    ch->elements[k].value.int_value = j++;
            }
    }
    return array; //return created array
}


generic_element_t get_element(generic_array_t genarr, const int index) { //Retrieve the element from the given index and array
    // TODO: Implement this function
    generic_element_t element;
    if (index < 0 || index >= genarr.total_elements){ //Make sure in proper bounds
            return create_generic_element(false);
    }

    int ChunkIndex = index; //Indexs remaining for currently itterated chunk
    for (int i = 0; i < genarr.chunk_capacity; ++i){ //Itterating through all active chunks
            int capacity = genarr.chunks[i].element_capacity;
            if (ChunkIndex < capacity){ //Found the correct chunk
                    element = genarr.chunks[i].elements[ChunkIndex];
                    if (element.type == GENERIC_TYPE_ERROR){ //If index slot is ERROR
                            return create_generic_element(false);
                    }
                    return element;
            }
            ChunkIndex = ChunkIndex - capacity; //Skip the current chunk
    }
    return create_generic_element(false); //In case given array is weird
}



generic_array_response_t set_element_int(generic_array_t genarr, const int index, const int value) { //Seting a index in a array to a integer value
    // TODO: Implement this function
    generic_array_response_t ar;
    ar.success = false;
    ar.array = genarr;
    if (index < 0 || index >= genarr.total_elements){ //Checking for OOB
            return ar;
    }
    int ChunkIndex = index; //Reamining idex's
    for (int i = 0; i < genarr.chunk_capacity; ++i){ //Itterate through chunks
            int cap = genarr.chunks[i].element_capacity; //The usable size
            if (ChunkIndex < cap){//Index is in currently itterated chunk
                    generic_element_t *element = &ar.array.chunks[i].elements[ChunkIndex]; //targeting the index element
                    if (element->type == GENERIC_TYPE_ERROR){ //Invalid slot
                            return ar;
                    }
                    element->type = GENERIC_TYPE_INT; //Proper type
                    element->value.int_value = value;
                    ar.success = true;
                    return ar;
            }
            ChunkIndex = ChunkIndex - cap; //Move into next chunk
    }
    return ar; //In case given array is weird
}


generic_array_response_t set_element_float(generic_array_t genarr, const int index, const float value) { //Setting a index value in the array to a float
    // TODO: Implement this function
    generic_array_response_t response;
    response.success = false;
    response.array = genarr;
    if (index < 0 || index >= genarr.total_elements){ //OOB check
            return response;
    }
    int ChunkIndex = index;
    for (int i = 0; i < genarr.chunk_capacity; ++i){ //Itterating through chunks
            int capacity = genarr.chunks[i].element_capacity; //The usable size
            if (ChunkIndex < capacity){ //Found chunk where index is in
                    generic_element_t *element = &response.array.chunks[i].elements[ChunkIndex]; //The element from the index
                    if (element->type == GENERIC_TYPE_ERROR){
                            return response;
                    }
                    element->type = GENERIC_TYPE_FLOAT; //Setting to proper type
                    element->value.float_value = value;
                    response.success = true;
                    return response;
            }
            ChunkIndex = ChunkIndex - capacity; //Moving to the next chunk
    }
    return response; //In case the given array is weird
}


generic_array_response_t set_element_double(generic_array_t genarr, const int index, const double value) { //Setting a index value in the array to a double
    // TODO: Implement this function
    generic_array_response_t response;
    response.success = false;
    response.array = genarr;
    if (index < 0 || index >= genarr.total_elements){ //OOB check
            return response;
    }
    int ChunkIndex = index; //The reamining indexs
    for (int i = 0; i < genarr.chunk_capacity; ++i){ //Itterating through chunks
            int capacity = genarr.chunks[i].element_capacity; //The usable size
            if (ChunkIndex < capacity){
                    generic_element_t *element = &response.array.chunks[i].elements[ChunkIndex]; //the index element
                    if (element->type == GENERIC_TYPE_ERROR){ //Not a possible index
                            return response;
                    }
                    element->type = GENERIC_TYPE_DOUBLE; //Setting to proper type
                    element->value.double_value = value;
                    response.success = true;
                    return response;
            }
            ChunkIndex = ChunkIndex - capacity; //Moving to the next chunk
    }
    return response; //In case the given array is weird
}


generic_array_response_t set_element_char(generic_array_t genarr, const int index, const char value) { //Setting a index value in the array to a char
    // TODO: Implement this function
    generic_array_response_t response;
    response.success = false;
    response.array = genarr;
    if (index < 0 || index >= genarr.total_elements){ //00B check
            return response;
    }
    int ChunkIndex = index; //The reamining indexs
    for (int i = 0; i < genarr.chunk_capacity; ++i){ //Itterating through chunks
            int capacity = genarr.chunks[i].element_capacity; //The usable size
            if (ChunkIndex < capacity){
                    generic_element_t *element = &response.array.chunks[i].elements[ChunkIndex]; //the index element
                    if (element->type == GENERIC_TYPE_ERROR){ //Not a possible index
                            return response;
                    }
                    element->type = GENERIC_TYPE_CHAR; //Setting to proper type
                    element->value.char_value = value;
                    response.success = true;
                    return response;
            }
            ChunkIndex = ChunkIndex - capacity; //Moving to the next chunk
    }
    return response; //In case the given array is weird
}


generic_array_response_t set_element_short(generic_array_t genarr, const int index, const short value) { //Setting a index value in the array to a short
    // TODO: Implement this function
    generic_array_response_t response;
    response.success = false;
    response.array = genarr;
    if (index < 0 || index >= genarr.total_elements){ //00B check
            return response;
    }
    int ChunkIndex = index; //The reamining indexs
    for (int i = 0; i < genarr.chunk_capacity; ++i){ //Itterating through chunks
            int capacity = genarr.chunks[i].element_capacity; //The usable size
            if (ChunkIndex < capacity){
                    generic_element_t *element = &response.array.chunks[i].elements[ChunkIndex]; //the index element
                    if (element->type == GENERIC_TYPE_ERROR){ //Not a possible index
                            return response;
                    }
                    element->type = GENERIC_TYPE_SHORT; //Setting to a proper type
                    element->value.short_value = value;
                    response.success = true;
                    return response;
            }
            ChunkIndex = ChunkIndex - capacity; //Moving to the next chunk
    }
    return response; //In case the given array is weird
}


generic_array_response_t set_element_bool(generic_array_t genarr, const int index, const bool value) { //Setting a index value in the array to a bool
    // TODO: Implement this function
    generic_array_response_t response;
    response.success = false;
    response.array = genarr;
    if (index < 0 || index >= genarr.total_elements){ //00B check
            return response;
    }

    int ChunkIndex = index; //The remaining indexs
    for (int i = 0; i < genarr.chunk_capacity; ++i){ //Itterating through chunks
            int capacity = genarr.chunks[i].element_capacity; //The usable size
            if (ChunkIndex < capacity){
                    generic_element_t *element = &response.array.chunks[i].elements[ChunkIndex]; //the index element
                    if (element->type == GENERIC_TYPE_ERROR){ //Not a possible index
                            return response;
                    }
                    element->type = GENERIC_TYPE_BOOL; //Setting to a proper type
                    element->value.bool_value = value;
                    response.success = true;
                    return response;
            }
            ChunkIndex = ChunkIndex - capacity; //Moving to the next chunk
    }
    return response; //In case the given array is weird
}


generic_array_response_t set_element_string(generic_array_t genarr, const int index, const char value[]) { //Setting a index value in the array to a string
    // TODO: Implement this function
    generic_array_response_t response;
    response.success = false;
    response.array = genarr;

    if (index < 0 || index >= genarr.total_elements){ //00B check
            return response;
    }
    int ChunkIndex = index; //The remaining indexs
    for (int i = 0; i < genarr.chunk_capacity; ++i){ //Itterating through chunks
            int capacity = genarr.chunks[i].element_capacity; //The usable size
            if (ChunkIndex < capacity){
                    generic_element_t *element = &response.array.chunks[i].elements[ChunkIndex]; //the index element
                    if (element->type == GENERIC_TYPE_ERROR){ //Not a possible index
                            return response;
                    }
                    element->type = GENERIC_TYPE_STRING; //Setting to a proper type
                    strncpy(element->value.string_value, 
				    value, MAX_CHAR_ARRAY_LENGTH - 1);
                    element->value.string_value[MAX_CHAR_ARRAY_LENGTH - 1] = '\0';
                    response.success = true;
                    return response;
            }
            ChunkIndex = ChunkIndex - capacity; //Moving to the next chunk
    }
    return response; //In case the given array is weird
}


bool is_equal_element(generic_element_t elem1, generic_element_t elem2) { //Compares two sepecific elements by value and type
    // TODO: Implement this function
    if (elem1.type != elem2.type){ //If they have different types
            return false;
    }
    if (elem1.type == GENERIC_TYPE_INT){ //Comparing INTS
            return elem1.value.int_value == elem2.value.int_value;
    }
    if (elem1.type == GENERIC_TYPE_FLOAT){ //comparing FLOATS
            return elem1.value.float_value == elem2.value.float_value;
    }
    if (elem1.type == GENERIC_TYPE_DOUBLE){ //comparing DOUBLES
            return elem1.value.double_value == elem2.value.double_value;
    }
    if (elem1.type == GENERIC_TYPE_CHAR){ //comparing CHARS
            return elem1.value.char_value == elem2.value.char_value;
    }
    if (elem1.type == GENERIC_TYPE_SHORT){ //comparing SHORTS
            return elem1.value.short_value == elem2.value.short_value;
    }
    if (elem1.type == GENERIC_TYPE_BOOL){ //comparing BOOLS
            return elem1.value.bool_value == elem2.value.bool_value;
    }
    if (elem1.type == GENERIC_TYPE_STRING){ //comarping STRINGS
            return strncmp(elem1.value.string_value, elem2.value.string_value, MAX_CHAR_ARRAY_LENGTH) == 0;
    }
    if (elem1.type == GENERIC_TYPE_VOID || elem1.type == GENERIC_TYPE_ERROR){ //If they are a VOID or ERROR type
            return true;
    }
    return false; //unkown
}


bool is_equal(generic_array_t genarr1, generic_array_t genarr2) { //Comparing 2 arrays
    // TODO: Implement this function
    if (genarr1.total_elements != genarr2.total_elements){ //If their sizes match
            return false;
    }
    for (int i = 0; i < genarr1.total_elements; ++i){ //Iterate through the elements of both
            generic_element_t a = get_element(genarr1, i); //Currently itterated element of array
            generic_element_t b = get_element(genarr2, i); //Currently itterated element of array
            if (!is_equal_element(a, b)){ //If the two elements from the current itteration equal
                    return false;
            }
    }
    return true; //All elements matched
}
