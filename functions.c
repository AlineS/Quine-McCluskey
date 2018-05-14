#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
        }
        index++;
    }

    // Allocates memory to group the minterms
    n_ones_groups = n_vars - ignore_counter + 1;
    minterm_group *groups = malloc(sizeof(minterm_group)*(n_ones_groups));
    for (int i = 0; i < n_ones_groups; i++){
        groups[i].n_ones = tab_minterms[i][0];
        groups[i].n_elems = tab_minterms[i][1];
        groups[i].m = malloc(sizeof(minterm)*groups[i].n_elems);
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

// Copy values from a minor vector to a greater vector
void copy_half_int(int *a, int *b, int beg, int end){
    int i = 0;
    while (beg < end){
        a[beg] = b[i];
        beg++; i++;
    }
}

// Calculates the number of integers a minterm have
int count_dontcare(char *s, int len){
    int counter = 0;
    for (int i = 0; i < len; i++){
        if (s[i] == '-'){
            counter++;
        }
    }
    return pow(2, counter);
}

// Compare adjacent groups looking for binaries with just one bit of difference
minterm_group compare_groups(minterm_group **groups){
    int i, j, k, l, m, n, diff_counter, diff_pos, n_minterm, g_index, m_index, mult = 2, eq_counter, pi_index = 0;
    minterm_group pi;
    pi.m = malloc(sizeof(minterm)*n_mint);
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
        // Store prime implicants
        for (j = 0; j < n_ones_groups; j++){
            for (k = 0; k < groups[i][j].n_elems; k++){
                if (groups[i][j].m[k].checked != 'v' && pi_index < n_mint){
                    pi.m[pi_index].v_bit = malloc(sizeof(char)*n_vars);
                    strcpy(pi.m[pi_index].v_bit, groups[i][j].m[k].v_bit);
                    pi.m[pi_index].ones = count_dontcare(groups[i][j].m[k].v_bit, n_vars);
                    pi.m[pi_index].v_int = malloc(sizeof(int)*pi.m[pi_index].ones);
                    pi.m[pi_index++].v_int = groups[i][j].m[k].v_int;
                    pi.n_elems = pi_index;
                }
            }
        }
        n_ones_groups--;
        mult *= 2;
    }
    return pi;
}

// Calculates the number that each value appears
void implicants_table(int *count_pi, minterm_group prime_implicants, int n_pi){
    for (int i = 0; i < n_pi; i++){
        count_pi[i] = 0;
        for (int j = 0; j < prime_implicants.n_elems; j++){
            for (int k = 0; k < prime_implicants.m[j].ones; k++){
                if (prime_implicants.m[j].v_int[k] == i){
                    count_pi[i]++;
                }
            }
        }
    }
}

// Identifies the prime implicants
char *find_pi(minterm_group pi, int var){
    for (int i = 0; i < pi.n_elems; i++){
        for (int j = 0; j < pi.m[i].ones; j++){
            if (pi.m[i].v_int[j] == var){
                if (pi.m[i].checked == 'v'){
                    return "none";
                }
                return pi.m[i].v_bit;
            }
        }
    }
}

// Updates the prime implicant table, removing the essencial prime implicants
void override_epis(int *count_pi, minterm_group pi, int n_pi){
    int *aux_override = malloc(sizeof(int)*n_mint);
    int o_index = 0, repeated = 0;
    for (int i = 0; i < n_pi; i++){
        if (count_pi[i] == 1){ // counter vector
            printf("intizinho: %d\n", i);
            for (int j = 0; j < pi.n_elems; j++){ // implicants
                for (int k = 0; k < pi.m[j].ones; k++){ // integer implicants
                    if (pi.m[j].v_int[k] == i){
                        for (int l = 0; l < pi.m[j].ones; l++){ // integer implicants
                            for (int m = 0; m < o_index; m++){
                                if (pi.m[j].v_int[l] == aux_override[m] || count_pi[pi.m[j].v_int[l]] <= 0){
                                    repeated = 1;
                                }
                            }
                            if (repeated == 0){
                                aux_override[o_index++] = pi.m[j].v_int[l];
                            }
                            repeated = 0;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < o_index; i++){
        count_pi[aux_override[i]] = 0;
        for (int j = 0; j < pi.n_elems; j++){
            for (int k = 0; k < pi.m[j].ones; k++){
                if (aux_override[i] == pi.m[j].v_int[k]){
                    pi.m[j].v_int[k] = -1;
                }
            }
        }
    }
}

int check_pi(int *count_pi, int n_pi){
    for (int i = 0; i < n_pi; i++){
        if (count_pi[i] > 0){
            return 1;
        }
    }
    return 0;
}

int *count_elements(minterm_group pi){
    int *aux = malloc(sizeof(int)*pi.n_elems);
    for (int i = 0; i < pi.n_elems; i++){
        aux[i] = 0;
        for (int j = 0; j < pi.m[i].ones; j++){
            if (pi.m[i].v_int[j] != -1){
                aux[i]++;
                // printf("elementos: %d | valor: %d\n", aux[i], pi.m[i].v_int[j]);
            }
        }
    }
    return aux;
}

void remove_repeated(int *count_pi, minterm_group pi, int n_pi){
    for (int i = 0; i < n_pi; i++){
        for (int j = i; j < n_pi; j++){

        }
    }
}

int last_epis (int *count_pi, minterm_group pi, int n_pi, char **e_pi, int epi_index){
    int greater, g_index, epi_counter = 0;
    int *num_elems = malloc(sizeof(int)*pi.n_elems);
    // while (check_pi(count_pi, n_pi)) {
    num_elems = count_elements(pi);
    for (int g = 0; g < 3; g++){
        for (int i = 0; i < pi.n_elems; i++){
            for (int j = 0; j < pi.m[i].ones; j++){
                    printf("elementos: %d | valor: %d\n", num_elems[i], pi.m[i].v_int[j]);
            }
        }
        greater = 0;
        for (int i = 0; i < pi.n_elems; i++){
            if (greater < num_elems[i] && num_elems[i] != -1){
                greater = num_elems[i];
                g_index = i;
                printf("greater: %d | index: %d\n", greater, g_index);
            }
        }
        e_pi[epi_index] = pi.m[g_index].v_bit;
        epi_index++;
        epi_counter++;
        pi.m[g_index].checked = 'v';
        for (int i = 0; i < n_pi; i++){
            for (int j = 0; j < greater; j++){
                if (pi.m[g_index].v_int[j] == i){
                    count_pi[i] = 0;
                }
            }
        }
        int aux_int;
        for (int i = 0; i < pi.m[greater].ones; i++){
            remove_repeated(count_pi, pi, n_pi);
            printf("VALORR: %d\n", pi.m[g_index].v_int[i]);
            if (pi.m[g_index].v_int[i] != -1){
                aux_int = pi.m[g_index].v_int[i];
                printf("aux_int = %d\n", aux_int);
                for (int j = 0; j < pi.n_elems; j++){
                    for (int k = 0; k < pi.m[j].ones; k++){
                        if (pi.m[j].v_int[k] == aux_int && pi.m[j].v_int[k] != -1){
                            printf("removed: %d | greater: %d\n", pi.m[j].v_int[k], greater);
                            pi.m[j].v_int[k] = -1;
                            num_elems[j]--;
                        }
                    }
                }
            }
        }
    }
    return epi_counter;
}

// Identifies essencial prime implicants from prime implicants list
int essencials_pi(int *count_pi, minterm_group prime_implicants, int n_pi, char **essencial_pi){
    int epi_index = 0;
    char *epi_aux = malloc(sizeof(char)*n_vars);
    // while (check_pi(count_pi, n_pi)){
    for(int j = 0; j < 2; j++){
        for (int i = 0; i < n_pi; i++){
            if (count_pi[i] == 1){
                epi_aux = find_pi(prime_implicants, i);
                if (strcmp(epi_aux, "none") != 0){
                    strcpy(essencial_pi[epi_index], epi_aux);
                    prime_implicants.m[i].checked = 'v';
                    epi_index++;
                }
            }
        }
        override_epis(count_pi, prime_implicants, n_pi);
    }
    epi_index += last_epis(count_pi, prime_implicants, n_pi, essencial_pi, epi_index);
    for (int j = 0; j < n_pi; j++){
        printf("result1: %d %d | #minterms: %d\n", j, count_pi[j], epi_index);
    }
    return epi_index;
}

// Prints the equation result
void minimization(char **essencial_pi, int epi_index){
    for (int i = 0; i < epi_index; i++){
        for (int j = 0; j < n_vars; j++){
            if (essencial_pi[i][j] != '-'){
                printf("%c", (char)(j + 65));
            }
            if (essencial_pi[i][j] == '0'){
                printf("%s", "'");
            }
        }
        if (i+1 != epi_index)
            printf("%s", " + ");
    }
    printf("\n");
}
