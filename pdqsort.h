/* pdqsort.h - v1.0 - Public Domain QuickSORT -- public domain
                    no warranty is offered or implied; use this code at your own risk

     This is a single header file with a public domain Quicksort implementation 

 ============================================================================
     Just include the file, like this:

            #include "pdqsort.h"

 ============================================================================

 Version History
        1.00  initial release

 This is the well-known sorting algorithm developed by C. A. R. Hoare. It is a comparison 
 sort and in this implementation, it is not a stable sort. Since the PDQSORT_SWAP is a macro
 it is possible to change it int a swap which can co-sort a dependent array

 LICENSE

 This software is dual-licensed to the public domain and under the following
 license: you are granted a perpetual, irrevocable license to copy, modify,
 publish, and distribute this file as you see fit.

 CREDITS
 Remi Dufour for the initial QuickSORT implementation (Public domain)

 Written by Gerben Voshol.
*/

#include <limits.h> // CHAR_BIT

/* Insertion sort threshold shift
 *
 * This macro defines the threshold shift (power of 2) at which the insertion
 * sort algorithm replaces the Quicksort.  A zero threshold shift disables the
 * insertion sort completely.
 *
 * The value is optimized for Linux and MacOS on the Intel x86 platform.
 */
#ifndef PDQSORT_INSERTION_SORT_THRESHOLD_SHIFT
# if defined (__APPLE__) && defined (__MACH__)
#  define PDQSORT_INSERTION_SORT_THRESHOLD_SHIFT (0)
# else
#  define PDQSORT_INSERTION_SORT_THRESHOLD_SHIFT (2)
# endif
#endif

/* Macro PDQSORT_DEFAULT_SWAP and PDQSORT_SWAP
 *
 * Swaps the elements of two arrays.
 *
 * The length of the swap is determined by the value of "SIZE".  While both
 * arrays can't overlap, the case in which both pointers are the same works.
 */
#define PDQSORT_DEFAULT_SWAP(A,B,SIZE)               \
    do {                                             \
        register char       *a_byte = A;             \
        register char       *b_byte = B;             \
        register const char *a_end = a_byte + SIZE;  \
                                                     \
        while (a_byte < a_end)                       \
        {                                            \
            register const char swap_byte = *b_byte; \
            *b_byte++ = *a_byte;                     \
            *a_byte++ = swap_byte;                   \
        }                                            \
    } while (0)

#ifndef PDQSORT_SWAP
#define PDQSORT_SWAP(A,B,SIZE) DEFAULT_SWAP(A, B, SIZE)
#endif

/* Performs a recursion to the left */
#define PDQSORT_RECURSE_LEFT             \
    if (left < store - size)             \
    {                                    \
        (++recursion)->left = left;      \
        recursion->right = store - size; \
    }

/* Performs a recursion to the right */
#define PDQSORT_RECURSE_RIGHT               \
    if (store + size < right)               \
    {                                       \
        (++recursion)->left = store + size; \
        recursion->right = right;           \
    }

/* Insertion sort inner-loop */
#define PDQSORT_INSERTION_SORT_LOOP(LEFT)                         \
    {                                                             \
        register char *trail = index - size;                      \
        while (trail >= LEFT && compare(trail, trail + size) > 0) \
        {                                                         \
            PDQSORT_SWAP(trail, trail+size, size);                        \
            trail -= size;                                        \
        }                                                         \
    }

/* Performs insertion sort left of the pivot */
#define PDQSORT_INSERTION_PDQSORT_SORT_LEFT                \
    for (index = left + size; index < store; index +=size) \
        PDQSORT_INSERTION_SORT_LOOP(left)

/* Performs insertion sort right of the pivot */
#define PDQSORT_INSERTION_PDQSORT_SORT_RIGHT                        \
    for (index = store + (size << 1); index <= right; index +=size) \
        PDQSORT_INSERTION_SORT_LOOP(store + size)

/* Sorts to the left */
#if PDQSORT_INSERTION_SORT_THRESHOLD_SHIFT == 0
# define PDQSORT_SORT_LEFT PDQSORT_RECURSE_LEFT
#else
# define PDQSORT_SORT_LEFT                   \
    if (store - left <= threshold)           \
    {                                        \
        PDQSORT_INSERTION_PDQSORT_SORT_LEFT  \
    }                                        \
    else                                     \
    {                                        \
        PDQSORT_RECURSE_LEFT                 \
    }
#endif

/* Sorts to the right */
#if PDQSORT_INSERTION_SORT_THRESHOLD_SHIFT == 0
# define PDQSORT_SORT_RIGHT PDQSORT_RECURSE_RIGHT
#else
# define PDQSORT_SORT_RIGHT                  \
    if (right - store <= threshold)          \
    {                                        \
        PDQSORT_INSERTION_PDQSORT_SORT_RIGHT \
    }                                        \
    else                                     \
    {                                        \
        PDQSORT_RECURSE_RIGHT                \
    }
#endif

/* Function pdqsort (Public Domain QuickSORT)
 *
 * This function performs a basic Quicksort.  This implementation is the
 * in-place version of the algorithm and is done in he following way:
 *
 * 1. In the middle of the array, we determine a pivot that we temporarily swap
 *    to the end.
 * 2. From the beginning to the end of the array, we swap any elements smaller
 *    than this pivot to the start, adjacent to other elements that were
 *    already moved.
 * 3. We swap the pivot next to these smaller elements.
 * 4. For both sub-arrays on sides of the pivot, we repeat this process
 *    recursively.
 * 5. For a sub-array smaller than a certain threshold, the insertion sort
 *    algorithm takes over.
 *
 * As an optimization, rather than performing a real recursion, we keep a
 * global stack to track boundaries for each recursion level.
 *
 * To ensure that at most O(log2 N) space is used, we recurse into the smaller
 * partition first.  The log2 of the highest unsigned value of an integer type
 * is the number of bits needed to store that integer.
 */
#define pdqsort(A, L, S, C)                                               \
do {                                                                      \
    void *array = A;                                                      \
    size_t length = L;                                                    \
    size_t size = S;                                                      \
    int(*compare)(const void *, const void *) = C;                        \
                                                                          \
    /* Recursive stacks for array boundaries (both inclusive) */          \
    struct stackframe                                                     \
    {                                                                     \
        void *left;                                                       \
        void *right;                                                      \
    } stack[CHAR_BIT * sizeof(void *)];                                   \
                                                                          \
    /* Recursion level */                                                 \
    struct stackframe *recursion = stack;                                 \
                                                                          \
    /* Insertion sort threshold */                                        \
    const int threshold = size << PDQSORT_INSERTION_SORT_THRESHOLD_SHIFT; \
                                                                          \
    /* Assign the first recursion level of the sorting */                 \
    recursion->left = array;                                              \
    recursion->right = (char *)array + size * (length - 1);               \
                                                                          \
    do                                                                    \
    {                                                                     \
        /* Partition the array */                                         \
        register char *index = recursion->left;                           \
        register char *right = recursion->right;                          \
        char          *left  = index;                                     \
                                                                          \
        /* Assigning store to the left */                                 \
        register char *store = index;                                     \
                                                                          \
        /* Pop the stack */                                               \
        --recursion;                                                      \
                                                                          \
        /* Determine a pivot (in the middle) and move it to the end */    \
        const size_t middle = (right - left) >> 1;                        \
        PDQSORT_SWAP(left + middle - middle % size,right,size);           \
                                                                          \
        /* From left to right */                                          \
        while (index < right)                                             \
        {                                                                 \
            /* If item is smaller than pivot */                           \
            if (compare(right, index) > 0)                                \
            {                                                             \
                /* Swap item and store */                                 \
                PDQSORT_SWAP(index,store,size);                           \
                                                                          \
                /* We increment store */                                  \
                store += size;                                            \
            }                                                             \
                                                                          \
            index += size;                                                \
        }                                                                 \
                                                                          \
        /* Move the pivot to its final place */                           \
        PDQSORT_SWAP(right,store,size);                                   \
                                                                          \
        /* Recurse into the smaller partition first */                    \
        if (store - left < right - store)                                 \
        {                                                                 \
            /* Left side is smaller */                                    \
            PDQSORT_SORT_RIGHT                                            \
            PDQSORT_SORT_LEFT                                             \
                                                                          \
            continue;                                                     \
        }                                                                 \
                                                                          \
        /* Right side is smaller */                                       \
        PDQSORT_SORT_LEFT                                                 \
        PDQSORT_SORT_RIGHT                                                \
                                                                          \
    }                                                                     \
    while (recursion >= stack);                                           \
} while (0)
