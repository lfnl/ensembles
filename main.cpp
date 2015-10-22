#include "classifier.h"
#include "classifier_factory.h"
#include "boost/filesystem.hpp"

using namespace VW;


Classifier* train_all_on_same_data(string training_data){
	// vw* vw_lin = initialize("--binary --adaptive --power_t 0.2 -c -f predictors/lin_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/lin.dat --cache_file cache/lin.cache");
	// vw* vw_nn = initialize("--binary --nn 1 -f predictors/nn_predi.dat --readable_model readable_models/nn.dat --cache_file cache/nn.cache");
	// vw* vw_lda = initialize("--binary --lda 2 --lda_alpha 0.1 --lda_rho 0.1 --lda_D 75963 --minibatch 256 --power_t 0.5 --initial_t 1 -b 16 --cache_file cache/lda.cache --passes 2 -p predictors/lda_predi.dat --readable_model readable_models/lda_topics.dat");
	// vw* vw_boost = initialize("--binary --boosting 5 -p predictors/boost_predi.dat --cache_file cache/boost.cache");
	// // vw* vw_lin2 = initialize("-c -f predictor.dat --passes 200 --l1 1.9e-06 --sort_features -a");
	// Classifier lin(training_data, vw_lin); // could take different parts of training data
	// Classifier nn(training_data, vw_nn);
	// Classifier lda(training_data, vw_lda);
	// Classifier boost(training_data, vw_boost);

	// Classifier all[] = {lin, nn, lda, boost}; // array of classifiers

	Classifier_Factory factory;
	Classifier lin = factory.get_classifier(training_data,"--binary --adaptive --power_t 0.2 -c -f predictors/lin_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/lin.dat --cache_file cache/lin.cache");
	vector<Classifier> all = factory.get_instances();


	ifstream in(training_data);
	string line;
    cout << "...reading " + training_data << endl;				
	while (getline(in, line))						// read file linewise
	{
	    istringstream iss(line);
	    // cout << line << endl;	
	    // lin.process_example(together, line);// read line and process example for every classifier			
	    for (int i=0; i<instance_count; ++i){
			vw* vw = all[i].vw_var;
		    example* ex = read_example(*vw, line);		// parse line as training example
		    vw->learn(ex);								// process example
		    finish_example(*vw, ex);					// important to finish ex!
	    }	
	}
	cout << "number of classifier instances: " << instance_count << endl;
	// finish(*vw_lin);	// finish if program ends...after prediction...
	// finish(*vw_nn);
	// finish(*vw_lda);
	return all;
}

void write_prediction_line(string predictions){
	ofstream outfile;
	outfile.open("test.txt", std::ios_base::app);
	outfile << predictions; 
}

void predict(string predictors_dir){
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

void predict(Classifier all[], string test_file){
	/*
	*/
	// compare to gold
	ifstream in(test_file);
	string line;
	ofstream outfile;
	outfile.open("test_predictions.txt", std::ios_base::app);
	// float tmp[instance_count]; 	// instance_count var def in classifier.h
    cout << "...reading " + test_file << endl;				
	while (getline(in, line))						// read file linewise
	{
	    istringstream iss(line);
	    for (int i=0; i<instance_count; ++i){
			vw* vw = all[i].vw_var;
		    example* ex = read_example(*vw, line);		// parse line as training example
		    outfile << get_prediction(ex);
		    if (i != instance_count - 1){
		    	outfile << "\t";
		    } else {
		    	outfile << "\n";
		    }
		    // tmp[i] = get_prediction(ex); // save preds in file
		    finish_example(*vw, ex);					// important to finish ex!
	    }	
	}
}
	  

int main(int argc, char *argv[]){
	if (argc < 3){
		cout << "usage: \n$ ./ensemble <training_file> <test_file>"<< endl;
		//msg
	} else {
		Classifier* allC;
		allC = train_all_on_same_data(argv[1]);
		predict(allC, argv[2]);
		// predict("predictors/");
	}
	

//	in destructor: VW::finish(*vw_var); ????
}