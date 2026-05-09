#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#pragma pack(push, 1)

typedef struct { char listId[4]; uint32_t listSize; char listType[4]; } ListChunk;

typedef struct 
{
    uint32_t microsecondsPerFrame;
    uint32_t maxBytesPerSecond;
    uint32_t paddingGranularity;
    uint32_t flags;
    uint32_t totalFrames;
    uint32_t initialFrames;
    uint32_t streamCount;
    uint32_t suggestedBufferSize;
    uint32_t videoWidth;
    uint32_t videoHeight;
    uint32_t reserved[4];
} AviMainHeader;

typedef struct 
{
    char     streamType[4];
    char     codecHandler[4];
    uint32_t flags;
    uint16_t priority;
    uint16_t language;
    uint32_t initialFrames;
    uint32_t timeScale;
    uint32_t playbackRate;
    uint32_t startTime;
    uint32_t streamLength;
    uint32_t suggestedBufferSize;
    uint32_t quality;
    uint32_t sampleSize;
    int16_t  regionLeft, regionTop, regionRight, regionBottom;
} StreamHeader;

typedef struct 
{
    uint32_t headerSize;
    int32_t  imageWidth;
    int32_t  imageHeight;
    uint16_t colorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compressionType;
    uint32_t imageSizeBytes;
    int32_t  horizontalPixelsPerMeter;
    int32_t  verticalPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t importantColors;
} BitmapInfoHeader;

typedef struct { char ChunkId[4]; uint32_t ChunkSize; } Chunk;

typedef struct 
{
    uint32_t ChunkId;
    uint32_t flags;
    uint32_t ChunkOffsetInFile;
    uint32_t ChunkDataLength;
} AviIndexEntry;

#pragma pack(pop)

class Avi_Video
{
    public:
    uint8_t* buffer;
    int bufferSize;
    Avi_Video(char* fileName)
    {
        int fd=open(fileName,O_RDONLY);
        struct stat st;
        fstat(fd,&st);
        bufferSize=st.st_size;
        buffer=(uint8_t*)mmap(NULL,bufferSize, PROT_READ, MAP_SHARED, fd, 0);
    }
    Avi_Video(uint8_t** videoFrames, int frameCount, int imageWidth, int imageHeight)
    {
        int rowStride = ((imageWidth * 3) + 3) & ~3;
        int frameSizeBytes = rowStride * imageHeight;
        bufferSize = sizeof(ListChunk) + sizeof(ListChunk) + sizeof(Chunk) + sizeof(AviMainHeader)
            + sizeof(ListChunk) + sizeof(Chunk) + sizeof(StreamHeader) + sizeof(Chunk) + sizeof(BitmapInfoHeader)
            + sizeof(ListChunk) + frameCount * (sizeof(Chunk) + frameSizeBytes)
            + sizeof(Chunk) + frameCount * sizeof(AviIndexEntry);

        buffer = (uint8_t*)malloc(bufferSize);

        ListChunk* riffChunk       = (ListChunk*)&buffer[0];
        ListChunk* hdrlListChunk   = (ListChunk*)&buffer[12];
        Chunk*     avihChunk       = (Chunk*)&buffer[24];
        AviMainHeader* aviMainHeader = (AviMainHeader*)&buffer[32];
        ListChunk* strlListChunk   = (ListChunk*)&buffer[88];
        Chunk*     streamHeaderChunk = (Chunk*)&buffer[100];
        StreamHeader* streamHeaderStruct = (StreamHeader*)&buffer[108];
        Chunk*     strfChunk       = (Chunk*)&buffer[164];
        BitmapInfoHeader* bitmapInfoHeader = (BitmapInfoHeader*)&buffer[172];
        ListChunk* moviListChunk   = (ListChunk*)&buffer[212];
        Chunk*     idx1Chunk       = (Chunk*)&buffer[224 + frameCount * (sizeof(Chunk) + frameSizeBytes)];
        AviIndexEntry* indexEntries = (AviIndexEntry*)&buffer[224 + frameCount * (sizeof(Chunk) + frameSizeBytes) + sizeof(Chunk)];

        memcpy(riffChunk->listId, "RIFF", 4);
        memcpy(riffChunk->listType, "AVI ", 4);
        riffChunk->listSize = bufferSize - 8;

        memcpy(hdrlListChunk->listId, "LIST", 4);
        memcpy(hdrlListChunk->listType, "hdrl", 4);
        hdrlListChunk->listSize = sizeof(Chunk) + sizeof(AviMainHeader) + sizeof(ListChunk) + sizeof(Chunk) + sizeof(StreamHeader) + sizeof(Chunk) + sizeof(BitmapInfoHeader);

        memcpy(avihChunk->ChunkId, "avih", 4);
        avihChunk->ChunkSize = sizeof(AviMainHeader);

        aviMainHeader->microsecondsPerFrame = 33333;
        aviMainHeader->maxBytesPerSecond = frameSizeBytes * 30;
        aviMainHeader->paddingGranularity = 0;
        aviMainHeader->flags = 0x10;
        aviMainHeader->totalFrames = frameCount;
        aviMainHeader->initialFrames = 0;
        aviMainHeader->streamCount = 1;
        aviMainHeader->suggestedBufferSize = frameSizeBytes;
        aviMainHeader->videoWidth = imageWidth;
        aviMainHeader->videoHeight = imageHeight;
        memset(aviMainHeader->reserved, 0, sizeof(aviMainHeader->reserved));

        memcpy(strlListChunk->listId, "LIST", 4);
        memcpy(strlListChunk->listType, "strl", 4);
        strlListChunk->listSize = sizeof(Chunk) + sizeof(StreamHeader) + sizeof(Chunk) + sizeof(BitmapInfoHeader);

        memcpy(streamHeaderChunk->ChunkId, "strh", 4);
        streamHeaderChunk->ChunkSize = sizeof(StreamHeader);

        memcpy(streamHeaderStruct->streamType, "vids", 4);
        memcpy(streamHeaderStruct->codecHandler, "DIB ", 4);
        streamHeaderStruct->flags = 0;
        streamHeaderStruct->priority = 0;
        streamHeaderStruct->language = 0;
        streamHeaderStruct->initialFrames = 0;
        streamHeaderStruct->timeScale = 1;
        streamHeaderStruct->playbackRate = 30;
        streamHeaderStruct->startTime = 0;
        streamHeaderStruct->streamLength = frameCount;
        streamHeaderStruct->suggestedBufferSize = frameSizeBytes;
        streamHeaderStruct->quality = 0;
        streamHeaderStruct->sampleSize = 0;
        streamHeaderStruct->regionLeft = 0;
        streamHeaderStruct->regionTop = 0;
        streamHeaderStruct->regionRight = imageWidth;
        streamHeaderStruct->regionBottom = imageHeight;

        memcpy(strfChunk->ChunkId, "strf", 4);
        strfChunk->ChunkSize = sizeof(BitmapInfoHeader);

        bitmapInfoHeader->headerSize = sizeof(BitmapInfoHeader);
        bitmapInfoHeader->imageWidth = imageWidth;
        bitmapInfoHeader->imageHeight = imageHeight;
        bitmapInfoHeader->colorPlanes = 1;
        bitmapInfoHeader->bitsPerPixel = 24;
        bitmapInfoHeader->compressionType = 0;
        bitmapInfoHeader->imageSizeBytes = frameSizeBytes;
        bitmapInfoHeader->horizontalPixelsPerMeter = 0;
        bitmapInfoHeader->verticalPixelsPerMeter = 0;
        bitmapInfoHeader->colorsUsed = 0;
        bitmapInfoHeader->importantColors = 0;

        memcpy(moviListChunk->listId, "LIST", 4);
        memcpy(moviListChunk->listType, "movi", 4);
        moviListChunk->listSize = 4 + frameCount * (sizeof(Chunk) + frameSizeBytes);

        for (int f = 0; f < frameCount; f++)
        {
            Chunk* frameChunk = (Chunk*)&buffer[224 + f * (sizeof(Chunk) + frameSizeBytes)];
            memcpy(frameChunk->ChunkId, "00dc", 4);
            frameChunk->ChunkSize = frameSizeBytes;
            uint8_t* frameData = &buffer[224 + f * (sizeof(Chunk) + frameSizeBytes) + sizeof(Chunk)];
            for (int y = imageHeight - 1; y >= 0; y--)
                memcpy(frameData + (imageHeight - 1 - y) * rowStride, videoFrames[f] + y * imageWidth * 3, imageWidth * 3);
        }

        memcpy(idx1Chunk->ChunkId, "idx1", 4);
        idx1Chunk->ChunkSize = frameCount * sizeof(AviIndexEntry);

        for (int f = 0; f < frameCount; f++)
        {
            indexEntries[f].ChunkId = 0x63643030;
            indexEntries[f].flags = 0x10;
            indexEntries[f].ChunkOffsetInFile = 4 + f * (sizeof(Chunk) + frameSizeBytes);
            indexEntries[f].ChunkDataLength = frameSizeBytes;
        }
    }

    void saveFile(char* fileName)
    {
        int fd=open(fileName,O_CREAT|O_RDWR,0666);
        write(fd,(void*)buffer,bufferSize);
        close(fd);
    }
    ~Avi_Video(){delete[] buffer;}
};