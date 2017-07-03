#ifndef image_h
#define image_h

//Í¼Ïñ½á¹¹
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
//ÔØÈëÍ¼Ïñ
int ImageLoad(char *filename, Image *image);
#endif