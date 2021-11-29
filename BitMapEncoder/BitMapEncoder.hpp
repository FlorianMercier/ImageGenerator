#ifndef BITMAPENCODER_HPP
#define BITMAPENCODER_HPP

#include <cstdint>   // Fixed width integer types
#include <fstream>   // libraries to creates, reads, and writes to files (o out, i in, nothing both)
#include <iostream>
#include <span>

class BitMapEncoder
{
  public:
    // Generate a Bitmap image out of the given pixel array, the given sizes and the given image name
    static bool encode(const std::span<const std::byte>& imageData,
                       const std::uint32_t width,
                       const std::uint32_t height,
                       const std::string& filePath,
                       bool hasAlphaChannel);

  private:
    static const std::uint32_t BITMAP_FILE_HEADER_SIZE = 14u;
    static const std::uint32_t BITMAP_V5_HEADER_SIZE = 124u;
    static const std::uint32_t FULL_HEADER_SIZE = BITMAP_FILE_HEADER_SIZE + BITMAP_V5_HEADER_SIZE;
    static const std::uint32_t RESOLUTION = 2835u;   // 72 dpi × 39.3701 ipm = 2834.6472
    static const std::uint32_t LCS_WINDOWS_COLOR_SPACE = 0x73524742;
    static const std::uint16_t MB = 0x4D42;
    static const std::uint32_t BI_RGB = 0u;
    static const std::uint32_t BI_BITFIELDS = 3u;
    static const std::uint32_t EMPTY = 0u;
    // Disallow construction
    BitMapEncoder() = default;
    ~BitMapEncoder() = default;
    // Every value is little endian encoded (i.e. least-significant byte first)
    static void writeBinary8bit(std::ostream& out, const std::byte* c, std::size_t n);
    static void writeBinary16bit(std::ostream& out, std::uint16_t v);
    static void writeBinary32bit(std::ostream& out, std::uint32_t v);
};

#endif