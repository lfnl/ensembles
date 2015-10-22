#include "classifier_factory.h"
#include "classifier.h"

using namespace std;

Classifier Classifier_Factory::get_classifier(string training_file, string init){
	Classifier c(training_file, init);
	instances.push_back(c);
	return c;
}