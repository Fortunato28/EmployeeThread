#ifndef EMPLOYEETHREADS_H
#define EMPLOYEETHREADS_H

#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::queue;
using std::getchar;
using std::string;
using std::mutex;
using std::lock_guard;
using std::thread;
using std::ofstream;
using std::ios_base;
using std::condition_variable;
/*************************Макросы**********************
**
 * @brief максимальное количество рядовых работников, которыми менеджер может управлять
 */
#define MAXSUB 2

class Employee
{
public:
    Employee(string fio, string rank, string birthDate, bool printed);
    bool wasPrinted;
    vector<class Employee *> subordinate;
    bool printEmployee();
    ~Employee();
private:
    string employeeFio;
    string employeeRank;
    string employeeBirthDate;
};


/*************************Переменные**********************/
// Мьютекс для блокировки очереди
mutex queueSaver;
// Мьютекс для блокировки дерева
mutex treeSaver;
// Мьютекс для блокировки печати
mutex lockPrint;
// Условная переменная для работы вывода
condition_variable haveNewNode;

// Очередь, в которую добавляются введённые учетные записи
queue<string> enteredAccounts;

// Корень дерева работников (директор)
Employee *employeeTree;

// Размер дерева работников (количество узлов)
size_t treeSize;

// Флаг для остановки потока вывода
bool stopFlag;
// Флаг для отслеживания ложного пробуждения
bool spuriosWakeup;
#endif // EMPLOYEETHREADS_H
