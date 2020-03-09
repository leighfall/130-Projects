//Lab 4
//COSC 130
//Autumn Henderson
//February 8th, 2019

#include <cstdio>

using namespace std;

struct Real {
	//sign bit
	int sign;
	//Unbiased exponent
	long exponent;
	//Fraction including implied 1 at the bit index 23
	unsigned long fraction;
};

Real Decode(int float_value);
int Encode(Real real_value);
Real Normalize(Real value);
Real Multiply(Real left, Real right);
Real Add(Real left, Real right);
int sign(int value);
int twos(int value);
long add(long left, long right);
long sub(long left, long right);
long mul(long left, long right);

int main(int argc, char *argv[]) {
	int left, right;
	char op;
	int value;
	Real rLeft, rRight, result;
	if (argc < 4) {
		printf("Usage: %s <left> <op> <right>\n", argv[0]);
		return -1;
	}

	sscanf(argv[1], "%f", (float *)&left);
	sscanf(argv[2], "%c", &op);
	sscanf(argv[3], "%f", (float *)&right);

	rLeft = Decode(left);
	rRight = Decode(right);

	if(op == 'x') {
		result = Multiply(rLeft, rRight);
	}
	else if(op == '+') {
	result = Add(rLeft, rRight);
	}
	else {
		printf("Unknown operator '%c'\n", op);
		return -2;
	}

	value = Encode(result);
	printf("%.3f %c %.3f = %.3f (0x%08x)\n",
			*((float*)&left),
			op,
			*((float*)&right),
			*((float*)&value),
			value
		  );
	return 0;
}

//Function 'Decode' takes an integer as a parameter and returns a Real object that is
//the parameter in IEEE-754 format
Real Decode(int float_value) {
	
	Real decodes;
	decodes.sign = (float_value >> 31) & 1;
	decodes.exponent = ((float_value >> 23) & 0xff) - 127;
	decodes.fraction = float_value & 0x7fffff;

	return decodes;
}

//Function 'Encode' takes a Real object as a parameter and returns an integer that is the
//parameter taken out of IEEE-754 format
int Encode(Real real_value) {

	int encode = real_value.sign << 31;
	encode |= (((real_value.exponent + 127) & 0xff) << 23);
	encode |= (real_value.fraction & 0x7fffff);

	return encode;
}

//Function 'Normalize' takes a Real object as a parameter and returns a Real object that
//is in format 1.<Real.fraction>
Real Normalize(Real value) {
	//Compare between 'value' and it OR by 1 << 23?
	//If not there, shift and then compare again?
	
	return value;
}

//Function 'Multiply' takes two Real objects as parameters and returns a Real object that is the
//product of the parameters
Real Multiply(Real left, Real right) {

	//Get these values in IEEE format
	Real result;

	//Variables: Fraction with implicit '1'
	Real normLeft;
	Real normRight;
	normLeft = Normalize(left);
	normRight = Normalize(right);

	//Variables: Exponents UNBIASED
	long lExp = normLeft.exponent;
	long rExp = normRight.exponent;

	//Add exponents
	result.exponent = add(lExp, rExp);

	//multiply using binary arithmetic (use multiply function)
	result.fraction = mul(normLeft.fraction, normRight.fraction);

	//Determine the sign (use .sign and similar to bool expression in binary multiply)
	if (normLeft.sign != normRight.sign) result.sign = 1;

	//Renormalize and return
	return Normalize(result);
}

//Function 'Add' takes two Real objects as parameters and returns a Real object that is the
//sum of the parameters.
Real Add(Real left, Real right) {

	//Get these values in IEEE format
	Real result;

	//Variables: Fraction with implicit '1'
	Real normLeft;
	Real normRight;
	normLeft = Normalize(left);
	normRight = Normalize(right);

	//Variables: Shift Exponent
	long shiftExp;

	//Match exponents
	if (normLeft.exponent != normRight.exponent) {
		//Subtract exponents to figure out shift
		shiftExp = sub(normLeft.exponent, normRight.exponent);

		//Shift right version
		if (shiftExp < 0) normRight.fraction = (normRight.fraction << twos(shiftExp));
		else normRight.fraction = normRight.fraction >> shiftExp;
	}

	result.fraction = add(normLeft.fraction, normRight.fraction);
	
	result.exponent = normLeft.exponent;

	return Normalize(result);
}

//Function 'sign' takes an integer value and returns an integer value
//indicating whether the value is negative or positive.
int sign(int value) {return (value >> 31) & 1;}

//Function 'twos' takes an integer vallue and returns an integer value
//that is the former's two's compliment.
int twos(int value) {return (add(~value, 1));}

//Function 'add' takes two long values and returns a long value
//that is the sum of the former two values.
long add(long left, long right) {
	int carry = 0;
	long sum = 0;

	for(int i = 0; i < 32; ++i) {
		int l = (left >> i) & 1;
		int r = (right >> i) & 1;
		int s = l ^ r ^ carry;
		carry = (l & r) | (r & carry) | (l & carry);
		sum = sum | (s << i);
	}

	return sum;
}

//Function 'sub' takes two long values and returns a long value
//that is the difference of the parameters.
long sub(long left, long right) {
	long newRight = twos(right);
	return add(left, newRight);
}

//Function 'mul' takes two long values and returns a long value
//that is the difference of the former two values.
long mul(long left, long right) {

	long product = 0;
	bool oneWasNeg = false;
	bool otherWasNeg = false;
	long newLeft = left;
	long newRight = right;

	//Checks sign to make sure multiplying by positivec numbers and to check
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
	while (newRight >0) {
		product += newLeft;
		--newRight;
	}

	//Makes the product negative if one value was negative
	if (oneWasNeg != otherWasNeg) product = twos(product);

	return product;
}
