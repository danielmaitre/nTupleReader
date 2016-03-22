#include "InfoStruct.h"
#include <iostream>
#include <fstream>

using namespace std;

ClassImp(InfoStruct)

InfoStruct::InfoStruct(){};
InfoStruct::~InfoStruct(){};

void InfoStruct::printInfo() const {
	cout << "runInfo:\n";
	cout << runInfo;
	cout << "\n";
}


ClassImp(InfoFile)

InfoFile::InfoFile(){};
InfoFile::InfoFile(const std::string& name,const std::string& file): d_name(name) {
	  ifstream in(file.c_str());
	  string line;
	  while(getline(in, line))
	    d_fileContent += line + "\n";
}
InfoFile::~InfoFile(){};
const char* InfoFile::GetName() const {
	return d_name.c_str();
}
const std::string& InfoFile::getContent() const {
	return d_fileContent;
} ;

