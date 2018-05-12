#include "Storage.hpp"
#include "Path.hpp"
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

void trim( string& src )
{
  //cout << "hello" << endl;
 src.erase(0,src.find_first_not_of("\""));
 src.erase(src.find_last_not_of("\"")+1);
}

vector<string> handleParticipator( string src )
{
 vector<string> output;
 int i = 0;
 while( i < src.size() )
 {
  if( src[i] == '&' )
  {
   i++;
  }
  else
  {
   int k = i;
   string temp = "";
   while( src[k] != '&' && k < src.size() )
   {
    temp += src[k];
    k++;
   }
   output.push_back(temp);
   i = k;
  }
 }
 return output;
}

string makelist( vector<string> src )
{
  //cout <<"hello" << endl;
  if(src.size() == 0 )
  {
    string s = "";
    return s;
  }
 string output;
 int i = 0;
 for( i = 0; i < src.size()-1; i++ )
 {
  output += src[i];
  output += "&";
 }
 output += src[i];

 return output;
}

std::shared_ptr<Storage> Storage::m_instance = nullptr;

//private
 Storage::Storage()
 {
  m_dirty = false;
  readFromFile();
 }

 /**
  *   read file content into memory
  *   @return if success, true will be returned
  */
  bool Storage::readFromFile(void)
  {
   ifstream user_myfile;
   ifstream meeting_myfile;
   user_myfile.open(Path::userPath);
   if(!user_myfile)
   	cout<<1<<endl;
   
   meeting_myfile.open(Path::meetingPath);

   if( user_myfile.is_open() == false || meeting_myfile.is_open() == false )
   {
    //cout << "fault" << endl;
    user_myfile.close();
    meeting_myfile.close();
    return false;
   }
    //handle user data
   string temp;
     while( getline(user_myfile,temp) )
    {
     //char t[1000] = {0};
     //user_myfile.getline( t,100 );
     //string temp = t;
      //cout << "hello" << endl;
      if( temp.size() == 0 )
      {
        continue;
      }
     int i = 0;
     string name, password, email, phone;
     while( temp[i] != ',' )
     {
      name += temp[i];
      i++;
     }
     i += 1; 
     while( temp[i] != ',' )
     {
      password += temp[i];
      i++;
     }
     i += 1;
     while( temp[i] != ',' )
     {
      email += temp[i];
      i++;
     }
     i += 1;
     while( i < temp.size() )
     {
      phone += temp[i];
      i++;
     }

     //cout << "hello" << endl;


     trim(name);
     trim(password);
     trim(email);
     trim(phone);
     //cout << "hello" << endl;
     User u( name, password, email, phone );
     this->m_userList.push_back(u);
     //cout << m_userList.size() << endl;

    }

    //handle meeting data
    //cout << "hello" << endl;
    string temp1;
    while( getline(meeting_myfile,temp1) )
    {
     //char t[1000] ={0};
     //user_myfile.getline( t,100 );
     ///string temp = t;
     //cout << temp1 << endl;
      //cout << "hello" << endl;
      if( temp1.size() == 0 )
      {
        continue;
      }
     int i = 0;
     string sponsor, participators, startDate, endDate, title;
     while( temp1[i] != ',' )
     {
      //cout << "1" << endl;
      sponsor += temp1[i];
      i++;
     }
     i += 1;
     while( temp1[i] != ',' )
     {
      participators += temp1[i];
      i++;
     }
     i += 1;
     while( temp1[i] != ',' )
     {
      startDate += temp1[i];
      i++;
     }
     i += 1;
     while( temp1[i] != ',' )
     {
      endDate += temp1[i];
      i++;
     }
     i += 1;
     while( i < temp1.size() )
     {
      title += temp1[i];
      i++;
     }
     //cout << "hello" << endl;
     trim(sponsor);
     trim(participators);
     trim(startDate);
     trim(endDate);
     trim(title);
     //cout << "hello" << endl;
     Date start(startDate);
     Date end(endDate);
     //cout << "hello" << endl;
     Meeting m( sponsor, handleParticipator(participators), start, end, title );
     this->m_meetingList.push_back(m);
    }
    //cout << "hello" << endl;
    user_myfile.close();
    meeting_myfile.close();
    return true;



  }

  /**
  *   write file content from memory
  *   @return if success, true will be returned
  */
  bool Storage::writeToFile(void)
  {

   ofstream user_myfile;
   ofstream meeting_myfile;
   user_myfile.open(Path::userPath, ios::out);
   meeting_myfile.open(Path::meetingPath, ios::out);

   if( user_myfile.is_open() == false || meeting_myfile.is_open() == false )
   {
    user_myfile.close();
    meeting_myfile.close();
    return false;
   }
   /*
   if(m_userList.size() == 0 )
   {
    string temp = "";
    user_myfile << temp;
   }
   if(m_meetingList.size() == 0 )
   {
    string temp = "";
    meeting_myfile << temp;
   }*/
   
   
     //cout << m_userList.size() << endl;
   for( auto it = m_userList.begin(); it != m_userList.end(); it++ )
   {
    string temp;
    temp = "\"" + (*it).getName() + "\",\"" + (*it).getPassword() + "\",\"" + (*it).getEmail() + "\",\"" + (*it).getPhone() + "\"\n";
    user_myfile << temp;
    //cout << "a" << endl;
   }
   //cout << "a"<<endl;
   for( auto it = m_meetingList.begin(); it != m_meetingList.end(); it++ )
   {
    string temp;
    Date d1 = (*it).getStartDate();
    Date d2 = (*it).getEndDate();
    string start, end;
    start = d1.dateToString(d1);
    end = d2.dateToString(d2);
    string par = makelist( (*it).getParticipator() ); // !!!!!!

    temp = "\"" + (*it).getSponsor() + "\",\"" + par + "\",\"" + start + "\",\"" + end + "\",\"" + (*it).getTitle() + "\"\n";
    //cout << "a" << endl;
    meeting_myfile << temp;
   }
   
   

   user_myfile.close();
   meeting_myfile.close();
   return true;
  }



  //public
  std::shared_ptr<Storage> Storage::getInstance(void)
  {
   if( m_instance == NULL )
   {
    m_instance = std::shared_ptr<Storage>( new Storage );
   }
   return m_instance;
  }

  /**
  *   destructor
  */
  Storage::~Storage()
  {
   sync();
  } 

  // CRUD for User & Meeting
  // using C++11 Function Template and Lambda Expressions

  /**
  * create a user
  * @param a user object
  */
  void Storage::createUser(const User &t_user)
  {
   for( auto it = this->m_userList.begin(); it != this->m_userList.end(); it++ )
   {
    if( (*it).getName() == t_user.getName() )
    {
     return;
    }
   }
   this->m_userList.push_back(t_user);
   //cout << "h" << endl;
   m_dirty = true;
  }

  /**
  * query users
  * @param a lambda function as the filter
  * @return a list of fitted users
  */
  std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const
  {
   list<User> found_list;
   for( auto it = this->m_userList.begin(); it != this->m_userList.end(); it++ )
   {
    if( filter(*it) )
    {
     found_list.push_back(*it);
    }
   }

   return found_list;
  }

  /**
  * update users
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the user
  * @return the number of updated users
  */
  int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher)
  {
   int update_num = 0;
   for( auto it = this->m_userList.begin(); it != this->m_userList.end(); it++ )
   {
    if( filter(*it) )
    {
     switcher(*it);
     update_num++;
    }
   }
   m_dirty = true;
   return update_num;
  }

  /**
  * delete users
  * @param a lambda function as the filter
  * @return the number of deleted users
  */
  int Storage::deleteUser(std::function<bool(const User &)> filter)
  {
   int delete_num = 0;
   for( auto it = this->m_userList.begin(); it != this->m_userList.end(); )
   {
    if( filter(*it) )
    {
     it = this->m_userList.erase(it);
     
     delete_num++;
    }
    else
    {
      it++;
    }
   }
   m_dirty = true;

   return delete_num;
  }

  /**
  * create a meeting
  * @param a meeting object
  */
  void Storage::createMeeting(const Meeting &t_meeting)
  {
   for( auto it= this->m_meetingList.begin(); it != this->m_meetingList.end(); it++ )
   {
    if( (*it).getTitle() == t_meeting.getTitle() )
    {
     return;
    }
   }
   this->m_meetingList.push_back(t_meeting);
   m_dirty = true;
  }

  /**
  * query meetings
  * @param a lambda function as the filter
  * @return a list of fitted meetings
  */
  std::list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const
  {
   list<Meeting> found_list;
   for( auto it = this->m_meetingList.begin(); it != this->m_meetingList.end(); it++ )
   {
    if( filter(*it) )
    {
     found_list.push_back(*it);
    }
   }

   return found_list;
  }

  /**
  * update meetings
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the meeting
  * @return the number of updated meetings
  */
  int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher)
  {
   int update_num = 0;
   for( auto it = this->m_meetingList.begin(); it != this->m_meetingList.end(); it++)
   {
    //cout <<"1" << endl;
    if( filter(*it) )
    {
      //cout <<"1" << endl;
     switcher(*it);
     update_num++;
    }
    //cout << "2" << endl;
   }
   m_dirty = true;
   //cout << "big" << endl;
   return update_num;
  }

  /**
  * delete meetings
  * @param a lambda function as the filter
  * @return the number of deleted meetings
  */
  int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter)
  {
   int delete_num = 0;
   for( auto it = this->m_meetingList.begin(); it != this->m_meetingList.end(); )
   {
    if( filter(*it) )
    {
     it = this->m_meetingList.erase(it);
     
     delete_num++;
    }
    else
    {
      it++;
    }
   }
   m_dirty = true;
   return delete_num;
  }

  /**
  * sync with the file
  */
  bool Storage::sync(void)
  {
   if( m_dirty == true )
   {
    m_dirty = false;
    writeToFile();
   }
   return true;
  }
