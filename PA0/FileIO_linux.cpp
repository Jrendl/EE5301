// FileIO.cpp : Defines the entry point for the console application.
//

#include <iostream> // basic C++ input/output (e.g., cin, cout)
#include <fstream>	// needed to open files in C++
#include <sstream>	// needed if you are using sstream (C++)
#include <stdio.h>	// needed to open files in C
#include <string.h>
#include <locale>
#include <vector>

using namespace std;

constexpr int MAX_NUM_GATES = 100;
struct gate_t
{
	string name = "";
	//true for delay table
	//false for skew table
	bool table_indicator = true;
	double capacitance = 0.0;
	double cell_delay[7][7] = {0.0};
	double output_slew[7][7] = {0.0};
};

// returns the next character in file without actually reading it (i.e., going past it)
int fpeek(FILE *stream);
int parseFileCppFormat(char *fName);

gate_t gate_list[MAX_NUM_GATES]; // Global for now for ease of programming.

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "I need one parameter, which is the file name." << endl;
		return -1;
	}

	parseFileCppFormat(argv[1]);
	return 0;
}

// Kia got this function from stackoverflow https://stackoverflow.com/questions/2082743/c-equivalent-to-fstreams-peek
int fpeek(FILE *stream)
{
	int c;

	c = fgetc(stream);
	ungetc(c, stream);

	return c;
}

int parseFileCppFormat(char *fName)
{
	char lineBuf[1024];
	cout << "Parsing input file " << fName << " using C++ syntax." << endl;
	ifstream ifs(fName);
	if (ifs.is_open() == 0)
	{ // or we could say if (!ifs)
		cout << "Error opening file " << fName << endl;
		return -1;
	}

	// set up istream masking for iss
	const auto temp = ctype<char>::classic_table();
	vector<ctype<char>::mask> delimiters(temp, temp + ctype<char>::table_size);

	delimiters['('] |= ctype_base::space;
	delimiters[')'] |= ctype_base::space;
	delimiters[','] |= ctype_base::space;
	delimiters['\"'] |= ctype_base::space;
	delimiters[';'] |= ctype_base::space;
	delimiters[':'] |= ctype_base::space;
	int gate_number = 0; //iterator between gates
	int delay_table = 0; //iterator between rows of the delay table
	int output_slew = 0; //iterator between rows of the output slew table
	while (ifs.good())
	{
		ifs.getline(lineBuf, 1023); // read one line
		string lineStr(lineBuf);	// convert to C++ string
		if (lineStr.empty())		// is empty line?
			continue;

		istringstream iss(lineStr);
		iss.imbue(locale(iss.getloc(), new ctype<char>(delimiters.data())));

		string firstWord;
		iss >> firstWord;
		cout << "first word:" << firstWord<< endl;
		if (firstWord.find("cell") != string::npos)
		{ // found the word cell

			string cellName;
			iss >> cellName;
			if (cellName.find("Timing") != string::npos)
			{
				continue;
			}
			cout << "Found Cell " << cellName << endl;
			gate_list[gate_number].name = cellName;
			// want to get rid of the ')' at the end of the cell name?
			// You can get rid of the last character, but a cooler and probably much more useful technique is to
			// define your delimiter to be space and ')'.
			// This would be useful when later we want to get rid of " and , as well to get the 7x7 delay table.
			// See this post and the solution https://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
		}
		else if (firstWord.compare("capacitance") == 0)
		{
			string cap;
			iss >> cap;
			cout << "capacitance =" << cap << endl;
			gate_list[gate_number].capacitance = stod(cap);
		}
		else if (firstWord.compare("values") == 0)
		{
			if (gate_list[gate_number].table_indicator)
			{ //we are in the delay_table
				//take the first row of the table
				string val;
				for (int i = 0; i < 7; i++)
				{
					iss >> val;
					cout << "delay table value: " << val << endl;
					gate_list[gate_number].cell_delay[delay_table][i] = stod(val);
				}
				delay_table++;
			}else{ //in the output slew table
				//take the first row of the table
				string val;
				for (int i = 0; i < 7; i++)
				{
					iss >> val;
					cout << "output slew value: " << val << endl;
					gate_list[gate_number].output_slew[output_slew][i] = stod(val);
				}
				output_slew++;
			}
		}else if(firstWord.find("0.") != string::npos){
			if (gate_list[gate_number].table_indicator)
			{ //we are in the delay_table
				gate_list[gate_number].cell_delay[delay_table][0] = stod(firstWord);
				string val;
				for (int i = 1; i < 7; i++)
				{
					iss >> val;
					cout << "delay table value: " << val << endl;
					gate_list[gate_number].cell_delay[delay_table][i] = stod(val);
				}
				delay_table++;
				//check if we've fully populated the delay table
				if (delay_table >= 7){
					delay_table = 0;
					gate_list[gate_number].table_indicator = false;
				}
			}else{ //in the output slew table
				gate_list[gate_number].output_slew[output_slew][0] = stod(firstWord);
				string val;
				for (int i = 1; i < 7; i++)
				{
					iss >> val;
					cout << "output slew value: " << val << endl;
					gate_list[gate_number].output_slew[output_slew][i] = stod(val);
				}
				output_slew++;
				//check if we've fully populated the output slew table
				if (output_slew >= 7){
					output_slew = 0;
					gate_number++; //we're done with this gate
				}
			}
		}
	}

	return 0;
}
