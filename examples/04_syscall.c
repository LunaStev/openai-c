#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openai.h"

char* read_syscall_log(const char* filename) {
  FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror('fopen');
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);
    return buffer;
}

int main() {
    const char* api_key = "sk-...";
    const char* model = "gpt-3.5-turbo";

    if (strlen(api_key) == 0) {
        fprintf(stderr, "ERROR: OpenAI API key is missing.\n");
        return 1;
    }

    openai_init(api_key);

    char* syscall_log = read_syscall_log("example_strace.log");
    if (!syscall_log) {
        fprintf(stderr, "ERROR: Failed to read syscall log.\n");
        openai_cleanup();
        return 1;
    }

    const char* header = "The following is a Linux system call trace (strace) log. Please explain what this log is doing, and analyze whether there are any suspicious or potentially malicious behaviors from a security perspective.\n\n";
    size_t prompt_size = strlen(header) + strlen(syscall_log) + 1;
    char* prompt = malloc(prompt_size);
    if (!prompt) {
        fprintf(stderr, "ERROR: Failed to allocate memory for prompt.\n");
        free(syscall_log);
        openai_cleanup();
        return 1;
    }

    snprintf(prompt, prompt_size, "%s%s", header, syscall_log);

    char* res = openai_chat_with_model(prompt, model);
    if (res) {
        printf("Response:\n%s\n", res);
        free(res);
    } else {
        fprintf(stderr, "ERROR: Failed to get response from OpenAI.\n");
    }

    free(prompt);
    free(syscall_log);
    openai_cleanup();
    return 0;
}