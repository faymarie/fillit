#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "tetrimino.h"
char * readFile(char fileName[500]);
int countChr(char *buf);
int validateLength(char *buf, int size);
int validateChr(char *buf);
int validateLB(char *buf, int retSize);
int validateMaxChr(char *buf, int retSize);
int getMinosCount(int retSize);
char ** createArray(char *buf, char **tetriminos, int minosCount);
int validateMaxDots(char **retTetriminos, int minosCount);
int validateNonCross(char **retTetriminos, int minosCount);
int validateEmptyRow(char **retTetriminos, int minosCount);
char *createGrid(char *grid, int minosCount, int * pGridSize);
char ** createCoordinates(char ** coordinates, char **retTetriminos, int minosCount);
int * selectType(int *type, char ** retCoordinates, int minosCount);
char arrangeGrid(char ** retCoordinates, char * retGrid, int minosCount, char * alphabet, int *pGridSize, int *retType);



 int main()
{
    //Declare variables
    int i = 0;
    char *retBuf;
    int minosCount = 0;
    char ** tetriminos;
    char ** retTetriminos;
    char * grid;
    char * retGrid;
    int * type;
    int * retType;
    char ** coordinates;
    char ** retCoordinates;
    int ret = 0;
    int retSize = 0;
    char fileName[500] = "C:/Users/marie/codingprojects/ecole42/fillit/test.txt";
    char * alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int * pGridSize;

    retBuf = readFile(fileName);
    retSize = countChr(retBuf);
    printf("Array Size: %d", retSize);
    ret = validateLength(retBuf, retSize);
    ret = validateChr(retBuf);
    ret = validateLB(retBuf, retSize);
    ret = validateMaxChr(retBuf, retSize);
    minosCount = getMinosCount(retSize);
    printf(" \n Number tetriminos: %d", minosCount);
    retTetriminos = createArray(retBuf, tetriminos, minosCount);
    ret = validateMaxDots(retTetriminos, minosCount);
    ret = validateNonCross(retTetriminos, minosCount);
    ret = validateEmptyRow(retTetriminos, minosCount);
    retGrid = createGrid(grid, minosCount, pGridSize);
    printf(" \n \t gridsize %d", *pGridSize);
    retCoordinates = createCoordinates(coordinates, retTetriminos, minosCount);
    retType = selectType(type, retCoordinates, minosCount);
    ret = arrangeGrid(retCoordinates, retGrid, minosCount, alphabet, pGridSize, retType);

    //free buffer
    free(retBuf);


    //free memory of arrays...

    return(0);

}

//READ FILE FUNCTION
char * readFile(char fileName[500])
{

 // Declare variables
    int i=0;
    char *buf; // creates buffer placeholder
	int fp; // creates file pointer
	int ret; // creates return value

// Allocate memory
	buf = (char *)malloc(sizeof(char) * 500);

// Open input file
	fp = open(fileName, O_RDONLY);

// File check
	if (fp == -1)
	{
		printf("Failed opening file.\n");
		return (0);
	}

	ret = read(fp, buf, 500);
	if (ret == -1)
	{
		printf("Failed reading file.\n");
		return (0);
	}

	buf[ret] = '\0';
	close(fp);

    return (buf);
}

//VALIDATE FILE DATA

//COUNT NUMBER OF CHARACTERS IN ARRAY

int countChr(char *buf)
{
    int arraySize = 0;
    int i = 0;

    for (i=0; i<=500; i++){
        if (buf[i]=='\0'){
            arraySize = i;
            break;
        }
    }
    return(arraySize);
}

// DATA VALIDATION FUNCTIONS


 //1st condition: must be divisible by 21 and must contain less than 21x26 characters
int validateLength(char *buf, int retSize)
{
    int i = 0;

    if ( (retSize % 21 != 0) || (retSize > 21*26) ){
        printf("File contains incomplete Tetriminos");
        exit(0);
        }
        return(0);
}

// 2nd condition: only '.' or '#' or '\n' allowed

int validateChr(char *buf){

    int i = 0;

    while (buf[i] != '\0') {
        if (buf[i] == '.' || buf[i] == '#' || buf[i] == '\n'){
            i++;
            } else {
                printf(" ERROR ");
                exit(0);
            }
	}
	return(0);

}

//3rd condition: every 5th element of Tetriminos and every 21st element of file is == '\n'

int validateLB(char *buf, int retSize){

    int i = 0;

    for (i=4; i<=retSize; i+=21 ){
        if (buf[i] == '\n'){
            if (buf[i+5] == '\n'){
                if (buf[i+10] == '\n'){
                    if (buf[i+15] == '\n'){
                        if (buf[i+16] == '\n'){
                        }
                    }
                }
            }
            }else{
                printf(" \n ERROR. File contains invalid tetriminos ");
                exit(0);
            }
        }
    return(0);

 }
//4th condition: count hashes and empty spaces, 4 blocks and 12 dots allowed per 4x4 grid

int validateMaxChr(char *buf, int retSize){

    int blockCount = 0;
    int dotCount = 0;
    int i = 0;
    int j = 0;

    for (i=0; i<=retSize; i+=21 ){
        // check for blocks
        for (j=i; j<=i+20; j++){
           if (buf[j] == '#'){
                ++blockCount;
                if (blockCount > 4) {
                    printf("ERROR - too many blocks");
                    exit(0);
                }
            }
            // check for dots
            if (buf[j]== '.'){
                ++dotCount;
                if (dotCount > 12) {
                printf(" ERROR. File contains invalid tetriminos ");
                exit(0);
                }
            }
        }
        blockCount = 0;
        dotCount = 0;
    }
    return(0);
}

 //5th condition: Validate shape of blocks

 // Check if less than 4 dots are between two hashes of one tetrimino

//COUNT NUMBER OF TETRIMINOS IN ARRAY

int getMinosCount(int retSize){

    int minosCount = retSize/21;
    return(minosCount);

}

char ** createArray(char *buf, char **tetriminos, int minosCount){

    int p = 0;
    int i = 0;
    int j = 0;

    int num_cols = 16;
    int num_rows = minosCount;

    //allocate memory for every tetrimino
    tetriminos = malloc(minosCount * sizeof(char*));

    for (i=0; i<minosCount; i++){
        tetriminos[i] = malloc(num_cols * sizeof(char));
    }


    while (buf[i] != '\0') {
        ++i;
    }

    // Remove line breaks from buf to create array

       while (buf[p] != '\0'){
        // Remove line breaks from array
        if (buf[p] != '\n' ){
            // loop through number of tetriminos
            for(i = 0; i <= minosCount-1; i++ ){
                if (buf[p] == '\n'){ ++p;}

                // loop through columns
                for (j = 0; j<=15; j++){
                    tetriminos [i][j] = buf[p];
                    ++p;
                    if (buf[p] == '\n'){ ++p;}
                    }
                }
            }else {++p;}
     }

    return(tetriminos);
}

int validateMaxDots(char **retTetriminos, int minosCount){

// Check if less than 4 dots are between two hashes of one tetrimino
    int temp = 0;
    int block = 0;
    int j1 = 0;
    int i = 0;
    int j = 0;

    // loop through tetriminos
    for(i = 0; i <= minosCount-1; i++ ){

        //reset variables after each tetriminos
        block = 0;
        j1 = 0;

        // loop through columns
        for (j = 0; j<=15; j++){
            // stop at any hash
            if(retTetriminos[i][j] == '#'){
                ++block;

                if (block>1){
                temp = j-j1;
                temp = temp -1;
                    // check if dot-spacing between two hashes is greater than 3
                    if ( temp > 3){
                        printf(" File contains invalid tetriminos ");
                        exit(0);
                    }
                }
            j1 = j;
            }
        }

    }
   return(0);
}

int validateNonCross(char **retTetriminos, int minosCount){
 // 6th condition: There mustn't be any hash on its n-5th and n-4th position
 // tetriminos can't cross a line break

    int i = 0;
    int j = 0;

    //loop through tetriminos
    for(i = 0; i <= minosCount-1; i++ ){
        // loop through columns
        for (j = 0; j<=15; j++){
            // stop at any hash
            if(retTetriminos[i][j] == '#'){
                //check every 5th element ( j=4,8,10)
                if(j % 4 == 0){
                    //check if previous element is a block
                    if (retTetriminos[i][j-1] == '#'){
                    printf(" ERROR. File contains invalid tetriminos ");
                    exit(0);
                    }
                }
            }
        }
    }

    return(0);
}

int validateEmptyRow(char **retTetriminos, int minosCount){

// 7th condition:
// There must always be an empty 4 dot row, except for the long vertical tetriminos

    int vertical = 0;
    int i = 0;
    int j = 0;
    int temp = 0;
    //loop through tetriminos
    for(i = 0; i <= minosCount-1; i++){
        temp = 0;
        // loop through columns, every 4th element
        for (j = 0; j<=15; j+=4){
        // check if there are 4 dots after another
            if(retTetriminos[i][j] == '.'){
                if(retTetriminos[i][j+1] == '.'){
                    if(retTetriminos[i][j+2] == '.'){
                        if(retTetriminos[i][j+3] == '.'){
                        ++temp;
                        }
                    }
                }
            }
        }
        //check if vertical
        if(temp==0){
            for (j = 0; j<=15; j++){
                if(retTetriminos[i][j] == '#'){
                    if(retTetriminos[i][j+4] == '#'){
                        if(retTetriminos[i][j+8] == '#'){
                            if(retTetriminos[i][j+12] == '#'){
                                vertical = 1;
                            }
                        }
                    }
                }
            }
                if(vertical != 1){
                printf(" ERROR. File contains invalid tetriminos ");
                exit(0);
                }
        }

    }
    return(0);
}

// SOLVE

char * createGrid(char * grid, int minosCount, int * pGridSize){

    // count hashes to find out min grid size
    int hashCount = 0;
    hashCount = minosCount*4;
    int gridSize = 1;
    int i = 0;

    //Calculate grid size
     for(i=1; i*i<=hashCount; i++){
        printf("this is i: %d", i);
        ++gridSize;
     }

    //allocate memory for grid
    grid = malloc(gridSize*gridSize * sizeof(char*));

        // fill with dots
        for(i = 0; i < gridSize*gridSize; i++){
            grid[i] = '.';
        }

    //make gridsize available globally
    *pGridSize = gridSize;
    return(grid);
}

char ** createCoordinates(char ** coordinates, char **retTetriminos, int minosCount){

    int k = 0;
    int i = 0;
    int j = 0;
    int startValue = 0;

    //allocate memory for coordinates of 4 blocks of every tetriminos
    coordinates = malloc(minosCount * sizeof(char*));

    for (i=0; i<minosCount; i++){
        coordinates[i] = malloc(4 * sizeof(char));
    }

    //loop through tetriminos
    for(i = 0; i <= minosCount-1; i++){

        // loop through columns
            for (j = 0; j<=15; j++){
                //assign position of blocks to coordinates array
                if(retTetriminos[i][j] != '.'){
                    coordinates[i][k] = j;
                    ++k;
                }
            }
            k = 0;
    }

    //set coordinates to the first possible position
    for(i = 0; i <= minosCount-1; i++){
        startValue = coordinates[i][0];
        // loop through blocks
            for (k = 0; k<=3; k++){
               coordinates[i][k] = coordinates[i][k] - startValue;
            }
    }

    return(coordinates);

}

int * selectType(int *type, char ** retCoordinates, int minosCount){

    int i = 0;
    //allocate memory
    type = malloc(minosCount * sizeof(char*));


    for(i = 0; i <= minosCount-1; i++){

        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 4 && retCoordinates[i][3] == 5){
                type[i] = 1; //square
        }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 2 && retCoordinates[i][3] == 3){
                type[i] = 2; //horizontal
        }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 4 && retCoordinates[i][2] == 8 && retCoordinates[i][3] == 12){
                type[i] = 3; //vertical
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 3 && retCoordinates[i][2] == 4 && retCoordinates[i][3] == 5){
                type[i] = 4; //tribune up
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 2 && retCoordinates[i][3] == 5){
                type[i] = 5; //tribune bottom
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 3 && retCoordinates[i][2] == 4 && retCoordinates[i][3] == 8){
                type[i] = 6; //tribune left
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 4 && retCoordinates[i][2] == 5 && retCoordinates[i][3] == 8){
                type[i] = 7; //tribune right
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 4 && retCoordinates[i][2] == 8 && retCoordinates[i][3] == 9){
                type[i] = 8; //L up
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 2 && retCoordinates[i][3] == 4){
                type[i] = 9; //L right
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 5 && retCoordinates[i][3] == 9){
                type[i] = 10; //L down
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 2 && retCoordinates[i][2] == 3 && retCoordinates[i][3] == 4){
                type[i] = 11; //L left
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 4 && retCoordinates[i][2] == 7 && retCoordinates[i][3] == 8){
                type[i] = 12; //opposite L up
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 4 && retCoordinates[i][2] == 5 && retCoordinates[i][3] == 6){
                type[i] = 13; //opposite L right
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 4 && retCoordinates[i][3] == 8){
                type[i] = 14; //opposite L down
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 2 && retCoordinates[i][3] == 6){
                type[i] = 15; //opposite L left
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 5 && retCoordinates[i][3] == 6){
                type[i] = 16; //z horizontal
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 3 && retCoordinates[i][2] == 4 && retCoordinates[i][3] == 7){
                type[i] = 17; //z up
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 1 && retCoordinates[i][2] == 3 && retCoordinates[i][3] == 4){
                type[i] = 18; //s horizontal
            }
        if (retCoordinates[i][0] == 0 && retCoordinates[i][1] == 4 && retCoordinates[i][2] == 5 && retCoordinates[i][3] == 9){
                type[i] = 19; //s up
            }

        }

        return(type);
}

// To do: Create expand grid function

// start arranging grid
char arrangeGrid(char ** retCoordinates, char * retGrid, int minosCount, char * alphabet, int *pGridSize, int * retType){

int i = 0;
int j = 0;
int k = 0;
int k1 = 0;
int k2 = 0;
int k3 = 0;
int k4 = 0;
int count = 0;


int square = *pGridSize; // points dynamically to size of grid

    for(i=0; i < minosCount; i++){

            //assign the position of any block of any tetriminos
            k1 = retCoordinates[i][0];
            k2 = retCoordinates[i][1];
            k3 = retCoordinates[i][2];
            k4 = retCoordinates[i][3];

            // correction of coordinates, some blocks can't start at zero position
            if (retType[i] == 4 || retType[i] == 6 || retType[i] == 11 || retType[i] == 12 || retType[i] == 17 || retType[i] == 18 ){
                ++k1;
                ++k2;
                ++k3;
                ++k4;
            }

            // type 11 starts at 2nd position
            if (retType[i] == 11 ){
                ++k1;
                ++k2;
                ++k3;
                ++k4;
            }

            printf(" \n coordinates: %d,\t %d,\t %d,\t %d, \t %d", i, k1, k2, k3, k4);


                //check if grid is empty at any position
                if (retGrid[k1] == '.'){
                    if (retGrid[k2] == '.'){
                        if (retGrid[k3] == '.'){
                         if (retGrid[k4] == '.'){
                            retGrid[k1] = alphabet[i];
                            retGrid[k2] = alphabet[i];
                            retGrid[k3] = alphabet[i];
                            retGrid[k4] = alphabet[i];
                            printf(" \n retGrid: %d,\t %c,\t %c,\t %c, \t %c", i, retGrid[k1], retGrid[k2], retGrid[k3], retGrid[k4]);
                            break;
                         }
                        }
                    }

                }else {

                //while no empty spaces, shift blocks to the right according to tetrimino type
                while(retGrid[k1] != '.' && retGrid[k2] != '.' && retGrid[k3] != '.' && retGrid[k4] != '.'){

                if (retType[i] == 1  || retType[i] == 6 || retType[i] == 8 || retType[i] == 10 || retType[i] == 12 || retType[i] == 19){

                    if (k4%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=2;
                        k2+=2;
                        k3+=2;
                        k4+=2;
                    }
                    if(k4 >= square*square - 1){
                        printf("EXPAND at %d", i);
                        break;
                        // expand
                    }
                }
                //horizontal
                if (retType[i] == 2){
                    if (k4%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=4;
                        k2+=4;
                        k3+=4;
                        k4+=4;
                    }
                    if(k4 >= square*square - 1){
                        printf("EXPAND at %d", i);
                        break;
                    }
                }
                //vertical
                if (retType[i] == 3){
                    if (k4%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=1;
                        k2+=1;
                        k3+=1;
                        k4+=1;
                    }
                    if(k4 >= square*square - 1){
                        printf("EXPAND at %d", i);
                        break;
                    }
                }
                //tribune right
                if (retType[i] == 7){
                    if (k3%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=2;
                        k2+=2;
                        k3+=2;
                        k4+=2;
                        }
                    if(k4 >= square*square - 2){
                        printf("EXPAND at %d", i);
                        break;
                        }
                }
                //L left, z horizontal, tribune up
                if ( retType[i] == 4 || retType[i] == 11 || retType[i] == 13 || retType[i] == 16 ){
                    if (k4%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=3;
                        k2+=3;
                        k3+=3;
                        k4+=3;
                        }
                    if(k4 >= square*square - 1){
                        printf("EXPAND at %d", i);
                        break;
                        }
                }
                //tribune down
                if ( retType[i] == 5 ){
                    if (k3%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=3;
                        k2+=3;
                        k3+=3;
                        k4+=3;
                        }
                    if(k4 >= square*square - 2){
                        printf("EXPAND at %d", i);
                        break;
                        }
                }

                //opp l left, z horizontal
                if ( retType[i] == 15 || retType[i] == 16 ){
                    if (k4%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=3;
                        k2+=3;
                        k3+=3;
                        k4+=3;
                        }
                    if(k4 >= square*square - 1){
                        printf("EXPAND at %d", i);
                        break;
                        }
                }
                //L right
                if ( retType[i] == 9 ){
                    if (k3%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=3;
                        k2+=3;
                        k3+=3;
                        k4+=3;
                        }
                    if(k4 >= square*square - 3){
                        printf("EXPAND at %d", i);
                        break;
                        }
                }

                //s horizontal
                if (  retType[i] == 18 ){
                    if (k2%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=3;
                        k2+=3;
                        k3+=3;
                        k4+=3;
                        }
                    if(k4 >= square*square - 2){
                        printf("EXPAND at %d", i);
                        break;
                        }
                }
                // opp L down, z up
                if ( retType[i] == 14 || retType[i] == 17 ){
                    if (k2%(square-1) != 0){
                        ++k1;
                        ++k2;
                        ++k3;
                        ++k4;
                    }else{
                        k1+=2;
                        k2+=2;
                        k3+=2;
                        k4+=2;
                        }
                    if(k4 >= square*square - 2){
                        printf("EXPAND at %d", i);
                        break;
                        }
                }

                //check again if empty space
                if (retGrid[k1] == '.'){
                    if (retGrid[k2] == '.'){
                        if (retGrid[k3] == '.'){
                         if (retGrid[k4] == '.'){
                            retGrid[k1] = alphabet[i];
                            retGrid[k2] = alphabet[i];
                            retGrid[k3] = alphabet[i];
                            retGrid[k4] = alphabet[i];
                            break;
                         }}}
                    }
                }
                }
            }

        printf("\n ");
        for(i = 0; i<square*square-1; i++){
        printf("\n %c ", retGrid[i]);
    }

    return(0);
}
//Look for a better option, while recursively re-arranging tetriminos
