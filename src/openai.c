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

#include "openai.h"

static char* api_key = NULL;

void openai_init(const char* key) {
    if (api_key) free(api_key);
    api_key = strdup(key);
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void openai_cleanup() {
    if (api_key) free(api_key);
    api_key = NULL;
    curl_global_cleanup();
}

struct memory {
    char* response;
    size_t size;
};

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct memory* mem = (struct memory*)userp;

    char* ptr = realloc(mem->response, mem->size + realsize + 1);
    if (!ptr) return 0;

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

char* openai_chat_with_model(const char* prompt, const char* model) {
    if (!api_key || !prompt || !model) return NULL;

    CURL* curl = curl_easy_init();
    if (!curl) return NULL;

    struct memory chunk = {malloc(1), 0};
    struct curl_slist* headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
    headers = curl_slist_append(headers, auth_header);

    char* escaped_prompt = curl_easy_escape(curl, prompt, 0);
    char* escaped_model = curl_easy_escape(curl, model, 0);

    cJSON* rt = cJSON_CreateObject();
    cJSON_AddStringToObject(rt, "model", model);

    cJSON* messages = cJSON_AddArrayToObject(rt, "messages");
    cJSON* msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "role", "user");
    cJSON_AddStringToObject(msg, "content", prompt);
    cJSON_AddItemToArray(messages, msg);

    char* json = cJSON_PrintUnformatted(rt);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_free(escaped_prompt);
    curl_free(escaped_model);

    if (res != CURLE_OK) {
        free(chunk.response);
        return NULL;
    }

    cJSON* root = cJSON_Parse(chunk.response);
    if (!root) {
        free(chunk.response);
        return NULL;
    }

    cJSON* choices = cJSON_GetObjectItem(root, "choices");
    cJSON* first_choice = cJSON_GetArrayItem(choices, 0);
    cJSON* message = cJSON_GetObjectItem(first_choice, "message");
    cJSON* content = cJSON_GetObjectItem(message, "content");

    if (!cJSON_IsString(content)) {
        cJSON_Delete(root);
        free(chunk.response);
        return NULL;
    }

    char* result = strdup(content->valuestring);
    cJSON_Delete(root);
    free(chunk.response);
    return result;
}

char* openai_chat(const char* prompt) {
    return openai_chat_with_model(prompt, "gpt-3.5-turbo");
}

char* openai_generate_image(const char* prompt, int n, const char* size) {
    if (!api_key || !prompt || !size || n <= 0 || n > 10) return NULL;

    CURL* curl = curl_easy_init();
    if (!curl) return NULL;

    struct memory chunk = {malloc(1), 0};
    struct curl_slist* headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
    headers = curl_slist_append(headers, auth_header);

    char json[2048];
    snprintf(json, sizeof(json),
        "{ \"prompt\": \"%s\", \"n\": %d, \"size\": \"%s\" }", prompt, n, size);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/images/generations");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        free(chunk.response);
        return NULL;
    }

    cJSON* root = cJSON_Parse(chunk.response);
    if (!root) {
        free(chunk.response);
        return NULL;
    }

    cJSON* data = cJSON_GetObjectItem(root, "data");
    if (!cJSON_IsArray(data)) {
        cJSON_Delete(root);
        free(chunk.response);
        return NULL;
    }

    char* result = cJSON_PrintUnformatted(data);
    cJSON_Delete(root);
    free(chunk.response);
    return result;
}