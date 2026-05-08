#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#pragma pack(push, 1)

typedef struct { char fcc[4]; uint32_t cb; char type[4]; } LISTCHUNK;

typedef struct 
{
    uint32_t dwMicroSecPerFrame;
    uint32_t dwMaxBytesPerSec;
    uint32_t dwPaddingGranularity;
    uint32_t dwFlags;
    uint32_t dwTotalFrames;
    uint32_t dwInitialFrames;
    uint32_t dwStreams;
    uint32_t dwSuggestedBufferSize;
    uint32_t dwWidth;
    uint32_t dwHeight;
    uint32_t dwReserved[4];
} AVIHDR;

typedef struct 
{
    char     fccType[4];
    char     fccHandler[4];
    uint32_t dwFlags;
    uint16_t wPriority;
    uint16_t wLanguage;
    uint32_t dwInitialFrames;
    uint32_t dwScale;
    uint32_t dwRate;
    uint32_t dwStart;
    uint32_t dwLength;
    uint32_t dwSuggestedBufferSize;
    uint32_t dwQuality;
    uint32_t dwSampleSize;
    int16_t  left, top, right, bottom;
} STRH;

typedef struct 
{
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct { char fcc[4]; uint32_t cb; } CHUNK;

typedef struct 
{
    uint32_t dwChunkId;
    uint32_t dwFlags;
    uint32_t dwChunkOffset;
    uint32_t dwChunkLength;
} AVIINDEXENTRY;

#pragma pack(pop)

void aviConverter(uint8_t** frames, int num_frames, int width, int height, const char* filename) 
{
    int stride = ((width * 3) + 3) & ~3;
    int fsz = stride * height;

    int total = sizeof(LISTCHUNK)
        + sizeof(LISTCHUNK) + sizeof(CHUNK) + sizeof(AVIHDR)
        + sizeof(LISTCHUNK) + sizeof(CHUNK) + sizeof(STRH) + sizeof(CHUNK) + sizeof(BITMAPINFOHEADER)
        + sizeof(LISTCHUNK) + num_frames * (sizeof(CHUNK) + fsz)
        + sizeof(CHUNK) + num_frames * sizeof(AVIINDEXENTRY);

    uint8_t* buf = (uint8_t*)calloc(1, total);
    if (!buf) return;
    int off = 0;

    LISTCHUNK* riff = (LISTCHUNK*)&buf[off]; 
    off += sizeof(LISTCHUNK);
    memcpy(riff->fcc, "RIFF", 4); 
    memcpy(riff->type, "AVI ", 4); 
    riff->cb = total - 8;

    LISTCHUNK* hdrl = (LISTCHUNK*)&buf[off]; 
    off += sizeof(LISTCHUNK);
    memcpy(hdrl->fcc, "LIST", 4); 
    memcpy(hdrl->type, "hdrl", 4);

    CHUNK* avih_chunk = (CHUNK*)&buf[off]; 
    off += sizeof(CHUNK);
    memcpy(avih_chunk->fcc, "avih", 4); 
    avih_chunk->cb = sizeof(AVIHDR);

    AVIHDR* avih = (AVIHDR*)&buf[off]; 
    off += sizeof(AVIHDR);
    avih->dwMicroSecPerFrame = 33333;
    avih->dwMaxBytesPerSec = fsz * 30;
    avih->dwFlags = 0x110;
    avih->dwTotalFrames = num_frames;
    avih->dwStreams = 1;
    avih->dwSuggestedBufferSize = fsz;
    avih->dwWidth = width;
    avih->dwHeight = height;

    LISTCHUNK* strl = (LISTCHUNK*)&buf[off]; 
    off += sizeof(LISTCHUNK);
    memcpy(strl->fcc, "LIST", 4); 
    memcpy(strl->type, "strl", 4);

    CHUNK* strh_chunk = (CHUNK*)&buf[off]; 
    off += sizeof(CHUNK);
    memcpy(strh_chunk->fcc, "strh", 4); 
    strh_chunk->cb = sizeof(STRH);

    STRH* strh = (STRH*)&buf[off]; 
    off += sizeof(STRH);
    memcpy(strh->fccType, "vids", 4); 
    memcpy(strh->fccHandler, "DIB ", 4);
    strh->dwScale = 1; strh->dwRate = 30;
    strh->dwLength = num_frames;
    strh->dwSuggestedBufferSize = fsz;
    strh->dwQuality = 10000;
    strh->right = width; strh->bottom = height;

    CHUNK* strf_chunk = (CHUNK*)&buf[off]; 
    off += sizeof(CHUNK);
    memcpy(strf_chunk->fcc, "strf", 4); 
    strf_chunk->cb = sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER* bmi = (BITMAPINFOHEADER*)&buf[off]; 
    off += sizeof(BITMAPINFOHEADER);
    bmi->biSize = sizeof(BITMAPINFOHEADER);
    bmi->biWidth = width; 
    bmi->biHeight = height;
    bmi->biPlanes = 1; 
    bmi->biBitCount = 24;
    bmi->biSizeImage = fsz;

    hdrl->cb = (uint32_t)(off - ((uint8_t*)hdrl - buf) - 8);
    strl->cb = (uint32_t)(off - ((uint8_t*)strl - buf) - 8);

    LISTCHUNK* movi = (LISTCHUNK*)&buf[off]; 
    off += sizeof(LISTCHUNK);
    memcpy(movi->fcc, "LIST", 4); 
    memcpy(movi->type, "movi", 4);
    int movi_start_off = off;

    for (int f = 0; f < num_frames; f++) 
    {
        CHUNK* fc = (CHUNK*)&buf[off]; 
        off += sizeof(CHUNK);
        memcpy(fc->fcc, "00dc", 4); 
        fc->cb = fsz;
        for (int y = height-1; y >= 0; y--) 
        {
            memcpy(&buf[off], frames[f] + y * width * 3, width * 3);
            off += stride;
        }
    }
    movi->cb = (uint32_t)(off - ((uint8_t*)movi - buf) - 8);

    CHUNK* idx1 = (CHUNK*)&buf[off]; 
    off += sizeof(CHUNK);
    memcpy(idx1->fcc, "idx1", 4); 
    idx1->cb = num_frames * sizeof(AVIINDEXENTRY);

    for (int f = 0; f < num_frames; f++) 
    {
        AVIINDEXENTRY* e = (AVIINDEXENTRY*)&buf[off]; 
        off += sizeof(AVIINDEXENTRY);
        e->dwChunkId = 0x63643030;
        e->dwFlags = 0x10;
        e->dwChunkOffset = movi_start_off - sizeof(LISTCHUNK) + f * (sizeof(CHUNK) + fsz);
        e->dwChunkLength = fsz;
    }

    int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0644);
    if (fd >= 0) { write(fd, buf, off); close(fd); }
    free(buf);
}