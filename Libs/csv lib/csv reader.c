#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int row;
    int column;
    char ***array;
}CSV;

void readCSV(FILE *, CSV *);
void printCSV(CSV);

int main(){
    FILE *file;
    CSV csv;
    
    file = fopen("file.csv", "r");
    readCSV(file, &csv);
    printCSV(csv);
}

void readCSV(FILE *f, CSV *c){
    char texto[100], *word;
    int i, j, column, row;
    
    c -> array = NULL;
    c -> array = (char ***)malloc(sizeof(char **) * 4);
    for(i=0;i<4;i++) {
        c->array[i] = (char **)malloc(sizeof(char *) * 3);
    }

    row = 0;
    while(fgets(texto, 100, f)) {
        column = 0;
        
        texto[strlen(texto) - 1] = '\0';
        word = strtok(texto, ",");

        while(word) {
            int lengthOfWord = strlen(word);
            c->array[row][column] = (char *)malloc(sizeof(char) * (lengthOfWord + 1));
            strcpy(c->array[row][column], word);
            column++;

            word = strtok(NULL, ",");
        }
        row++;
    }
    c->column = column;
    c->row = row;
}

void printCSV(CSV csv){
    for(int i = 0; i < csv.row; i++) {
        for(int j = 0; j < csv.column; j++) {
            printf("%s, ", csv.array[i][j]);
        }
        printf("\b\b\n");
    }
}
