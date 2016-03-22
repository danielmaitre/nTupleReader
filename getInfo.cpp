#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
#include "TString.h"
#include "TSystem.h"
#include <string> 
#include <iostream> 
#include <sstream>
#include "TH1F.h"
#include "TList.h"
#include "TKey.h"
#include "TBuffer.h"

#include "InfoStruct.h"

#include <string>
#include <iostream>
#include <fstream>


using namespace std;

void fillString(const string& filename,string& s) {
  ifstream in(filename.c_str());
  string line;
  while(getline(in, line))
    s += line + "\n";
} 



bool readInfo(const string& rootFile,const string& file,std::ostream& os=std::cout){

	TFile f(rootFile.c_str(),"readonly");

	string fileContent;
	fillString(file,fileContent);


	InfoFile* info=dynamic_cast<InfoFile*>(f.Get(file.c_str()));
	if( info ){
		os << info->getContent();
		return true;
	} else {
		return false;
	}

}

long getEventNumber(const string& rootFile){

	TFile f(rootFile.c_str(),"readonly");

	TTree* t=dynamic_cast<TTree*>(f.Get("BHSntuples"));
	if( t ){
		return t->GetEntries();
	} else {
		return -1;
	}

}



int main(int ac, char* av[]){
	


	if (ac != 2) {
	    cout << "Usage: " << av[0] <<  " FILE\n";
	    return 0;
	}


	cout << "\n----- Process information -----\n" ;
	bool res=readInfo(av[1],"Info");

	if( !res ){
		cerr << "No information in this file.\n";
	}


	string pi;
	stringstream sspi(pi);
	res=readInfo(av[1],"ProcessInfo",sspi);
	if( res ){
		cout << sspi.str()<< std::endl;
	}

	string hi;
	stringstream ss(hi);
	res=readInfo(av[1],"HistogramsInfo",ss);
	if( res ){
		cout << "\n----- Histograms information -----\n" ;
		cout << ss.str()<< std::endl;
	}


	long n=getEventNumber(av[1]);
	if( n!=-1 ){
	  cout << "\nNumber of entries: " << n << " \n" ;
	}



	return 0;

}


