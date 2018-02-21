#ifndef EMPLOYEETHREADS_H
#define EMPLOYEETHREADS_H

#include <string>

using namespace std;

struct Employeer
{
    string fio;
    string rank;
    string birthDate;

    struct Employeer *chief;
    struct Employeer *subordinate;

};



#endif // EMPLOYEETHREADS_H
