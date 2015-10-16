#include "learner.h"

int main(){
	vw* vw_lin = VW::initialize("--adaptive --power_t 0.2 -c -f lin_predi.dat --passes 200 --l1 5e-8 --l2 5e-8 --sort_features");
	// vw* vw_nn = VW::initialize("--nn 5 -f nn_predi.dat");
	// vw* vw_lin2 = VW::initialize("-c -f predictor.dat --passes 200 --l1 1.9e-06 --sort_features -a");
	Classifier l("../wmt15_qe/training/features.vw", vw_lin);

	l.say_hello();
	l.training();
	return 0;
}