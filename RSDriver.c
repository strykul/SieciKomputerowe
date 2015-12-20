#define ERROR -1
#define SUCCESS 0
#define BUFFER_SIZE 150

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
char inc_frame[BUFFER_SIZE];
char frame[BUFFER_SIZE];
char adress[3];
char order[3];
char data[25];
char crc[3]="00\n";
char inc_adress[3];
char inc_order[3];
char inc_data[25];
char inc_crc[3]="00\n";
char crcCalculated[3] ="00\n";
int fd, i, fp;
void frame_decoder(char *inc_frame);
int order_recognition(char *inc_adress, char *inc_order, char *inc_data, char *inc_crc,char *crcCalculated);
void frame_generator(char *start_symbol, char *adress, char *order, char *data, char *stop_symbol);
int communication();
int data_input();


void main()
{
    while(1){
    printf("\n\n");
    data_input();
    communication();
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
    memcpy(inc_adress,&inc_frame[1],2);
    inc_adress[2]=0;
    memcpy(inc_order,&inc_frame[3],2);
    inc_order[2]=0;
    memcpy(inc_crc,&inc_frame[5+data_length],2);
    inc_crc[2]=0;
    memcpy(inc_data,&inc_frame[5],data_length);
    inc_data[data_length]=0;


    for (index = 0; index < length-3; index++){
        crc_recalculated += inc_frame[index];
		}
    calculated = crc_recalculated;
    memset(crcCalculated,0,sizeof(crcCalculated));
    itoa(calculated,crcCalculated, 16);

}
int order_recognition(char *inc_adress, char *inc_order, char *inc_data, char *inc_crc, char *crcCalculated)
{
    char temp_order[3];
    int temp_calculations, temp_length, temp_index;

    temp_order[3] = inc_order[3];

    temp_length = strlen(crcCalculated);

    for(temp_index = 0; temp_index < temp_length; temp_index++){
    crcCalculated[temp_index]=toupper(crcCalculated[temp_index]);
    }

        if(Wx == order[0] || Rx == order[0]){
            if (strcmp(crcCalculated,crc) == 0){

            printf("Adress is %s \n", inc_adress);
    		printf("Order is %s \n", inc_order);
   			printf("Data are %s \n", inc_data);
            printf("CRC comming from message is %s \n", inc_crc);
            return SUCCESS;
        }
        else{
        printf("Calculated CRC is %s \n", crcCalculated);
		printf("CRC comming from message is %s \n", inc_crc);
            printf("Order Incorrect");
            return communication();
            }


		else{
        printf("Sending N0 answer \n");
        printf("Requesting again \n");
        printf("Calculated CRC is %s \n", crcCalculated);
		printf("CRC comming from message is %s \n", inc_crc);
        return communication();
        }

}

int data_input()
{

	int temp_len;
    printf("Podaj wartosc adresu - od 01-0f \n");
    scanf("%s",&adress);
    printf("\nPodaj wartosc rozkazu dla rejestru (Wx, Rx) \n");
    scanf("%s", &order);
    temp_len = strlen(order);
    for(i=0; i < temp_len; i++){
    order[i] = toupper(order[i]);
    }
    if(order[0] == 'W'){
    printf("\nPodaj dane \n");
    scanf("%s", &data);
    printf("\n");
    return SUCCESS;
    }
    else if(order[0] == 'R'){
    printf("Dane z rejestru %s zostana odczytane\n", order);
    return SUCCESS;
    }
    else{
    printf("Blad rozkazu\n\n");
    return data_input();
    }
}

void frame_generator(char *start_symbol, char *adress, char *order, char *data, char *stop_symbol)
{

	char crc = 0;
	char crcCalculated[3];
	char temp[BUFFER_SIZE];
	int index = 0;
	int length;
	unsigned int tmp;
	unsigned int calculated;

	memset(frame, 0, sizeof(temp));

	strcpy(temp, start_symbol);
	strcat(temp, adress);
	strcat(temp, order);
	if(order[0]=='W'){
	strcat(temp, data);
	}
	length = strlen(temp);
	for (index = 0; index < length; index++){
        crc += temp[index];
		}
    calculated = crc;
    itoa(calculated,crcCalculated, 16);
    for(index = 0; index < strlen(crcCalculated); index++){
            crcCalculated[index] = toupper(crcCalculated[index]);
    }
	strcat(temp, crcCalculated);
	strcat(temp, stop_symbol);
	memset(frame, 0, sizeof(temp));
	strcat(frame, temp);
	printf("frame_gen %s \n", frame);

}


int communication()
{
frame_generator(start_symbol, adress, order,  data, stop_symbol);
fp = open("/dev/ser1", O_RDWR);
printf("FrameInComm = %s", frame);
write(fp, frame, sizeof(frame));
delay(1000);
dev_read(fp, inc_frame, sizeof(inc_frame), 1, 0, 0, 0, 0);
frame_decoder(inc_frame);
order_recognition(inc_adress, inc_order, inc_data, inc_crc, crcCalculated);
close(fp);
return 0;
}
