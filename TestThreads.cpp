// TestThreads.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <string>
#include <queue>
#include <mutex>

using namespace std;

mutex queueSafer;

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

// Первый поток
void createTree(queue<string> employees)
{
	cout << "Create employees tree" << endl; 
	lock_guard<mutex> firstGuard(queueSafer); // Умное блокирование очереди
	//queueSafer.lock();
	printQueue(employees);  // Использование общих данных
	//queueSafer.unlock();
}

//Второй поток
void printTree()
{
	cout << "Print employees tree" << endl;
}

int main()
{
	string buff = "";
	queue<string> enteredAccounts;
	while (true)
	{
		getline(cin, buff);
		queueSafer.lock(); // Блокирование мьютекса для защиты очереди
		enteredAccounts.push(buff);
		queueSafer.unlock();

		thread th1(createTree, enteredAccounts);
		th1.join();

		// Условие завершения
		if (buff == "stop")
		{
			break;
		}
	}

	cout << enteredAccounts.size() << endl;

	/*
	thread th1(createTree);
	thread th2(printTree);


	th1.join();
	th2.join();
	*/

    return 0;
}

