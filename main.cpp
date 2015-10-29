#include "classifier.h"
#include "classifier_factory.h"

using namespace VW;

vector<Classifier> instances; // global var defined in classifier.h
	  

int main(int argc, char *argv[]){
	if (argc < 3){
		cout << "usage: \n$ ./ensemble <training_file> <test_file>"<< endl;
	} else {
		// train base classifiers:
		train_all_on_same_data(argv[1]); // classifiers used are defined at beginning of function
		cout << "num of instantiated base classifiers: " << instances.size() << endl;
		// predict with base classifiers
		// vector<vector<float>> preds = predict_from_instances(argv[2], "base_predictions.txt"); // uses classifiers that are stored in global instances vector
		vector<vector<float>> preds = predict_from_instances(argv[1], "base_predictions_tr.txt", true); // saves vw readable model of preds for meta learner

		// for (auto x : preds){
		// 	for (auto y : x){
		// 		cout << y << "\t";
		// 	}
		// 	cout << "\n";
		// }
		// train meta classifier on held out set or full training set?
		Classifier meta("base_predictions_tr.txt", "--binary --adaptive --power_t 0.2 -c -f predictors/meta_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/meta.dat --cache_file cache/meta.cache");
		vector<float> testset_preds;
		testset_preds = meta.predict_full_file("base_predictions.txt");
		// if (argc == 3){
		// 	evaluate(argv[3], testset_preds);
		// }
		for (float x : testset_preds){
			cout << x << endl;
		}
		
	}
	

//	in destructor: VW::finish(*vw_var); ????
}