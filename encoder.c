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
char adress[3];
char order[3];
char data[500];
char frame[1024];
int fp, i;
int temp_len;
void frame_generator(char *start_symbol, char *adress, char *order, char *data, char *stop_symbol);

int main()
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
    fp = open("/dev/ser1", O_WRONLY);
	frame_generator(start_symbol, adress, order,  data, stop_symbol);
	printf(" Generated frame %s \n", frame);
	write(fp, frame, 1024 );
	close(fp);
	return 0;
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


