// ca1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int doubleNum(int x) 
{
	return 2 * x;
}

int main()
{
	int x;
	cout << "Please input a number: ";
	cin >> x;
	cout << x << " doubled is " << doubleNum(x) << endl;
	return 0;
}

