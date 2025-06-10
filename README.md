<img src=".github/image/OpenAI-logos-2025/SVGs/OpenAI-white-monoblossom.svg" height="32" align="right" alt="OpenAI logo"/>

# OpenAI C

**OpenAI C** is an unofficial C client library for the OpenAI API.

It provides a lightweight wrapper for calling ChatGPT (e.g. GPT-3.5 or GPT-4) using `libcurl` and `cJSON`.

---

## Support

If you find this project helpful, consider supporting it:

[![Sponsor LunaStev](https://img.shields.io/badge/Sponsor-LunaStev-ff69b4?logo=GitHub%20Sponsors&style=for-the-badge)](https://github.com/sponsors/LunaStev)

<a href="https://ko-fi.com/lunasev" target="_blank">
  <img src="https://ko-fi.com/img/githubbutton_sm.svg" alt="Support me on Ko-fi" />
</a>

## Features

- Lightweight C library for OpenAI API
- Uses standard C with no runtime dependencies beyond `libcurl` and `cJSON`
- Suitable for embedded or low-level systems
- Optional example CLI program (`openai_example`)
- Easy installation with CMake
- Licensed under MIT

---

## Requirements

- `libcurl` development package
- `cJSON` development package
- CMake ≥ 3.10
- C compiler (C99 or later)

### On Debian/Ubuntu:

```bash
sudo apt install build-essential cmake libcurl4-openssl-dev libcjson-dev
```

---

## Build and Install

```bash
git clone https://github.com/LunaStev/openai-c.git
./scripts/install.sh
```

or:

```bash
git clone https://github.com/LunaStev/openai-c.git
cd openai-c
mkdir build
cd build
cmake ..
make
sudo make install
```

- `libopenai.a` → installed to `/usr/local/lib/static`
- `libopenai.so` → installed to `/usr/local/lib/shared`
- `openai.h` → installed to `/usr/local/include`

---

## Example Usage

### Chat

This example demonstrates how to send a chat message to the OpenAI API using the `gpt-3.5-turbo` model. It sends a prompt ("Hello, who are you?") and prints the assistant’s response.

```c
#include "openai.h"

int main() {
    openai_init("sk-...");

    char* res = openai_chat_with_model("Hello, who are you?", "gpt-3.5-turbo");
    if (res) {
        printf("Response: %s\n", res);
        free(res);
    }

    openai_cleanup();
    return 0;
}
```

Output:

```text
Response: Hello! I am an AI digital assistant here to help you with any questions or tasks you may have. How can I assist you today?
```

### Image

This example shows how to use the OpenAI image generation API (DALL·E) to generate two 512x512 images based on the prompt "Draw me a cute cat." The result is a list of image URLs.

```c
#include "openai.h"

int main() {
    const char* api_key = "sk-...";
    const char* prompt = "Draw me a cute cat.";

    if (strlen(api_key) == 0) {
        fprintf(stderr, "ERROR: OpenAI API key is missing.\n");
        return 1;
    }

    openai_init(api_key);

    char* urls = openai_generate_image(prompt, 2, "512x512");
    if (urls) {
        printf("Image URLs: %s\n", urls);
        free(urls);
    } else {
        printf("Failed to generate image\n");
    }
}
```

Output:

![02_image1.png](.github/image/02_image1.png)

![02_image2.png](.github/image/02_image2.png)


### Audio

This example demonstrates how to use OpenAI’s audio transcription and translation features. It transcribes a `.wav` file to text and also translates it into English using Whisper.

```c
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
        printf("translation result:\n%s\n", result_trans);
        free(result);
    } else {
        printf("Failed to transcribe audio.\n");
    }
}
```

Output:

```text
Transcription result:
Estoy presente ahora. Me amo. Estoy libre de mi ira. Estoy libre de mi tristedad. El amor es mi experiencia.
translation result:
I am present now. I love myself. I am free from my anger. I am free from my sadness. Love is my experience.
```

### Exec Guard

This advanced example analyzes a system command (like `/bin/ls -la`) using GPT before actually executing it. GPT gives a security analysis, and the command only runs afterward. It’s a prototype for AI-driven security checks.

```c
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
```

Output:

```text
[GPT Analysis]
The command /bin/ls -la will list all files and directories in the specified directory in a long format, including hidden files (denoted by a dot at the beginning of the file/directory name) and detailed information such as file permissions, owner, size, and last modified date.

From a security perspective, this command is generally not considered dangerous as it is a common and necessary tool for system administration and file management. However, it could potentially leak sensitive information if used without caution in a shared or public environment, as it reveals detailed information about files and directories that could be confidential or proprietary.

It is important for system administrators to be mindful of where and how they execute commands like ls -la, and to ensure that they have appropriate permissions to access the information being displayed. Regularly reviewing and monitoring file listings can also help to prevent unauthorized access or exposure of sensitive data.

[Running Actual Command: /bin/ls -la]
total 636
drwxrwxrwx 1 user user   4096 Jun  9  2025 .
drwxrwxrwx 1 user user   4096 Jun  9 13:19 ..
drwxrwxrwx 1 user user   4096 Jun  9 12:29 .cmake
-rwxrwxrwx 1 user user  48552 Jun  9 12:48 01_chat
-rwxrwxrwx 1 user user  48112 Jun  9 12:48 02_image
-rwxrwxrwx 1 user user  48704 Jun  9 13:02 03_audio
-rwxrwxrwx 1 user user  53656 Jun  9 13:30 04_syscall
-rwxrwxrwx 1 user user  48848 Jun  9  2025 05_exec_guard
-rwxrwxrwx 1 user user  12653 Jun  9 12:29 CMakeCache.txt
drwxrwxrwx 1 user user   4096 Jun  9  2025 CMakeFiles
-rwxrwxrwx 1 user user  16476 Jun  9 13:35 Makefile
drwxrwxrwx 1 user user   4096 Jun  9 12:29 Testing
-rwxrwxrwx 1 user user 199200 Jun  9 13:02 aolloy.mp3
-rwxrwxrwx 1 user user   2379 Jun  9 12:29 cmake_install.cmake
-rwxrwxrwx 1 user user  11584 Jun  9 13:29 example_strace.log
-rwxrwxrwx 1 user user 140278 Jun  9 12:48 libopenai.a
```

---

## License

This project is licensed under the [MIT](https://mit-license.org/).
