#ifndef _CLASSIFIER_H_INCLUDED
#define _CLASSIFIER_H_INCLUDED
#include <string>
#include <iostream>
#include <fstream>
#include "../vowpal_wabbit/vowpalwabbit/parser.h"
#include "../vowpal_wabbit/vowpalwabbit/vw.h"
#include "../vowpal_wabbit/vowpalwabbit/ezexample.h"

extern int instance_count;

class Classifier{
	/*
	to do:
	- find_good_weights method -> hypersearch?
	- increase diversity
	- 
	*/
	std::string tr_file;
	std::string init;
	void process_example(vw* vw, std::string line);

public:
	vw* vw_var;
	std::string get_tr_file() {return tr_file;};
	// vw* get_vw(){return vw_var;};
	Classifier(std::string file, vw* vw_arg): tr_file(file), vw_var(vw_arg) {++instance_count;};
	Classifier(std::string file, std::string init_str): tr_file(file), init(init_str){
		++instance_count;
		vw* vw_var = VW::initialize(init);
	}
	int training();
	void process_example(Classifier classifiers[], std::string line);
	void process_example(std::string line);
	void say_hello();
};


#endif