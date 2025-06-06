# OpenAI C

**OpenAI C** is an unofficial C client library for the OpenAI Chat API.

It provides a lightweight wrapper for calling ChatGPT (e.g. GPT-3.5 or GPT-4) using `libcurl` and `cJSON`.

> If you prefer Make over CMake, a simple Makefile is also provided for manual builds and testing. However, CMake is the recommended build method.

---

## Features

- Lightweight C library for OpenAI Chat API
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

Inside `examples/chat.c`:

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

---

## License

This project is licensed under the [MIT](https://mit-license.org/).