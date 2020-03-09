//Autumn Henderson
//Lab 3
//February 1st, 2019
//This program asks the user for two values and an operation and returns the
//result of the operation.

#include<cstdio>

using namespace std;

int sign(int value);
int twos(int value);
int add(int left, int right);
int sub(int left, int right);
int mul(int left, int right);

int main(int argc, char *argv[]) {

	if (argc < 4) {
		printf("Usage: %s <num> <op> <num>\n", argv[0]);
		return -1;
	}

	int left;
	int right;
	char op;
	int result;

	sscanf(argv[1], "%d", &left);
	sscanf(argv[2], "%c", &op);
	sscanf(argv[3], "%d", &right);

	switch(op) {
		case '+':
			 result = add(left, right);
			 break;
		case '-':
			 result = sub(left, right);
			 break;
		case 'x':
			 result = mul(left, right);
			 break;
		default:
			 printf("Unknown operation '%c'\n", op);
			 return -2;
	}

	printf("%d %c %d = %d\n", left, op, right, result);

	return 0;
}

//Function 'sign' takes an integer value and returns an integer value
//indicating whether the value is negative or positive.
int sign(int value) {
	return (value >> 31) & 1;
}

//Function 'twos' takes an integer value and returns an integer value
//that is the former's two's compliment.
int twos(int value) {
	return (add(~value, 1));
}

//Function 'add' takes two integer values and returns an integer value
//that is the sum of the former two values.
int add(int left, int right) {
	int carry = 0;
	int sum = 0;

	for (int i = 0; i < 32; ++i) {
		int l = (left >> i) & 1;
		int r = (right >> i) & 1;
		int s = l ^ r ^ carry;
		carry = (l & r) | (r & carry) | (l & carry);
		sum = sum | (s << i);
	}

	return sum;
}

//Function 'sub' takes two integer values and returns an integer value
//that is the difference of the former two values.
int sub(int left, int right) {
	int newRight = twos(right);
	return add(left, newRight);
}

//Function 'mul' takes two integer values and returns an integer value
//that is the difference of the former two values.
int mul(int left, int right) {
	
	int product = 0;
	bool oneWasNeg = false;
	bool otherWasNeg = false;
	int newLeft = left;
	int newRight = right;
	
	//Checks signs to make sure multiplying by positive numbers and to check
	//at the end if there was a negative value
	if (sign(left) == 1) {
		newLeft = twos(left);
		oneWasNeg = true;
	}

	if (sign(right) == 1) {
		newRight = twos(right);
		otherWasNeg = true;
	}

	//Multiplying
	while (newRight > 0) {
		product += newLeft;
		--newRight;
	}

	//Makes the product negative if one value was negative
	if (oneWasNeg != otherWasNeg) product = twos(product);

	return product;
}
