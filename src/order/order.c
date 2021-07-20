
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "order.h"

// Simple merge sort ordenation

void Order_Vehicles(Vehicle_t** array, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left)/2;

    Order_Vehicles(array, left, mid);
    Order_Vehicles(array, mid+1, right);


    Vehicle_t** new = calloc (right-left+1, sizeof(Vehicle_t*));
    int leftIterator = left;
    int midIterator = mid+1;
    int newIterator = 0;
    
    while (leftIterator <= mid && midIterator <= right) {
        if (array[leftIterator]->lineCode <= array[midIterator]->lineCode){
            new[newIterator] = array[leftIterator];
            leftIterator++;
        } else {
            new[newIterator] = array[midIterator];
            midIterator++;
        }
        newIterator++;
    }
    
    while (leftIterator <= mid){
        new[newIterator] = array[leftIterator];
        leftIterator++;
        newIterator++;
    }

    while (midIterator <= right){
        new[newIterator] = array[midIterator];
        midIterator++;
        newIterator++;
    }

    for (int i=left, k=0; i<=right; i++, k++) {
        array[i] = new[k];
    }

    free(new);
    return;
}

void Order_BusLines(BusLine_t** array, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left)/2;

    Order_BusLines(array, left, mid);
    Order_BusLines(array, mid+1, right);

    BusLine_t** new = calloc (right-left+1, sizeof(BusLine_t*));
    int leftIterator = left;
    int midIterator = mid+1;
    int newIterator = 0;
    
    while (leftIterator <= mid && midIterator <= right) {
        if (array[leftIterator]->lineCode <= array[midIterator]->lineCode){
            new[newIterator] = array[leftIterator];
            leftIterator++;
        } else {
            new[newIterator] = array[midIterator];
            midIterator++;
        }
        newIterator++;
    }
    
    while (leftIterator <= mid){
        new[newIterator] = array[leftIterator];
        leftIterator++;
        newIterator++;
    }

    while (midIterator <= right){
        new[newIterator] = array[midIterator];
        midIterator++;
        newIterator++;
    }

    for (int i = left, k = 0; i <= right; i++, k++) {
        array[i] = new[k];
    }

    free(new);
    return;
}



