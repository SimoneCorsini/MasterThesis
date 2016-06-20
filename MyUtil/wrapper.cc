#include "glucoselib.h"
#include <algorithm>

int glucoseSAT(char *the_cnf, int num_var, int num_cl, int *result)
{
	vector<int> *res = new vector<int>();
	stringstream *ss = new stringstream();
	ss->str(the_cnf);
	int value = glucose_lib_orig(ss, num_var, num_cl, res);
	std::copy(res->begin(), res->end(), result);
	return value;
}
