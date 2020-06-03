#include "SDL.hpp"

#include "../Platform.hpp"

#include <string>
#include <vector>

void FileLoadTexture2D(
    std::string resource_id,
    uint8_t& bpp,
    uint32_t& width,
    uint32_t& height,
    std::vector<uint8_t>& data);

class File
{
private:
    size_t length;

#if defined (IS_PLATFORM_EMSCRIPTEN)
    FILE* handle;
#else
    SDL_RWops* handle;
#endif

public:
    File(std::string path, std::string mode);
    virtual ~File();

    size_t Read(void* buffer, size_t size, size_t count);

    std::string ReadString(uint16_t count);
    std::string ReadString();

    size_t Length();
};
