// TestThreads.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include "employeethreads.h"

using namespace std;

mutex queueSafer;
queue<string> enteredAccounts;

// Печать очереди
void printQueue(queue<string> input)
{
	queue<string> buff = input;

	while (!buff.empty())
	{
		string val;
		val = buff.front();
		buff.pop();
		cout << val << endl;
	}

}

// Добавление узла
Employee* addNode(Employee *employeeNode, string fio, string rank, string birthDate)
{
	if (!treeSize && (rank != "director"))               // Сначала нужно задать директора предприятия
	{
		cout << "Please, enter director firstly." << endl;
		return NULL;
	}
	if ((treeSize == 1) && (rank != "manager"))			// А после директора хотя бы одного менеджера
	{
		cout << "Please, enter manager." << endl;
		return NULL;
	}
	if ((treeSize > 1) && (rank == "director"))			// Директор может быть только один
	{
		cout << "Company can have only one director." << endl;
		return NULL;
	}
	if ((treeSize > 1) && ((rank != "manager") || (rank != "worker")))
	{

		cout << treeSize <<"Please, enter correct rank: manager or worker" << endl;
		return NULL;
	}
	if (!employeeNode)				// Если узел пуст
	{
		employeeNode = new Employee;
		employeeNode->fio = fio;
		employeeNode->rank = rank;
		employeeNode->birthDate = birthDate;
		employeeNode->subordinate.push_back(NULL);
	}
	
	++treeSize;											// В дереве стало больше на один узел
	return employeeNode;
}

// Парсим строку
Employee * parceAccount(string workerData)
{
    size_t commaPosition;
    commaPosition = workerData.find(",");
    string fio = workerData.substr(0, commaPosition);
    ++commaPosition;                                                            // Для сдвига
    workerData = workerData.substr(commaPosition, workerData.length() - 1);     // Сдвиг строки
    //cout << "FIO is " << fio << endl;

    commaPosition = workerData.find(",");
    string rank = workerData.substr(0, commaPosition);
    //cout << "Rank is " << rank << endl;

    commaPosition++;
    string birthDate = workerData.substr(commaPosition, workerData.length() - 1);     // Сдвиг строки
    //cout << "Date of Birth is " << birthDate << endl; 

    Employee *employeeNode = NULL;

	employeeNode = addNode(employeeNode, fio, rank, birthDate);

    return employeeNode;
}

// Первый поток
void createTree()
{
	while (true)
	{
		if (!enteredAccounts.empty())
		{
			lock_guard<mutex> firstGuard(queueSafer); // Умное блокирование очереди

			if (enteredAccounts.front() == "stop")
			{
				// TODO: написать включение последнего элемента "stop"
				return;
			}
			//printQueue(enteredAccounts);  // Использование общих данных
			Employee *currentEmployee = parceAccount(enteredAccounts.front());
			enteredAccounts.pop();
		}
	}
}

//Второй поток
void printTree()
{
	cout << "Print employees tree" << endl;
}

int main()
{
	string buff = "";
	treeSize = 0;

	thread th1(createTree);

    while (true)
	{
		getline(cin, buff);
		queueSafer.lock(); // Блокирование мьютекса для защиты очереди
		enteredAccounts.push(buff);
		queueSafer.unlock();

		// Условие завершения
        if (buff == "stop")
		{
			break;
		}
	}

	th1.join();

    //cout << "Size of queue is " << enteredAccounts.size() << endl;

	getchar();

    return 0;
}

