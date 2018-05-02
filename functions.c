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
    int id;
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
        printf("minterm = %s    1s = %d\n", minterms[i].v_bit, minterms[i].ones);
    }
}

// Separates the minterm's bits in groups depend on the number of 1s
void classify_groups(minterm *minterms){
    int counter, aux_mint[n_vars][2], ignore[n_vars], ignore_counter = 0;
    for (int i = 0; i < n_vars; i++){
        counter = 0;
        for (int j = 0; j < n_mint; j++){
            if (minterms[j].ones == i){
                counter++;
            }
        }
        if (counter == 0){
            ignore[ignore_counter] = i;
            printf("ignore = %d\n", ignore[ignore_counter]);
            ignore_counter++;
        } else{
            aux_mint[i][0] = i;
            aux_mint[i][1] = counter;
            printf("%d0: %d     %d1: %d\n", i, i, aux_mint[i][0], aux_mint[i][1]);
        }
    }
    printf("ignore_counter = %d\n", ignore_counter);

    minterm_group *groups = malloc(sizeof(minterm)*(n_vars - ignore_counter));
    for (int i = 0; i <= n_vars - ignore_counter; i++){
        for (int j = 0; j < ignore_counter; j++){
            if (i == ignore[j]){
                continue;
            }
            groups[i].id = i;
            groups[i].m = malloc(sizeof(minterm)*aux_mint[i][1]);
            printf("i = %d  aux_mint[i][1] = %d     groups = %d\n", i, aux_mint[i][1], groups[i].id);
        }
    }
    // *armazenar os mintermos dentro de cada grupo*
    for (int i = 0; i < groups[i].id; i++){
        for (int j = 0; j < n_vars - ignore_counter; j++){
            for (int k = 0; k < n_mint; k++){
                for (int l = 0; l < ignore_counter; l++){
                    if (minterms[k].ones == ignore[l]){
                        continue;
                    }
                    groups[i].m[j].v_bit = minterms[k].v_bit;
                    printf("groups = %s     i = %d j = %d k = %d\n", groups[i].m[j].v_bit, i, j, k);
                }
            }
        }
    }
}
