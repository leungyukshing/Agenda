#include "AgendaService.hpp"
#include "Date.hpp"
#include "Meeting.hpp"
#include "User.hpp"
#include "Storage.hpp"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	AgendaService s1;
	std::vector<string> par;
	par.push_back("test1");
	//par.push_back("b");
	//cout << "hello" << endl;
	int i = s1.deleteUser("test4","123");
	cout << i << endl;
	std::list<Meeting> m = s1.listAllMeetings("test4");
	cout << m.size() << endl;
	return 0;
}