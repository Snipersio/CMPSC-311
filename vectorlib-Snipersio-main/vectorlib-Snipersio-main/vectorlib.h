#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* 
This is a header file for vectorlib.c. It implements a dynamic vector data structure that can store
integers using chunked memory allocation.

You will be implementing a dynamic length integer array, also known as a vector, in C. 
To enable a dynamic array length, you will use malloc and free for memory management. 
The vector uses a chunked allocation strategy where each extend operation allocates a new chunk of memory, 
but you can also coalesce all chunks into a single large allocation for better performance.

## What is a Vector?

A vector is a dynamic array that can grow and shrink as needed. Unlike a regular array, you don't need to 
know the size beforehand. Our vector implementation uses "chunks" - small arrays linked together to 
create one large logical array.

## Visual Example of Vector Structure

Let's say we create a vector and add some elements:

```c
vector_t* vec = vector_create(3);  // Create vector with initial capacity of 3
```

This creates:
```
vector_t {
    num_chunks: 1
    num_elements: 3
    head: -> [chunk1]
    tail: -> [chunk1]
}

chunk1 {
    elements: [0, 0, 0]  // 3 uninitialized integers
    num_elements: 3
    next: NULL
}
```

Now let's add some values:
```c
vector_set(vec, 0, 10);  // Set element 0 to 10
vector_set(vec, 1, 20);  // Set element 1 to 20
vector_set(vec, 2, 30);  // Set element 2 to 30
```

The vector now looks like:
```
vector_t {
    num_chunks: 1
    num_elements: 3
    head: -> [chunk1]
    tail: -> [chunk1]
}

chunk1 {
    elements: [10, 20, 30]
    num_elements: 3
    next: NULL
}
```

Now let's extend the vector:
```c
vector_extend(vec, 4);  // Add a new chunk with 4 elements
```

The vector now has TWO chunks:
```
vector_t {
    num_chunks: 2
    num_elements: 7  // 3 + 4 = 7 total elements
    head: -> [chunk1] -> [chunk2]
    tail: -> [chunk2]
}

chunk1 {
    elements: [10, 20, 30]
    num_elements: 3
    next: -> [chunk2]
}

chunk2 {
    elements: [0, 0, 0, 0]  // 4 uninitialized integers
    num_elements: 4
    next: NULL
}
```

Let's add more values:
```c
vector_set(vec, 3, 40);  // Set element 3 to 40 (in chunk2)
vector_set(vec, 4, 50);  // Set element 4 to 50 (in chunk2)
vector_set(vec, 5, 60);  // Set element 5 to 60 (in chunk2)
vector_set(vec, 6, 70);  // Set element 6 to 70 (in chunk2)
```

Now the vector looks like:
```
vector_t {
    num_chunks: 2
    num_elements: 7
    head: -> [chunk1] -> [chunk2]
    tail: -> [chunk2]
}

chunk1 {
    elements: [10, 20, 30]
    num_elements: 3
    next: -> [chunk2]
}

chunk2 {
    elements: [40, 50, 60, 70]
    num_elements: 4
    next: NULL
}
```

## Indexing Across Chunks

Even though the data is stored in separate chunks, you access it as one continuous array:

```c
int* elem0 = vector_get(vec, 0);   // Returns pointer to 10 (in chunk1)
int* elem1 = vector_get(vec, 1);   // Returns pointer to 20 (in chunk1)
int* elem2 = vector_get(vec, 2);   // Returns pointer to 30 (in chunk1)
int* elem3 = vector_get(vec, 3);   // Returns pointer to 40 (in chunk2)
int* elem4 = vector_get(vec, 4);   // Returns pointer to 50 (in chunk2)
int* elem5 = vector_get(vec, 5);   // Returns pointer to 60 (in chunk2)
int* elem6 = vector_get(vec, 6);   // Returns pointer to 70 (in chunk2)
```

## Coalescing: Merging Chunks

You can merge all chunks into one large chunk for better performance:

```c
vector_coalesce(vec);  // Merge all chunks into one
```

After coalescing:
```
vector_t {
    num_chunks: 1
    num_elements: 7
    head: -> [chunk1]
    tail: -> [chunk1]
}

chunk1 {
    elements: [10, 20, 30, 40, 50, 60, 70]  // All elements in one chunk
    num_elements: 7
    next: NULL
}
```

## Key Concepts to Understand

1. **Chunked Storage**: Data is stored in multiple small arrays (chunks) linked together
2. **Logical Continuity**: Even though data is in separate chunks, you access it as one array
3. **Dynamic Growth**: You can extend the vector by adding new chunks
4. **Memory Management**: Each chunk is allocated with malloc() and destroyed with free()
5. **Coalescing**: You can merge all chunks into one for better access performance

Your task is to implement all the functions that make this vector work!

*/

// Struct representing a single chunk of the vector
typedef struct VectorChunk {
    int* elements;                     // Pointer to an array of integer elements (use malloc to create the array)
    unsigned int num_elements;         // Number of elements the array can hold
    struct VectorChunk* next;          // Pointer to the next chunk (NULL if last)
} vector_chunk_t;

// Main integer vector structure
typedef struct Vector {
    unsigned int num_chunks;           // Number of chunks currently allocated
    unsigned int num_elements;         // Total number of elements across all chunks
    vector_chunk_t* head;              // Pointer to the first chunk
    vector_chunk_t* tail;              // Pointer to the last chunk
} vector_t;



/*
Creates a new vector with the specified initial capacity. Start by allocating the vector structure with malloc. After initilaizing the values,
call the vector_extend function to allocate the first chunk, if necessary. It should be noted that no chunk can hold 0 elements, i.e. you 
cannot malloc(0) for the array of elements. All elements in the vector should be initialized to 0. If the initial capacity is 0, just return the vector structure with no chunks.
@param initial_capacity: The initial capacity of integer elements for the vector
@return: Pointer to newly created vector, or NULL if something fails
@examples:
vector_t* empty_integer_vector = vector_create(0);
    -> vector has 0 chunks and 0 elements
vector_t* integer_vector_with_capacity_10 = vector_create(10);
    -> vector has 1 chunk, that chunk having 10 elements
*/
vector_t* vector_create(unsigned int initial_capacity);


/*
Destroys a vector and frees all associated memory. Use the free() function to deallocate the vector structure. Keep in mind the struct structure and what mallocs have been made.
@param vector: Pointer to the vector to destroy
@return: 0 on success, -1 on failure
@hint: Use free to deallocate all vector chunks first, then free the vector structure itself.
@examples:
vector_t* vec = vector_create(10);
int result = vector_destroy(vec);   // Returns 0
vector_destroy(NULL);               // Returns -1
*/
int vector_destroy(vector_t* vector);


/*
Extends the vector by allocating a new chunk (with malloc) with the specified capacity and adding it to the end of the vector. If the number of elements is 0, just return 0 and do not add a new chunk.
You cannot malloc(0) bytes for the array. Respect the zero_fill parameter.
@param vector: Pointer to the vector to extend
@param num_elements: Number of elements to extend the vector by
@param zero_fill: Whether to zero-fill (set to zero) the new elements. If zero fill is false, just leave the elements as is.
@return: 0 on success, -1 on failure
@examples:
vector_t* vec = vector_create(10);
int result = vector_extend(vec, 10);  // Adds a new chunk with capacity 10, the vector now has 20 elements across 2 chunks, the first chunk having 10 elements, the second chunk having 10 elements
*/
int vector_extend(vector_t* vector, unsigned int num_elements, bool zero_fill);


/*
Gets the total number of elements in the vector.
@param vector: Pointer to vector
@return: Number of elements, or 0 if vector is empty, NULL, or invalid
@examples:
vector_t* vec = vector_create(5);
unsigned int size = vector_size(vec);  // Returns 5
vector_extend(vec, 10);
size = vector_size(vec);  // Returns 15
*/
unsigned int vector_size(vector_t* vector);


/*
Extends the vector by the number of elements in the array and sets the elements of the new chunk to the values of the array. If the array size is 0, just return 0 and do nothing.
@param vector: Pointer to the vector to extend
@param array: Array of elements to set the new vector elements to
@param array_size: Number of elements in the array
@return: 0 on success, -1 on failure
@examples:
vector_t* vec = vector_create(10);
int array[] = {1, 2, 3, 4, 5};
int result = vector_set_array(vec, array, 5);  // Extend the vector by 5 elements and set those elements to those in array. 
// The vector will now have 15 elements across 2 chunks; 10 elements in the first chunk, 5 elements in the last, with the values in the last chunk being 1, 2, 3, 4, 5.
// Overall the vector would like like an array of [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5]
*/
int vector_extend_array(vector_t* vector, int* array, unsigned int array_size);


/*
Gets an element from the vector at the specified index.
@param vector: Pointer to the vector
@param index: Index of the element to retrieve
@return: Pointer to element at index, or NULL if index is invalid/OOB
@examples:
vector_t* vec = vector_create(10);
int* elem = vector_get(vec, 5);
if (elem != NULL) {
    printf("Element: %d\n", *elem);
}
*/
int* vector_get(vector_t* vector, unsigned int index);


/*
Sets an element in the vector at the specified index.
@param vector: Pointer to vector
@param index: Index where to set the element
@param value: Value to set the element to
@return: 0 on success, -1 on failure (invalid index or OOB)
@examples:
vector_t* vec = vector_create(10);
int result = vector_set(vec, 5, 42);
*/
int vector_set(vector_t* vector, unsigned int index, int value);


/*
Coalesces all chunks into a single large allocation for better performance.
This eliminates chunking behavior and creates one contiguous memory block, reducing the number of chunks to 1 but keeping the same total number of elements.
Existing values in the vector should be copied to the new large chunk before being destroyed (freed)
You can either allocate one new chunk and do a copy and reset of the vector, or you could use realloc to resize the first chunk. Your choice. (or try both for practice)
@param vector: Pointer to vector to coalesce
@return: 0 on success, -1 on failure
@examples:
vector_t* vec = vector_create(5);
vector_extend(vec, 10);
vector_extend(vec, 8);
// The vector has 3 chunks, with 5, 10, and 8 elements respectively.
int result = vector_coalesce(vec);  // Merges all chunks into one chunk with 23 elements in 1 chunk. vector_get(i) should equal vector_get(i) for all i before and after coalescing.
*/
int vector_coalesce(vector_t* vector);
