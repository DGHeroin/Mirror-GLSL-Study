#ifndef image_h
#define image_h

//ͼ��ṹ
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
//����ͼ��
int ImageLoad(char *filename, Image *image);
#endif