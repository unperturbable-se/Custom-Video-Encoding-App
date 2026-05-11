#include "globals.h"
bool g_closeWindow=false;
int g_saves=0;
bool g_abort_process=false;
int g_num_threads[3]={2,2,2};

int g_num_images=100;
int g_images_decoded=-1;
int g_images_created=-1;
int g_images_encoded=-1;
pthread_mutex_t g_images_encoded_lock;
pthread_mutex_t g_images_decoded_lock;
pthread_mutex_t g_images_created_lock;

bool g_start_encoding=false;
bool g_start_decoding=false;
bool g_start_creating=false;