#include <stdio.h>
#include <stdlib.h>

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

    // if (size < 8) {
    //     return insertion_sort(arr, size);
    // }

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

void sort_file(char* filename, char sorting_alg) {

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s", filename);
        return;
    }

    int capacity = 10;
    int* arr = (int*) malloc(capacity * sizeof(int));
    int size = 0;
    printf("Before Sort: ");
    while (!feof(file))
    {
        // Increase array size if needed
        if (size == capacity) {
            capacity *= 2;
            int *new_array = realloc(arr, capacity * sizeof(int));
            if (new_array == NULL) {
                printf("Error allocating memory\n");
                free(arr);
                return;
            }
            arr = new_array;
        }
        int res = fscanf(file, "%d", &arr[size++]);
        if (res != 1) {
            printf("Invalid file format\n");
            return;
        }

        printf("%d ", arr[size-1]);
    }
    printf("\n");
    printf("---------------\n");

    if (sorting_alg == 'm') {
        merge_sort(arr, size);
    }
    else if (sorting_alg == 'i')
    {
        insertion_sort(arr, size);
    }
    

    printf("After Sort: ");
    for (int j = 0; j < size; j++) {
        printf("%d ", arr[j]);
    }
    printf("\n");
    free(arr);

    fclose(file);

}


#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int opt;
    char sorting_alg = 'm';
	
    // Parse Command line options
	while((opt = getopt(argc, argv, ":mbih")) != -1)
	{
		switch(opt)
		{
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