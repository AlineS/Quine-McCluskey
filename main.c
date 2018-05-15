#include "functions.c"

int main(int argc, char const *argv[]) {
    int i, j, k, n_pi, index = 0, epi_index, mult = 2, *count_pi;
    char **essencial_pi;
    minterm *minterms;
    minterm_group **groups, prime_implicants;

    // Read the entry minterms numbers
    minterms = read_minterms(argc, argv);

    // Transform minterms numbers to bits
    for(i = 0; i < n_mint; i++){
        minterms[i].v_bit = int_to_bin(minterms[i].v_int[0]);
    }

    // Count how much ones each minterm bit have
    count_ones(minterms);

    // Separates minterms in groups depend on the number of ones
    groups = malloc(sizeof(minterm_group*)*n_vars-1);
    groups[index++] = classify_groups(minterms);
    // printf("n_ones_groups = %d\n", n_ones_groups);
    // for (i = 0; i < n_ones_groups; i++){
    //     for (j = 0; j < groups[0][i].n_elems; j++){
    //         printf("group:%d | bit: %s | int: %d\n", groups[0][i].n_ones, groups[0][i].m[j].v_bit, groups[0][i].m[j].v_int[0]);
    //     }
    // }

    // Compare all groups of minterms
    for (index, i = 1; index < n_vars; index++, i++){
        groups[index] = malloc(sizeof(minterm_group)*(n_ones_groups-i));
    }
    prime_implicants = compare_groups(groups);

    // for (i = 0; i < prime_implicants.n_elems; i++){
    //     printf("prime implicants: %s ",prime_implicants.m[i].v_bit);
    //     for (j = 0; j < prime_implicants.m[i].ones; j++){
    //         printf("%d ", prime_implicants.m[i].v_int[j]);
    //     }
    //     printf("\n" );
    // }

    // Counts the number of occurence of each variable
    n_pi = pow(2, n_vars);
    count_pi = malloc(sizeof(int)*n_pi);
    implicants_table(count_pi, prime_implicants, n_pi);

    for (int i = 0; i < n_pi; i++){
        printf("o nº %d aparece %d vezes\n", i, count_pi[i]);
    }

    // Selects all the essencial prime implicants
    essencial_pi = malloc(sizeof(char*)*n_pi);
    for (i = 0; i < n_pi; i++){
        essencial_pi[i] = malloc(sizeof(char)*n_vars);
    }
    epi_index = essencials_pi(count_pi, prime_implicants, n_pi, essencial_pi);

    // Minimizes the equation e print the minimal equation
    minimization(essencial_pi, epi_index);


    free(minterms);
    free(groups);
    return 0;
}
