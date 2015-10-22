#ifndef _CLASSIFIER_FACTORY_H_INCLUDED
#define _CLASSIFIER_FACTORY_H_INCLUDED
#include <vector>
#include "classifier.h"


int instance_count = 0;

class Classifier_Factory{
	vector<Classifier> instances;
public:
	Classifier get_classifier(string training_file, string init);
	vector<Classifier> get_instances(){return instances;}
};

#endif