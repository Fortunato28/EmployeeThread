// TestThreads.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <fstream>
#include "employeethreads.h"

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

// Конструктор
Employee::Employee(string fio, string rank, string birthDate, bool printed = false)
{
    employeeFio = fio;
    employeeRank = rank;
    employeeBirthDate = birthDate;
}

// Деструктор
Employee::~Employee()
{
    cout << "Destructor worked!" << endl;
}

// Вывод работника в файл
bool Employee::printEmployee()
{
    ofstream fout("employeeTree.txt", ios_base::out | ios_base::app);

    if(!fout.is_open())                                     // Открыт ли файл
    {
        cout << "Have no output file!" << endl;
        return 1;
    }

    fout << "Name: " << employeeFio << endl;
    fout << "Rank: " << employeeRank << endl;
    fout << "Date of birth: " << employeeBirthDate << endl << endl;
    wasPrinted = true;

    fout.close();
    return 0;
}

// Освобождение памяти, занятой деревом
void removeTree()
{
        if(employeeTree)
        {
            // Освобождение памяти, занятой менеджерами
            for(size_t i = 0; i < employeeTree->subordinate.size(); ++i)
            {
                // Освобождение памяти, занятой подчинёнными конкретного менеджера
                for(size_t j = 0; j < employeeTree->subordinate.at(i)->subordinate.size(); ++j)     // Проход по всем подчинённым конкретного менеджера
                {
                    delete employeeTree->subordinate.at(i)->subordinate.at(j);
                }
                delete employeeTree->subordinate.at(i);
            }
            // Освобождение памяти, занятой директором
            delete employeeTree;
        }
}
// Добавление узла
void addNode(string fio, string rank, string birthDate)
{
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

    std::unique_lock<mutex> locker(treeSaver);      // Блокируем дерево сотрудников

    // Блок добавления узла в зависимости от должности
    if(!employeeTree)               // По сути, это добавление корня дерева - директора
    {
       employeeTree = new Employee(fio, rank, birthDate);
    }
    else                                                                            // Кроме менеджера и работника сюда ничего не придёт
    {
        Employee *newNode = new Employee(fio, rank, birthDate);                     // Новый узел
        if(rank == "manager")
        {
            employeeTree->subordinate.push_back(newNode);                           // Создаём менеджера
        }
        if(rank == "worker")
        {
            for(size_t i = 0; i < employeeTree->subordinate.size(); ++i)            // Проходим циклом по всем имеющимся менеджерам
            {
               if (employeeTree->subordinate.at(i)->subordinate.size() < MAXSUB)    // Количество подчинённых у конкретного менеджера
               {
                   employeeTree->subordinate.at(i)->subordinate.push_back(newNode); // Новый подчинённый у менеджера
                   break;                                                           // К другим менеджерам добавлять работника не надо
               }
               if(i == (employeeTree->subordinate.size() - 1))                      // У менеджеров нет свободного места
               {
                   cout << "Too few managers. Enter more managers, please." << endl;
                   delete newNode;                                                 // Освобождаем память работника, который в дерево включён не будет
                   return;
               }
            }
        }
    }

    ++treeSize;                                                                     // Дерево подросло
    cout << "Tree size is " << treeSize << endl;
}

void fillTheTree(string workerData)
{
    // Блок парсинга входящей строки
    size_t commaPosition;
    commaPosition = workerData.find(",");
    string fio = workerData.substr(0, commaPosition);
    ++commaPosition;                                                                    // Для сдвига
    workerData = workerData.substr(commaPosition, workerData.length() - 1);             // Сдвиг строки
    //cout << "FIO is " << fio << endl;

    commaPosition = workerData.find(",");
    string rank = workerData.substr(0, commaPosition);
    //cout << "Rank is " << rank << endl;

    commaPosition++;
    string birthDate = workerData.substr(commaPosition, workerData.length() - 1);       // Сдвиг строки
    //cout << "Date of Birth is " << birthDate << endl; 

    // Добавление узла работника
    addNode(fio, rank, birthDate);
    spuriosWakeup = true;                           // Указываем, что действительно хотим пробудить второй поток
    haveNewNode.notify_one();                       // Сигнал ожидающему потоку
}

// Первый поток
void createTree()
{
	while (true)
	{
		if (!enteredAccounts.empty())
		{
            lock_guard<mutex> firstGuard(queueSaver); // Умное блокирование очереди

			if (enteredAccounts.front() == "stop")
			{
				// TODO: написать включение последнего элемента "stop"
                spuriosWakeup = true;
                stopFlag = true;
                haveNewNode.notify_one();
                return;
			}
            fillTheTree(enteredAccounts.front());
            enteredAccounts.pop();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));             // А это чтобы бесконечный цикл не слишком сильно грузил проц
	}
}

//Второй поток
void outputTree()
{
    while(true)
    {
        std::unique_lock<mutex> locker(treeSaver);      // Блокировка для условной переменной
        while(!spuriosWakeup)                           // Защита от случайных пробуждений
        {
           haveNewNode.wait(locker);
        }

        // Условие остановки
        if(stopFlag)
        {
            removeTree();
            break;
        }

        // Обход дерева и вывод
        if(employeeTree)
        {
            if(!employeeTree->wasPrinted)
            {
                employeeTree->printEmployee();
            }

            // Вывод менеджеров
            for(size_t i = 0; i < employeeTree->subordinate.size(); ++i)
            {
                if(!employeeTree->subordinate.at(i)->wasPrinted)        // Если до этого не был напечатан
                employeeTree->subordinate.at(i)->printEmployee();       // То выведем

                // Вывод простых работников
                for(size_t j = 0; j < employeeTree->subordinate.at(i)->subordinate.size(); ++j)     // Проход по всем подчинённым конкретного менеджера
                {
                    if(!employeeTree->subordinate.at(i)->subordinate.at(j)->wasPrinted)
                    employeeTree->subordinate.at(i)->subordinate.at(j)->printEmployee();
                }
            }
        }

        spuriosWakeup = false;
   }
    return;
}

int main()
{
	string buff = "";
    treeSize = 0;                                       // Дерево пусто
    employeeTree = NULL;                                // Директора нет
    stopFlag = false;                                   // Управление выводом
    spuriosWakeup = false;                              // Защита от ложных пробуждений
    ofstream fout("employeeTree.txt", ios_base::out | ios_base::trunc); // Открываем и очищаем файл
    fout.close();

	thread th1(createTree);
    thread th2(outputTree);

    while (true)
	{
		getline(cin, buff);
        queueSaver.lock();                              // Блокирование мьютекса для защиты очереди
		enteredAccounts.push(buff);
        queueSaver.unlock();

		// Условие завершения
        if (buff == "stop")
        {
			break;
		}
	}

	th1.join();
    th2.join();

	getchar();

    return 0;
}

