#include learner_factory.h

Classifier_Factory::get_neural_net(int hidden_units){
	
	return VW::initialize("--nn" + hidden_units + " -f nn_predi.dat")
}