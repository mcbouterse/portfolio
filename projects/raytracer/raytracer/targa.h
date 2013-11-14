#ifndef MB_INC_TGA_H
#define MB_INC_TGA_H

#include <string>


bool writeTGAFile(const std::string& p_filename, int p_width, int p_height, unsigned char* p_imageData);


#endif // MB_INC_TGA_H
