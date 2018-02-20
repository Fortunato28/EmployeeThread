// TestThreads.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <string>
#include <queue>
#include <mutex>

using namespace std;

mutex queueSafer;

// ������ �������
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

// ������ �����
void createTree(queue<string> employees)
{
	cout << "Create employees tree" << endl; 
	lock_guard<mutex> firstGuard(queueSafer); // ����� ������������ �������
	//queueSafer.lock();
	printQueue(employees);  // ������������� ����� ������
	//queueSafer.unlock();
}

//������ �����
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
		queueSafer.lock(); // ������������ �������� ��� ������ �������
		enteredAccounts.push(buff);
		queueSafer.unlock();

		thread th1(createTree, enteredAccounts);
		th1.join();

		// ������� ����������
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

