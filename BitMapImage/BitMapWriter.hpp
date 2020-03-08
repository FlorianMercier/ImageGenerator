#ifndef BITMAPWRITER_HPP
#define BITMAPWRITER_HPP

class BitMapWriter
{
  public:
    // Generate a Bitmap image out of the given pixel array, the given sizes and the given image name
    // Fill the file unsigned char array such as FileHeader, InfoHeader and Padding
    static void generateBitmapImage(unsigned char* image,
                                    const unsigned int height,
                                    const unsigned int width,
                                    const char* imageFileName,
                                    const unsigned short int bytesPerPixel);

  private:
    // Disallow construction
    BitMapWriter();
    ~BitMapWriter();

    // Set the 4 Bytes of given unsigned int into a given unsigned char array
    // Every value is stored little endian (i.e. least-significant byte first)
    static void setUnsignedIntToArray(unsigned char* bitmap, const unsigned int offset, const unsigned int val);

    // Set the 2 Bytes of given unsigned short int into a given unsigned char array
    // Every value is stored little endian (i.e. least-significant byte first)
    static void setUnsignedShortIntToArray(unsigned char* bitmap,
                                           const unsigned int offset,
                                           const unsigned short int val);

    // Fill info and data headers in the given unsigned char array
    static void fillBitmapHeader(unsigned char* header,
                                 const unsigned int height,
                                 const unsigned int width,
                                 const unsigned short int bytesPerPixel,
                                 const unsigned int paddingSize);
};

#endif
