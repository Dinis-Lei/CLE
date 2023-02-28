#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

void insertion_sort(int* arr, int size) {
    int i, key, j;
    for (i = 1; i < size; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void merge_sort(int* arr, int size) {

    if (size == 1) {
        return;
    }

    int new_size = size/2;

    int* left = (int*) malloc(new_size * sizeof(int));
    int* right = (int*) malloc((size - new_size) * sizeof(int));
    for (int i = 0; i < new_size; i++) {
        left[i] = arr[i];
    }

    for (int i = 0; i < size - new_size; i++) {
        right[i] = arr[i + new_size];
    }

    merge_sort(left, new_size);
    merge_sort(right, size - new_size);

    int l = 0;
    int r = 0;

    while( l < new_size && r < size - new_size) {
        if (left[l] < right[r]) {
            arr[l+r] = left[l];
            l++;
        }
        else {
            arr[l+r] = right[r];
            r++;
        }
    }

    while (l < new_size) {
        arr[l+r] = left[l];
        l++;
    }

    while (r < size - new_size) {
        arr[l+r] = right[r];
        r++;
    }

    free(left);
    free(right);

}


/*
    compare and swap if needed
    asc: determines if its ascending or descending order
*/ 

void swap(int* arr, int i, int j, int asc) {
    if (asc && arr[i] > arr[j]) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    else if(!asc && arr[i] < arr[j]) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    
}

/*
    Organize the array in mutiple bitonic sequences                 / \ / \ / \ / \ / \ / \ / \ / \ 
    and then merge them in alternating ascending                    /   \   /   \   /   \   /   \
    and descending order to form a new  set of                      /       \       /       \
    bitonic sequences, repeat until its ordered.                    /               \
                                                                    /
*/

void bitonic_merge(int* arr, int size, int start, int asc) {

    int v = size >> 1;
    int nL = 1;
    int n, u;
    for (int m = 0; m < log2(size); m++) {
        n = 0;
        u = 0;
        while (n < nL) {
            for (int t = 0; t < v; t++) {
                swap(arr, start+t+u, start+t+u+v, asc);    
            }
            u += (v << 1);
            n += 1;
        }
        v >>= 1;
        nL <<= 1;
    }
}

int bitonic_sort(int* arr, int size) {
    
    if ((size & (size - 1)) != 0) {
        printf("Array must be power of two!");
        return 1;
    } 
    for (int j = 1; j <= log2(size); j++) {
        int is_asc = 1;
        int N = pow(2, j);
        for (int i = 0; i < size; i += N) {
            bitonic_merge(arr, N, i, is_asc);
            is_asc = !is_asc;
        }
    }
    return 0;
}

void sort_file(char* filename, char sorting_alg) {

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file %s", filename);
        return;
    }

    int size = 0;
    int res = fread(&size, sizeof(int), 1, file);
    if (ferror(file)) {
        printf("Invalid file format\n");
        return;
    }

    int* arr = (int*) malloc(size * sizeof(int));
    int count = 0;
    //printf("Before Sort: ");
    while (1) {
        res = fread(&arr[count++], sizeof(int), 1, file);
        if (feof(file)) {
            break;
        }
        else if (ferror(file)) {
            printf("Invalid file format\n");
            return;
        }
        //printf("%d, ", arr[count-1]);
    }

    if (res != 0) {
        printf("Some Unexpected error occured\n");
        return;
    }

    printf("\n");

    if (sorting_alg == 'm') {
        merge_sort(arr, size);
    }
    else if (sorting_alg == 'b') {
        bitonic_sort(arr, size);
    }
    else if (sorting_alg == 'i')
    {
        insertion_sort(arr, size);
    }
    
    // printf("After Sort: ");
    // for (int j = 0; j < size; j++) {
    //     printf("%d, ", arr[j]);
    // }
    // printf("\n");

    int i;
    for (i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i+1]){ 
            printf ("Error in position %d between element %d and %d\n", i, arr[i], arr[i+1]);
            break;
        }
    }

    if (i == (size - 1)) {
        printf ("Everything is OK!\n");
    }

    free(arr);
    fclose(file);

}

int main(int argc, char *argv[])
{
	int opt;
    char sorting_alg = 'm';
	
    // Parse Command line options
	while((opt = getopt(argc, argv, ":mbih")) != -1)
	{
		switch(opt)
		{
            case 'b':
            case 'm':
			case 'i':
				//printf("option: %c\n", opt);
                sorting_alg = opt;
				break;
            case 'h':
                printf("Usage: sort_files [OPTION] [FILES]...\n");
                printf("Sorts content of files containing integers\n");
                printf("\n");
                printf("\t-m\tmerge sort\n");
                printf("\t-i\tinsertion sort\n");
                printf("\t-b\tbitonic sort");
                printf("\t-h\tdisplay help\n");
                printf("\n");
                return 0;
			case '?':
				printf("unknown option: %c\n", optopt);
				break;
		}
	}

    if (argc < 2) {
        printf("Input some files to process!\n");
        return 0;
    }
	
	for(; optind < argc; optind++){	
		printf("File name: %s\n", argv[optind]);
        sort_file(argv[optind], sorting_alg);
	}
	
    return 0;
}