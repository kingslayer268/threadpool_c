# threadpool_c

A small cross-platform thread pool library written in C.

The library provides a fixed pool of worker threads that execute submitted tasks concurrently. It supports **Linux (pthreads)** and **Windows (Win32 threads)** with a minimal and portable API.

## Build

The project uses CMake.

```bash
git clone https://github.com/kingslayer268/threadpool_c.git
cd threadpool_c

cmake -B build
cmake --build build
```

## Usage

Include the header:

```c
#include "threadpool.h"
```

Example:

```c
#include "threadpool.h"
#include <stdio.h>

void task(void *arg) {
    int id = *(int *)arg;
    printf("Task %d running\n", id);
}

int main() {
    threadpool_t *pool = threadpool_create(4);

    int args[10];
    for (int i = 0; i < 10; i++) {
        args[i] = i;
        threadpool_submit(pool, task, &args[i]);
    }

    threadpool_wait(pool);
    threadpool_destroy(pool);
}
```

## Using with CMake

```cmake
include(FetchContent)

FetchContent_Declare(
    threadpool
    GIT_REPOSITORY https://github.com/kingslayer268/threadpool_c.git
    GIT_TAG main
)

FetchContent_MakeAvailable(threadpool)

target_link_libraries(myapp threadpool)
```
