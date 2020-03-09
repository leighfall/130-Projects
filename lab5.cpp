//Autumn Henderson
//Lab 5
//CS 130
//February 15th, 2019
//This program is a simple MUD that supports a single user and several rooms.

#include<string>
#include<fstream>
#include<stdlib.h>
#include<sstream>

using namespace std;

struct Room{
	string room_num;
	string description;
	//Will represent n, e, s, w. Will be 1 if direction exists
	int room_direction[4] = {0, 0, 0, 0};
	//Will store corresponding room index
	int room_index[4] = {0, 0, 0, 0};
};

int main(int argc, char *argv[]) {

	string file_name;
	ifstream fin1;
	ifstream fin2;
	string line;
	char direction;
	int index;
	stringstream sout;
	int i = 0;

	int num_rooms = 0;
	int num_tildes = 0;
	Room *ptr = NULL;

	//Game Variables
	char command;
	int curr_game_index = 0;
	char discard;

	//OPENS FILE, PROCESSES, AND CLOSES FILE

	//Checks argc
	if(argc < 2) {
		printf("Usage: <./lab5> <file_name>\n");
		return -1;
	}

	//Opens file and checks to ensure file was able to be opened
	file_name = argv[1];
	fin1.open(file_name);

	if (!fin1) {
		printf("Unable to open file\n");
		return -2;
	}
	
	//While loop counts rooms in file to allocate memory
	while(getline(fin1, line)) {
		//Checks for tildes. If there is one, updates num_tildes.
		if(line == "~") {
			++num_tildes;
			//Checks to see if three tildes on lines on their own have been found.
			//If so, updates num_rooms.
			if((num_tildes + 3) % 3 == 0) ++num_rooms;
		}
		else continue;
	}

	//Uses while loop information to allocate memory
	ptr = new Room[num_rooms];

	//Checks pointer validity
	if(!ptr) {
		printf("Failed to allocate memory.\n");
		return -3;
	}
	fin1.close();

	//2nd While loop gathers information into stringstream and puts it in memory
	sout.clear();
	num_tildes = 0;
	line = "";
	fin2.open(file_name);
	if (!fin2) {
		printf("Unable to open file.\n");
		return -4;
	}

	while(getline(fin2, line)) {
		//Checks for tildes. If there is one, updates num_tildes
		if(line == "~") {
			++num_tildes;
			//Places information into memory
			if(num_tildes == 1) {
				ptr[i].room_num = sout.str();
				sout.clear();
				sout.str("");
			}
			else if(num_tildes == 2) {
				ptr[i].description = sout.str();
				sout.clear();
				sout.str("");
			}
			else if(num_tildes == 3) {
				//While loop divides up room direction and room index
				while(sout >> direction) {
					sout >> index;
					switch (direction) {
						case 'n': ptr[i].room_direction[0] = 1;
								  ptr[i].room_index[0] = index;
								  break;
						case 'e': ptr[i].room_direction[1] = 1;
								  ptr[i].room_index[1] = index;
								  break;
						case 's': ptr[i].room_direction[2] = 1;
								  ptr[i].room_index[2] = index;
								  break;
						case 'w': ptr[i].room_direction[3] = 1;
								  ptr[i].room_index[3] = index;
								  break;
					}
				}

				sout.clear();
				sout.str("");
				++i;
				num_tildes = 0;
			}	
		}
		
		else sout << line << "\n";
	}

	//Closes file
	fin2.close();

	//GAME BEGINS
	do{
		printf("> ");
		scanf("%c%c", &command, &discard);

		//Quits the game
		if (command == 'q') break;

		if (command == 'l') {
			string exits = "";

			printf("%s", ptr[curr_game_index].room_num.c_str());
			printf("%s\n", ptr[curr_game_index].description.c_str());
			printf("Exits: ");
			//For loop prints exits
			for(int j = 0; j < 4; ++j) {
				if (ptr[curr_game_index].room_direction[j] == 1) {
					switch (j) {
						case 0: printf("n ");
							break;
						case 1: printf("e ");
							break;
						case 2: printf("s ");
							break;
						case 3: printf("w ");
							break;
					}
				}
			}
			printf("\n");
		}
		//Checks commands to determine direction to move
		else if (command == 'n') {
			if(ptr[curr_game_index].room_direction[0] == 0) {
				printf("You can't go NORTH!\n");
				continue;
			}
			else {
				printf("You moved NORTH.\n");
				curr_game_index = ptr[curr_game_index].room_index[0];
			}
		}
		else if (command == 'e') {
			if(ptr[curr_game_index].room_direction[1] == 0) {
				printf("You can't go EAST!\n");
				continue;
			}
			else {
				printf("You moved EAST.\n");
				curr_game_index = ptr[curr_game_index].room_index[1];
			}
		}
		else if (command == 's') {
			if(ptr[curr_game_index].room_direction[2] == 0) {
				printf("You can't go SOUTH!\n");
				continue;
			}
			else {
				printf("You moved SOUTH.\n");
				curr_game_index = ptr[curr_game_index].room_index[2];
			}
		}
		else if (command == 'w') {
			if(ptr[curr_game_index].room_direction[3] == 0) {
				printf("You can't go WEST!\n");
				continue;
			}
			else {
				printf("You moved West.\n");
				curr_game_index = ptr[curr_game_index].room_index[3];
			}
		}
		else {
			printf("Command does not exist.\n");
			continue;
		}
	}while(true);


	delete[] ptr;

	return 0;
}


