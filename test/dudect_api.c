
#include <api.h>
#include <time.h>
// define DUDECT implementation env before including dudect.h header library
#define DUDECT_IMPLEMENTATION
#ifndef DUDECT_TIMEOUT
#define DUDECT_TIMEOUT 300
#endif

#include <dudect/dudect.h>
#include <xkcp/SimpleFIPS202.h>


// dudect do_one_computation implementation
uint8_t do_one_computation(uint8_t *data) {
    perm_t p = {0};
    perm_set_random(p, data);
    return p[0] ^ 0x0;
}

// dudect prepare_input implementation
void prepare_inputs(dudect_config_t *c, uint8_t *input_data, uint8_t *classes) {
    randombytes(input_data, c->number_measurements * c->chunk_size);
    for (size_t i = 0; i < c->number_measurements; i++) {
        classes[i] = randombit();
        if (classes[i] == 0) {
            memset(input_data + (size_t) i * c->chunk_size, 0x00, c->chunk_size);
        } else {
            // leave random
        }
    }
}

int exit_code(dudect_state_t *state) {
    if ((*state) == DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void timeout(double start, double timeout, dudect_state_t *state) {
    double elapsed_time = (clock() - start) / ((double) CLOCKS_PER_SEC);
    if (elapsed_time > timeout) {
        exit(exit_code(state));
    }
}

int main(int argc, char **argv) {

    (void) argc;
    (void) argv;
    double start_time;


    dudect_config_t config = {
            .chunk_size = SEED_BYTES,
            .number_measurements = 10000,
    };

    dudect_state_t state = DUDECT_NO_LEAKAGE_EVIDENCE_YET;

    dudect_ctx_t ctx;

    dudect_init(&ctx, &config);

    start_time = clock();
    while (state == DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
        state = dudect_main(&ctx);
        timeout(start_time, DUDECT_TIMEOUT, &state);
    }

    dudect_free(&ctx);

    return exit_code(&state);
}
