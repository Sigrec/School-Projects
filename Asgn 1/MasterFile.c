#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

int main(){
    FILE *source = fopen("nums(1)(1)", "r");
    if (source == NULL){
        fprintf(stderr, "Error Opening File!");
        exit(1);
    }

    unsigned int line_buf_size = 0, line_size, lineCount = 0;
    char *line = NULL;

    int num1, num2, max = INT_MIN, min = INT_MAX;
    double avg = 0;

    while(line_size = getline(&line, &line_buf_size, source) >= 0){
        lineCount++;
        sscanf(line, "%d %d", &num1, &num2);

        if (max < num1) { max = num1; }
        if (max < num2) { max = num2; }

        if (min > num1) { min = num1; }
        if (min > num2) { min = num2; }

        avg += num1 + num2;
    }
    avg /= lineCount * 2;
    
    fprintf(stdout, "Number of Lines in Files: %d\n\n", lineCount);
    fprintf(stdout, "Integer Average of all Numbers in File: %f\n\n", avg);
    fprintf(stdout, "Max Value: %d\nMin Value: %d\n", max, min);

    rewind(source);

    double variance = 0, standard_div;
    while(line_size = getline(&line, &line_buf_size, source) >= 0){
        sscanf(line, "%d %d", &num1, &num2);
        variance += pow(num1 - avg, 2) + pow(num2 - avg, 2);
    }
    fclose(source);

    free(line);
    line = NULL;

    variance /= (lineCount * 2) - 1;
    standard_div = sqrt(variance);
    fprintf(stdout, "Variance: %f\nStandard Deviation: %f\n\n", variance, standard_div);
    
    // Determine the endianness of the machine
    short int word = 0x0001;
    char *b = (char *)&word;
    fprintf(stdout, "Endianness: %s\n", b[0] == 1 ? "Little Endian" : "Big Endian");
}