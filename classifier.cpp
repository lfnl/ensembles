#include "classifier.h"
#include <sstream>
#include "boost/filesystem.hpp"

/*
g++ -std=c++11 main.cpp classifier.cpp classifier.h ../vowpal_wabbit/vowpalwabbit/!(main|active_interactor).cc -l boost_program_options -l pthread -l z
*/

int instance_count = 0;
using namespace std;

// int instance_count = 0; // global var from classifier.h
Classifier::Classifier(std::string init_str, bool meta){
	vw_var = VW::initialize(init_str);
	if (!meta){
		instances.push_back(*this);
		++instance_count;
	}
}
		

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

int Classifier::training(string training_file){
	ifstream in(training_file);
	string line;
    cout << "\t...reading training file: " + training_file << endl;				
	while (std::getline(in, line))						// read file linewise
	{
	    std::istringstream iss(line);
	    Classifier::process_example(vw_var, line);
	    // cout << line << endl;				
	}
	// VW::finish(*vw_var);
	return 0;
}

void Classifier::training(vector<string> training_data){
	for (string line:training_data){
		Classifier::process_example(line);
	}
}


vector<float> Classifier::predict_full_file(string test_file){
	ifstream in(test_file);
	string line;
	vector<float> predictions;
    cout << "\t...reading " + test_file << endl;				
	while (getline(in, line)){
		example* ex = VW::read_example(*vw_var, line);	// parse line as training example
		if (ex != nullptr){
		    predictions.push_back(VW::get_prediction(ex));	
		    VW::finish_example(*vw_var, ex);				// important to finish ex!
		} else{
			exit(EXIT_FAILURE);
		}
	}
	return predictions;
}


float Classifier::predict(std::string line){
	example* ex = VW::read_example(*vw_var, line);	// parse line as training example
    float pred = VW::get_prediction(ex);			// process example
    VW::finish_example(*vw_var, ex);				// important to finish ex!

    return pred;
}

vector<float> Classifier::predict(vector<string> test_file, string pred_file){
	ofstream outfile;
	outfile.open(pred_file, ios::out);
	vector<float> preds;
	float prediction;

	for (string line:test_file){
		prediction = Classifier::predict(line);
		preds.push_back(prediction);
		outfile << prediction << endl;
	}
	return preds;
}


void Classifier::say_hello(){
	cout << "Hi, I'm a Classifier." << endl;
}

using namespace VW;

// Classifier* train_all_on_same_data(string training_data){
void train_all_on_same_data(string training_data){
	// vw* vw_lda = initialize("--binary --lda 2 --lda_alpha 0.1 --lda_rho 0.1 --lda_D 75963 --minibatch 256 --power_t 0.5 --initial_t 1 -b 16 --cache_file cache/lda.cache --passes 2 -p predictors/lda_predi.dat --readable_model readable_models/lda_topics.dat");
	// vw* vw_boost = initialize("--binary --boosting 5 -p predictors/boost_predi.dat --cache_file cache/boost.cache");
	// vw* vw_lin2 = initialize("-c -f predictor.dat --passes 200 --l1 1.9e-06 --sort_features -a");

	ifstream in(training_data);
	string line;
    cout << "\t...reading training data file " + training_data << endl;				
	while (getline(in, line))						// read file linewise
	{
	    istringstream iss(line);
	    for (auto classifier:instances){
			classifier.process_example(line);
	    }	
	}
	// finish(*vw_lin);	// finish if program ends...after prediction...
	// finish(*vw_nn);
	// finish(*vw_lda);
}

void train_all_on_same_data(vector<string> training_data){
	for (string line:training_data){
		for (Classifier c:instances){
			c.process_example(line);
		}
	}
}


void predict_from_predictor_files(string predictors_dir){
	/*
	takes path to directory containing the predictos files as input
	*/
	namespace fs = boost::filesystem;
	fs::path predi_dir(predictors_dir);
	fs::directory_iterator end_iter;

	if ( fs::exists(predi_dir) && fs::is_directory(predi_dir)){
		for( fs::directory_iterator dir_iter(predi_dir) ; dir_iter != end_iter ; ++dir_iter){
		    if (fs::is_regular_file(dir_iter->status()) ){
		    	cout << "ok"<<endl;
		    }		
		}
	}
}

vector<string> predict_from_instances(string test_file, string out_file, bool save_label_to_file){
	/*
	*/
	// compare to gold
	ifstream in(test_file);
	string line;
	ofstream outfile;
	outfile.open(out_file, ios::out);

	vector<string> predictions;

    cout << "\t...reading test data from " << test_file << endl;		
	while(getline(in,line)){				// read file linewise
		string example_line;					// to save preds for one example
	    for (int i=0; i<instances.size(); ++i){
			vw* vw = instances[i].vw_var;

			if (vw != nullptr){
				example* ex = read_example(*vw, line);		// parse line as training example
				if (i == 0 && save_label_to_file){
					example_line += to_string(int(get_label(ex))) + " "; // int() to save memory space
				}
				example_line += "| ";
				if (ex != nullptr){
					example_line += to_string(i) + ":" + to_string(get_prediction(ex));
					if (i != instances.size()-1){
						example_line += " ";
					}
				    finish_example(*vw, ex);					// important to finish ex!
				} else {
					cerr << "example is nullptr!!!" << endl;
					exit(EXIT_FAILURE);
				}
			} else{
				cerr << "vw is nullptr! Something went wrong in 'predict'" << endl;
		    	exit(EXIT_FAILURE);
			}		    
	    }
	    outfile << example_line << "\n";
	    predictions.push_back(example_line); // format pred_for_ex: [label, p1, p2, p3, ...]
	}
	return predictions;
}

vector<string> predict_w_all(vector<string> test_set, string out_file, bool save_label_to_file, float bad_weight){
	/*
	*/
	ofstream outfile;
	outfile.open(out_file, ios::out);

	vector<string> predictions;

    cout << "\t...reading test data from vector" << endl;		
	for (string line:test_set){						// read file linewise
		string example_line;					// to save preds for one example
	    for (int i=0; i<instances.size(); ++i){
			vw* vw = instances[i].vw_var;

			if (vw != nullptr){
				example* ex = read_example(*vw, line);		// parse line as training example
				if (i == 0){
					if(save_label_to_file){
						example_line += to_string(int(get_label(ex))) + " "; // int() to save memory space
						if(bad_weight != 1.0 && int(get_label(ex)) == -1){
							example_line += to_string(bad_weight) + " ";
						}
					}
					example_line += "| ";
				}
				if (ex != nullptr){
					example_line += to_string(i) + ":" + to_string(get_prediction(ex));
					if (i != instances.size()-1){
						example_line += " ";
					}
				    finish_example(*vw, ex);					// important to finish ex!
				} else {
					cerr << "example is nullptr!!!" << endl;
					exit(EXIT_FAILURE);
				}
			} else{
				cerr << "vw is nullptr! Something went wrong in 'predict'" << endl;
		    	exit(EXIT_FAILURE);
			}		    
	    }
	    outfile << example_line << "\n";
	    predictions.push_back(example_line); // format pred_for_ex: [label, p1, p2, p3, ...]
	}
	return predictions;
}

void finish_all(){
	for (Classifier classifier:instances){
		vw* vw_inst = classifier.vw_var;
		if (vw_inst != nullptr){
			finish(*vw_inst);
		} else{
			cerr << "Error while finishing classifier" << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void reset_instances(){
	instances.clear();
	instance_count = 0;
	cout << "\n-- instances vector reset! --\n" << endl;
}

vector<vector<string>> divide_folds(vector<string> lines, int fold_size, int i, float k){
	vector<string> tr_set;
	vector<string> tr_set2;
	vector<string> test_set;
	cout << "\n-------- Training and Dev set specs --------\n";
	if (i != k-1){
		// one part of k is the test set -> e.g. [t] [r] [r] [r] [r]
		vector<string> test_set(lines.begin() + (i*fold_size), lines.begin() + (i*fold_size + fold_size) );
		cout << "size of test_set: " << test_set.size() << endl;
		// second part of training set
		vector<string> tr_set2(lines.begin() + (i*fold_size) + fold_size, lines.end());
		cout << "size of snd tr_slice: " << tr_set2.size() << endl;
		// first part of training set:
		vector<string> tr_set(lines.begin(), lines.begin() + (i*fold_size));
		cout << "size of first tr_slice: " << tr_set.size() << endl;
		// contruction of training_set
		tr_set.insert(tr_set.end(), tr_set2.begin(), tr_set2.end());
		cout << "size of full tr_slice: " << tr_set.size() << endl;
		vector<vector<string>> a = {tr_set, test_set};
		cout << "-------- -------- -------- -------- --------\n";
		return a;

	} else { // special case that last part is test set
		vector<string> test_set(lines.begin() + (i*fold_size), lines.end() );
		cout << "size of slice: " << test_set.size() << endl;
		// first part of training set:
		vector<string> tr_set(lines.begin(), lines.begin() + (i*fold_size));
		cout << "size of first tr_slice: " << tr_set.size() << endl;
		vector<vector<string>> a = {tr_set, test_set};
		cout << "-------- -------- -------- -------- --------\n";
		return a;
	}
}

void evaluate(string gold_file, vector<float> preds){
	ifstream in(gold_file);
	string line;
    cout << "\t...reading gold data file " + gold_file << endl;		
	int tp = 0;
	int total = 0;

	// for (float x:preds){
	// 	if(getline(in, line)){
	// 		if(x == stof(line)){
	// 			++tp;
	// 		}
	// 		++total;
	// 	}
	// }
	// cout << "ACC: " << tp/total;
}
