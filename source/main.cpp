#include <stdio.h>
#include <string>
#include <sstream>
#include "trained.hpp"
#include "trained_weight.hpp"            // keep the weights in ROM for now, we have plenty of internal flash

#define BUFFER_SIZE                             10 * 1024 * 1024
#define INFERENCING_OUT_TENSOR_NAME             "y_pred/Softmax:0"

void classify(std::vector<float> data) {
    RamTensor<float> *input_x = new RamTensor<float>({ 1, 33 });
    float *buff = (float*)input_x->write(0, 0);

    for (size_t ix = 0; ix < data.size(); ix++) {
        buff[ix] = data[ix];
    }

    Context ctx;
    get_trained_ctx(ctx, input_x);
    ctx.eval();

    S_TENSOR pred_tensor = ctx.get(INFERENCING_OUT_TENSOR_NAME);  // getting a reference to the output tensor

    uint32_t output_neurons = pred_tensor->getShape()[1];
    // printf("output_neurons %u\n", output_neurons);

    const float* ptr_pred = pred_tensor->read<float>(0, 0);

    printf("    [ ");

    for (uint32_t ix = 0; ix < output_neurons; ix++) {
        printf("%f", *(ptr_pred + ix));
        if (ix != output_neurons - 1) {
            printf(", ");
        }
        // printf("%lu: %f\n", ix + 1, *(ptr_pred + ix));
    }
    printf(" ]");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Missing filename, usage:\n\tutensor-fuzzer FILE_NAME\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        printf("Failed to open file '%s'\n", argv[1]);
        return 1;
    }

    char *buffer = (char*)malloc(BUFFER_SIZE);
    if (!buffer) {
        printf("Failed to allocate buffer\n");
        return 1;
    }

    size_t read = fread(buffer, 1, BUFFER_SIZE, file);

    if (read == BUFFER_SIZE) {
        printf("Reached end of buffer, you should enlarge it\n");
        return 1;
    }

    printf("Begin output\n");
    printf("[\n");

    std::stringstream ss(buffer);
    std::string to;

    bool first = true;

    while (std::getline(ss, to, '\n')) {
        if (!first) {
            printf(",\n");
        }

        first = false;

        std::vector<float> data;
        std::string data_token;
        std::stringstream to_stream(to);

        while (std::getline(to_stream, data_token, ' ')) {
            data.push_back(std::stof(data_token));
        }

        classify(data);
    }

    free(buffer);

    printf("]\n");
    printf("End output\n");
}
