#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string>

using namespace std;

int main (int argc, char *argv[]) {

	unsigned long address;
	unsigned long page_offset;
	unsigned long vpn;
	unsigned long byte_offset;

	string level = "Level";
	string index = "Index";
	string byte = "Byte Index";

	if (argc != 2) {
		printf("Usage: ./lab 0x<address>\n");
		return -1;
	}

	sscanf(argv[1], "%lx", &address);
	printf("This is address: %lx\n", address);

	page_offset = address & 0xFFF;

	printf("  %s   %s  %s\n", level.c_str(), index.c_str(), byte.c_str());

	for (int i = 2; i >= 0; --i) {
		vpn = (address >> (12 + (i * 9))) & 0x1FF;
		byte_offset = vpn * 8;
		printf("VPN[00%d] = %lu    (%4lu)\n", i, vpn, byte_offset);
	}

	printf("PO       = %lx\n", page_offset);

	return 0;
}
