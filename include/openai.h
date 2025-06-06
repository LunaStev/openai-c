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
 * @brief Initializes the OpenAI client with the given API key.
 *
 * This function must be called before using any other OpenAI-related function.
 *
 * @param api_key Your OpenAI API key.
 */
void openai_init(const char* api_key);

/**
 * @brief Cleans up any resources used by the OpenAI client.
 *
 * Call this function after you're done using the OpenAI client to release resources.
 */
void openai_cleanup(void);

/**
 * @brief Sends a prompt to OpenAI using a specific model.
 *
 * This function sends the given prompt to the OpenAI API and returns the model's response.
 *
 * @param prompt The message to send.
 * @param model The name of the model to use (e.g., "gpt-3.5-turbo").
 * @return The response string (must be freed by the caller), or NULL on error.
 */
char* openai_chat_with_model(const char* prompt, const char* model);

/**
 * @brief Sends a prompt to OpenAI using the default model.
 *
 * Equivalent to calling openai_chat_with_model() with a default model like "gpt-3.5-turbo".
 *
 * @param prompt The message to send.
 * @return The response string (must be freed by the caller), or NULL on error.
 */
char* openai_chat(const char* prompt);

#ifdef __cplusplus
}
#endif

#endif // OPENAI_H