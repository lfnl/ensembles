#include "classifier.h"
#include "boost/filesystem.hpp"

using namespace VW;

vector<Classifier> instances; // global var defined in classifier.h 

int main(int argc, char *argv[]){
	if (argc < 3){
		cout << "usage: \n$ ./ensemble <training> <test>"<< endl;
	} else {

		float k = 5.0;
		ifstream inf(argv[1]);
		vector<string> lines;
		string line;
		for (unsigned int i=1; getline(inf,line); ++i)
			lines.push_back(line);

		int fold_size = ceil(lines.size()/k);

		for (int i=0; i<k; ++i){
			cout << i << endl;
			vector<vector<string>> o = divide_folds(lines, fold_size, i, k);
			vector<string> training_set = o[0];
			vector<string> test_set = o[1];




				// vector<int> training_set(lines.begin() + (i*fold_size), lines.end());
				// cout << "size of tr_slice: " << training_set.size() << endl;

		}

		// vector<int> v;
		// for (int i= 0; i < 10; ++i) { v.push_back(i); }

		// slice(v.begin() + 1, v.begin() + 5);
		// slice(v.begin() + 2, v.begin() + 4);



		/*
		// k-fold cross validation to train all classifiers
		Classifier meta("--binary --adaptive --power_t 0.2 -c -f predictors/meta_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/meta.dat --cache_file cache/meta.cache", true);

		// loop through directory containing k training files
		namespace fs = boost::filesystem;
		fs::path training_dir(argv[1]);
		fs::directory_iterator end_iter;

		if ( fs::exists(training_dir) && fs::is_directory(training_dir)){
			for( fs::directory_iterator dir_iter(training_dir) ; dir_iter != end_iter ; ++dir_iter){
				if (fs::is_regular_file(dir_iter->status()) ){
					// init a few base classifiers
					Classifier nn("--binary --nn 100 -f predictors/nn_predi.dat --readable_model readable_models/nn.dat --cache_file cache/nn.cache");
					Classifier lin("--binary --adaptive --power_t 0.2 -c -f predictors/lin_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/lin.dat --cache_file cache/lin.cache");
					Classifier boost("--binary --boosting 5 -p predictors/boost_predi.dat --cache_file cache/boost.cache");	

					cout << "----->>>>" << dir_iter->path().string() << endl;
					train_all_on_same_data(dir_iter->path().string());


					// vector<vector<float>> preds = predict_from_instances(argv[2]+"", "base_predictions_e.txt", true); // saves vw readable model of preds for meta learner

					finish_all();

				}		
			}
		}

		*/

		/*
		// init a few base classifiers
		Classifier nn("--binary --nn 100 -f predictors/nn_predi.dat --readable_model readable_models/nn.dat --cache_file cache/nn.cache");
		Classifier lin("--binary --adaptive --power_t 0.2 -c -f predictors/lin_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/lin.dat --cache_file cache/lin.cache");
		Classifier boost("--binary --boosting 5 -p predictors/boost_predi.dat --cache_file cache/boost.cache");	

		// train base classifiers:
		train_all_on_same_data(argv[1]);
		cout << "num of instantiated base classifiers: " << instances.size() << endl;
		// predict with base classifiers
		// vector<vector<float>> preds = predict_from_instances(argv[2], "base_predictions.txt"); // uses classifiers that are stored in global instances vector
		vector<vector<float>> preds = predict_from_instances(argv[2], "base_predictions_ab.txt", true); // saves vw readable model of preds for meta learner

		Classifier meta("--binary --adaptive --power_t 0.2 -c -f predictors/meta_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/meta.dat --cache_file cache/meta.cache", true);
		// train meta classifier on held out set or full training set?
		meta.training("base_predictions_ab.txt");
		cout << "num of instantiated classifiers (with meta?): " << instances.size() << endl;
		// vector<float> testset_preds;
		// testset_preds = meta.predict_full_file("base_predictions.txt");
		// if (argc == 3){
		// 	evaluate(argv[3], testset_preds);
		// }
		// for (float x : testset_preds){
		// 	cout << x << endl;
		// }

		*/
		
	}
	

//	in destructor: VW::finish(*vw_var); ????
}