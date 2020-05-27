#include <stdio.h>

#include "pdqsort.h"

int compd(const void *a, const void *b) 
{
	if (*(double *)a < *(double *)b) {
		return -1;
	}

	return 1;
}

int compd2(const void *a, const void *b) 
{
	if (*(double *)a < *(double *)b) {
		return 1;
	}

	return -1;
}

#define CUSTOM_SWAP(A,B,SIZE)                        \
    do {                                             \
        char *a_byte = A;                            \
        char *b_byte = B;                            \
                                                     \
        int idxA = ((double *)a_byte - data);        \
        int idxB = ((double *)b_byte - data);        \
                                                     \
        double temp;                                 \
        temp = data[idxA];                           \
        data[idxA] = data[idxB];                     \
        data[idxB] = temp;                           \
                                                     \
        temp = data2[idxA];                          \
        data2[idxA] = data2[idxB];                   \
        data2[idxB] = temp;                          \
                                                     \
    } while (0)

int main(int argc, char const *argv[])
{
	double data[] = {0.0, 1.0, 10.0, 5.0};
	double data2[] = {0.0, 1.0, 10.0, 5.0};

// use the swap function to co-sort a dependent array
#undef PDQSORT_SWAP
#define PDQSORT_SWAP(A,B,SIZE) CUSTOM_SWAP(A, B, SIZE)

	pdqsort(data, 4, sizeof(double), compd);
	printf("Sorted ascending\n");
	for (int i = 0; i < 4; i++) {
		printf("%lf\t", data[i]);
	}
	printf("\n");
	printf("Co-sorted ascending\n");
	for (int i = 0; i < 4; i++) {
		printf("%lf\t", data2[i]);
	}
	printf("\n");

// Use the default SWAP function
#undef PDQSORT_SWAP
#define PDQSORT_SWAP(A,B,SIZE) PDQSORT_DEFAULT_SWAP(A, B, SIZE)

	pdqsort(data, 4, sizeof(double), compd2);
	printf("Sorted descending\n");
	for (int i = 0; i < 4; i++) {
		printf("%lf\t", data[i]);
	}
	printf("\n");
	printf("Not sorted again, was already sorted ascending before\n");
	for (int i = 0; i < 4; i++) {
		printf("%lf\t", data2[i]);
	}
	printf("\n");

}
