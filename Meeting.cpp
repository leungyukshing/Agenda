#include "Meeting.hpp"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

  /**
  *   @brief constructor with argument
  */
  Meeting::Meeting(const std::string &t_sponsor,
          const std::vector<std::string> &t_participator,
          const Date &t_startTime, const Date &t_endTime,
          const std::string &t_title)
  {
    this->m_sponsor = t_sponsor;
    this->m_participators = t_participator;
    this->m_startDate = t_startTime;
    this->m_endDate = t_endTime;
    this->m_title = t_title;
  }

  /**
  * @brief copy constructor of left value
  */
  Meeting::Meeting(const Meeting &t_meeting)
  {
    this->m_sponsor = t_meeting.m_sponsor;
    this->m_participators = t_meeting.m_participators;
    this->m_startDate = t_meeting.m_startDate;
    this->m_endDate = t_meeting.m_endDate;
    this->m_title = t_meeting.m_title;
  }

  /**
  *   @brief get the meeting's sponsor
  *   @return a string indicate sponsor
  */
  std::string Meeting::getSponsor(void) const
  {
    return this->m_sponsor;
  }

  /**
  * @brief set the sponsor of a meeting
  * @param  the new sponsor string
  */
  void Meeting::setSponsor(const std::string &t_sponsor)
  {
    this->m_sponsor = t_sponsor;
  }

  /**
  * @brief  get the participators of a meeting
  * @return return a string vector indicate participators
  */
  std::vector<std::string> Meeting::getParticipator(void) const
  {
    return this->m_participators;
  }

  /**
  *   @brief set the new participators of a meeting
  *   @param the new participators vector
  */
  void Meeting::setParticipator(const std::vector<std::string> &t_participators)
  {
    this->m_participators = t_participators;
  }

  /**
  * @brief add a new participator to the meeting
  * @param the new participator
  */
  void Meeting::addParticipator(const std::string &t_participator)
  {
    if( isParticipator(t_participator) )
    {
      return;
    }
    
    this->m_participators.push_back( t_participator );
  }

  /**
  * @brief remove a participator of the meeting
  * @param the participator to be removed
  */
  void Meeting::removeParticipator(const std::string &t_participator)
  {
    if( m_participators.size() == 0 )
    {
      return;
    }
    for( auto it = this->m_participators.begin(); it != this->m_participators.end(); it++ )
    {
      if( (*it) == t_participator )
      {
         this->m_participators.erase( it );
         break;
      }
    }
  }

  /**
  * @brief get the startDate of a meeting
  * @return return a string indicate startDate
  */
  Date Meeting::getStartDate(void) const
  {
    return this->m_startDate;
  }

  /**
  * @brief  set the startDate of a meeting
  * @param  the new startdate of a meeting
  */
  void Meeting::setStartDate(const Date &t_startTime)
  {
    this->m_startDate = t_startTime;
  }

  /**
  * @brief get the endDate of a meeting
  * @return a date indicate the endDate
  */
  Date Meeting::getEndDate(void) const
  {
    return this->m_endDate;
  }

  /**
  * @brief  set the endDate of a meeting
  * @param  the new enddate of a meeting
  */
  void Meeting::setEndDate(const Date &t_endTime)
  {
    this->m_endDate = t_endTime;
  }

  /**
  * @brief get the title of a meeting
  * @return a date title the endDate
  */
  std::string Meeting::getTitle(void) const
  {
    return this->m_title;
  }

  /**
  * @brief  set the title of a meeting
  * @param  the new title of a meeting
  */
  void Meeting::setTitle(const std::string &t_title)
  {
    this->m_title = t_title;
  }

  /**
  * @brief check if the user take part in this meeting
  * @param t_username the source username
  * @return if the user take part in this meeting
  */
  bool Meeting::isParticipator(const std::string &t_username) const
  {
    //cout <<"1" << endl;
    if( m_participators.size() == 0 )
    {
      return false;
    }
    for( auto it = this->m_participators.begin(); it != this->m_participators.end(); it++ )
    {
      if( *it == t_username )
      {
        return true;
      }
    }

    return false;
  }