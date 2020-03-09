//Lab1
//COSC130
//Description: This program takes two values and an operation and converts values
//to integers, performs the operation and converts the value back to strings.
//Autumn Henderson
//January 18th, 2019

#include <iostream>
#include <string>
using namespace std;

int CharToInt(char v);
char IntToChar(int v);
int StringToInt(string val);
string IntToString(int val);

int main(int argc, char *argv[])
{
	string sresult;
	int left;
	int right;
	char op;

	if (4 != argc) {
		printf("Usage: %s <left> <op> <right>\n", argv[0]);
		return -1;
	}
	//Notice that this calls your StringToInt. So, make sure you debug
	//StringToInt() to make sure that left and right get a proper
	//value.
	
	left = StringToInt(argv[1]);
	right = StringToInt(argv[3]);
	op = argv[2][0];
	
	//Calculate based on the op. Notice that this calls IntToString,
	//so debug your IntToString() function to make sure that sresult
	//is given the proper result.This assumes your StringToInt already
	//works.
	switch (op)
	{
		case 'x':
			sresult = IntToString(left * right);
			break;
		case '/':
			sresult = IntToString(left / right);
			break;
		case '+':
			sresult = IntToString(left + right);
			break;
		case '-':
			sresult = IntToString(left - right);
			break;
		case '%':
			sresult = IntToString(left % right);
			break;
		default:
			sresult = IntToString(left);
			break;
	}

	//Remember, printf is the only output function you may use for this lab!
	//The format string is %d %c %d = %s. This means that the first argument is %d (decimal / integer),
	//%c (characer), %d (decimal/integer),
	//%s (string). Notice that because printf() is a C-style function, you
	//must pass strings as character arrays. We can convert a C++ string
	//to a character array (C-style string) by using the c_str() member function.
	printf("%d %c %d = %s\n", left, op, right, sresult.c_str());
	return 0;
}

//This function converts a character to an integer
int CharToInt(char v) {
	return v - 48;
}

//This function converts an integer to a character
char IntToChar(int v) {
	return v + 48;
}

//This function takes a string and converts it into an integer
int StringToInt(string val) {
	
	int i = val.size() - 1;
	bool valueIsNegative = false;
	int digit = 0;
	int tensPower = 1;
	int newValue = 0;

	//Handles negative
	if (val[i] == '-') {
		valueIsNegative = true;
		--i;
	}

	//Examines string at index i, multiplies by
	//appropriate 10s power, then adds values together
	for (i = i; i >= 0; --i) {

		digit = CharToInt(val[2]);
		
		for (int j = 0; j < i; ++j) {
			tensPower *= 10;
		}
		
		newValue += (digit * tensPower);
		
		tensPower = 1;
	}

	//Converts value to negative
	if (valueIsNegative) {
		newValue = 0 - newValue;
	}

	return newValue;
}

//This function takes an integer and converts it to a string
string IntToString(int val) {
	
	string finalAnswer = "";
	int digit;
	char newLetter;
	string tempAnswer = "";

	//Converts val to positive and adds negative character
	if (val < 0) {
		val = val * -1;
		finalAnswer += "-";
	}
	
	//Takes individual digit, converts to character, adds it to a
	//temporary answer
	do {
		digit = val % 10;
		newLetter = IntToChar(digit);

		tempAnswer = newLetter + tempAnswer;
		val /= 10;
	}while (val > 0);

	//temporary answer is added to final answer to handle the negative
	//if needed
	finalAnswer += tempAnswer;

	return finalAnswer;
}


