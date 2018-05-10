#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMINTERM 2

int n_mint;
int n_vars;
int n_ones_groups;
int n_steps = 0;

typedef struct minterm{
    int *v_int;
    int diff_pos;
    char *v_bit;
    int ones;
    char checked;
} minterm;

typedef struct minterm_group{
    minterm *m;
    int n_elems;
    int n_ones;
} minterm_group;

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
    for (int i = 0; i < n_mint; i++){
        minterms[i].v_int = malloc(sizeof(int));
    }
    for (int i = 2; i < argc; i++) {
        aux = strtoul(argv[i], NULL, 10);
        if (i > 2){
            // Garantee no repeated arguments
            for (int j = 0; j < n_mint; j++){
                if (minterms[j].v_int[0] == aux){
                    printf("Cannot repeat minterms. Minterm %d is already in the list.\n", aux);
                    exit(1);
                }
            }
        }
        minterms[i-2].v_int[0] = aux;
        minterms[i-2].ones = 0;
        printf("m[%d] = %d\n", i-2, minterms[i-2].v_int[0]);
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
    }
}

// Separates the minterm's bits in groups depend on the number of 1s
minterm_group *classify_groups(minterm *minterms){
    int ignore_counter = 0, index = 0, tab_minterms[n_vars][2], counter;

    // Identifies group of minterms that can be formed
    for (int i = 0; i < n_vars + 1; i++){
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
    n_ones_groups = n_vars - ignore_counter + 1;
    minterm_group *groups = malloc(sizeof(minterm_group)*(n_ones_groups));
    for (int i = 0; i < n_ones_groups; i++){
        groups[i].n_ones = tab_minterms[i][0];
        groups[i].n_elems = tab_minterms[i][1];
        groups[i].m = malloc(sizeof(minterm)*groups[i].n_elems);
        printf("#1s = %d    #grupos: %d\n", tab_minterms[i][0], tab_minterms[i][1]);
    }

    // Store minterms in each correspondent group
    for (int i = 0; i < n_ones_groups; i++){
        for (int j = 0, k = 0; j < groups[i].n_elems && k < n_mint; k++){
            if (minterms[k].ones == groups[i].n_ones){
                groups[i].m[j].v_bit = minterms[k].v_bit;
                groups[i].m[j].v_int = minterms[k].v_int;
                j++;
            }
        }
    }
    return groups;
}

void copy_half_int(int *a, int *b, int beg, int end){
    int i = 0;
    while (beg < end){
        a[beg] = b[i];
        beg++; i++;
    }
}

// Compare adjacent groups looking for binaries with just one bit of difference
void compare_groups(minterm_group **groups){
    int i, j, k, l, m, n, diff_counter, diff_pos, n_minterm, g_index, m_index, mult = 2, eq_counter, iter = n_vars;
    char *aux_bit = malloc(sizeof(char)*n_vars);
    for (i = 0; i < n_vars-1; i++){ //columns/steps
        g_index = 0;
        for (j = 0; j < n_ones_groups-1; j++){ //groups in a column/step
            m_index = 0;
            if (groups[i][j].n_ones+1 == groups[i][j+1].n_ones || i > 0){
                n_minterm = groups[i][j].n_elems*groups[i][j+1].n_elems;
                groups[i+1][g_index].m = malloc(sizeof(minterm)*n_minterm);
                for (k = 0; k < groups[i][j].n_elems; k++){ //mintems in a group
                    for (l = 0; l < groups[i][j+1].n_elems; l++){ //mintems in another group
                        diff_counter = 0;
                        if (i == 0 || groups[i][j].m[k].diff_pos == groups[i][j+1].m[l].diff_pos){
                            for (m = 0; m < n_vars; m++){ //mintems bits
                                if (groups[i][j].m[k].v_bit[m] != groups[i][j+1].m[l].v_bit[m]){
                                    diff_counter++;
                                    diff_pos = m;
                                }
                            }
                        } else {
                            continue;
                        }
                        if (diff_counter == 1){
                            strcpy(aux_bit, groups[i][j].m[k].v_bit);
                            aux_bit[diff_pos] = '-';
                            eq_counter = 0;
                            // Guarantee that repeated minterms will not be stored
                            for (n = 0; n < m_index; n++){
                                if (strcmp(aux_bit, groups[i+1][g_index].m[n].v_bit) == 0){
                                    eq_counter++;
                                }
                            }
                            if (eq_counter == 0){
                                groups[i+1][g_index].m[m_index].v_bit = malloc(sizeof(char)*n_vars);
                                groups[i+1][g_index].m[m_index].v_int = malloc(sizeof(int)*mult);
                                copy_half_int(groups[i+1][g_index].m[m_index].v_int, groups[i][j].m[k].v_int, 0, mult/2);
                                copy_half_int(groups[i+1][g_index].m[m_index].v_int, groups[i][j+1].m[l].v_int, mult/2, mult);
                                strcpy(groups[i+1][g_index].m[m_index].v_bit, aux_bit);
                                groups[i+1][g_index].m[m_index].diff_pos = diff_pos;
                                printf("%dx%d | %sx%s | %dx%d | res: %s\n", groups[i][j].n_ones, groups[i][j+1].n_ones, groups[i][j].m[k].v_bit, groups[i][j+1].m[l].v_bit, groups[i+1][g_index].m[m_index].v_int[0], groups[i+1][g_index].m[m_index].v_int[1], groups[i+1][g_index].m[m_index].v_bit);
                                m_index++;
                            }
                            groups[i][j].m[k].checked = 'v';
                            groups[i][j+1].m[l].checked = 'v';
                        }
                    }
                }
                groups[i+1][g_index].n_elems = m_index;
                g_index++;
            }
        }
        // Not combined minterms
        for (j = 0; j < n_ones_groups-1; j++){
            for (k = 0; k < groups[i][j].n_elems; k++){
                if (groups[i][j].m[k].checked != 'v'){
                    printf("not combined: %s\n", groups[i][j].m[k].v_bit);
                }
            }
        }
        n_ones_groups--;
        mult *= 2;
    }
}
