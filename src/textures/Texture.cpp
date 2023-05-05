#include "Texture.h"

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

void Texture::loadTexture(const std::string& texturePath)
{
    std::string header(124, 0);
    std::ifstream is(texturePath, std::ios::binary);
    if (!is) {
        printf("%s could not be opened. Are you in the right directory?\n", texturePath);
        return;
    }
    std::string filecode(4, 0);
    is.read(&filecode[0], filecode.size());
    if (filecode != "DDS ") {
        return;
    }
    // десериализация текстуры и получение информации
    is.read(&header[0], 124);
    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    unsigned int bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    std::string buffer(bufsize * sizeof(unsigned char), 0);
    is.read(&buffer[0], buffer.size());
    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        format = 0;
        break;
    }

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned int blockSize = (format ==
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, &buffer[offset]);
        offset += size;
        width /= 2;
        height /= 2;
        if (width < 1) width = 1;
        if (height < 1) height = 1;
    }
}

Texture::Texture(const std::string& texturePath)
{
    loadTexture(texturePath);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}
