#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* 
This is a header file for genarraylib.c. It implements a chunked array data structure that can store
arbitrary types using a union-based generic element system.

You will be implementing a special array that has a generic type per element. The `generic_element_t` type is a union of most all primitive types. 
This allows us to have one array that can store any type of data. Of course, we could just declare an array of this type, i.e. `generic_element_t array[N]`, but that would be too easy.
Instead, we will introduce the concept of **used**, **unused**, and **invalid** elements. Consider an array that looks like this:

```
my_array = []
```

This array is empty, and we cannot add more elements to it. Instead, what if we preallocated elements which we *could* later use.

```
my_array = [VOID, VOID, VOID]
```

Here, this array has 3 elements; not initialized, but usable. Let's instead add some valid types to the array.

```
my_array = [INT, FLOAT, VOID]
```

Here, this array has 3 elements, 2 are used/allocated and 1 is not. The last element is not used, but could be later. Finally, let's introduce the idea of invalid elements.

```
my_array = [VOID, VOID, VOID, ERROR, ERROR]
```

Here, we have declared an array that has 5 elements, 3 are unused but able to be allocated, and 2 are invalid and never able to be used. Essentially this is an array of 3 useable elements. 
Let's use 2 elements:

```
my_array = [INT, BOOL, VOID, ERROR, ERROR]
```

Now, we have an array with 2 used elements, 1 usable but not yet used, and 2 invalid elements. We can add one more element, but no more than that as we would be overwriting an ERROR element. In the above example, the array has a `MAX_ELEMENT` of 5 but a **capacity** of 3.

What we have above is the `generic_array_chunk_t`. Effectively this struct is an array of a `MAX_ELEMENTS_PER_CHUNK` number of `generic_element_t`'s. This struct also holds a **capacity** which dictates how many elements of that array are valid and usable.

Now, let us create a new chunk with 10 `MAX_ELEMENTS`, a capacity of 5, and 0 initial elements. We would get:

```
[VOID, VOID, VOID, VOID, VOID, ERROR, ERROR, ERROR, ERROR, ERROR]
```

If instead we created a new chunk with `MAX_ELEMENTS` of 10, a capacity of 5, and initialzed the first 2 elements as `INT`s, we would get:

```
[INT, INT, VOID, VOID, VOID, ERROR, ERROR, ERROR, ERROR, ERROR]
```

Let's now take it up another step; consider having multiple of these chunks. Each chunk has a different capacity, but all chunks work together as a contiguous array. 
For example, consider I have three chunks:

- chunk A with MAX_ELEMENTS = 5, capacity = 5
- chunk B with MAX_ELEMENTS = 5, capacity = 3
- chunk C with MAX_ELEMENTS = 5, capacity = 2

They would look like this:

```
chunk A = [VOID, VOID, VOID,  VOID,  VOID]
chunk B = [VOID, VOID, VOID,  ERROR, ERROR]
chunk C = [VOID, VOID, ERROR, ERROR, ERROR]
```

Despite chunk A only having 5 usable elements, chunk B 3 usable elements, and chunk C 2 usable elements, if they worked together we could store 10 elements in the VOID spots.

The `generic_array_t` struct is a collection of these chunks. It has a maximum number of chunks, only some of which are actually used, while the others have capacity 0 / are fully invalid. 

For example, let's consider a new generic array with a `MAX_NUM_CHUNKS` of 5 chunks, 3 of which are used, and those three chunks having capacities of 5, 3, and 2 respectively. We would get:

```
array = [
    chunk[0] = [VOID,  VOID,  VOID,  VOID,  VOID ]
    chunk[1] = [VOID,  VOID,  VOID,  ERROR, ERROR]
    chunk[2] = [VOID,  VOID,  ERROR, ERROR, ERROR]
    chunk[3] = [ERROR, ERROR, ERROR, ERROR, ERROR]
    chunk[4] = [ERROR, ERROR, ERROR, ERROR, ERROR]
]
```

This array effectively has 10 usable elements, and are indexed from 0 to 9 like a usual array. The indexing of this array however is a bit nuanced due to the ERROR elements and chunking behavior. 

Consider element at array index 6. Chunk 0 has a capacity of 5, so the value for that index must not reside here. Moving on to the chunk 1, we find that `6 (index) - 5 (capacity) = 1 (index in next chunk)` which is within this chunk's range. If we change this element to a BOOL value, we would get:

```
array = [
    chunk[0] = [VOID,  VOID,  VOID,  VOID,  VOID ]
    chunk[1] = [VOID, *BOOL*, VOID,  ERROR, ERROR]
    chunk[2] = [VOID,  VOID,  ERROR, ERROR, ERROR]
    chunk[3] = [ERROR, ERROR, ERROR, ERROR, ERROR]
    chunk[4] = [ERROR, ERROR, ERROR, ERROR, ERROR]
]
```

Your task is to implement the functions to manipulate and use this array.

*/

enum Limits {
    MAX_ELEMENTS_PER_CHUNK = 25,
    MAX_CHUNKS = 10,
    MAX_CHAR_ARRAY_LENGTH = 8
};

// Enum representing the type of a generic element.
typedef enum GenericType {
    GENERIC_TYPE_INT,
    GENERIC_TYPE_FLOAT,
    GENERIC_TYPE_DOUBLE,
    GENERIC_TYPE_CHAR,
    GENERIC_TYPE_SHORT,
    GENERIC_TYPE_BOOL,
    GENERIC_TYPE_STRING,
    GENERIC_TYPE_VOID=0xAAAAAAAAAAAAAAAA,   // Uninitialized, but able to be set
    GENERIC_TYPE_ERROR=0xFFFFFFFFFFFFFFFF,  // Invalid, not initialized
} generic_type_t;

// Struct representing a generic element.
typedef struct GenericElement {
    // type of the element
    generic_type_t type;
    // value of the element
    union {
        int int_value;
        float float_value;
        double double_value;
        char char_value;
        short short_value;
        bool bool_value;
        char string_value[MAX_CHAR_ARRAY_LENGTH];
    } value;
} generic_element_t;


typedef struct GenericArrayChunk {
    // number of non-ERROR type elements that can be stored
    int element_capacity;
    // array of elements
    generic_element_t elements[MAX_ELEMENTS_PER_CHUNK];
} generic_array_chunk_t;


typedef struct GenericArray {
    // number of non-zero capacity chunks that can be used
    int chunk_capacity;
    // sum of all chunk sizes (basically all non-ERROR elements)
    int total_elements;
    // array of chunks
    generic_array_chunk_t chunks[MAX_CHUNKS];
} generic_array_t;


typedef struct GenericArrayResponse {
    // whether the operation was successful
    bool success;
    // the generic array that was possibly modified
    generic_array_t array;
} generic_array_response_t;


/*
This function creates a new generic element with specified usable state.
@param usable: If true, creates a VOID element; if false, creates an ERROR element
@return: A generic_element_t struct with type set to GENERIC_TYPE_VOID (if usable=true) or GENERIC_TYPE_ERROR (if usable=false), and value set to 0
@examples:
create_generic_element(true) -> returns {type: GENERIC_TYPE_VOID, value: {int_value: 0}}
create_generic_element(false) -> returns {type: GENERIC_TYPE_ERROR, value: {int_value: 0}}
*/
generic_element_t create_generic_element(bool usable);

/*
This function creates a new generic array chunk with the specified capacity.
Each chunk will hold up to capacity elements, but no more than that. If the capacity is greater than MAX_ELEMENTS_PER_CHUNK, it will be set to MAX_ELEMENTS_PER_CHUNK, less than 0 will be set to 0.
@param capacity: The maximum number of elements that can be stored in this chunk (capped at MAX_ELEMENTS_PER_CHUNK, less than 0 will be set to 0)
@return: A generic_array_chunk_t struct with the given capacity. Elements up to capacity are initialized VOID, remaining elements are not initialized (set as ERROR). Hint: use create_generic_element.
@examples:
create_generic_array_chunk(3) -> returns {element_capacity: 3, elements: [VOID, VOID, VOID, ERROR, ...]}
create_generic_array_chunk(50) -> returns {element_capacity: 25, elements: [VOID, VOID, ..., VOID]}
create_generic_array_chunk(0) -> returns {element_capacity: 0, elements: [ERROR, ERROR, ..., ERROR]}
*/
generic_array_chunk_t create_generic_array_chunk(const int capacity);

/*
This function creates a new generic array with the specified number of chunks, each of their capacities, and a number of initial elements.
chunk_capacity is always guaranteed to be the size of the per_chunk_capacities array. 
@param chunk_capacity: The maximum number of chunks that can be stored (capped at MAX_CHUNKS, less than 0 will be set to 0)
@param per_chunk_capacities: The capacity of each chunk in order (hint: use create_generic_array_chunk)
@param num_initial_elements: The number of elements to initially set as INT type with a value starting at 0 and incrementing by 1 for each element. If the number of initial elements is greater than the total capacity, the remaining elements should be ignored. Do not write into ERROR elements or alter chunk capacities. If the number of initial elements is negative, assume 0.
@return: A generic_array_t struct with the specified chunks and initial elements. Elements within chunk capacity are initialized as VOID, elements beyond capacity are initialized as ERROR. If num_initial_elements is non-zero, all elements will hold INT value starting from 0 and incrementing by 1 for each element.
@examples:
create_generic_array(1, [2], 0) -> returns generic_array_t with 1 chunk and capacity of 2; 0 elements initialized, 2 elements as VOID
    [VOID, VOID, ERROR, ERROR, ...] [ERROR, ERROR, ...]...
create_generic_array(1, [5], 3) -> returns generic_array_t with 1 chunk and capacity of 5; 3 elements initialized as INT with a value starting at 0 and incrementing by 1 for each element, 2 elements as VOID
    [0, 1, 2, VOID, VOID, ERROR, ...] [ERROR, ERROR, ...]...
create_generic_array(2, [3, 5], 0) -> returns generic_array_t with 2 chunks and capacities of 3 and 5; 0 elements initialized, 8 elements as VOID
    [VOID, VOID, VOID, ERROR, ...] [VOID, VOID, VOID, VOID, VOID, ERROR, ...] [ERROR, ERROR, ...]...
create_generic_array(3, [5, 0, 5], 8) -> returns generic_array_t with 3 chunks and capacities of 5, 0, and 5; 8 elements initialized as INT with a value starting at 0 and incrementing by 1 for each element, 2 elements as VOID
    [0, 1, 2, 3, 4, ERROR, ...] [ERROR, ERROR, ...] [5, 6, 7, VOID, VOID, ERROR, ...] [ERROR, ERROR, ...]...
create_generic_array(0, NULL, 0) -> returns generic_array_t with all ERROR elements and 0 capacity chunks
    [ERROR, ERROR, ...]...
@hints:
- First allocate the chunks, using create_generic_array_chunk.
- Then, if num_initial_elements is non-zero, set the first num_initial_elements elements to INT type with a value starting at 0 and incrementing by 1 for each element.
    - You could use the extend function, set_element_int, or write your own loop.
*/
generic_array_t create_generic_array(const int chunk_capacity, const int per_chunk_capacities[], const int num_initial_elements);

/*
This function retrieves an element from the generic array at the specified index. If it is an ERROR type or OOB location, return an ERROR element.
@param genarr: The generic array to get the element from
@param index: The index of the element to retrieve
@return: A generic_element_t struct containing the element, or GENERIC_TYPE_ERROR if index is invalid
@examples:
genarr = [[0, 1, 2, 3, 4, ERROR, ...], [ERROR, ERROR, ...], [4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ERROR, ...], ...]
get_element(genarr, 0) -> returns {type: GENERIC_TYPE_INT, value: {int_value: 0}}
get_element(genarr, 3) -> returns {type: GENERIC_TYPE_INT, value: {int_value: 3}}
get_element(genarr, 7) -> returns {type: GENERIC_TYPE_STRING, value: {string_value: "hia"}}
get_element(genarr, 8) -> returns {type: GENERIC_TYPE_VOID, value: {int_value: 0}}
get_element(genarr, 100) -> returns {type: GENERIC_TYPE_ERROR, value: {int_value: 0}}
*/
generic_element_t get_element(generic_array_t genarr, const int index);

/*
This function sets an integer value at the specified index in the generic array.
@param genarr: The generic array to modify
@param index: The index where to set the value
@param value: The integer value to set
@return: {success: true, array: ...} if successful, {success: false, array: ...} if index is invalid (targets a ERROR or OOB location)
@examples:
genarr = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
set_element_int(genarr, 0, 255) -> returns {success: true, array: [[255, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_int(genarr, 3, 255) -> returns {success: true, array: [[4, 1.2, "hia", 255, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_int(genarr, 7, 255) -> returns {success: false, array: [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
*/
generic_array_response_t set_element_int(generic_array_t genarr, const int index, const int value);

/*
This function sets a float value at the specified index in the generic array.
@param genarr: The generic array to modify
@param index: The index where to set the value
@param value: The float value to set
@return: {success: true, array: ...} if successful, {success: false, array: ...} if index is invalid (targets a ERROR or OOB location)
@examples:
genarr = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
set_element_float(genarr, 0, 1.23) -> returns {success: true, array: [[1.23, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_float(genarr, 3, 1.23) -> returns {success: true, array: [[4, 1.2, "hia", 1.23, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_float(genarr, 7, 1.23) -> returns {success: false, array: [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
*/
generic_array_response_t set_element_float(generic_array_t genarr, const int index, const float value);

/*
This function sets a double value at the specified index in the generic array.
@param genarr: The generic array to modify
@param index: The index where to set the value
@param value: The double value to set
@return: {success: true, array: ...} if successful, {success: false, array: ...} if index is invalid (targets a ERROR or OOB location)
@examples:
genarr = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
set_element_double(genarr, 0, 1.23) -> returns {success: true, array: [[1.23, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_double(genarr, 3, 1.23) -> returns {success: true, array: [[4, 1.2, "hia", 1.23, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_double(genarr, 7, 1.23) -> returns {success: false, array: [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
*/
generic_array_response_t set_element_double(generic_array_t genarr, const int index, const double value);

/*
This function sets a char value at the specified index in the generic array.
@param genarr: The generic array to modify
@param index: The index where to set the value
@param value: The char value to set
@return: {success: true, array: ...} if successful, {success: false, array: ...} if index is invalid (targets a ERROR or OOB location)
@examples:
genarr = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
set_element_char(genarr, 0, 'c') -> returns {success: true, array: [['c', 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_char(genarr, 3, 'c') -> returns {success: true, array: [[4, 1.2, "hia", 'c', VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_char(genarr, 7, 'c') -> returns {success: false, array: [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
*/
generic_array_response_t set_element_char(generic_array_t genarr, const int index, const char value);

/*
This function sets a short value at the specified index in the generic array.
@param genarr: The generic array to modify
@param index: The index where to set the value
@param value: The short value to set
@return: {success: true, array: ...} if successful, {success: false, array: ...} if index is invalid (targets a ERROR or OOB location)
@examples:
genarr = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
set_element_short(genarr, 0, 255) -> returns {success: true, array: [[255, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_short(genarr, 3, 255) -> returns {success: true, array: [[4, 1.2, "hia", 255, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_short(genarr, 7, 255) -> returns {success: false, array: [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
*/
generic_array_response_t set_element_short(generic_array_t genarr, const int index, const short value);

/*
This function sets a bool value at the specified index in the generic array.
@param genarr: The generic array to modify
@param index: The index where to set the value
@param value: The bool value to set
@return: {success: true, array: ...} if successful, {success: false, array: ...} if index is invalid (targets a ERROR or OOB location)
@examples:
genarr = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
set_element_bool(genarr, 0, TRUE) -> returns {success: true, array: [[TRUE, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_bool(genarr, 3, TRUE) -> returns {success: true, array: [[4, 1.2, "hia", TRUE, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_bool(genarr, 7, FALSE) -> returns {success: false, array: [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
*/
generic_array_response_t set_element_bool(generic_array_t genarr, const int index, const bool value);

/*
This function sets a string value at the specified index in the generic array.
@param genarr: The generic array to modify
@param index: The index where to set the value
@param value: The string value to set (may or may not be null-terminated! Use safe string functions!) If the string is too long, truncate it.
@return: {success: true, array: ...} if successful, {success: false, array: ...} if index is invalid (targets a ERROR or OOB location)
@examples:
genarr = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
set_element_string(genarr, 0, "hello") -> returns {success: true, array: [["hello", 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_string(genarr, 0, "1234567890") -> returns {success: true, array: [["0123456", 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_string(genarr, 3, "sure") -> returns {success: true, array: [[4, 1.2, "hia", "sure", VOID, ERROR, ...], [ERROR, ...], ...]}
set_element_string(genarr, 7, "nope") -> returns {success: false, array: [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]}
*/
generic_array_response_t set_element_string(generic_array_t genarr, const int index, const char value[]);

/*
This function checks if two generic elements are equal in type and in value.
@param elem1: The first element to compare
@param elem2: The second element to compare
@return: True if the elements are equal, false otherwise
*/
bool is_equal_element(generic_element_t elem1, generic_element_t elem2);

/*
This function checks if two generic arrays are equal index-wise. 
Two arrays may have different capacities and structure, but may still have identical non-ERROR elements between them from start to end.
Hint: use the is_equal_element and get_element functions.
@param genarr1: The first generic array to compare
@param genarr2: The second generic array to compare
@return: true if arrays are equal, false otherwise
@examples:
genarr1 = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
genarr2 = [[4, ERROR, ...], [1.2, "hia", ERROR, ...], [ERROR, ERROR, ...], [ERROR, ...], ...]
genarr1 and genarr2 are equal

genarr1 = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
genarr2 = [[4, ERROR, ...], [VOID, VOID, ERROR, ...], [ERROR, ERROR, ...], [ERROR, ...], ...]
genarr1 and genarr2 are not equal (genarr1[1] (1.2) != genarr2[1] (VOID))

genarr1 = [[4, 1.2, "hia", VOID, VOID, ERROR, ...], [ERROR, ...], ...]
genarr2 = [[4, ERROR, ...], [VOID, ERROR, ERROR, ...], [ERROR, ERROR, ...], [ERROR, ...], ...]
genarr1 and genarr2 are not equal (different total sizes)
*/
bool is_equal(generic_array_t genarr1, generic_array_t genarr2);
