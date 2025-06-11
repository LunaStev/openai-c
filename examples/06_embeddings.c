#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openai.h"

int main() {
    const char* api_key = "sk-...";
    const char* model = "text-embedding-3-small";
    const char* prompt = "Hello world";

    if (strlen(api_key) == 0) {
        fprintf(stderr, "ERROR: OpenAI API key is missing.\n");
        return 1;
    }

    openai_init(api_key);

    char* res = openai_create_embedding_json(prompt, model);
    if (res) {
        printf("Response: %s\n", res);
        free(res);
    } else {
        fprintf(stderr, "ERROR: Failed to get response from OpenAI.\n");
    }

    size_t length;
    float* embedding = openai_create_embedding_array(prompt, model, &length);
    if (embedding) {
        printf("Embedding vector (%zu floats):\n", length);
        for (size_t i = 0; i < length; i++) {
            printf("%f ", embedding[i]);
            if ((i + 1) % 8 == 0) printf("\n");
        }
        printf("\n");
        free(embedding);
    } else {
        fprintf(stderr, "ERROR: Failed to get embedding vector from OpenAI.\n");
    }

    openai_cleanup();
    return 0;
}