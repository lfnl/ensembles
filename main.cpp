#include <algorithm>
#include <random>
#include <unistd.h>
#include "classifier.h"
#include "boost/filesystem.hpp"

using namespace VW;

vector<Classifier> instances; // global var defined in classifier.h 

void showhelpinfo(char *s){
	cout << "usage: \n$ ./ensemble -t <training file> -e <test file> [OPTIONS]"
			"\nOPTIONS:\n\t-h\tshow help\n"
			"\t-t\ttraining file in vw format (http://hunch.net/~vw/validate.html)\n"
			"\t-e\ttest file in vw format (https://github.com/JohnLangford/vowpal_wabbit/wiki/Input-format)\n"
			"\t-o\taffix of output files: <affix>[...].pr. Default: 'stacking'\n"
			"\t-k\tk-fold cross validation for training. Default: 5.0\n"
			"\t-b\tbinarize predictions of base classifiers. No Args\n"
			"\t-v\tshows current version\n" << endl;	
}

int main(int argc, char *argv[]){
	string pred_name = "stacking";
	string training_file;
	string test_file;
	string binary = "";
	float k = 5.0; // 5 fold cross validation to train the meta classifier on full data
	char tmp;
	if (argc < 3){
		showhelpinfo(argv[0]);
	    exit(1);
	} 
	while((tmp=getopt(argc,argv,"b:e:h:o:t:k:v"))!=-1){
		switch(tmp){
			case 'b':
				binary = "--binary";
				break;
			case 'h':
				showhelpinfo(argv[0]);
				break;
			case 'o':
				cout << ">> prediction files get written to " << optarg << "[...].pr" << endl;
				pred_name = optarg;
				break;
			case 't':
				training_file = optarg;
				cout << ">> getting training data from " << optarg << endl;
				break;
			case 'e':
				test_file = optarg;
				cout << ">> getting test data from " << optarg << endl;
				break;
			case 'k':
				k = atof(optarg);
				cout << ">> using a " << k << "-fold cross validation for training" << endl;
				break;
			case 'v':
				cout << ">> Version: 0.1" << endl;
				break;
		}
	}


	// Classifier meta("--binary --adaptive --power_t 0.2 -c -f predictors/meta_predi_s.dat "
	// 				"--passes 200 --sort_features --cache_file cache/meta.cache", true); // works 25.1, 71.9 / 63.1?
	// Classifier meta("-f predictors/meta_predi.dat "
	// 				"--readable_model readable_models/meta.dat "+binary+" --cache_file cache/meta.cache", true);		
	// Classifier meta("--quiet --binary --boosting 20 -f predictors/meta_predi.dat", true);	
	// Classifier meta("--quiet --binary --nn 160 -f predictors/meta_predi.dat", true);	// works 23.9
	Classifier meta("--binary --loss_function=logistic",true);

	ifstream inf(training_file);
	vector<string> training_full;
	string line;
	for (unsigned int i=1; getline(inf,line); ++i){
		training_full.push_back(line);
	}

	for (int i=0; i<=k; ++i){
	
		// init a few base classifiers
		Classifier squared("--quiet -f predictors/squared"+to_string(i)+".dat --passes 50 --holdout_off --cache_file squared.cache --loss_function squared");
		Classifier hinge("--quiet -f predictors/hinge"+to_string(i)+".dat --loss_function hinge");
		Classifier l1(" --quiet -f predictors/hinge"+to_string(i)+".dat --l1 1.2e-06");
		Classifier nn("--quiet -f predictors/nn"+to_string(i)+".dat --nn 100");
		// Classifier svm("--quiet -f predictors/svm"+to_string(i)+".dat --binary --ksvm --kernel poly --l2 1.2e-06");
		Classifier quant("--quiet -f predictors/quant"+to_string(i)+".dat --loss_function quantile --quantile_tau 0.89");

		if (i != k){
			// divide training data to get dev set
			int fold_size = ceil(training_full.size()/k);
			vector<vector<string>> o = divide_folds(training_full, fold_size, i, k);
			vector<string> training_set = o[0];
			vector<string> dev_set = o[1];
			// train base classifiers:
			train_all_on_same_data(training_set);
			cout << "num of instantiated base classifiers: " << instances.size() << endl;
			// save vw readable model of preds for meta learner:
			vector<string> preds = predict_w_all(dev_set, pred_name +to_string(i)+".pr", true, 4.0); //std way to do it
			// vector<string> preds = predict_w_all_bayes(dev_set, pred_name +to_string(i)+".pr", stats, true, 4.0); // additional bayes feature
			finish_all();
			reset_instances();
			// train meta learner on data that hasn't been seen by base classifiers
			meta.training(preds);
		}
	}
	// retrain base classifiers on all of the data // or rest of data
	random_shuffle(training_full.begin(), training_full.end());
	train_all_on_same_data(training_full);
	cout << "num of base classifiers ready to roll: " << instances.size() << endl;
	// predict on test set with baseys
	vector<string> test_preds = predict_from_instances(test_file, pred_name + "_test_base.pr");
	// predict with meta on basey preds:
	meta.predict(test_preds, pred_name + "_test_meta.pr");
	meta.finish();
	
//	in destructor: VW::finish(*vw_var); ????
}