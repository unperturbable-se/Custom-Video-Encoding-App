# Video Conversion Pipeline

High-performance BMP to AVI converter using a multi-process and multi-threaded architecture.

---

## Features

* **Parallel Encoding:** Multi-process workload with internal pthread synchronization.
* **Shared Memory:** Uses `mmap` and Mutexes for high-speed frame buffering.
* **Bi-directional:** Supports BMP $\leftrightarrow$ AVI conversion.
* **MiniApp:** Real-time UI for monitoring process status and FPS.