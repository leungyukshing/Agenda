#include "Date.hpp"
#include "Meeting.hpp"
#include "User.hpp"
#include "Storage.hpp"
#include <iostream>
using namespace std;

int main()
{
	/*std::shared_ptr<Storage> p = getInstance();
	User u1("Jack","123","456@qq.com","888");
	Ueser u2("Bob","222","hello@sina.com","666");

	vector<string> test_meeting;
	test_meeting.push_back("Jane");
	test_meeting.push_back("Mike");

	Date d1("2016-06-09/18:00");
	Date d2("2016-06-11/14:00");

	Meeting m1("Nike",test_meeting,d1,d2,"sport");
	Meeting m2("Benz",test_meeting,d1,d2,"car");
	p->createUser(u1);
	p->createUser(u2);
	p->createMeeting(m1);
	p->createMeeting(m2);*/
	auto temp = Storage::getInstance();
	cout << "User test:" << endl;
	std::list<User> uu = temp->queryUser([](const User &a){return true;} );
    cout << uu.size() << endl;
    for(auto i = uu.begin(); i != uu.end(); i++ )
    {
    	cout << i->getName() << endl;
    	//cout << i->getTitle() << endl;
    }

	User u1("Jack1dnsu","123","456@qq.com","888");
	User u2("Bob1","222","hello@sina.com","666");
	temp->createUser(u1);
	temp->createUser(u2);

	
	//temp->deleteUser()
    int num1 = temp->updateUser([&](const User a){ return a.getName() == "Bob1";},[&](User &a)
    																				{
    																					a.setName("football");
    																				} );

    cout << num1 << endl;
    std::list<User> ss = temp->queryUser([&](const User a){return true;} );
    
    for(auto i = ss.begin(); i != ss.end(); i++ )
    {
    	cout << i->getName() << endl;
    	//cout << i->getTitle() << endl;
    }
    temp->deleteUser([&](const User a){return false;} );
    


    cout << "Meeting test:" << endl;
    Date d1("2016-06-09/18:00");
	Date d2("2016-06-11/14:00");
    vector<string> test_meeting;

    std::list<Meeting> tt = temp->queryMeeting([&](const Meeting a){return true;} );
    //int num = temp->deleteMeeting([&](const Meeting a){return false;} );
    //cout << num << endl;
    for(auto i = tt.begin(); i != tt.end(); i++ )
    {
    	cout << i->getSponsor() << endl;
    	cout << i->getTitle() << endl;
    }

    //std::vector<string> v;
	test_meeting.push_back("Jane");
	test_meeting.push_back("Mike");
	Meeting m1("Nike",test_meeting,d1,d2,"sport");
	Meeting m2("Benz",test_meeting,d1,d2,"car");
	temp->createMeeting(m1);
	temp->createMeeting(m2);

	std::list<Meeting> ee = temp->queryMeeting([&](const Meeting a){return true;} );
    //int num = temp->deleteMeeting([&](const Meeting a){return false;} );
    //cout << num << endl;
    for(auto i = ee.begin(); i != ee.end(); i++ )
    {
    	cout << i->getSponsor() << endl;
    	cout << i->getTitle() << endl;
    }
	int num = temp->deleteMeeting([&](const Meeting a){return true;} );
    cout << num << endl;


	return 0;
}