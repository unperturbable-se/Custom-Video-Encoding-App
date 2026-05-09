#pragma once
#include <pthread.h>
extern bool g_closeWindow;
extern int g_saves;
extern bool g_abort_process;
extern int g_num_threads_w1;
extern int g_num_threads_w2;
extern int g_num_threads_w3;
extern int g_num_threads_w4;
extern int g_num_threads_w5;
extern int g_num_images;
extern int g_images_processed;
extern pthread_mutex_t g_images_processed_lock;