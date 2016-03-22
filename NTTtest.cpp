#include "nTupleReader.h"

using namespace std;

int main(){

	std::vector<std::string> fs;
	fs.push_back("/home/daniel/workspace/NtupleAnalysis/build_fedora/install_dir/share/sample.root");

	nTupleReader r;


	r.setPDF("cteq6ll.LHpdf");
	r.addFiles(fs);

	double sumX=0;
	double sumY=0;
	while(r.nextEntry()){
		cout << "ID: " << r.getID()<< std::endl;
		sumX+=r.getX(0);
	}
	std::cout << "Sum X: " << sumX << std::endl;

	return 0;

}


