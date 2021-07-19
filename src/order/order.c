
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "order.h"

void Order_Vehicles(Vehicle_t** v, int ini, int end){
    printf("(%d) -> (%d)\n\n\n", ini, end);
    if (end - ini <= 2) return;
    int mid = 1 + (ini + end)/2;

    Order_Vehicles(v, ini, mid);
    Order_Vehicles(v, mid+1, end);


    Vehicle_t** new = calloc (end-ini+1, sizeof(Vehicle_t*));
    int it_i = 0;
    int it_m = mid+1;
    int it_n = 0;
    
    while (it_i < mid+1 && it_m < end) {
        if (v[it_i]->lineCode <= v[it_m]->lineCode){
            new[it_n] = v[it_i];
            it_i++;
        } else {
            new[it_n] = v[it_m];
            it_m++;
        }
        it_n++;
    }
    
    while (it_i < mid+1){
        new[it_n] = v[it_i];
        it_i++;
        it_n++;
    }

    while (it_m < end){
        new[it_n] = v[it_m];
        it_m++;
        it_n++;
    }

    for (int i=ini, k=0; i<end; i++, k++) v[i] = new[k];

    free(new);
    return;
}


void Order_BusLines(BusLine_t** v, int ini, int end){
    printf("(%d) -> (%d)\n", ini, end);
    if (end - ini <= 2) return;
    int mid = 1 + (ini + end)/2;

    Order_BusLines(v, ini, mid);
    Order_BusLines(v, mid+1, end);


    BusLine_t** new = calloc (end-ini+1, sizeof(BusLine_t*));
    int it_i = 0;
    int it_m = mid+1;
    int it_n = 0;
    
    while (it_i < mid+1 && it_m < end) {
        if (v[it_i]->lineCode <= v[it_m]->lineCode){
            new[it_n] = v[it_i];
            it_i++;
        } else {
            new[it_n] = v[it_m];
            it_m++;
        }
        it_n++;
    }
    
    while (it_i < mid+1){
        new[it_n] = v[it_i];
        it_i++;
        it_n++;
    }

    while (it_m < end){
        new[it_n] = v[it_m];
        it_m++;
        it_n++;
    }

    for (int i=ini, k=0; i<end; i++, k++) v[i] = new[k];

    free(new);
    return;
}



