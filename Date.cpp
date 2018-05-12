#include "Date.hpp"
#include <string>
#include <sstream>
using namespace std;

  int daysOfMonthy( int y, int m )
  {
    int month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if( m != 2 )
    {
      return month[ m-1 ];
    }
    else
    {
      if( y % 4 == 0 && y % 100 != 0 || y % 400 == 0 )
        return 29;
      else
      {
        return 28;
      }
    }
  }


  Date::Date()
  {
    this->m_year = 0;
    this->m_month = 0;
    this->m_day = 0;
    this->m_hour = 0;
    this->m_minute = 0;
  }

  /**
  * @brief constructor with arguments
  */
  Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
  {
    this->m_year = t_year;
    this->m_month = t_month;
    this->m_day = t_day;
    this->m_hour = t_hour;
    this->m_minute = t_minute;
  }

  /**
  * @brief constructor with a string
  */
  Date::Date(const std::string &dateString)
  {
    *this = stringToDate( dateString );
  }
  /**
  * @brief return the year of a Date
  * @return   a integer indicate the year of a date
  */
  int Date::getYear(void) const
  {
    return this->m_year;
  }

  /**
  * @brief set the year of a date
  * @param a integer indicate the new year of a date
  */
  void Date::setYear(const int t_year)
  {
    this->m_year = t_year;
  }

  /**
  * @brief return the month of a Date
  * @return   a integer indicate the month of a date
  */
  int Date::getMonth(void) const
  {
    return this->m_month;
  }

  /**
  * @brief set the month of a date
  * @param a integer indicate the new month of a date
  */
  void Date::setMonth(const int t_month)
  {
    this->m_month = t_month;
  }

  /**
  * @brief return the day of a Date
  * @return   a integer indicate the day of a date
  */
  int Date::getDay(void) const
  {
    return this->m_day;
  }

  /**
  * @brief set the day of a date
  * @param a integer indicate the new day of a date
  */
  void Date::setDay(const int t_day)
  {
    this->m_day = t_day;
  }

  /**
  * @brief return the hour of a Date
  * @return   a integer indicate the hour of a date
  */
  int Date::getHour(void) const
  {
    return this->m_hour;
  }

  /**
  * @brief set the hour of a date
  * @param a integer indicate the new hour of a date
  */
  void Date::setHour(const int t_hour)
  {
    this->m_hour = t_hour;
  }

  /**
  * @brief return the minute of a Date
  * @return   a integer indicate the minute of a date
  */
  int Date::getMinute(void) const
  {
    return this->m_minute;
  }

  /**
  * @brief set the minute of a date
  * @param a integer indicate the new minute of a date
  */
  void Date::setMinute(const int t_minute)
  {
      this->m_minute = t_minute;
  }

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  */
  bool Date::isValid(const Date &t_date)
  {
    if( t_date.m_year < 1000 || t_date.m_year > 9999 )
    {
      return false;
    }
    else
    {
      if( t_date.m_month <= 0 || t_date.m_month > 12 )
      {
        return false;
      }
      else
      {
        if( t_date.m_day <= 0 || t_date.m_day > daysOfMonthy( t_date.m_year, t_date.m_month ) )
        {
          return false;
        }
        else
        {
          if( t_date.m_hour < 0 || t_date.m_hour > 23 )
          {
              return false;
          }
          else
          {
            if( t_date.m_minute < 0 || t_date.m_minute > 59 )
            {
              return false;
            }
          }
        }
      }
    }
    
    return true;
  }

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
  Date Date::stringToDate(const std::string &t_dateString)
  {
    int flag = 0;
    for( int i = 0; i < t_dateString.size(); i++ )
    {
      if( i == 4 || i == 7 || i == 10 || i == 13 )
      {
        continue;
      }
      else
      {
        if( t_dateString[ i ] < '0' || t_dateString[ i ] > '9' )
        {
          flag = 1;
          break;
        }
      }
    }
    if(t_dateString[4] != '-' || t_dateString[7] != '-' || t_dateString[10] !='/' || t_dateString[13] !=':' )
    {
      flag = 1;
    }
    if( t_dateString.size() != 16 )
    {
      flag = 1;
    }
    if( flag == 1 )
    {
      Date result( 0, 0, 0, 0, 0 );
      return result;
    }
    else
    {
      int y = 0, mon = 0, d = 0, h = 0, min = 0;
      char junk = 0;
      stringstream temp;
      temp << t_dateString;
      temp >> y >> junk >> mon >> junk >> d >> junk >> h >> junk >> min;

      Date result( y, mon, d, h, min );
      return result;
    }
    
    

  }

  /**
  * @brief convert a date to string, if the format is not correct return
  * 0000-00-00/00:00
  */
  std::string Date::dateToString(const Date &t_date)
  {
    string temp;
    if( isValid( t_date ) == false )
    {
      temp = "0000-00-00/00:00";
    }
    else
    {
      char y[ 5 ] = {0};
      sprintf( y, "%4d-", t_date.m_year );
      temp.append( y );

      char mon[ 3 ] = {0};
      sprintf( mon, "%02d-", t_date.m_month );
      temp.append( mon );

      char d[ 3 ] = {0};
      sprintf( d, "%02d/", t_date.m_day );
      temp.append( d );

      char h[ 3 ] = {0};
      sprintf( h, "%02d:", t_date.m_hour );
      temp.append( h );

      char min[ 2 ] = {0};
      sprintf( min, "%02d", t_date.m_minute );
      temp.append( min );
    }

    return temp;
  }

  /**
  *  @brief overload the assign operator
  */
  Date &Date::operator=(const Date &t_date)
  {
    this->m_year = t_date.m_year;
    this->m_month = t_date.m_month;
    this->m_day = t_date.m_day;
    this->m_hour = t_date.m_hour;
    this->m_minute = t_date.m_minute;

    return *this;
  }

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
  bool Date::operator==(const Date &t_date) const
  {
    Date d1(*this);
    if( d1.dateToString(d1) == t_date.dateToString(t_date) )
    {
      return true;
    }
    return false;
    /*if( this->m_year != t_date.m_year || this->m_month != t_date.m_month || this->m_day != t_date.m_day || this->m_hour != t_date.m_hour || this->m_minute != t_date.m_minute )
    {
      return false;
    }
    return true;*/
  }

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
  bool Date::operator>(const Date &t_date) const
  {
    Date d1(*this);
    if( d1.dateToString(d1) > t_date.dateToString(t_date) )
    {
      return true;
    }
    return false;
    /*if( this->m_year > t_date.m_year )
    {
      return true;
    }
    else
    {
      if( this->m_year == t_date.m_year )
      {
        if( this->m_month > t_date.m_month )
        {
          return true;
        }
        else
        {
          if( this->m_month == t_date.m_month )
          {
            if( this->m_day > t_date.m_day )
            {
              return true;
            }
            else
            {
              if( this->m_day == t_date.m_day )
              {
                if( this->m_hour > t_date.m_hour )
                {
                  return true;
                }
                else
                {
                  if( this->m_hour == t_date.m_hour )
                  {
                    if( this->m_minute > t_date.m_minute )
                    {
                      return true;
                    }
                    else
                    {
                      return false;
                    }
                  }
                  else
                  {
                    return false;
                  }
                }
              }
            }
          }
          else
          {
            return false;
          }
        }
      }
      else
      {
        return false;
      }
    }*/
  }

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
  bool Date::operator<(const Date &t_date) const
  {
    if( *this > t_date || *this == t_date )
    {
      return false;
    }
    return true;
  }

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
  bool Date::operator>=(const Date &t_date) const
  {
    if( *this < t_date )
    {
      return false;
    }
    return true;
  }

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool Date::operator<=(const Date &t_date) const
  {
    if( *this > t_date )
    {
      return false;
    }
    return true;
  }

