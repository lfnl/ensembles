#ifndef _CLASSIFIER_H_INCLUDED
#define _CLASSIFIER_H_INCLUDED
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../vowpal_wabbit/vowpalwabbit/parser.h"
#include "../vowpal_wabbit/vowpalwabbit/vw.h"
// #include "../vowpal_wabbit/vowpalwabbit/ezexample.h"

class Classifier;

extern int instance_count;
extern vector<Classifier> instances;


class Classifier{
	/*
	to do:
	- find_good_weights method -> hypersearch?
	- increase diversity
	- 
	*/
	string tr_file;
	string init;
	void process_example(vw* vw, std::string line);
	// vector<Classifier*> instances;

public:
	vw* vw_var;
	std::string get_tr_file() {return tr_file;};
	// vector<Classifier*> get_all_instances() {return instances;};
	// vw* get_vw(){return vw_var;};
	Classifier(std::string file, vw* vw_arg): tr_file(file), vw_var(vw_arg) {++instance_count;};
	Classifier(std::string init_str, bool = false);
	int training(string training_file);
	void process_example(Classifier classifiers[], std::string line);
	void process_example(std::string line);
	vector<float> predict_full_file(string test_file);
	void predict(std::string line, string pred_file);
	void say_hello();
};

void train_all_on_same_data(string training_data);
void predict_from_predictor_files(string predictors_dir);
vector<vector<float>> predict_from_instances(string test_file, string out_file, bool = false);
void finish_all();
vector<vector<string>> divide_folds(vector<string> lines, int fold_size, int i, float k);
void evaluate(string gold_file, vector<float> preds);

#endif