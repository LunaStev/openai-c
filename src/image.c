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
#include "openai_internal.h"

extern char* api_key;
extern size_t write_callback(void*, size_t, size_t, void*);

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

    cJSON* rt = cJSON_CreateObject();
    cJSON_AddStringToObject(rt, "prompt", prompt);
    cJSON_AddNumberToObject(rt, "n", n);
    cJSON_AddStringToObject(rt, "size", size);

    char* json = cJSON_PrintUnformatted(rt);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/images/generations");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(json);
    cJSON_Delete(rt);

    if (res != CURLE_OK) {
        free(chunk.response);
        return NULL;
    }

    cJSON* root = cJSON_Parse(chunk.response);
    cJSON* data = cJSON_GetObjectItem(root, "data");
    char* result = (cJSON_IsArray(data)) ? cJSON_PrintUnformatted(data) : NULL;

    cJSON_Delete(root);
    free(chunk.response);
    return result;
}
