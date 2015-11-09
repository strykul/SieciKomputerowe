#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*#include <sys/dev.h>
#include <sys/proxy.h>
#include <sys/kernel.h>*/
#include <string.h>

char inc_frame[1024] = ":01W01234521\n";
char adress[3] = "01";
char order[3] = "W0";
char data[500]="12345";
char crc[3];
char crcCalculated[3];
void frame_decoder(char *inc_frame);



int main()
{

    frame_decoder(inc_frame);
    if (strcmp(crcCalculated,crc) == 0)
        printf("Control Sum is ok \n");
    else
        printf("CRC ERROR \n");
    return 0;

}

void frame_decoder(char *inc_frame)
{
    int length = 0;
    int data_length = 0;
    int index = 0;
    unsigned char crc_recalculated = 0;
    unsigned int calculated = 0;
    char crcCalculated[3];

    length = strlen(inc_frame);
    data_length = length - 8;
    memcpy(adress,&inc_frame[1],2);
    memcpy(order,&inc_frame[3],2);
    memcpy(crc,&inc_frame[5+data_length],2);
    //*Dodaæ null terminated
    memcpy(data,&inc_frame[5],data_length);
    //*printf("%s \n", adress);
    //*printf("%s \n", order);
    printf("%s \n", crc);
    //*printf("%s \n", data);
    for (index = 0; index < length-3; index++)
		{
        crc_recalculated += inc_frame[index];
		}
    calculated = crc_recalculated;
    itoa(calculated,crcCalculated, 16);
    printf("%s \n", crcCalculated);



}


