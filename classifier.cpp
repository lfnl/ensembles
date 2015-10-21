#include "classifier.h"
#include <sstream>

/*
g++ -std=c++11 main.cpp classifier.cpp classifier.h ../vowpal_wabbit/vowpalwabbit/!(main|active_interactor).cc -l boost_program_options -l pthread -l z
*/
using namespace std;

int instance_count = 0; // global var from classifier.h

void Classifier::process_example(string line){
	// process example with one classifiers
    example* ex = VW::read_example(*vw_var, line);	// parse line as training example
    vw_var->learn(ex);								// process example
    // cout << VW::get_label(ex) << "\tpred: " << VW::get_prediction(ex) << endl;
    VW::finish_example(*vw_var, ex);				// important to finish ex!
}

void Classifier::process_example(vw* vw, string line){
	// process example with one classifiers
    example* ex = VW::read_example(*vw, line);	// parse line as training example
    vw->learn(ex);								// process example
    // cout << VW::get_label(ex) << "\tpred: " << VW::get_prediction(ex) << endl;
    VW::finish_example(*vw, ex);				// important to finish ex!
}

void Classifier::process_example(Classifier classifiers[], string line){
	// process same example with multiple classifiers
	for (int i=0; i<instance_count; ++i){
		vw* vw = classifiers[i].vw_var;
	    example* ex = VW::read_example(*vw, line);	// parse line as training example
	    vw->learn(ex);								// process example
	    VW::finish_example(*vw, ex);				// important to finish ex!
    }				
}

int Classifier::training(){
	ifstream in(Classifier::get_tr_file());
	string line;
    cout << "...reading " + Classifier::get_tr_file() << endl;				
	while (std::getline(in, line))						// read file linewise
	{
	    std::istringstream iss(line);
	    Classifier::process_example(vw_var, line);
	    // cout << line << endl;				
	}
	VW::finish(*vw_var);
	return 0;
}

void Classifier::say_hello(){
	cout << "Hi, I'm a Classifier." << endl;
}