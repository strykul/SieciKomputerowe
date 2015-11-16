#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/dev.h>
#include <sys/proxy.h>
#include <sys/kernel.h>
#include <string.h>
#include<sys/stat.h>
#include<fcntl.h>

const char Wx = 'W';
const char Rx = 'R';
char inc_frame[1024];
char adress[3];
char order[3];
char data[500];
char crc[3];
char crcCalculated[3];
int fd;
void frame_decoder(char *inc_frame);
void order_recognition(char *adress, char *order, char *data, char *crc,char *crcCalculated);



int main()
{

    fd = open("/dev/ser1", O_RDONLY);
 	while(1){
    dev_read(fd, inc_frame, 1024, 1, 0, 0, 0, 0);
    frame_decoder(inc_frame);
    order_recognition(adress, order, data, crc, crcCalculated);
    }
    close(fd);


}

void frame_decoder(char *inc_frame)
{
    int length = 0;
    int data_length = 0;
    int index = 0;
    unsigned char crc_recalculated = 0;
    unsigned int calculated = 0;

    length = strlen(inc_frame);
    data_length = length - 8;
    memcpy(adress,&inc_frame[1],2);
    memcpy(order,&inc_frame[3],2);
    memcpy(crc,&inc_frame[5+data_length],2);
    memcpy(data,&inc_frame[5],data_length);
    printf("Adress is %s \n", adress);
    printf("Order is %s \n", order);
    printf("Data are %s \n", data);
    printf("CRC comming from message is %s \n", crc);

    for (index = 0; index < length-3; index++)
		{
        crc_recalculated += inc_frame[index];
		}
    calculated = crc_recalculated;
    itoa(calculated,crcCalculated, 16);
    printf("Calculated CRC is %s \n", crcCalculated);
}
void order_recognition(char *adress, char *order, char *data, char *crc, char *crcCalculated)
{
    char temp_order[3];
    int temp_calculations;

    temp_order[3] = order[3];

    if (strcmp(crcCalculated,crc) == 0)
    {
        /*CheckSum is correct*/
        if(Wx == order[0] || Rx == order[0])
        {
            printf("Order Correct");
        }
        else
            printf("Order Incorrect");
    }

    else
        printf("Sending N0 answer \n");
}

