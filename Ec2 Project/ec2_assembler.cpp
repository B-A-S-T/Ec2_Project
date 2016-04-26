/*
	EC-2 ASSEMBLER
	Ian Thomas 12/4/2015

	CompileInstructions:
	g++ ec2_assembler.cpp -o ec2_assembler
	./ec2_assembler <inputFile> <outputFile>
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

unsigned char convert(string address);
int findInstruction(string temp);

int main(int argc, char* argv[]){
	//check to see if bad command input
	if (argc != 3)
	{
		cout << "Error with command line input." << endl;
		return 0;
	}
	//streams
	ofstream bin(argv[2]);
	ifstream assem(argv[1]);
	if (!assem.is_open())
	{
		cout << "Couldnt open input file." << endl;
		return 0;
	}
	if(!bin.is_open()){
		cout << "Couldnt open output file." << endl;
		return 0;
	}

	int swch = 0; //which instruction
	int count = 0;	// number of bytes
	/*
		The loop works like so:
		Get the line,
		Put in a stringstream so we can ignore whitespaces.
		Get the first string, it should one of the 8 instructions,
		if not then go to next line
		If it is then find the address after "A," for 1-4, after JZ/JNNEG
		Calculate the value of this address,
		then add the value of with the value of the instruction bits.
		Write it to the bin file.
	 */
	while(!assem.eof()){
		if(count > 32){
			cout << "More than 32 bytes long" << endl;
			return 0;
		}
		// couple of temps
		string temp = "";
		string inst = "";
		string address = "";

		unsigned char toWrite = 0;	 // written to bin file
		getline(assem, temp); // get line.
		stringstream buf(temp); 
		buf >> inst; // get the first instruction
		swch = findInstruction(inst); //find the correct instruction
		switch(swch){
			//LOAD
			case 1:
				{
				// check case A, no in line
				if (temp.find("A,") == std::string::npos)
				{
					cout << "invalid line." << endl;
					break;
				}
				stringstream instbuf(temp);
				//get rid of first string which is the instruction
				instbuf >> address;
				//take in the next string should be "A,"
				instbuf >> address;
				//check case B, A,
				if(address != "A,"){
					cout << "Invalid line." << endl;
					break;
				}
				//now take in the address
				instbuf >> address;
				// check case 10000000110
				if(address.length() != 5){
					cout << "Invalid line." << endl;
					break;
				}
				toWrite = convert(address); // converts i.e 10101 to its integer
				bin << toWrite;
				count++;
				}
				break;
			//STORE
			case 2:
				{
				if (temp.find("A,") == std::string::npos)
				{
					cout << "invalid line." << endl;
					break;
				}
				stringstream instbuf(temp);
				instbuf >> address;
				instbuf >> address;
				if(address != "A,"){
					cout << "Invalid line." << endl;
					break;
				}
				instbuf >> address;
				if(address.length() != 5){
					cout << "Invalid line." << endl;
					break;
				}
				toWrite = convert(address); 
				toWrite += 32; 
				bin << toWrite;
				count++;
				}
				break;
			//ADD
			case 3:
				{
				if (temp.find("A,") == std::string::npos)
				{
					cout << "invalid line." << endl;
					break;
				}
				stringstream instbuf(temp);
				instbuf >> address;
				instbuf >> address;
				if(address != "A,"){
					cout << "Invalid line." << endl;
					break;
				}
				instbuf >> address;
				if(address.length() != 5){
					cout << "Invalid line." << endl;
					break;
				}
				toWrite = convert(address);
				toWrite += 64;
				bin << toWrite;
				count++;
				}
				break;
			//SUB
			case 4:
				{
				if (temp.find("A,") == std::string::npos)
				{
					cout << "invalid line." << endl;
					break;
				}
				stringstream instbuf(temp);
				instbuf >> address;
				instbuf >> address;
				if(address != "A,"){
					cout << "Invalid line." << endl;
					break;
				}
				instbuf >> address;
				if(address.length() != 5){
					cout << "Invalid line.." << endl;
					break;
				}
				toWrite = convert(address);
				toWrite += 96;
				bin << toWrite;
				count++;
				}
				break;
			//IN
			//only need the value 10000000
			case 5:
				toWrite = 128;
				bin << toWrite;
				count++;
				break;
			//JZ
			case 6:
				{
				temp = temp.substr(temp.find("Z")+1);
				stringstream instbuf(temp);
				instbuf >> address;
				//next line should be the address
				if(address.length() != 5){
					cout << "Invalid line.." << endl;
					break;
				}
				toWrite = convert(address);
				toWrite += 160;
				bin << toWrite;
				count++;
				}
				break;
			//JNNEG
			case 7:
				{
				temp = temp.substr(temp.find("G")+1);
				stringstream instbuf(temp);
				instbuf >> address;
				if(address.length() != 5){
					cout << "Invalid line.." << endl;
					break;
				}
				toWrite = convert(address);
				toWrite += 192;
				bin << toWrite;
				count++;
				}
				break;
				//halt
				//just need to write the value 11100000
			case 8:
				toWrite = 224;
				bin << toWrite;
				count++;
				break;
			default:
				cout << "Blank/Comment" << endl;
			break;
		}
	}

}
/*
	Converts the value of a string that is binary to an unsigned charcter
	with the value of the string
	Returns: unsigned char

 */
unsigned char convert(string address){
	double expp = 4.0;
	unsigned char calc = 0;
	int test = 0;

	for (int i = 0; i < 5; ++i)
	{
		if(address[i] == '1'){
			calc += (int) pow(2.0, expp);
		}
		expp -= 1.0;
	}
	return calc;
}
	/*
		Finds the instruction so it can be used in the switch statement.
		Returns: int
	 */
int findInstruction(string temp){
		if(temp.find("LOAD") != std::string::npos){return 1;}
		if(temp.find("STORE") != std::string::npos){return 2;}
		if(temp.find("ADD") != std::string::npos){return 3;}
		if(temp.find("SUB") != std::string::npos){return 4;}
		if(temp.find("IN") != std::string::npos){return 5;}
		if(temp.find("JZ") != std::string::npos){return 6;}
		if(temp.find("JNNEG") != std::string::npos){return 7;}
		if(temp.find("HALT") != std::string::npos){return 8;}
		return 0;
}