#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openai.h"

int main() {
    const char* api_key = "sk-...";
    const char* model = "gpt-3.5-turbo";
    const char* prompt = "Hello, who are you?";

    if (strlen(api_key) == 0) {
        fprintf(stderr, "ERROR: OpenAI API key is missing.\n");
        return 1;
    }

    openai_init(api_key);

    char* res = openai_chat_with_model(prompt, model);
    if (res) {
        printf("Response: %s\n", res);
        free(res);
    } else {
        fprintf(stderr, "ERROR: Failed to get response from OpenAI.\n");
    }

    openai_cleanup();
    return 0;
}