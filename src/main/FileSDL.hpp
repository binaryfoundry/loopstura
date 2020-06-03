#include "SDL.hpp"

#include "../Platform.hpp"

#include <string>

void FileLoadTexture2D(std::string resource_id);

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
