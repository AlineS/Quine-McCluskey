#include "functions.c"

int main(int argc, char const *argv[]) {
    int i, j, k, n_pi, index = 0, epi_index, mult = 2, *count_pi;
    char **essencial_pi;
    minterm *minterms;
    minterm_group **groups, prime_implicants;

    clock_gettime(CLOCK_MONOTONIC, &start);
    // Read the entry minterms numbers
    minterms = read_minterms(argc, argv);

    // Transform minterms numbers to bits
    // TODO paralelizar
    for(i = 0; i < n_mint; i++){
        minterms[i].v_bit = int_to_bin(minterms[i].v_int[0]);
    }

    // Count how much ones each minterm bit have
    // TODO paralelizar
    count_ones(minterms);

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("- Define terms: %f s\n", time_function(&end) - time_function(&start));


    clock_gettime(CLOCK_MONOTONIC, &start);
    // Separates minterms in groups depend on the number of ones
    groups = malloc(sizeof(minterm_group*)*n_vars-1);
    groups[index++] = classify_groups(minterms);

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("- Classify all the bits into groups of ones: %f s\n", time_function(&end) - time_function(&start));


    clock_gettime(CLOCK_MONOTONIC, &start);
    // Compare all groups of minterms
    for (index, i = 1; index < n_vars; index++, i++){
        groups[index] = malloc(sizeof(minterm_group)*(n_ones_groups-i));
    }

    // TODO paralelizar comparações entre grupos
    prime_implicants = compare_groups(groups);

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("- Compare bits in adjacent groups: %f s\n", time_function(&end) - time_function(&start));


    clock_gettime(CLOCK_MONOTONIC, &start);
    // Counts the number of occurence of each variable
    n_pi = pow(2, n_vars);
    count_pi = malloc(sizeof(int)*n_pi);
    // TODO paralelizar
    implicants_table(count_pi, prime_implicants, n_pi);

    // Selects all the essencial prime implicants
    essencial_pi = malloc(sizeof(char*)*n_pi);
    for (i = 0; i < n_pi; i++){
        essencial_pi[i] = malloc(sizeof(char)*n_vars);
    }
    epi_index = essencials_pi(count_pi, prime_implicants, n_pi, essencial_pi);

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("- Find essencial prime implicants: %f s\n", time_function(&end) - time_function(&start));


    clock_gettime(CLOCK_MONOTONIC, &start);
    // Minimizes the equation e print the minimal equation
    minimization(essencial_pi, epi_index);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("- Prints the minimal function: %f s\n", time_function(&end) - time_function(&start));

    free(minterms);
    free(groups);
    return 0;
}
