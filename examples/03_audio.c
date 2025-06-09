#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openai.h"

int main() {
    const char* api_key = "sk-...";
    const char* audio_file = "alloy.wav";

    if (strlen(api_key) == 0) {
        fprintf(stderr, "ERROR: OpenAI API key is missing.\n");
        return 1;
    }

    openai_init(api_key);

    char* result = openai_transcribe_audio(audio_file);
    char* result_trans = openai_translate_audio(audio_file);
    if (result) {
        printf("Transcription result:\n%s\n", result);
        free(result);
    } else {
        printf("Failed to transcribe audio.\n");
    }
}