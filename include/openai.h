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

/**
 * Initialize the OpenAI client with an API key.
 */
void openai_init(const char* api_key);

/**
 * Cleanup resources used by the OpenAI client.
 */
void openai_cleanup();

/**
 * Send a chat prompt with a custom model.
 */
char* openai_chat_with_model(const char* prompt, const char* model);

/**
 * Send a chat prompt using the default model.
 */
char* openai_chat(const char* prompt);

#ifdef __cplusplus
}
#endif

#endif // OPENAI_H