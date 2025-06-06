#ifndef OPENAI_H
#define OPENAI_H

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdio.h>
#include <cjson/cJSON.h>

#ifdef __cplusplus
extern "C" {
#endif

void openai_init(const char* api_key);

void openai_cleanup();

char* openai_chat_with_model(const char* prompt, const char* model);
char* openai_chat(const char* prompt);

#ifdef __cplusplus
}
#endif

#endif // OPENAI_H