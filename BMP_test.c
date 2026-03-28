#define _width 20
#define _height 20
#define _img_size _width*_height
#define _file_size 54+4*_img_size
#include <stdio.h>

int main()
{
    int f=_file_size;
    printf("%d",f);
    return 0;
}