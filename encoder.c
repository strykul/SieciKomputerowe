#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*#include <sys/dev.h>
#include <sys/proxy.h>
#include <sys/kernel.h>*/
#include <string.h>

char start_symbol[2] = ":";
char stop_symbol[2] = "\n";
char adress[3] = "01";
char order[3] = "WO";
char data[500]="12345";
char frame[1024];
void frame_generator(char *start_symbol, char *adress, char *order, char *data, char *stop_symbol);

int main()
{

	frame_generator(start_symbol, adress, order,  data, stop_symbol);
	printf("Frame is: %s", &frame);
	return 0;
}


void frame_generator(char *start_symbol, char *adress, char *order, char *data, char *stop_symbol)
{

	char crc = 0;
	char crcCalculated[3];
	char temp[1024];
	int index = 0;
	int length;
	unsigned int calculated;

	strcat(temp, start_symbol);
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
	strcat(temp, crcCalculated);
	strcat(temp, stop_symbol);
	strcat(frame, temp);
}


