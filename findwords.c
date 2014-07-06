#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/*
*       findwords.c
*       dev.kpyc@gmail.com
*/

// for storing direction
typedef enum Direction {
    DOWN = 0,
    RIGHT = 1
    //DIAGONAL = 2
} Direction;

// puzzle dimension
#define DIM 16

// array in which to store puzzle characters
char puzzle[DIM][DIM];
// buffer for storing words to place in the puzzle
char words[DIM*DIM/2];

// number of times to attempt placement of the word before giving up
static const int num_tries = 100;
// name of the file to read the words from
static const char * fname = "words.csv";

// place the word at location x, y in the given direction if possible
bool place(char * word, int x, int y, Direction direction) {
    int len = strlen(word);
    switch(direction) {
        case DOWN:
            for(int i=0; i<len; i++) {
                if(puzzle[y+i][x]!='\0' && puzzle[y+i][x]!=word[i]) {
                    return false;
                }
            }
            for (int i=0; i<len;i++) {
                puzzle[y+i][x] = word[i];
            }
            break;
        case RIGHT:
            for (int i=0; i<len; i++) {
                if (puzzle[y][x+i]!='\0' && puzzle[y][x+i]!=word[i]) {
                    return false;
                }
            }
            for (int i=0; i<len; i++) {
                puzzle[y][x+i] = word[i];
            }
            break;
    }
    return true;
}

void place_word(char * word) {

    int len = strlen(word);
    int xpos, ypos;

    // find a random location to place the word.
    for(int tries=0; tries<100; tries++) {
        int direction = rand() % 2;
        switch(direction) {
            case DOWN:
                xpos = rand() % DIM;
                ypos = rand() % (DIM-len);
                break;
            case RIGHT:
                xpos = rand() % (DIM-len);
                ypos = rand() % DIM;
                break;
        }
        if (place(word, xpos, ypos, direction)) {
            return;
        }
    }
    printf("Could not place \"%s\" in the puzzle after %d tries. Please increase the puzzle size or decrease the number of words and try again.\n", word, num_tries);
    exit(1);
}

int main(int argc, char** argv) {

    // initialize random seed for word placement
    srand(time(NULL));

    // zero out the puzzle array
    for (int i=0; i<DIM; i++) {
        for (int j=0; j<DIM; j++) {
            puzzle[i][j] = '\0';
        }
    }
    
    // open file to read in words
    FILE * wfp = fopen(fname,"r");
    if (wfp == NULL) {
        printf("Could not open required file %s", fname);
        return 1;
    }
    
    // read file into buffer
    fgets(words, DIM*DIM/2, wfp);

    // tokenize by comma or newline
    char * word = strtok(words,",\n");

    // place words into the puzzle
    while(word != NULL) {
        if(strlen(word) > DIM) {
            printf("Cannot place \"%s\" into the puzzle. Please increase the size of the puzzle and try again.\n", word);
            exit(1);
        }
        place_word(word);
        word = strtok(NULL,",\n");
    }
    fclose(wfp);

    // fill out remaining spots in the puzzle & print
    for (int i=0;i<DIM; i++) {
        for (int j=0; j<DIM; j++) {
            if(puzzle[i][j]=='\0') puzzle[i][j] = 'a' + rand() % 26;
            printf("%c ", puzzle[i][j]);
        }
        printf("\n");
    }

    return 0;
}