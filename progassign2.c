

#include <stdio.h>
#include <stdlib.h>

void fileType (FILE* file_name);
void printContents (unsigned char *buffer);
unsigned short loadAddress (FILE* file_name, int c1);
unsigned short byteCount (FILE* file_name);
unsigned char* buff;

int main (int argc, char** argv)
{
	char* filename = argv[1];
	buff =  calloc(135000, sizeof(char));
	unsigned short address;
	unsigned short count;
	/*ensure there is enough memory*/
	if (buff == NULL)
	{
		printf("Out of memory.\n");
		return 1;
	}

	FILE* fp;	/*declare file pointer*/
	fp = fopen(filename, "rb");
	/*if file is NULL, does not exist*/
	if (fp == NULL)
	{
		printf("File does not exist.\n");
		return 1;
	}


	/*check if valid object file */
	int c1;

	fileType (fp);
	while (1)
	{
		int i;
		/*if EOF reached after reading a block, then file reading is complete*/
		if ((c1 = fgetc(fp)) == EOF) break;
		address = loadAddress(fp, c1);
		count = byteCount (fp);
		/*if address values exceed range*/
		if (address + count > 0xFFFF)
		{
			printf("The Y86 Object file is corrupted.\n");
			return 1;
		}
		/*loop through contents*/
		for (i = 0; i < count; i++)
		{
			c1 = fgetc(fp);
			/*if EOF reached before byteCount full, invalid file*/
			if (c1 == EOF)
			{
				printf("The Y86 Object file is corrupted.\n");
				return 1;
			}
			else
				buff[address++] = (unsigned char) c1;
		}
	}
	printContents(buff);
	fclose(fp);
	free (buff);
	return 0;
}

void fileType (FILE * file_name)
{
	int c1 = fgetc(file_name);
	int c2 = fgetc(file_name);
	/*check if file is Y86 object*/
	if (!(c1 == 0x79 && c2 == 0x62))
	{
		printf ("The file is not a Y86 object file.\n");
		exit (EXIT_FAILURE);
	}
}

unsigned short loadAddress(FILE* file_name, int c1)
{
	unsigned short s;
	unsigned char c2 = fgetc(file_name);
	/*if there is a partial load address without any data*/
	if ((int)c2 == EOF)
	{
		printf("The Y86 Object file is corrupted.\n");
		exit (EXIT_FAILURE);
	}
	/*return the load address in positive format*/
	s = ((((unsigned char)c1) << 8) | c2);
	return s;
}

unsigned short byteCount (FILE* file_name)
{
	short s;
	unsigned char c1 = fgetc(file_name);
	if ((int)c1 == EOF)
	{
		printf("The Y86 Object file is corrupted.\n");
		exit (EXIT_FAILURE);
	}
	unsigned char c2 = fgetc(file_name);
	/*if there is a partial bytecount without any data*/
	if ((int)c2 == EOF)
	{
		printf("The Y86 Object file is corrupted.\n");
		exit (EXIT_FAILURE);
	}
	/*return the byteCount*/
	s = ((c1 << 8) | c2);
	return (unsigned) s;
}

void printContents (unsigned char *buffer)
{
	int i;
	for (i = 0; i < 135000; i++)
	{
		if (buffer[i] != 0)
			printf ("%04X: %02X\n",i , buffer[i]);
	}

}
