/*
	EC-2 SIMULATOR
	Ian Thomas 12/3/2015

	CompileInstructions:
	g++ ec2_.cpp -o ec2
	./ec2 <filename>
 */
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

enum Opcode{LOAD = 0, STORE = 1, ADD = 2, SUB = 3, IN = 4, JZ = 5, JNNEG = 6, HALT = 7};

int main(int argc, char* argv[]){
	unsigned char ir, pc; //uint8_t;
	char accum; // needed to deal with negatives 
	unsigned char mem[32];
	int input = 0;
	// no argument or more than one arguments
	if (argc != 2)
	{
		cout << "Error with command line input" << endl;
		return 0;
	}
	ifstream myfile(argv[1], ios::in | ios::binary);
	/** 
	 * checking for a larger than 32 byte file
	 *
	 */
	myfile.read((char *) mem, 33);
	if(myfile.gcount() == 33){
		cout << "More than 32 bytes" << endl;
		return 0;
	}
	//checking for an empty file
	if (myfile.gcount() == 0)
	{
		cout << "empty file" << endl;
		return 0;
	}
	//file doesnt exist
	if(!myfile.is_open()){
		cout << "promblem with file" << endl;
		return 0;
	}
	//if it was good file, then read in correctly
	myfile.clear();
	myfile.read((char *) mem, 32);
	myfile.close();
	enum Opcode opcode;
	accum = ir = pc = 0;

	while(1){
		//fetch
		ir = mem[pc];
		pc++;
		pc &= 0x1f; // 5 bits so & with 0x1f
		//decode
		opcode = (enum Opcode)(ir >> 5);
		//execute
		switch(opcode){
			case LOAD:
				accum = mem[(ir & 0x1f)];
				break;
			case STORE:
				mem[(ir & 0x1f)] = accum;
				break;
			case ADD:
				accum = accum + mem[(ir & 0x1f)];
				break;
			case SUB:
				accum = accum - mem[(ir & 0x1f)];
				break;
			case IN:
				while(1){
				cout << "Enter a number: ";
				cin >> input;
					if ((input > 127) || (input < -128))
					{
						cout << "Invalid Input must be between -128 to 127" << endl;
						continue;
					}
					else{
						accum = input;
						break;
					}
				}
				break;
			case JZ:
				if(accum == 0) pc = ir & 0x1f;
				break;
			case JNNEG:
				if(accum >= 0) pc = ir & 0x1f;
				break;
			case HALT:
				cout << accum << endl;
				return 0;
		}
	}
}
