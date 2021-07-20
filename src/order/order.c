
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "order.h"

// Simple merge sort ordenation

void Order_Vehicles(Vehicle_t** v, int ini, int end){
    if (ini >= end) return;
    int mid = ini + (end - ini)/2;

    Order_Vehicles(v, ini, mid);
    Order_Vehicles(v, mid+1, end);


    Vehicle_t** new = calloc (end-ini+1, sizeof(Vehicle_t*));
    int it_i = ini;
    int it_m = mid+1;
    int it_n = 0;
    
    while (it_i <= mid && it_m <= end) {
        if (v[it_i]->lineCode <= v[it_m]->lineCode){
            new[it_n] = v[it_i];
            it_i++;
        } else {
            new[it_n] = v[it_m];
            it_m++;
        }
        it_n++;
    }
    
    while (it_i <= mid){
        new[it_n] = v[it_i];
        it_i++;
        it_n++;
    }

    while (it_m <= end){
        new[it_n] = v[it_m];
        it_m++;
        it_n++;
    }

    for (int i=ini, k=0; i<=end; i++, k++) v[i] = new[k];

    free(new);
    return;
}

void Order_BusLines(BusLine_t** v, int ini, int end){
    if (ini >= end) return;
    int mid = ini + (end - ini)/2;

    Order_BusLines(v, ini, mid);
    Order_BusLines(v, mid+1, end);


    BusLine_t** new = calloc (end-ini+1, sizeof(BusLine_t*));
    int it_i = ini;
    int it_m = mid+1;
    int it_n = 0;
    
    while (it_i <= mid && it_m <= end) {
        if (v[it_i]->lineCode <= v[it_m]->lineCode){
            new[it_n] = v[it_i];
            it_i++;
        } else {
            new[it_n] = v[it_m];
            it_m++;
        }
        it_n++;
    }
    
    while (it_i <= mid){
        new[it_n] = v[it_i];
        it_i++;
        it_n++;
    }

    while (it_m <= end){
        new[it_n] = v[it_m];
        it_m++;
        it_n++;
    }

    for (int i=ini, k=0; i<=end; i++, k++) v[i] = new[k];

    free(new);
    return;
}



