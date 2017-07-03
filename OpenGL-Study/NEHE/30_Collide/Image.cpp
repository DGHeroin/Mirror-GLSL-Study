#include <stdio.h>
#include <stdlib.h>
#include "image.h"

//返回一个4字节的颜色值
static unsigned int getint(FILE *fp)
{
  int c, c1, c2, c3;

  c = getc(fp);  
  c1 = getc(fp);  
  c2 = getc(fp);  
  c3 = getc(fp);
  
  return ((unsigned int) c) +   
    (((unsigned int) c1) << 8) + 
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

//返回2字节
static unsigned int getshort(FILE *fp)
{
  int c, c1;
  
  c = getc(fp);  
  c1 = getc(fp);
 
  return ((unsigned int) c) + (((unsigned int) c1) << 8);
 
}
    
//载入图像
int ImageLoad(char *filename, Image *image) 
{
    FILE *file;
    unsigned long size;                 // 图像的大小
    unsigned long i;                   
    unsigned short int planes;          // 必须为1
    unsigned short int bpp;             // 像素的位数，必须为24
    char temp;                         

    //打开文件
    if ((file = fopen(filename, "rb"))==NULL) {
      printf("File Not Found : %s\n",filename);
      return 0;
    }
    
    //跳过前18个字节的文件头
    fseek(file, 18, SEEK_CUR);

    //读取图像的宽，高
    image->sizeX = getint (file);
    printf("Width of %s: %lu\n", filename, image->sizeX);
    image->sizeY = getint (file);
    printf("Height of %s: %lu\n", filename, image->sizeY);
    
    //计算图像文件的大小
    size = image->sizeX * image->sizeY * 3;

    //读取位面数
    planes = getshort(file);
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    //读取像素的位数
    bpp = getshort(file);
    if (bpp != 24) {
	printf("Bpp from %s is not 24: %u\n", filename, bpp);
	return 0;
    }
	
    //移动文件指针到数据开始处
    fseek(file, 24, SEEK_CUR);

    //读取数据
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;	
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

    for (i=0;i<size;i+=3) { //变换为RGB数据格式
	temp = image->data[i];
	image->data[i] = image->data[i+2];
	image->data[i+2] = temp;
    }
    
    //完成
    return 1;
}