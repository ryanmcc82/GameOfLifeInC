/*****************************************************************************/
/**                                                                         **/
/**     AUTHOR: RYAN MCCONN                                                 **/
/**     ASSIGNMENT Game Of Life                                             **/
/**     DATE 02/16/2016                                                     **/
/**                                                                         **/
/*****************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <term.h>
#include "gameofLife.h"

int height;
int width;
double LIFEPROB = .25;


void ClearScreen() //from http://www.cplusplus.com/forum/articles/10515/#msg49080
{
    if (!cur_term)
    {
        int result;
        setupterm( NULL, STDOUT_FILENO, &result );
        if (result <= 0) return;
    }

    putp( tigetstr( "clear" ) );
}

void printTB(int size){
    int i;
    for (i = 0; i<size; i++){
        printf("#");
    }
}

//prints current array state
void print(bool **array){
    //system("clear");
    ClearScreen();
    int i, j;
    printTB(width+1);
    for(i = 0;i<height; i++){
        printf("#\n#");
        for(j = 0; j < width; j++){
            if(array[i][j] == 1){
                printf("*");
            }
            else{
                printf(" ");}
        }
    }
    printf("#\n");
    printTB(width+2);
    printf("\n");

}

bool nextBool()
{
    return (rand() <  LIFEPROB * ((double)RAND_MAX + 1.0));
}

//intiates Array with random values
void initArray(bool **array){
    int i, j;
    for( i = 1;i<height-1; i++){
        for(j = 1; j < width -1; j++){
            if(nextBool()){
                array[i][j] = 1;
            }
            else{
                array[i][j] = 0;}
        }
    }
}

int getAdjacentCellValue(bool **array, int row, int col){
//    if(row< 0||row >=height||
//       col < 0||col>=width) {return 0;}
    return (array[row][col]) ? 1 : 0;
}

int getNeighborhoodVallue(bool **array, int row, int col){
    int count = 0;
    count += getAdjacentCellValue(array, row + 1, col);
    count += getAdjacentCellValue(array, row + 1, col + 1);
    count += getAdjacentCellValue(array, row, col + 1);
    count += getAdjacentCellValue(array, row - 1, col + 1);
    count += getAdjacentCellValue(array, row - 1, col);
    count += getAdjacentCellValue(array, row - 1, col - 1);
    count += getAdjacentCellValue(array, row, col - 1);
    count += getAdjacentCellValue(array, row + 1, col - 1);

    return count;
}

void nextLive(bool **old, bool **new){
    int i, j, count;
    for( i = 1;i<height-1; i++){
        for(j = 1; j < width -1; j++){
            count = getNeighborhoodVallue(old,i,j);

            if(old[i][j]){
                new[i][j] = (count == 2 || count == 3) ? true : false;
            }
            else{
                new[i][j] = (count == 3);
            }

        }
    }
}

bool equ(bool **old, bool **new){
    int i, j;//count?
    bool result = true;
    for( i = 0;i<height; i++){
        for(j = 0; j < width; j++){

            if(old[i][j] != new[i][j]){
                result = false;
            }

        }
    }
    return result;
}

bool** create2DArray(int r, int c){
    int i;
    bool **array = (bool **)malloc(r * sizeof(bool *));// bool * is different size then bool
    for (i = 0; i< r; i++)
        array[i] = (bool *)malloc(c * sizeof(bool));

    return array;
    }

bool** create2DArrayB(int r, int c){
    int i;
    bool **array = (bool **)malloc(r * sizeof(bool *));
    array[0]= (bool*)malloc(r * c * sizeof(bool));//%P print pointer address

    for(i = 0; i < r; i++)
        array[i] = (*array + c * i);

    return array;
}

bool** create2DArrayC(int r, int c){
    int i;
    bool *temp;
    bool **array = (bool **)malloc(r * sizeof(bool *));
    array[0]= (bool*)malloc(r * c * sizeof(bool));

    for(i = 0; i < r; i++)
        array[i] = (*array + c * i);

    return array;
}


int main(int argc, char *argv[]){
    height = 10;
    width = 20;

    bool **arrayA = create2DArrayB(height, width);        //[height][width];
    bool **arrayB = create2DArrayB(height, width);        //[height][width];
    int count = 0;


    initArray(arrayA);
    print(arrayA);
    do{
        sleep(1);
        nextLive(arrayA,arrayB);
        print(arrayB);

        sleep(1);
        nextLive(arrayB,arrayA);
        print(arrayA);
        count++;
    }
    while(!equ(arrayA, arrayB) && count < 10 );

    return 0;
}