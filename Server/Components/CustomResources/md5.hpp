#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <openssl/md5.h>

using StringView = std::string_view;

static void MD5Checksum(uint8_t* buffer, size_t length, uint8_t* result)
{
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, buffer, length);
    MD5_Final(result, &ctx);
}

static size_t GetFileMD5Checksum(StringView filename, uint8_t* checksum)
{
    memset(checksum, 0, MD5_DIGEST_LENGTH);

    FILE* f = fopen(filename.data(), "rb");

    if (!f)
    {
        return 0;
    }

    uint8_t buf[1024];
    size_t file_size = 0;
    MD5_CTX ctx;
    MD5_Init(&ctx);

    while (!feof(f))
    {
        auto read = fread(buf, 1, 1024, f);
        MD5_Update(&ctx, buf, read);
        file_size += read;
    }

    MD5_Final(checksum, &ctx);
    fclose(f);
    return file_size;
}

static void PrintMD5Checksum(uint8_t* checksum)
{
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        printf("%02x", checksum[i]);
    }
    printf("\n");
}

