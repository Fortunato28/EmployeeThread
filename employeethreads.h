#ifndef EMPLOYEETHREADS_H
#define EMPLOYEETHREADS_H

#include <string>
#include <vector>

using namespace std;

struct Employee
{
	Employee()
	{
	
	}
    string fio;
    string rank;
    string birthDate;

    vector<struct Employee *> subordinate;
};


/*************************Переменные**********************/
size_t treeSize;


#endif // EMPLOYEETHREADS_H
