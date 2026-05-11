#pragma once
#include <pthread.h>
extern bool g_closeWindow;
extern int g_saves;
extern bool g_abort_process;
extern int g_num_threads[3];

extern int g_num_images;
extern int g_images_processed;
extern pthread_mutex_t g_images_decoded_lock;

extern int g_images_created;
extern int g_images_encoded;
extern int g_images_decoded;
extern pthread_mutex_t g_images_created_lock;
extern pthread_mutex_t g_images_encoded_lock;

extern bool g_start_encoding;
extern bool g_start_decoding;
extern bool g_start_creating;
