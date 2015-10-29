#include "classifier.h"
#include <sstream>
#include "boost/filesystem.hpp"

/*
g++ -std=c++11 main.cpp classifier.cpp classifier.h ../vowpal_wabbit/vowpalwabbit/!(main|active_interactor).cc -l boost_program_options -l pthread -l z
*/
using namespace std;

// int instance_count = 0; // global var from classifier.h

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

vector<float> Classifier::predict_full_file(string test_file){
	ifstream in(test_file);
	string line;
	vector<float> predictions;
    cout << "...reading " + test_file << endl;				
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


void Classifier::predict(std::string line, string pred_file){
	ofstream outfile;
	outfile.open(pred_file, ios::out);
	example* ex = VW::read_example(*vw_var, line);	// parse line as training example
    cout << VW::get_prediction(ex) << endl;								// process example
    outfile << VW::get_prediction(ex) << endl;								// process example
    // cout << VW::get_label(ex) << "\tpred: " << VW::get_prediction(ex) << endl;
    VW::finish_example(*vw_var, ex);				// important to finish ex!
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

	Classifier nn(training_data, "--nn 1 -f predictors/nn_predi.dat --readable_model readable_models/nn.dat --cache_file cache/nn.cache");
	Classifier lin(training_data, "--binary --adaptive --power_t 0.2 -c -f predictors/lin_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/lin.dat --cache_file cache/lin.cache");
	Classifier boost(training_data, "--binary --boosting 5 -p predictors/boost_predi.dat --cache_file cache/boost.cache");


	ifstream in(training_data);
	string line;
    cout << "...reading training data file " + training_data << endl;				
	while (getline(in, line))						// read file linewise
	{
	    istringstream iss(line);
	    for (const auto& classifier:instances){
			vw* vw = classifier.vw_var;
		    example* ex = read_example(*vw, line);		// parse line as training example
		    vw->learn(ex);								// process example
		    finish_example(*vw, ex);					// important to finish ex!
	    }	
	}
	// finish(*vw_lin);	// finish if program ends...after prediction...
	// finish(*vw_nn);
	// finish(*vw_lda);
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

vector<vector<float>> predict_from_instances(string test_file, string out_file, bool save_label_to_file){
	/*
	*/
	// compare to gold
	ifstream in(test_file);
	string line;

	ofstream outfile;
	outfile.open(out_file, ios::out);

	vector<vector<float>> predictions;

    cout << "...reading test data file " + test_file << endl;		
	while (getline(in, line)){						// read file linewise
	    istringstream iss(line);
		vector<float> pred_for_ex;					// to save preds for one example
	    for (Classifier classifier:instances){
			vw* vw = classifier.vw_var;

			if (vw != nullptr){
				example* ex = read_example(*vw, line);		// parse line as training example
				if (pred_for_ex.size() == 0){
					if (save_label_to_file){
						outfile << get_label(ex) << " ";
					}
					outfile << "| ";
				}
				if (ex != nullptr){
					outfile << pred_for_ex.size() << ":" << get_prediction(ex);
					pred_for_ex.push_back(get_prediction(ex));
				    finish_example(*vw, ex);					// important to finish ex!
				    if(pred_for_ex.size() != instance_count){
					    outfile << " ";
				    }
				} else {
					cerr << "example is nullptr!!!" << endl;
					exit(EXIT_FAILURE);
				}
			} else{
				cerr << "vw is nullptr! Something went wrong in 'predict'" << endl;
		    	exit(EXIT_FAILURE);
			}		    
	    }
	    outfile << "\n";
	    predictions.push_back(pred_for_ex);
	}
	return predictions;
}

void evaluate(string gold_file, vector<float> preds){
	ifstream in(gold_file);
	string line;
    cout << "...reading gold data file " + gold_file << endl;		
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
