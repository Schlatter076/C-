#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char sendcrc[] = {0x00, 0x00, 0x4E, 0x20, 0x00, 0x00, 0xD6, 0xD8, 0x00, 0x00, 0x75, 0x30};
char *str = "2.8908080843E-02 ";


unsigned int Get_CRC(unsigned char *se, unsigned char num)
{
    unsigned i, j;
    unsigned int wCrc = 0xFFFF;
    for(i = 0; i < num; i++)
    {
        wCrc ^= (unsigned int)(se[i]);
        for(j = 0; j < 8; j++)
        {
            if(wCrc & 1)
            {
                wCrc >>= 1;
                wCrc ^= 0xA001;
            }
            else
                wCrc >>= 1;
        }
    }
    return wCrc;
}
int main()
{
    unsigned int tem = Get_CRC(sendcrc, 12);
    printf("%x", tem);
    return 0; 
    /*
    double d = 0;
    char *s1;
    char *s2;
    s1 = strchr(str, '.');
    s2 = strchr(str, ' ');
    printf("s2 - s1 = %d\n", (s2 - s1));
    //char* str = "2.96E-03";
    d = atof(str);
    printf("atof(str)=%1f", d);//*/

}