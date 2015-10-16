#ifndef _LEARNER_FAC_H_INCLUDED
#define _LEARNER_FAC_H_INCLUDED
#include <string>
#include <iostream>
#include <fstream>
#include "learner.h"
#include "../vowpal_wabbit/vowpalwabbit/parser.h"
#include "../vowpal_wabbit/vowpalwabbit/vw.h"
#include "../vowpal_wabbit/vowpalwabbit/ezexample.h"

class Classifier_Factory{
	/*
	to do:
	-
	*/
	std::string tr_file;

public:
	vw* get_lin(float l1, float l2);
	vw* get_neural_net(int hidden_units);
};

#endif