//THIS IS A MORE OO APPROACH
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class CFileStringContainer{
	private:
		
		bool		initialized;
		string		delimiter, 
					file,
					theString;
		ifstream	infile;
		ofstream	outfile;
		int			stringCount;
		long		length;

	public:
					//could inline these but I'll leave it open for expansion
					CFileStringContainer();
					CFileStringContainer(const string & _file, const string & _delimiter);

		const bool	Initialize(const string & _file, const string & _delimiter);
		const bool	Initialized() {return initialized;};
		string		GetString(const int & index);
		bool		AppendString(const string & str);
		const int	NumStrings() { return stringCount; };
		long		GetFileLength();
};

CFileStringContainer::CFileStringContainer()
{
	initialized = false;
}

CFileStringContainer::CFileStringContainer(const string & _file, const string & _delimiter)
{
	Initialize(_file, _delimiter);
}

const bool 
CFileStringContainer::Initialize(const string & _file, const string & _delimiter)
{
	infile.open(_file.c_str(), ios::in);

	if(infile.is_open(), ios::in){	//may I read
		infile.close();
		outfile.open(_file.c_str(), ios::out | ios::app);
		if(outfile.is_open(), ios::out | ios::app){	//may I write
			outfile.close();
			initialized = true;
			file = _file;
			delimiter = _delimiter;
			stringCount = 0;
			theString = "";
		}
	}
	else{
		initialized = false;
	}

	return initialized;
}

bool CFileStringContainer::AppendString(const string & str)
{
	bool retVal;
	
	retVal = initialized;
	
	if(retVal && str != ""){
		outfile.open(file.c_str(), ios::out | ios::app);
		if(outfile.is_open(), ios::out | ios::app){
			outfile  << str + delimiter;
			outfile.close();
			stringCount++;
		}
		else{
			retVal = false;
		}
	}
	else{
		initialized ? cout << "The input string is empty\n" : cout << "Container is not initialized\n";
	}

	return retVal;
}

string CFileStringContainer::GetString(const int & index)
{
	int	i;

	theString = "";

	if(initialized && index > 0){
		infile.open(file.c_str(), ios::in);
		if(infile.is_open(), ios::in){
			for(i = 0; i < index && !infile.eof(); i++){
				infile >> theString;
			}
			infile.close();
		}
		else{
			cout << "Could not open " + file + " for reading.\n";
		}
	}
	else{
		initialized ? cout << "Index must be > 0\n" : cout << "Container is not intialized\n";
	}
	
	return theString;
}

long CFileStringContainer::GetFileLength()
{
	if(initialized){
		infile.open(file.c_str(), ios::in);
		infile.seekg (0, ios::end);
		length = infile.tellg();
		infile.seekg (0, ios::beg);
		infile.close();
	}
	else{
		length = 0;
	}

	return length;
}

/*
It would be used this way, or create a pointer to a CFileStringContainer in
which case there is no need to call the initalize method if the secondary
constructor is called the unit test would be similar to the previous one
*/

int main(int argc, char* argv[])
{
	int i;
	long len;

	string myString = "10";
	CFileStringContainer strings;
	
	if(strings.Initialize("OONewLineFile.txt", "\n")){
		cout << "Container is properly initialized\n";
		for(i = 0; i < 50; i++){
			strings.AppendString(myString);
			myString += "10";
		}
		
		cout << "File length is: " << strings.GetFileLength() << endl;

		for(i = 0; i <= 50; i++){
			myString = strings.GetString(i+1);
			cout << myString << endl;
		}
	}
	else{
		cout << "Container is NOT properly initialized\n";
	}

	return 0;
}


