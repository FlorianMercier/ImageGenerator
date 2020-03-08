#include "BitMapWriter.hpp"
#include <stdio.h>

BitMapWriter::BitMapWriter()
{
}

BitMapWriter::~BitMapWriter()
{
}

void BitMapWriter::setUnsignedIntToArray(unsigned char* bitmap, const unsigned int offset, const unsigned int val)
{
    bitmap[offset] = (unsigned char) (val);
    bitmap[offset + 1] = (unsigned char) (val >> 8);
    bitmap[offset + 2] = (unsigned char) (val >> 16);
    bitmap[offset + 3] = (unsigned char) (val >> 24);
}

void BitMapWriter::setUnsignedShortIntToArray(unsigned char* bitmap,
                                              const unsigned int offset,
                                              const unsigned short int val)
{
    bitmap[offset] = (unsigned char) (val);
    bitmap[offset + 1] = (unsigned char) (val >> 8);
}

void BitMapWriter::fillBitmapHeader(unsigned char* header,
                                    const unsigned int height,
                                    const unsigned int width,
                                    const unsigned short int bytesPerPixel,
                                    const unsigned int paddingSize)
{
    const unsigned int FULL_HEADER_SIZE = 54u;
    const unsigned int INFO_HEADER_SIZE = 40u;
    // Total size of the file
    const unsigned int fileSize = FULL_HEADER_SIZE + (bytesPerPixel * width + paddingSize) * height;
    // Convert bytes to bits
    const unsigned short int bitsPerPixel = (unsigned short int) (bytesPerPixel * 8u);

    // FILL FILE HEADER
    header[0u] = (unsigned char) ('B');   // Signature
    header[1u] = (unsigned char) ('M');
    setUnsignedIntToArray(header, 2u, fileSize);   // Image file size in bytes
    setUnsignedIntToArray(header, 6u, 0u);   // Reserved (to the app creating the file)
    setUnsignedIntToArray(header, 10u, FULL_HEADER_SIZE);   // Offset of pixel array (size of the two headers 14+40)

    // FILL FILE HEADER
    setUnsignedIntToArray(header, 14u, INFO_HEADER_SIZE);   // Info Header size (40)
    setUnsignedIntToArray(header, 18u, width);   // Image width
    setUnsignedIntToArray(header, 22u, height);   // Image height
    setUnsignedShortIntToArray(header, 26u, 1u);   // Number of color planes
    setUnsignedShortIntToArray(header, 28u, bitsPerPixel);   // Bits per pixel
    setUnsignedIntToArray(header, 30u, 0u);   // Compression
    setUnsignedIntToArray(header, 34u, 0u);   // Raw bitmap data size
    setUnsignedIntToArray(header, 38u, 0u);   // Horizontal resolution
    setUnsignedIntToArray(header, 42u, 0u);   // Vertical resolution
    setUnsignedIntToArray(header, 46u, 0u);   // Clors in color table
    setUnsignedIntToArray(header, 50u, 0u);   // Important color count
}

void BitMapWriter::generateBitmapImage(unsigned char* image,
                                       const unsigned int height,
                                       const unsigned int width,
                                       const char* imageFileName,
                                       const unsigned short int bytesPerPixel)
{
    const unsigned int FULL_HEADER_SIZE = 54u;
    // Size of padding so that the total width is a multiple of 4 bytes
    const unsigned int paddingSize = (4u - (width * bytesPerPixel) % 4u) % 4u;
    // Create an array of 0 to fill padding
    unsigned char* padding = new unsigned char[bytesPerPixel]();

    // Open file
    FILE* imageFile = fopen(imageFileName, "wb");

    // Fill and write headers
    unsigned char header[FULL_HEADER_SIZE];
    fillBitmapHeader(header, height, width, bytesPerPixel, paddingSize);
    fwrite(header, 1, FULL_HEADER_SIZE, imageFile);

    // Write image with color pixels and padding, starting with the botom row
    for(unsigned int i = 0u; i < height; i++)
    {
        fwrite(image + (width * (height - i - 1u) * bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1u, paddingSize, imageFile);
    }

    // Delete the padding array
    delete padding;
    // Close file
    fclose(imageFile);
}
