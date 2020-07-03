#pragma once

//STD Includes
#include <string>
#include <fstream>
#include <streambuf>

//Ravine Includes
#include "debug.hpp"

//Using directives
using std::ios;
using std::string;
using std::ifstream;
using std::istreambuf_iterator;

//Defines
#define rvLoadFile rv::io::loadFile

//This class is a fast text file loader
namespace rv
{
	namespace io
	{
		typedef const char* cstr;

		//Loads the entire file in the passed string
		//Return the size of the string loaded, zero or less is an error code
		static size_t loadFile(const string& path, string& output, bool binary_mode = false)
		{
			//Open Input File Stream
			ifstream inputFileStream(path, binary_mode ? ifstream::binary : ifstream::in);

			if (inputFileStream.is_open())
			{
				//Seek the End of File flag
				inputFileStream.seekg(0, ios::end);

				//Use the number of chars to reserve proper string size
				size_t length = inputFileStream.tellg();
				output.reserve(length);

				//Go back to the beginning of the file
				inputFileStream.seekg(0, ios::beg);

				//Assign all data to the string
				output.assign(istreambuf_iterator<char>(inputFileStream), istreambuf_iterator<char>());

				//Return size
				return length;
			}

			//Debug error
			rvDebugLogError("Failed to read file: " + path);

			//Return error code
			return 0;
		}

		//Loads the entire file in the passed char array
		//Returns the size of the array loaded, zero or less is an error code
		static size_t loadFile(const string& path, char*& output, bool binary_mode = false)
		{
			//Open Input File Stream
			ifstream inputFileStream(path, binary_mode ? ifstream::binary : ifstream::in);

			if (inputFileStream.is_open())
			{
				//Seek the End of File flag
				inputFileStream.seekg(0, std::ios::end);

				//Use the number of chars to allocate proper char array size
				size_t length = inputFileStream.tellg();
				output = new char[length + 1];

				//Go back to the beginning of the file
				inputFileStream.seekg(0, std::ios::beg);

				//Assign the all data to the char array
				inputFileStream.read(output, length);
				output[length] = '\0';

				//Return size
				return length + 1;
			}

			//Debug error
			rvDebugLogError("Failed to read file: " + path);

			//Return error code
			return 0;
		}
	}
}