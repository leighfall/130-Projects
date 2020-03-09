//Autumn Henderson
//CS 130
//February 21st, 2019
//Lab 6
//This lab takes an encrypted file, a dkey, and an nkey, and returns a decrypted message.

#include<stdio.h>
#include<cstdlib>
#include<stdlib.h>
#include<string>

using namespace std;

struct Block {
	char data;
	int index;
};

int main(int argc, char *argv[]) {

	string file_name;
	FILE *ptr;
	int file_size;
	int max_chunks;

	char dkey;
	int nkey;

	Block *blockptr;

	int data_index;
	char data;
	int discard;
	char new_data;
	int new_data_index;
	
	//Checks argc
	if(argc < 4) {
		printf("Usage: <program_name> <file_name> <dkey> <nkey>\n");
		return -1;
	}
	
	file_name = argv[1];
	sscanf(argv[2], "%c", &dkey);
	sscanf(argv[3], "%d", &nkey);

	//Opens file and checks to make sure file opened
	ptr = fopen(file_name.c_str(), "rb");
	if (ptr == NULL) {
		printf("Failed to open file %s\n", file_name.c_str());
		return -2;
	}

	//Tells file size and maximum chunks of data
	fseek(ptr, 0, SEEK_END);
	file_size = ftell(ptr);
	max_chunks = file_size / 8;

	//Creates dynamic memory for amount of blocks
	blockptr = new Block[max_chunks];
	if (!blockptr) {
		printf("Failed to allocate memory.\n");
		return -3;
	}

	//Pointer moves back to beginning of file
	fseek(ptr, 0, SEEK_SET);

	//Reads values into memory
	for(int i = 0; i < max_chunks; ++i) {

		//Gets data from first byte
		fread(&data, 1, 1, ptr);
		new_data = (data ^ dkey);
		
		//Discards next three bytes
		fread(&discard, 1, 3, ptr);
		
		//Reads the index from last 4 bytes
		fread(&data_index, 1, 4, ptr);	
		new_data_index = (data_index ^ nkey);
		
		//Checks indices and stores data if within range
		if (new_data_index > max_chunks) {
			printf("Error decoding. Decoded index is %d, but max chunks is %d", new_data_index, max_chunks);
		}
		else {
			blockptr[new_data_index].data = new_data;
			blockptr[new_data_index].index = new_data_index;
		}
	}

	//Prints data
	for (int i = 0; i < max_chunks; ++i) {
		printf("%c", blockptr[i].data);
	}
	
	fclose(ptr);
	delete [] blockptr;
	return 0;
}
