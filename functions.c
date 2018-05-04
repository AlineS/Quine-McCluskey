#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct minterm{
    int v_int;
    char *v_bit;
    int ones;
} minterm;

typedef struct minterm_group{
    minterm *m;
    int n_elems;
    int n_ones;
} minterm_group;

int n_vars;
int n_mint;

// Process the number of minterms e which are the minterms
minterm *read_minterms(int argc, char const *argv[]){
    // If there is no arguments
    if (argc <= 2){
        printf("%s\n", "There is no minterm.");
        exit(1);
    }
    // Otherwise get the arguments and put it in an array
    int aux;
    n_vars = strtoul(argv[1], NULL, 10);
    n_mint = argc - 2;
    printf("n_vars = %d     n_mint = %d\n", n_vars, n_mint);
    minterm *minterms = malloc(sizeof(minterm)*n_mint);
    for (int i = 2; i < argc; i++) {
        aux = strtoul(argv[i], NULL, 10);
        if (i > 2){
            // Garantee no repeated arguments
            for (int j = 0; j < n_mint; j++){
                if (minterms[j].v_int == aux){
                    printf("Cannot repeat minterms. Minterm %d is already in the list.\n", aux);
                    exit(1);
                }
            }
        }
        minterms[i-2].v_int = aux;
        minterms[i-2].ones = 0;
        printf("m[%d] = %d\n", i-2, minterms[i-2].v_int);
    }
    return minterms;
}

// Converts integer to binary
char* int_to_bin(int n_int) {
    char *bin = malloc(sizeof(char)*n_vars);
    for (int i = 0; i < n_vars; i++){
        strcpy(&bin[i], "0");
    }
    int bits_i = n_vars-1;
    for (; n_int > 0; n_int=(n_int>>1)){
        bin[bits_i--] = (n_int & 1) + '0';
    }
    return bin;
}

// Count the number of 1s in each minterm
void count_ones(minterm *minterms) {
    for (int i = 0; i < n_mint; i++){
        for (int j = 0; j < n_vars; j++){
            if (minterms[i].v_bit[j] == '1'){
                minterms[i].ones++;
            }
        }
        printf("minterm = %s    #   1s = %d\n", minterms[i].v_bit, minterms[i].ones);
    }
}

// Separates the minterm's bits in groups depend on the number of 1s
minterm_group *classify_groups(minterm *minterms){
    int ignore_counter = 0, index = 0, tab_minterms[n_vars][2], counter;

    // Identifies group of minterms that can be formed
    for (int i = 0; i <= n_vars; i++){
        counter = 0;
        for (int j = 0; j < n_mint; j++){
            if (minterms[j].ones == i){
                counter++;
            }
        }
        if (counter == 0){
            ignore_counter++;
            index--;
        } else{
            tab_minterms[index][0] = i;
            tab_minterms[index][1] = counter;
            printf("%d: #1s: %d     #grupos: %d\n", index, tab_minterms[index][0], tab_minterms[index][1]);
        }
        index++;
    }
    printf("------ignore_counter = %d\n", ignore_counter);

    // Allocates memory to group the minterms
    minterm_group *groups = malloc(sizeof(minterm)*(n_vars-ignore_counter+1));
    for (int i = 0; i <  n_vars-ignore_counter+1; i++){
        groups[i].n_ones = tab_minterms[i][0];
        groups[i].n_elems = tab_minterms[i][1];
        groups[i].m = malloc(sizeof(minterm)*tab_minterms[i][1]);
        printf("#1s = %d    #grupos: %d\n", tab_minterms[i][0], tab_minterms[i][1]);
    }

    // Store minterms in each correspondent group
    for (int i = 0; i < n_vars-ignore_counter+1; i++){
        for (int j = 0, k = 0; j < groups[i].n_elems && k < n_mint; k++){
            if (minterms[k].ones == groups[i].n_ones){
                groups[i].m[j].v_bit = minterms[k].v_bit;
                groups[i].m[j].v_int = minterms[k].v_int;
                j++;
            }
        }
    }
    for (int i = 0; i < n_vars-ignore_counter+1; i++)
        for (int j = 0; j < groups[i].n_elems; j++)
            printf("#1s: %d   bit: %s   int: %d\n", groups[i].n_ones, groups[i].m[j].v_bit, groups[i].m[j].v_int);
}

void compare_groups(minterm_group *groups){

}
