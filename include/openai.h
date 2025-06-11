/*
 * MIT License
 *
 * Copyright (c) 2025 LunaStev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

/**
 * @brief Generates images from a text prompt using DALL·E.
 *
 * This function sends a prompt to the OpenAI image generation endpoint and returns the image URLs.
 *
 * @param prompt The description of the image to generate.
 * @param n Number of images to generate (1–10).
 * @param size The size of the image (e.g., "256x256", "512x512", "1024x1024").
 * @return The image URL list in a JSON array string (must be freed by the caller), or NULL on error.
 */
char* openai_generate_image(const char* prompt, int n, const char* size);

/**
 * @brief Transcribes an audio file using OpenAI's Whisper model.
 *
 * This function sends an audio file to the OpenAI transcription API
 * and returns the transcribed text.
 *
 * @param filepath The path to the audio file to be transcribed (e.g., .mp3, .wav).
 * @return The transcribed text as a string (must be freed by the caller), or NULL on error.
 */
char* openai_transcribe_audio(const char* filepath);

/**
 * @brief Translates non-English audio to English using OpenAI's Whisper model.
 *
 * This function sends a non-English audio file to the OpenAI translation API
 * and returns the translated English text.
 *
 * @param filepath The path to the audio file to be translated (e.g., .mp3, .wav).
 * @return The translated English text as a string (must be freed by the caller), or NULL on error.
 */
char* openai_translate_audio(const char* filepath);

/**
 * @brief Creates a JSON string for generating text embeddings using OpenAI's embedding API.
 *
 * This function constructs a JSON-formatted string containing the input text and model name,
 * which is suitable for use in a request to OpenAI's embedding API.
 *
 * @param input The input text for which the embedding is to be generated.
 * @param model The name of the embedding model to be used (e.g., "text-embedding-ada-002").
 * @return A dynamically allocated JSON string (must be freed by the caller), or NULL on error.
 */
char* openai_create_embedding_json(const char* input, const char* model);

/**
 * @brief Generates an embedding vector for the given input text using OpenAI's embedding API.
 *
 * This function sends the input text and model to the OpenAI embedding API,
 * receives the resulting embedding vector, and returns it as a dynamically allocated float array.
 *
 * @param input The input text for which the embedding is to be generated.
 * @param model The name of the embedding model to use (e.g., "text-embedding-ada-002").
 * @param length A pointer to a size_t variable that will receive the length of the returned array.
 * @return A dynamically allocated array of floats representing the embedding (must be freed by the caller),
 *         or NULL on error. The length of the array is stored in *length.
 */
float* openai_create_embedding_array(const char* input, const char* model, size_t* length);

#ifdef __cplusplus
}
#endif

#endif // OPENAI_H