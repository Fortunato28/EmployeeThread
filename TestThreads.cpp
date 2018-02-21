// TestThreads.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <chrono>
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
void addNode(string fio, string rank, string birthDate)
{
    Employee node; // Текущий узел для добавления
    // Блок ограничений на входные параметры
	if (!treeSize && (rank != "director"))               // Сначала нужно задать директора предприятия
	{
		cout << "Please, enter director firstly." << endl;
        return;
    }
	if ((treeSize == 1) && (rank != "manager"))			// А после директора хотя бы одного менеджера
	{
		cout << "Please, enter manager." << endl;
        return;
    }
	if ((treeSize > 1) && (rank == "director"))			// Директор может быть только один
	{
		cout << "Company can have only one director." << endl;
        return;
	}
    if ((treeSize > 1) && ((rank != "manager") && (rank != "worker")))
	{

        cout <<"Please, enter correct rank: manager or worker" << endl;
        return;
	}

    // Блок добавления узла в зависимости от входных параметров
    if(!employeeTree)               // По сути, это добавление корня дерева - директора
    {
       employeeTree = new Employee;
       employeeTree->fio = fio;
       employeeTree->rank = rank;
       employeeTree->birthDate = birthDate;
    }
    else
    {
        if(rank == "manager")
        {
            employeeTree->subordinate.push_back(NULL);                  // Создаём слот для менеджера
            Employee *newNode = new Employee;                           // Выделили память для узла
            employeeTree->subordinate.back() = newNode;                 // Новый узел и его заполнение
            newNode->fio = fio;
            newNode->rank = rank;
            newNode->birthDate = birthDate;
        }
        if(rank == "worker")
        {
            cout << "Worker, epta!" << endl;
        }
    }
    ++treeSize;                                                         // Дерево подросло
    cout << "Tree size is " << treeSize << endl;
}

void fillTheTree(string workerData)
{
    // Блок парсинга входящей строки
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

    // Добавление узла работника

    addNode(fio, rank, birthDate);
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
            fillTheTree(enteredAccounts.front());
			enteredAccounts.pop();
		}
        this_thread::sleep_for(chrono::seconds(1));             // А это чтобы бесконечный цикл не слишком сильно грузил проц
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
    employeeTree = NULL;

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

