#include "functions.c"

int main(int argc, char const *argv[]) {
    int i, j, index = 0, mult = 2;
    minterm *minterms = read_minterms(argc, argv);
    for(i = 0; i < n_mint; i++){
        minterms[i].v_bit = int_to_bin(minterms[i].v_int);
    }
    count_ones(minterms);
    for(i = 0; i < n_mint; i++){
        printf("%d  minterm = %s    #1s = %d\n", minterms[i].v_int, minterms[i].v_bit, minterms[i].ones);
    }
    minterm_group **groups = malloc(sizeof(minterm_group*)*n_vars);
    groups[index++] = classify_groups(minterms);
    n_steps++;
    for (i = 0; i < n_ones_groups; i++){
        for (j = 0; j < groups[0][i].n_elems; j++){
            printf("group:%d | bit: %s\n", groups[0][i].n_ones, groups[0][i].m[j].v_bit);
        }
    }
    for (index, i = 1; index < n_vars; index++, i++){
        groups[index] = malloc(sizeof(minterm_group)*(n_ones_groups-i));
        for (j = 0; j < n_ones_groups-i; j++){
            groups[index][j].int_group = malloc(sizeof(int)*mult);
        }
        mult *= 2;
    }
    compare_groups(groups);
    return 0;
}
