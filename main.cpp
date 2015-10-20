#include "classifier.h"
#include <list>

void run_all_on_same_data(std::string training_data){
	vw* vw_lin = VW::initialize("--adaptive --power_t 0.2 -c -f predictors/lin_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features --readable_model readable_models/lin.dat");
	vw* vw_nn = VW::initialize("--nn 1 -f predictors/nn_predi.dat --readable_model readable_models/nn.dat");
	vw* vw_lda = VW::initialize("--lda 2 --lda_alpha 0.1 --lda_rho 0.1 --lda_D 75963 --minibatch 256 --power_t 0.5 --initial_t 1 -b 16 --cache_file /tmp/vw.cache --passes 2 -p predictors/lda_predi.dat --readable_model readable_models/lda_topics.dat");
	// vw* vw_nn = VW::initialize("--nn 5 -f nn_predi.dat");
	// vw* vw_lin2 = VW::initialize("-c -f predictor.dat --passes 200 --l1 1.9e-06 --sort_features -a");
	Classifier lin(training_data, vw_lin); // could take different parts of training data
	Classifier nn(training_data, vw_nn);
	Classifier lda(training_data, vw_lda);

	Classifier together[] = {lin, nn, lda}; // array of classifiers

	ifstream in(lin.get_tr_file());
	string line;
    cout << "...reading " + lin.get_tr_file() << endl;				
	// while (std::getline(in, line))						// read file linewise
	// {
	//     std::istringstream iss(line);
	//     // cout << line << endl;	
	//     // lin.process_example(together, line);// read line and process example for every classifier			
	//     for (int i=0; i<instance_count; ++i){
	// 		vw* vw = together[i].get_vw();
	// 	    example* ex = VW::read_example(*vw, line);	// parse line as training example
	// 	    vw->learn(ex);								// process example
	// 	    VW::finish_example(*vw, ex);				// important to finish ex!
	//     }	
	//     // lin.process_example(line);
	//     // nn.process_example(line);
	//     // lda.process_example(line);
	// }
	lin.training();
	nn.training();
	lda.training();
	// 
	std::cout << "number of classifier instances: " << instance_count << std::endl;
}

int main(){
	run_all_on_same_data("../wmt15_qe/training/features.vw");

//	in destructor: VW::finish(*vw_var); ????
}