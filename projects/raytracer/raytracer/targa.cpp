#include"targa.h"

#include <cstdio>
#include "types.h"


bool writeTGAFile(const std::string& p_filename, int p_width, int p_height, unsigned char* p_imageData)
{
	// Uncompressed TGA header (tgaHeader) and image header.
	unsigned char tgaHeader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char header[6];

	// Open the file and make sure it actually opened.
	FILE* file = fopen(p_filename.c_str(), "wb");
	if (file == nullptr)
	{
		return false;
	}
	
	// Default color mode and the bit components (RGB).
	const int channelCount = 3;
	const u8 bitsPerPixel = 8;
	
	// Tga header
	header[0] = static_cast<u8>(p_width  % 256);
	header[1] = static_cast<u8>(p_width  / 256);
	header[2] = static_cast<u8>(p_height % 256);
	header[3] = static_cast<u8>(p_height / 256);
	header[4] = bitsPerPixel * channelCount;
	header[5] = 0;
	
	// Write out TGA file headers.
	fwrite(tgaHeader, sizeof(tgaHeader), 1, file);
	fwrite(header, sizeof(header), 1, file);
	
	// Color swap from RGB to BGR => make local copy
	const int imageSize = p_width * p_height * channelCount;
	u8* imageData = new u8[imageSize];
	memcpy(imageData, p_imageData, imageSize);
	
	for (int i = 0; i < imageSize; i += channelCount)
	{
		std::swap(imageData[i], imageData[i+2]);
	}

	// Finally write the image.
	fwrite(imageData, imageSize, 1, file);

	// close the file.
	fclose(file);
	delete[] imageData;

	return true;
}