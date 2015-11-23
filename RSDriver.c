#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/dev.h>
#include <sys/proxy.h>
#include <sys/kernel.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

char start_symbol[2] = ":";
char stop_symbol[2] = "\n";
const char Wx = 'W';
const char Rx = 'R';
char inc_frame[1024];
char adress[3];
char order[3];
char data[500];
char crc[3];
char crcCalculated[3];
int fd, i, fp;
void frame_decoder(char *inc_frame);
void order_recognition(char *adress, char *order, char *data, char *crc,char *crcCalculated);
void frame_generator(char *start_symbol, char *adress, char *order, char *data, char *stop_symbol);
void data_input();


int main()
{
    while(1){
    data_input();
	frame_generator(start_symbol, adress, order,  data, stop_symbol);
    fp = open("/dev/ser1", O_WRONLY);
	printf(" Generated frame %s \n", frame);
	write(fp, frame, 1024 );
	close(fp);
    fd = open("/dev/ser1", O_RDONLY);
    dev_read(fd, inc_frame, 1024, 1, 0, 0, 0, 0);
    frame_decoder(inc_frame);
    order_recognition(adress, order, data, crc, crcCalculated);
    close(fd);
    }

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
}
void order_recognition(char *adress, char *order, char *data, char *crc, char *crcCalculated)
{
    char temp_order[3];
    int temp_calculations, temp_length, temp_index;

    temp_order[3] = order[3];

    temp_length = strlen(crcCalculated);

    for(temp_index = 0; temp_index < temp_length; temp_index++){
    crcCalculated[temp_index]=toupper(crcCalculated[temp_index]);

    }
	printf("Calculated CRC is %s \n", crcCalculated);
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

void data_input()
{
    printf("Podaj wartosc adresu - od 01-0f \n");
    scanf("%s",&adress);
    printf("\nPodaj wartosc rozkazu dla rejestru (Wx, Rx) \n");
    scanf("%s", &order);
    temp_len = strlen(order);
    for(i=0; i < temp_len; i++)
    {
    order[i] = toupper(order[i]);
    }
    if(order[0] == 'W'){
    printf("\nPodaj dane \n");
    scanf("%s", &data);
    printf("\n");
    }
    else
    {
    printf("Dane z rejestru %d zostana odczytane\n", order[1]);
    }
}

void frame_generator(char *start_symbol, char *adress, char *order, char *data, char *stop_symbol)
{

	char crc = 0;
	char crcCalculated[3];
	char temp[1024];
	int index = 0;
	int length;
	unsigned int tmp;
	unsigned int calculated;

	strcpy(temp, start_symbol);
	strcat(temp, adress);
	strcat(temp, order);
	strcat(temp, data);
	length = strlen(temp);
	for (index = 0; index < length; index++)
		{
        crc += temp[index];
		}
    calculated = crc;
    itoa(calculated,crcCalculated, 16);
    for(index = 0; index < strlen(crcCalculated); index++)
    {
            crcCalculated[index] = toupper(crcCalculated[index]);
    }
	strcat(temp, crcCalculated);
	strcat(temp, stop_symbol);
	strcat(frame, temp);
	length = strlen(frame);

}


