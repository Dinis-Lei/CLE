#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
    UTF-8 Characters Cheat Sheet:
    [0-9] -> 30-39
    [A-Z] -> 41-5a
    '_' -> 5f
    [a-z] -> 61-7a
    ÀÁÂÃ -> c380-c383
    ÇÈÉÊ -> c387-c38a
    ÌÍ -> c38c-c38d
    ÒÓÔÕ -> c392-c395
    ÙÚ -> c399-c39a
    àáâã -> c3a0-c3a3
    çèéê -> c3a7-c3aa
    ìí -> c3ac-c3ad
    òóôõ -> c3b2-c3b5
    ùú -> c3b9-c3ba
*/

int isalphanum(unsigned char* c) {
    
    return
        (c[0] >= 0x30   && c[0] <= 0x39)   || 
        (c[0] >= 0x41   && c[0] <= 0x5a)   ||
        (c[0] == 0x5f)                     ||
        (c[0] >= 0x61   && c[0] <= 0x7a)   ||
        (c[0] >= 0x61   && c[0] <= 0x7a)   ||
        (c[0] == 0xc3   && (
            (c[1] >= 0x80 && c[1] <= 0x83) ||
            (c[1] >= 0x87 && c[1] <= 0x8a) ||
            (c[1] >= 0x8c && c[1] <= 0x8d) ||
            (c[1] >= 0x92 && c[1] <= 0x95) ||
            (c[1] >= 0x99 && c[1] <= 0x9a) ||
            (c[1] >= 0xa0 && c[1] <= 0xa3) ||
            (c[1] >= 0xa7 && c[1] <= 0xaa) ||
            (c[1] >= 0xac && c[1] <= 0xad) ||
            (c[1] >= 0xb2 && c[1] <= 0xb5) ||
            (c[1] >= 0xb9 && c[1] <= 0xba) 
        ));
}

int isapostrofe(unsigned char* c) {
    return (c[0] == 0x27) || (c[0] == 0xe2 && c[1] == 0x80 && (c[2] == 0x98 || c[2] == 0x99));
}


int isvowel(unsigned char* c) {
    if ( c[0] == 0x41 || c[0] == 0x61 || (c[0] == 0xc3 && ((c[1] >= 0x80 && c[1] <= 0x83) || (c[1] >= 0xa0 && c[1] <= 0xa3)))) {
        return 0;
    }
    if ( c[0] == 0x45 || c[0] == 0x65 || (c[0] == 0xc3 && ((c[1] >= 0x88 && c[1] <= 0x8a) || (c[1] >= 0xa8 && c[1] <= 0xaa)))) {
        return 1;
    }
    if ( c[0] == 0x49 || c[0] == 0x69 || (c[0] == 0xc3 && ((c[1] >= 0x8c && c[1] <= 0x8d) || (c[1] >= 0xac && c[1] <= 0xad)))) {
        return 2;
    }
    if ( c[0] == 0x4f || c[0] == 0x6f || (c[0] == 0xc3 && ((c[1] >= 0x92 && c[1] <= 0x95) || (c[1] >= 0xb2 && c[1] <= 0xb5)))) {
        return 3;
    }
    if ( c[0] == 0x55 || c[0] == 0x75 || (c[0] == 0xc3 && ((c[1] >= 0x99 && c[1] <= 0x9a) || (c[1] >= 0xb9 && c[1] <= 0xba)))) {
        return 4;
    }
    if ((c[0] == 0x59 || c[0] == 0x79)) {
        return 5;
    }

    return -1;

}

void count_words(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s", filename);
        return;
    }

    int is_word, words = 0;
    char ch1;
    int vowel_count[6] = {0, 0, 0, 0, 0, 0};
    int has_counted[6] = {0, 0, 0, 0, 0, 0};

    while (1) {
        ch1 = fgetc(file);
        if (ch1 == EOF) {
            break;
        }

        unsigned char ch[4] = {ch1, 0, 0, 0};
        int n_bytes;

        if ((ch1 & 0xf0) == 0xf0) {
            n_bytes = 4;
        }
        else if ((ch1 & 0xe0) == 0xe0) {
            n_bytes = 3;
        }
        else if ((ch1 & 0xc0) == 0xc0) {
            ch[1] = fgetc(file);
            n_bytes = 2;
        }
        else if (!(ch1 & 0x80)) {
            n_bytes = 1;
        }
        else {
            printf("Error, invalid byte");
            return;
        }

        for (int i = 1; i < n_bytes; i++) {
            ch[i] = fgetc(file);
        }

        if (isalphanum(ch)) {
            if (!is_word) {
                words++;
            }
            is_word = 1;
            int vowel = isvowel(ch);

            if (vowel < 0) {
                continue;
            }

            if (!has_counted[vowel]) {
                has_counted[vowel] = 1;
                vowel_count[vowel]++;
            }
        }
        else {
            if (is_word && isapostrofe(ch)) {
                is_word = 1;
            }
            else {
                is_word = 0;
                for (int i = 0; i < sizeof(has_counted)/sizeof(has_counted[0]); i++) {
                    has_counted[i] = 0;
                }
            }
        }
    }
    printf("Total number of words = %d\n", words);
    printf("N. of words with an\n");
    printf("      A\t    E\t    I\t    O\t    U\t    Y\n  ");
    for (int i = 0; i < 6; i++) {
        printf("%5d\t", vowel_count[i]);
    }
    printf("\n");
    fclose(file);
}




int main (int argc, char *argv[]) {

    if (argc < 2) {
        printf("Input some files to process!\n");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        printf("File name: %s\n", argv[i]);
        count_words(argv[i]);
    }
    return 0;
}