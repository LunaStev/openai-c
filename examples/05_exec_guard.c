#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "openai.h"

int main() {
    const char* api_key = "sk-...";
    const char* model = "gpt-3.5-turbo";
    const char* program = "/bin/ls";
    const char* arg1 = "-la";

    char prompt[2048];
    snprintf(prompt, sizeof(prompt),
        "A process is about to execute the following command:\nPath: %s\nArguments: %s\n\nExplain what this command will do and assess whether it could be dangerous from a security perspective.",
        program, arg1);

    if (strlen(api_key) == 0) {
        fprintf(stderr, "ERROR: API key missing.\n");
        return 1;
    }

    openai_init(api_key);

    char* res = openai_chat_with_model(prompt, model);
    if (res) {
        printf("[GPT Analysis]\n%s\n", res);
        free(res);
    } else {
        fprintf(stderr, "ERROR: GPT analysis failed.\n");
    }

    openai_cleanup();

    printf("\n[Running Actual Command: %s %s]\n", program, arg1);
    execl(program, program, arg1, NULL);

    perror("execl failed");
    return 1;
}