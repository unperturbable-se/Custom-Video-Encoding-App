# Video Conversion Pipeline

High-performance BMP to AVI converter using a multi-process and multi-threaded architecture.

## Features

- **Parallel Encoding:** Multi-process workload with internal pthread synchronization.
- **Shared Memory:** Uses `mmap` and mutexes for high-speed frame buffering.
- **Bi-directional:** Supports BMP ↔ AVI conversion.
- **MiniApp:** Real-time UI for monitoring process status and FPS.

## How to Run

### First-time setup

For the first instance, you need to set up dependencies:
- If you are on Ubuntu, run `make apt`.
- If you are on Fedora, run `make rpm`.

After that, run `make all`.

### Editing and running

Now you’re good to go. Whenever you make changes to the code, you can test the program by running:

```bash
make test
```
You can also test the BMP tester code by running:

```bash
make bmp
```
