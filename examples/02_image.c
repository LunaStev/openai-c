#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openai.h"

int main() {
    const char* api_key = "sk-...";
    const char* prompt = "Draw me a cute cat.";

    if (strlen(api_key) == 0) {
        fprintf(stderr, "ERROR: OpenAI API key is missing.\n");
        return 1;
    }

    openai_init(api_key);

    char* urls = openai_generate_image(prompt, 2, "512x512");
    if (urls) {
        printf("Image URLs: %s\n", urls);
        free(urls);
    } else {
        printf("Failed to generate image\n");
    }
}