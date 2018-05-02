#include "functions.c"

int main(int argc, char const *argv[]) {
    char b[5];
    minterm *minterms = read_minterms(argc, argv);
    for(int i = 0; i < n_mint; i++){
        minterms[i].v_bit = int_to_bin(minterms[i].v_int);
    }
    count_ones(minterms);
    classify_groups(minterms);
    return 0;
}
