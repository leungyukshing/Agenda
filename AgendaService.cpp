#include "AgendaService.hpp"
#include "Storage.hpp"
#include "Date.hpp"
#include "Meeting.hpp"
#include "User.hpp"
#include <string>
#include <iostream>
using namespace std;
/**
   * constructor
   */
  AgendaService::AgendaService()
  {
   startAgenda();
  }

  /**
   * destructor
   */
  AgendaService::~AgendaService()
  {
   quitAgenda();
  }

  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
  bool AgendaService::userLogIn(const std::string &userName, const std::string &password)
  {
   std::list<User> temp = listAllUsers();
   for( auto i = temp.begin(); i != temp.end(); i++ )
   {
    if( (*i).getName() == userName && (*i).getPassword() == password )
    {
     return true;
    }
   }
   return false;
  }

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
  bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone)
  {
   std::list<User> temp = listAllUsers();
   for( auto i = temp.begin(); i != temp.end(); i++ )
   {
    if( (*i).getName() == userName )
    {
     return false;
    }
   }
   User u(userName,password,email,phone);
   m_storage->createUser(u);
   return true;
  }

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
  bool AgendaService::deleteUser(const std::string &userName, const std::string &password)
  {
   if(!userLogIn(userName,password))
   {
    return false;
   }
   int flag = m_storage->deleteUser( [&](const User &a)
       {
        if( a.getName() == userName && a.getPassword() == password )
        {
         return true;
        }
        return false;
       });
  
   if( flag == 0 )
   {
    return false;
   }
   else // handle side effect
   {
   	 //user is the sponsor of the meeting, so the meeting has to be deleted
	 m_storage->deleteMeeting( [&](const Meeting &a)
   					{
   						if( a.getSponsor() == userName )
   						{
   							return true;
   						}
   						return false;
   					});

	 //user is the participator of the meeting, so the user has to be removed from the meeting
	 m_storage->updateMeeting([&](const Meeting &a)
        {
         if( a.isParticipator(userName) )
         {
          return true;
         }
         return false;
        },
        [&](Meeting& b)
        {
         b.removeParticipator(userName);

        });

	 //after remove the user frome the meeting, delete all the meeting whose participator is empty.
   	 m_storage->deleteMeeting( [&](const Meeting& a)
   					{
   						if( a.getParticipator().size() == 0 )
   						{
   							return true;
   						}
   						return false;
   					});

   	 return true;
   }
  }

  /**
   * list all users from storage
   * @return a user list result
   */
  std::list<User> AgendaService::listAllUsers(void) const
  {
   std::list<User> output;
   output = m_storage->queryUser( [&](const User &a){return true;} );
   return output;
  }

  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startData the meeting's start date
   * @param endData the meeting's end date
   * @return if success, true will be returned
   */
  bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator)
  {
   Date start(startDate);
   Date end(endDate);
   if( !Date::isValid(start) || !Date::isValid(end) )
   {
    return false;
   }
   std::list<Meeting> temp;
   temp = m_storage->queryMeeting( [&](const Meeting &a)
         {
          if( a.getTitle() == title )
          {
           return true;
          }
          return false;
         } );
   
   if( temp.size() != 0 )
   {
    return false;
   }

   
   if( start >= end )
   {
    return false;
   }

   std::vector<string> all_man = participator;
   all_man.push_back(userName);

   std::list<User> u = m_storage->queryUser([&](const User& a)
       {
        for(auto i = all_man.begin(); i != all_man.end(); i++ )
        {
         if( (*i) == a.getName() )
         {
          return true;
         }
        }
        return false;
       });
   if( u.size() != all_man.size() )
   {
    return false;
   }

   for( auto i = participator.begin(); i != participator.end(); i++ )
   {
    if( (*i) == userName )
    {
     return false;
    }
   }

   
   for( auto i = all_man.begin(); i != all_man.end(); i++ )
   {
    //std::list<Meeting> m = meetingQuery( (*i), startDate, endDate );

    std::list<Meeting> m = m_storage->queryMeeting( [&](const Meeting& a)
          {
           if (a.getSponsor() == (*i) || a.isParticipator((*i)))
           {
            if (( end <= a.getStartDate() ) || ( start >= a.getEndDate()))
            {
            	return false;
            }
           	return true;
           }
           else
           {
           	return false;
           }
           //return false;
          });
    if( m.size() != 0 )
    {
     return false;
    }
   }


   Meeting m(userName, participator,start,end,title);
   m_storage->createMeeting(m);
   return true;
  }

  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator)
  {
   std::list<User> u = m_storage->queryUser([&](const User& a)
         {
          if( a.getName() == participator )
          {
           return true;
          }
          return false;
         });
   if( u.size() != 1 )
   {
    return false;
   }
   
    

   int flag = m_storage->updateMeeting([&](const Meeting &a)
        {
         if( a.getSponsor() == userName && a.getTitle() == title )
         {
          Date start = a.getStartDate();
          Date end = a.getEndDate();
          string startDate = Date::dateToString(start);
          string endDate = Date::dateToString(end);

          std::list<Meeting> m = m_storage->queryMeeting( [&](const Meeting& a)
          {
           if (a.getSponsor() == participator || a.isParticipator(participator))
           {
            if (( end <= a.getStartDate() ) || ( start >= a.getEndDate()))
            {
            	return false;
            }
           	return true;
           }
           else
           {
           	return false;
           }
           //return false;
          });


          if( m.size() != 0 )
          {
           return false;
          }
          return true;
         }
         return false;
        },
        [&](Meeting& b)
        {
         b.addParticipator(participator);
        });
   if( flag == 0 )
   {
    return false;
   }
   return true;
  }

  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator)
  {
   int flag = m_storage->updateMeeting([&](const Meeting &a)
        {
         if( a.getSponsor() == userName && a.getTitle() == title && a.isParticipator(participator) )
         {
          return true;
         }
         return false;
        },
        [&](Meeting& b)
        {
         b.removeParticipator(participator);

        });

   m_storage->deleteMeeting( [&](const Meeting& a)
   					{
   						if( a.getParticipator().size() == 0 )
   						{
   							return true;
   						}
   						return false;
   					});

   if( flag == 0 )
   {
    return false;
   }
   return true;
  }

  /**
   * quit from a meeting
   * @param userName the current userName. no need to be the sponsor
   * @param title the meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::quitMeeting(const std::string &userName, const std::string &title)
  {
  	//cout << "hello" << endl;
   int flag = m_storage->updateMeeting( [&](const Meeting& a)
        {
         if( a.getTitle() == title && a.isParticipator(userName) )
         {
         	//cout << "hello" << endl;
          return true;
         }
         return false;
        },
        [&](Meeting& b)
        {
        	//cout << "hello" << endl;
         
         	//cout << "hello" << endl;
          b.removeParticipator(userName);
         
        });

   m_storage->deleteMeeting( [&](const Meeting& a)
   					{
   						if( a.getParticipator().size() == 0 )
   						{
   							return true;
   						}
   						return false;
   					});
   //cout << "hello" << endl;
   if( flag == 0 )
   {
    return false;
   }
   return true;
  }

  /**
   * search a meeting by username and title
   * @param uesrName the sponsor's userName
   * @param title the meeting's title
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const
  {
   std::list<Meeting> output;
   output = m_storage->queryMeeting( [&](const Meeting& a)
       {
        if( (a.getSponsor() == userName || a.isParticipator(userName)) && a.getTitle() == title )
        {
         return true;
        }
        return false;
       });
   return output;
  }
  /**
   * search a meeting by username, time interval
   * @param uesrName the sponsor's userName
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const
  {
   Date start(startDate);
   Date end(endDate);
   if( !Date::isValid(start) || !Date::isValid(end) )
   {
   	std::list<Meeting> t;
   	return t;
   }

   if(start > end)
   {
    std::list<Meeting> m;
    return m;
   }
   std::list<Meeting> output;
   output = m_storage->queryMeeting( [&](const Meeting& a)
          {
           if ((a.getSponsor() == userName || a.isParticipator(userName)) )
           {
            if (( end < a.getStartDate() ) || ( start > a.getEndDate() ) )
            {
            	return false;
            }
           	return true;
           }
           else
           {
           	return false;
           }
           //return false;
          });
   return output;
  }

  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const
  {
   std::list<Meeting> output;
   output = m_storage->queryMeeting( [&](const Meeting a)
          {
           if( a.getSponsor() == userName || a.isParticipator(userName) )
           {
            return true;
           }
            return false;
           
          });
   return output;
  }

  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const
  {
   std::list<Meeting> output;
   output = m_storage->queryMeeting( [&](const Meeting a)
          {
           if( a.getSponsor() == userName )
           {
            return true;
           }
           return false;
          });
   return output;
  }

  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const
  {
   std::list<Meeting> output;
   output = m_storage->queryMeeting( [&](const Meeting &a)
          {
           /*std::vector<string> par = a.getParticipator();
           for( auto i = par.begin(); i != par.end(); i++ )
           {
            if( (*i) == userName ) // as a participator but not a sponsor
            {
             return true;
            }
           }
           return false;*/
           if( a.isParticipator(userName) )
           {
            return true;
           }
           return false;
          });
   return output;
  }

  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title)
  {
   int flag = m_storage->deleteMeeting( [&](const Meeting a)
          {
           if(a.getSponsor() == userName && a.getTitle() == title )
           {
            return true;
           }
           return false;
          } );
   if( flag == 0 ) //delete nothing
   {
    return false;
   }
   return true;
  }

  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
  bool AgendaService::deleteAllMeetings(const std::string &userName)
  {
   m_storage->deleteMeeting( [&](const Meeting &a){return a.getSponsor() == userName;} );
   return true;
  }

  /**
   * start Agenda service and connect to storage
   */
  void AgendaService::startAgenda(void)
  {
   m_storage = Storage::getInstance();
  }

  /**
   * quit Agenda service
   */
  void AgendaService::quitAgenda(void)
  {
   m_storage->sync();
  }