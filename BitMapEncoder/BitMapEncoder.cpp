#include "BitMapEncoder.hpp"

void BitMapEncoder::writeBinary8bit(std::ostream& out, const std::byte* c, std::size_t n)
{
    out.write(reinterpret_cast<const char*>(c), n);
}

void BitMapEncoder::writeBinary16bit(std::ostream& out, std::uint16_t v)
{
    out.put(v).put(v >> 8);
}

void BitMapEncoder::writeBinary32bit(std::ostream& out, std::uint32_t v)
{
    out.put(v).put(v >> 8).put(v >> 16).put(v >> 24);
}

bool BitMapEncoder::encode(const std::span<const std::byte>& imageData,
                           const std::uint32_t width,
                           const std::uint32_t height,
                           const std::string& filePath,
                           bool hasAlphaChannel = false)
{
    if(std::ofstream imageFile(filePath, std::ios::out | std::ios::binary); imageFile.is_open())
    {
        std::uint32_t bytesPerPixel = hasAlphaChannel ? 4u : 3u;
        std::uint32_t paddingSize = (4u - (width * bytesPerPixel) % 4u) % 4u;
        std::uint32_t bitmapSize = imageData.size() * sizeof(std::byte);
        std::uint32_t bytesWidth = bytesPerPixel * width;

        // Bitmap file header
        writeBinary16bit(imageFile, MB);   // signature
        writeBinary32bit(imageFile, FULL_HEADER_SIZE + bitmapSize + paddingSize * height);   // fileSize
        writeBinary32bit(imageFile, EMPTY);   // reserved
        writeBinary32bit(imageFile, FULL_HEADER_SIZE);   // offset
        // Bitmap information header file
        writeBinary32bit(imageFile, BITMAP_V5_HEADER_SIZE);   // DIBHeaderSize
        writeBinary32bit(imageFile, width);   // width
        writeBinary32bit(imageFile, height);   // height
        writeBinary16bit(imageFile, 1u);   // numberColorPlanes
        writeBinary16bit(imageFile, bytesPerPixel * 8u);   // bitsPerPixel
        writeBinary32bit(imageFile, hasAlphaChannel ? BI_BITFIELDS : BI_RGB);   // compression
        writeBinary32bit(imageFile, bitmapSize);   // rawBitmapDataSize
        writeBinary32bit(imageFile, RESOLUTION);   // horizontalResolution
        writeBinary32bit(imageFile, RESOLUTION);   // verticalResolution
        writeBinary32bit(imageFile, EMPTY);   // colorsTable
        writeBinary32bit(imageFile, EMPTY);   // importantColorCount
        writeBinary32bit(imageFile, hasAlphaChannel ? 0x00FF0000 : EMPTY);   // redBitmask
        writeBinary32bit(imageFile, hasAlphaChannel ? 0x0000FF00 : EMPTY);   // greenBitmask
        writeBinary32bit(imageFile, hasAlphaChannel ? 0x000000FF : EMPTY);   // blueBitmask
        writeBinary32bit(imageFile, hasAlphaChannel ? 0xFF000000 : EMPTY);   // alphaBitmask
        writeBinary32bit(imageFile, LCS_WINDOWS_COLOR_SPACE);   // colorSpaceType
        for(unsigned int i = 0; i < 9; ++i)
        {
            writeBinary32bit(imageFile, EMPTY);   // colorSpaceEndpoints
        }
        writeBinary32bit(imageFile, EMPTY);   // redGamma
        writeBinary32bit(imageFile, EMPTY);   // greenGamma
        writeBinary32bit(imageFile, EMPTY);   // blueGamma
        writeBinary32bit(imageFile, EMPTY);   // intent
        writeBinary32bit(imageFile, EMPTY);   // iccProfileData
        writeBinary32bit(imageFile, EMPTY);   // iccProfileSize
        writeBinary32bit(imageFile, EMPTY);   // reserved2
        // Bitmap Data
        for(int i = height - 1; i >= 0; --i)
        {
            writeBinary8bit(imageFile, &imageData[i * bytesWidth], bytesWidth);
            imageFile.seekp(paddingSize, std::ios::cur);
        }
        // No need to imageFile.close(), the file will close itself automatically
        // Error checking to be sure that the image data buffer isn't truncated
        return bool { imageFile.flush() };
    }
    else
    {
        return false;
    }
}