#ifndef _CLASSIFIER_H_INCLUDED
#define _CLASSIFIER_H_INCLUDED
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "../vowpal_wabbit/vowpalwabbit/parser.h"
#include "../vowpal_wabbit/vowpalwabbit/vw.h"
// #include "../vowpal_wabbit/vowpalwabbit/ezexample.h"

class Classifier;

extern int instance_count;
extern vector<Classifier> instances;


class Classifier{
	string tr_file;
	string init;
	void process_example(vw* vw, std::string line);
public:
	vw* vw_var;
	std::string get_tr_file() {return tr_file;};
	Classifier(std::string file, vw* vw_arg): tr_file(file), vw_var(vw_arg) {++instance_count;};
	Classifier(std::string init_str, bool = false);
	int training(string training_file);
	void training(vector<string> training_data);
	void process_example(Classifier classifiers[], std::string line);
	void process_example(std::string line);
	vector<float> predict_full_file(string test_file);
	float predict(std::string line);
	vector<float> predict(vector<string> test_file, string pred_file);
	vector<float> predict(string test_file, string pred_file);
	void say_hello();
	void finish();
};

void train_all_on_same_data(string training_data);
void train_all_on_same_data(vector<string> training_data);
void predict_from_predictor_files(string predictors_dir);
vector<string> predict_from_instances(string test_file, string out_file, bool = false);
// vector<vector<float>> predict_from_instances(vector<string> test_set, string out_file, bool = false);
vector<string> predict_w_all(vector<string> test_set, string out_file, bool = false, float = 1.0);
vector<string> base_predict(vector<string> test_set, string out_file);
void finish_all();
void reset_instances();
vector<vector<string>> divide_folds(vector<string> lines, int fold_size, int i, float k);
// void evaluate(string gold_file, vector<float> preds);
/*Measures of Diversity in Classifier Ensembles and Their Relationship with the Ensemble Accuracy, kuncheva:
	best measures: 	http://arxiv.org/pdf/0810.3525.pdf -> entropy based measures

	but still:
	"the use of measuring structural diversity in building good ensembles of classifiers still remains to be explored"

*/
#endif