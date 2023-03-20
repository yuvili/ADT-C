#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"


struct AdptArray_ {
    PElement* elements;
    int size;
    int capacity;
    COPY_FUNC copy_function;
    DEL_FUNC del_function;
    PRINT_FUNC print_function;
};

static const int INITIAL_SIZE = 10;
static const int GROWTH = 2;

PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func, PRINT_FUNC print_func){
    PAdptArray arr = (PAdptArray)malloc(sizeof(struct AdptArray_));

    if(arr != NULL){
        arr->elements = (PElement*)calloc(INITIAL_SIZE, sizeof(PElement));
        if(arr->elements != NULL){
            arr->size = 0;
            arr->capacity = INITIAL_SIZE;
            arr->copy_function = copy_func;
            arr->del_function = del_func;
            arr->print_function = print_func;
        }
        else{
            free(arr);
            arr = NULL; // remove dangling pointer
        }
    }
    return arr;
}

void DeleteAdptArray(PAdptArray arr){
    if(arr != NULL){
        if(arr->elements != NULL){
            for (int i = 0; i < arr->size; i++){
                if(arr->elements[i] != NULL){
                    arr->print_function(arr->elements[i]);
                    arr->del_function(arr->elements[i]);
                }
            }
            free(arr->elements);
            arr->elements = NULL;
        }
        free(arr);
        arr = NULL;
    }
}

Result SetAdptArrayAt(PAdptArray arr, int index, PElement element){
    Result result = FAIL;

    if(arr != NULL && index >=0){
        if(index >= arr->size){
            while (index >= arr->capacity){
                int new_capacity = arr->capacity * GROWTH;
                PElement* new_elements = (PElement*)realloc(arr->elements, new_capacity * sizeof(PElement));
                
                if(new_elements == NULL){
                    return FAIL;
                }

                arr->capacity = new_capacity;
                arr->elements = new_elements;
            }
            
            for (int i = arr->size; i < arr->capacity; i++){
                arr->elements[i] = NULL;
            }

            arr->size = index + 1;
        }

        if(arr->elements[index] != NULL){
            arr->del_function(arr->elements[index]);
        }
        arr->elements[index] = arr->copy_function(element);
        result = SUCCESS;
    }
    return result;
}

PElement GetAdptArrayAt(PAdptArray arr, int index){
    PElement answer = NULL;

    if(arr != NULL && index >=0 && index < arr->size){
        PElement element = arr->elements[index];
        if(element != NULL){
            answer = arr->copy_function(element);
        }
    }
    // In case index >= arr->size the will return NULL
    return answer;
}

int GetAdptArraySize(PAdptArray arr){
    int size = -1; // In case the inisiallization process failed

    if(arr != NULL){
        size = arr->size;
    }
    return size;
}

void PrintDB(PAdptArray arr){
    if(arr != NULL){
        for (int i = 0; i < arr->size; i++){
            if(arr->elements[i] != NULL){
                arr->print_function(arr->elements[i]);
            }
        }
        
    }
}
