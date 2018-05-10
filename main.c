#include "functions.c"

int main(int argc, char const *argv[]) {
    int i, j, index = 0, mult = 2;
    minterm *minterms = read_minterms(argc, argv);
    for(i = 0; i < n_mint; i++){
        minterms[i].v_bit = int_to_bin(minterms[i].v_int[0]);
    }
    count_ones(minterms);
    for(i = 0; i < n_mint; i++){
        printf("%d  minterm = %s    #1s = %d\n", minterms[i].v_int[0], minterms[i].v_bit, minterms[i].ones);
    }
    minterm_group **groups = malloc(sizeof(minterm_group*)*n_vars-1);
    groups[index++] = classify_groups(minterms);
    n_steps++;
    for (i = 0; i < n_ones_groups; i++){
        for (j = 0; j < groups[0][i].n_elems; j++){
            printf("group:%d | bit: %s | int: %d\n", groups[0][i].n_ones, groups[0][i].m[j].v_bit, groups[0][i].m[j].v_int[0]);
        }
    }
    for (index, i = 1; index < n_vars; index++, i++){
        groups[index] = malloc(sizeof(minterm_group)*(n_ones_groups-i));
    }
    compare_groups(groups);
    // printf("%s | %d %d\n", groups[1][0].m[2].v_bit, groups[1][0].m[2].v_int[0], groups[1][0].m[2].v_int[1]);
    return 0;
}
