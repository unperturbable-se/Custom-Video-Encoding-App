# Video Conversion Pipeline

High-performance BMP to AVI converter using a multi-process and multi-threaded architecture.

---

## Features

* **Parallel Encoding:** Multi-process workload with internal pthread synchronization.
* **Shared Memory:** Uses `mmap` and Mutexes for high-speed frame buffering.
* **Bi-directional:** Supports BMP $\leftrightarrow$ AVI conversion.
* **MiniApp:** Real-time UI for monitoring process status and FPS.

##How to run
#first time setup
* for the first instance you need to setup dependencies
* if you are in ubuntu run `make apt`, if you are in fedora run `make rpm`
* after that run `make all`

#editing and running
*now your good to go. whenever you make changes to the code, you can test the program by running: `make test`