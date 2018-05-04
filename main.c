#include "functions.c"

int main(int argc, char const *argv[]) {
    char b[5];
    minterm *minterms = read_minterms(argc, argv);
    for(int i = 0; i < n_mint; i++){
        minterms[i].v_bit = int_to_bin(minterms[i].v_int);
    }
    count_ones(minterms);
    minterm_group *groups = classify_groups(minterms);
    for (int i = 0; i < n_ones_groups; i++){
        printf("#1s: %d\n", groups[i].n_ones);
        for (int j = 0; j < groups[i].n_elems; j++){
            printf("   bit: %s   int: %d\n", groups[i].m[j].v_bit, groups[i].m[j].v_int);
        }
    }
    compare_groups(groups);
    return 0;
}
