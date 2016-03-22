#include "TObject.h"
#include "TString.h"
#include <string> 


class InfoStruct : public TObject {
	TString sherpaVersion;
	TString BHVersion;
	TString runInfo;
	TString sherpaArguments;
	TString BHsettings;
public:
	InfoStruct();
	InfoStruct(const std::string& str): runInfo(str){};
	void printInfo() const ;
	virtual ~InfoStruct();
	ClassDef(InfoStruct,1);// hhh
};


class InfoFile : public TObject {
	std::string d_name;
	std::string d_fileContent;
public:
	InfoFile();
	InfoFile(const std::string& name,const std::string& file);
	virtual const char* GetName() const;
	virtual ~InfoFile();
	const std::string& getContent() const ;
	ClassDef(InfoFile,1);// infoFile
};
