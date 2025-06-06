<p align="right">
  <a href="https://ko-fi.com/lunasev" target="_blank">
    <img src="https://ko-fi.com/img/githubbutton_sm.svg" alt="Ko-fi" />
  </a>
</p>

# OpenAI C

**OpenAI C** is an unofficial C client library for the OpenAI API.

It provides a lightweight wrapper for calling ChatGPT (e.g. GPT-3.5 or GPT-4) using `libcurl` and `cJSON`.

> If you prefer Make over CMake, a simple Makefile is also provided for manual builds and testing. However, CMake is the recommended build method.

---

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
git clone https://github.com/yourname/openai-c.git
cd openai-c
mkdir build
cd build
cmake ..
make
sudo make install
```

- `libopenai.a` → installed to `/usr/local/lib`
- `openai.h` → installed to `/usr/local/include`

---

## Example Usage

You can build and run the optional example program:

```bash
./openai_example
```

### Chat

Inside `examples/01_chat.c`:

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

Inside `examples/02_image.c`:

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

---

## License

This project is licensed under the [MIT](https://mit-license.org/).