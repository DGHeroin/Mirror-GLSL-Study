#include <stdio.h>
#include <stdlib.h>
#include "image.h"

//����һ��4�ֽڵ���ɫֵ
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

//����2�ֽ�
static unsigned int getshort(FILE *fp)
{
  int c, c1;
  
  c = getc(fp);  
  c1 = getc(fp);
 
  return ((unsigned int) c) + (((unsigned int) c1) << 8);
 
}
    
//����ͼ��
int ImageLoad(char *filename, Image *image) 
{
    FILE *file;
    unsigned long size;                 // ͼ��Ĵ�С
    unsigned long i;                   
    unsigned short int planes;          // ����Ϊ1
    unsigned short int bpp;             // ���ص�λ��������Ϊ24
    char temp;                         

    //���ļ�
    if ((file = fopen(filename, "rb"))==NULL) {
      printf("File Not Found : %s\n",filename);
      return 0;
    }
    
    //����ǰ18���ֽڵ��ļ�ͷ
    fseek(file, 18, SEEK_CUR);

    //��ȡͼ��Ŀ���
    image->sizeX = getint (file);
    printf("Width of %s: %lu\n", filename, image->sizeX);
    image->sizeY = getint (file);
    printf("Height of %s: %lu\n", filename, image->sizeY);
    
    //����ͼ���ļ��Ĵ�С
    size = image->sizeX * image->sizeY * 3;

    //��ȡλ����
    planes = getshort(file);
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    //��ȡ���ص�λ��
    bpp = getshort(file);
    if (bpp != 24) {
	printf("Bpp from %s is not 24: %u\n", filename, bpp);
	return 0;
    }
	
    //�ƶ��ļ�ָ�뵽���ݿ�ʼ��
    fseek(file, 24, SEEK_CUR);

    //��ȡ����
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;	
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

    for (i=0;i<size;i+=3) { //�任ΪRGB���ݸ�ʽ
	temp = image->data[i];
	image->data[i] = image->data[i+2];
	image->data[i+2] = temp;
    }
    
    //���
    return 1;
}