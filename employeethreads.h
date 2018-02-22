#ifndef EMPLOYEETHREADS_H
#define EMPLOYEETHREADS_H

#include <string>
#include <vector>
#include <queue>

using namespace std;

/*************************Макросы**********************
**
 * @brief максимальное количество рядовых работников, которыми менеджер может управлять
 */
#define MAXSUB 2

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
// Мьютекс для блокировки очереди
mutex queueSaver;
// Мьютекс для блокировки дерева
mutex treeSaver;

// Очередь, в которую добавляются введённые учетные записи
queue<string> enteredAccounts;

// Корень дерева работников
Employee *employeeTree;

// Размер дерева работников (количество узлов)
size_t treeSize;
#endif // EMPLOYEETHREADS_H
