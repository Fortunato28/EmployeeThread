// TestThreads.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include "employeethreads.h"

using namespace std;

mutex queueSafer;
queue<string> enteredAccounts;

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

// ������ ������
Employeer parceAccount(string workerData)
{
    size_t commaPosition;
    commaPosition = workerData.find(",");
    string fio = workerData.substr(0, commaPosition);
    ++commaPosition;                                                            // ��� ������
    workerData = workerData.substr(commaPosition, workerData.length() - 1);     // ����� ������
    cout << "FIO is " << fio << endl;

    commaPosition = workerData.find(",");
    string rank = workerData.substr(0, commaPosition);
    cout << "Rank is " << rank << endl;

    commaPosition++;
    string dateBirth = workerData.substr(commaPosition, workerData.length() - 1);     // ����� ������
    cout << "Date of Birth is " << dateBirth << endl;

    Employeer workerNode;
    return workerNode;
}

// ������ �����
void createTree()
{
    if(!enteredAccounts.empty())
    {
    lock_guard<mutex> firstGuard(queueSafer); // ����� ������������ �������

    if(enteredAccounts.front() == "stop")
    {
        // TODO: �������� ��������� ���������� �������� "stop"
        return;
    }
    printQueue(enteredAccounts);  // ������������� ����� ������
    Employeer currentEmployee = parceAccount(enteredAccounts.front());
    enteredAccounts.pop();
    }
}

//������ �����
void printTree()
{
	cout << "Print employees tree" << endl;
}

int main()
{
	string buff = "";
    //queue<string> enteredAccounts;
    while (true)
	{
		getline(cin, buff);
		queueSafer.lock(); // ������������ �������� ��� ������ �������
		enteredAccounts.push(buff);
		queueSafer.unlock();

        thread th1(createTree);
        th1.join();

		// ������� ����������
        if (buff == "stop")
		{
			break;
		}
	}

    cout << "Size of queue is " << enteredAccounts.size() << endl;

	/*
	thread th1(createTree);
	thread th2(printTree);


	th1.join();
	th2.join();
	*/

    return 0;
}

