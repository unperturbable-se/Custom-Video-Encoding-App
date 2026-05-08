#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include "bmp_format.h"

#pragma pack(push, 1)

struct RIFFHeader {
    char     id[4];          // "RIFF"
    uint32_t size;           // file size - 8
    char     type[4];        // "AVI "
};

struct Chunk {
    char     id[4];
    uint32_t size;
    // data follows
};

// Main AVI header ('avih')
struct MainAVIHeader {
    uint32_t microSecPerFrame;   // frame duration (µs)
    uint32_t maxBytesPerSec;
    uint32_t paddingGranularity;
    uint32_t flags;               // e.g., 0x10 = has index
    uint32_t totalFrames;
    uint32_t initialFrames;
    uint32_t streams;
    uint32_t suggestedBufferSize;
    uint32_t width;
    uint32_t height;
    uint32_t reserved[4];
};

// Stream header ('strh')
struct StreamHeader {
    char     fccType[4];         // "vids"
    char     fccHandler[4];      // codec FOURCC, e.g. 0x00000000 = uncompressed
    uint32_t flags;
    uint16_t priority;
    uint16_t language;
    uint32_t initialFrames;
    uint32_t scale;              // time scale
    uint32_t rate;               // rate/scale = fps
    uint32_t start;
    uint32_t length;             // number of frames
    uint32_t suggestedBufferSize;
    uint32_t quality;
    uint32_t sampleSize;
    struct {
        int16_t left, top, right, bottom;
    } frame;
};

// Stream format header ('strf') – BITMAPINFOHEADER for video
struct BitmapInfoHeader {
    uint32_t size;               // size of this header (40)
    int32_t  width;
    int32_t  height;
    uint16_t planes;             // must be 1
    uint16_t bitCount;           // 24 for RGB
    uint32_t compression;        // BI_RGB = 0
    uint32_t imageSize;
    int32_t  xPelsPerMeter;
    int32_t  yPelsPerMeter;
    uint32_t clrUsed;
    uint32_t clrImportant;
};

#pragma pack(pop)

//void createAviFile(uint8_t* bmpImages[10], int width, int height) {
//    const int frames = 10;
//    const int fps = 30;
//    const int bytes_per_frame = width * height * 3; 
//    int chunk_data_size = bytes_per_frame;
//    int chunk_padding = (chunk_data_size & 1) ? 1 : 0;
//    int frame_chunk_size = 8 + chunk_data_size + chunk_padding;
//    int total_movi_data = frames * frame_chunk_size;
//    int total_file_size = 400 + total_movi_data;
//    int size=sizeof()
//    uint8_t* buffer = (uint8_t*)malloc(total_file_size);
//    uint8_t* ptr = buffer;
//
//    // ---------- RIFF header ----------
//    RIFFHeader* rh=(RIFFHeader*) &buffer[0];
//    sprintf(rh->id,"RIFF");
//    rh->size=total_file_size-8;
//    sprintf(rh->type,"AVI");
//
//    // ---------- 'hdrl' LIST ----------
//    size_t hdrl_start = ptr - buffer;
//    write_fourcc(ptr, "LIST");
//    write_u32(ptr, 0);                    // placeholder for size
//    write_fourcc(ptr, "hdrl");
//
//    // 'avih' chunk
//    write_fourcc(ptr, "avih");
//    write_u32(ptr, 56);                   // chunk size
//    write_u32(ptr, 1000000 / fps);        // microSecPerFrame
//    write_u32(ptr, bytes_per_frame * fps);// maxBytesPerSec
//    write_u32(ptr, 2);                    // paddingGranularity
//    write_u32(ptr, 0x10);                 // flags (has index)
//    write_u32(ptr, frames);               // totalFrames
//    write_u32(ptr, 0);                    // initialFrames
//    write_u32(ptr, 1);                    // streams
//    write_u32(ptr, bytes_per_frame);      // suggestedBufferSize
//    write_u32(ptr, width);
//    write_u32(ptr, height);
//    for (int i = 0; i < 4; ++i) write_u32(ptr, 0); // reserved
//
//    // 'strl' LIST (video stream)
//    size_t strl_start = ptr - buffer;
//    write_fourcc(ptr, "LIST");
//    write_u32(ptr, 0);                    // placeholder
//    write_fourcc(ptr, "strl");
//
//    // 'strh' chunk
//    write_fourcc(ptr, "strh");
//    write_u32(ptr, 56);
//    write_fourcc(ptr, "vids");
//    write_fourcc(ptr, "\0\0\0\0");        // uncompressed
//    write_u32(ptr, 0);                    // flags
//    write_u16(ptr, 0);                    // priority
//    write_u16(ptr, 0);                    // language
//    write_u32(ptr, 0);                    // initialFrames
//    write_u32(ptr, 1);                    // scale
//    write_u32(ptr, fps);                  // rate
//    write_u32(ptr, 0);                    // start
//    write_u32(ptr, frames);               // length
//    write_u32(ptr, bytes_per_frame);      // suggestedBufferSize
//    write_u32(ptr, 0);                    // quality
//    write_u32(ptr, 0);                    // sampleSize
//    write_u16(ptr, 0); write_u16(ptr, 0); // left, top
//    write_u16(ptr, width); write_u16(ptr, height); // right, bottom
//
//    // 'strf' chunk (BITMAPINFOHEADER)
//    write_fourcc(ptr, "strf");
//    write_u32(ptr, 40);
//    write_u32(ptr, 40);                   // size of this header
//    write_u32(ptr, width);
//    write_u32(ptr, height);
//    write_u16(ptr, 1);                    // planes
//    write_u16(ptr, 24);                   // bitCount
//    write_u32(ptr, 0);                    // compression (BI_RGB)
//    write_u32(ptr, bytes_per_frame);      // imageSize
//    write_u32(ptr, 0);                    // xPelsPerMeter
//    write_u32(ptr, 0);                    // yPelsPerMeter
//    write_u32(ptr, 0);                    // clrUsed
//    write_u32(ptr, 0);                    // clrImportant
//
//    // Fix 'strl' LIST size
//    uint32_t strl_size = (ptr - buffer) - strl_start - 8;
//    uint8_t* temp = buffer + strl_start + 4;
//    write_u32(temp, strl_size);           // overwrite placeholder
//
//    // Fix 'hdrl' LIST size
//    uint32_t hdrl_size = (ptr - buffer) - hdrl_start - 8;
//    temp = buffer + hdrl_start + 4;
//    write_u32(temp, hdrl_size);
//
//    // ---------- 'movi' LIST ----------
//    size_t movi_start = ptr - buffer;
//    write_fourcc(ptr, "LIST");
//    write_u32(ptr, 0);                    // placeholder
//    write_fourcc(ptr, "movi");
//
//    uint32_t frame_offsets[10];           // offset from start of movi LIST (including header)
//    uint32_t frame_sizes[10];
//
//    // Write each frame as '00dc' chunk
//    for (int i = 0; i < frames; ++i) {
//        frame_offsets[i] = ptr - buffer - movi_start;
//        frame_sizes[i] = bytes_per_frame;
//
//        write_fourcc(ptr, "00dc");
//        write_u32(ptr, bytes_per_frame);
//
//        // Copy pixel data (assumed top‑down BGR24)
//        memcpy(ptr, bmpImages[i], bytes_per_frame);
//        ptr += bytes_per_frame;
//
//        // Pad to even size if needed
//        if (bytes_per_frame & 1) {
//            *ptr++ = 0;
//        }
//    }
//
//    // Fix 'movi' LIST size
//    uint32_t movi_size = (ptr - buffer) - movi_start - 8;
//    temp = buffer + movi_start + 4;
//    write_u32(temp, movi_size);
//
//    // ---------- 'idx1' chunk (index) ----------
//    write_fourcc(ptr, "idx1");
//    write_u32(ptr, frames * 16);          // total index size
//
//    for (int i = 0; i < frames; ++i) {
//        // write "00dc" as fourCC
//        write_fourcc(ptr, "00dc");
//        write_u32(ptr, 0x10);             // keyframe flag
//        // Offset from start of 'movi' data (skip the 12‑byte LIST header)
//        uint32_t rel_offset = frame_offsets[i] - 12;
//        write_u32(ptr, rel_offset);
//        write_u32(ptr, frame_sizes[i]);
//    }
//
//    // At this point ptr == buffer + total_file_size.
//    // The complete AVI file is now stored in 'buffer'.
//    // (The caller is responsible for freeing the buffer or writing it to disk.)
//}