#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

const int DEFAULT_MONTH = 1;
const int DEFAULT_DAY = 1;
const int DEFAULT_YEAR = 1900;

class Date {
private:
  int month;
  int day;
  int year;

  void resetToDefault();
  bool isLeapYear(int y) const;
  int lastDay(int m, int y) const;
  std::string getMonthName(int m) const;

  void incrementDate();
  void decrementDate();

  long toDays() const;

public:
  Date(int m = DEFAULT_MONTH, int d = DEFAULT_DAY, int y = DEFAULT_YEAR);

  void setDate(int m, int d, int y);
  int getMonth() const { return month; }
  int getDay() const { return day; }
  int getYear() const { return year; }

  std::string printFormat1() const;
  std::string printFormat2() const;
  std::string printFormat3() const;

  Date &operator++();

  Date operator++(int);

  Date &operator--();

  Date operator--(int);

  long operator-(const Date &other) const;

  friend ostream &operator<<(ostream &os, const Date &d);
  friend istream &operator>>(istream &is, Date &d);
};


void Date::resetToDefault() {
  month = DEFAULT_MONTH;
  day = DEFAULT_DAY;
  year = DEFAULT_YEAR;
}

bool Date::isLeapYear(int y) const {
  if (y < 1)
    return false;
  return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

int Date::lastDay(int m, int y) const {
  if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) {
    return 31;
  } else if (m == 4 || m == 6 || m == 9 || m == 11) {
    return 30;
  } else if (m == 2) {
    return isLeapYear(y) ? 29 : 28;
  }
  return 0; 
}

std::string Date::getMonthName(int m) const {
  switch (m) {
  case 1:
    return "January";
  case 2:
    return "February";
  case 3:
    return "March";
  case 4:
    return "April";
  case 5:
    return "May";
  case 6:
    return "June";
  case 7:
    return "July";
  case 8:
    return "August";
  case 9:
    return "September";
  case 10:
    return "October";
  case 11:
    return "November";
  case 12:
    return "December";
  default:
    return "Invalid Month";
  }
}


long Date::toDays() const {
  long days = 0;

  for (int y = DEFAULT_YEAR; y < year; ++y) {
    days += isLeapYear(y) ? 366 : 365;
  }

  const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  for (int m = 1; m < month; ++m) {
    if (m == 2 && isLeapYear(year)) {
      days += 29;
    } else {
      days += daysInMonth[m];
    }
  }

  days += day - 1;

  return days;
}


void Date::incrementDate() {
  day++;
  int maxDay = lastDay(month, year);

  if (day > maxDay) {
    day = 1;
    month++;

    
    if (month > 12) {
      month = 1;
      year++;
    }
  }
}

void Date::decrementDate() {
  day--;

  if (day < 1) {
    month--;

    if (month < 1) {
      month = 12;
      year--;
    }
    day = lastDay(month, year);
  }
}

Date::Date(int m, int d, int y) {
  resetToDefault();
  setDate(m, d, y);
}

void Date::setDate(int m, int d, int y) {
  
  if (m < 1 || m > 12) {
    cout << "Error: Month invalid (" << m
         << "). Setting date to default 1/1/1900.\n";
    resetToDefault();
    return;
  }

  if (y < 1) {
    cout << "Error: Year invalid (" << y
         << "). Setting date to default 1/1/1900.\n";
    resetToDefault();
    return;
  }

  int maxDay = lastDay(m, y);

  if (d < 1 || d > maxDay) {
    cout << "Error: Day invalid (" << d << "). Day must be 1-" << maxDay
         << " for " << getMonthName(m) << " " << y
         << ". Setting date to default 1/1/1900.\n";
    resetToDefault();
    return;
  }

  month = m;
  day = d;
  year = y;
}

Date &Date::operator++() {
  incrementDate();
  return *this;
}

Date Date::operator++(int) {
  Date temp = *this;
  incrementDate();   
  return temp;       
}

Date &Date::operator--() {
  decrementDate();
  return *this;
}


Date Date::operator--(int) {
  Date temp = *this; 
  decrementDate();   
  return temp;      
}


long Date::operator-(const Date &other) const {
 
  return std::abs(this->toDays() - other.toDays());
}
ostream &operator<<(ostream &os, const Date &d) {
  
  os << d.printFormat2();
  return os;
}

istream &operator>>(istream &is, Date &d) {
  int m, day, y;

  cout << "\nEnter Date (Format M D Y, e.g., 5 15 2025): ";

  if (!(is >> m >> day >> y)) {
   
    is.clear();
    is.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input format detected. Date set to default.\n";
    d.resetToDefault();
  } else {
    
    d.setDate(m, day, y);
  }
  return is;
}

std::string Date::printFormat1() const {
  stringstream ss;
  ss << month << "/" << day << "/" << year;
  return ss.str();
}

std::string Date::printFormat2() const {
  stringstream ss;
  ss << getMonthName(month) << " " << day << ", " << year;
  return ss.str();
}

std::string Date::printFormat3() const {
  stringstream ss;
  ss << day << " " << getMonthName(month) << " " << year;
  return ss.str();
}

void runTests() {
  cout << "========================================================\n";
  cout << "           Date Class Operator Overload Tests\n";
  cout << "========================================================\n\n";

  Date d1, d2; 
  cout << "--- Test 1: Default Constructor (d1) ---\n";
  d1 = Date();
  cout << "Result (Format 1): " << d1.printFormat1() << endl;
  cout << "Expected: 1/1/1900\n\n";
  cout << "--- Test 2: Parameterized Constructor (d2) ---\n";
  d2 = Date(5, 15, 2025);
  cout << "Result (Format 2): " << d2.printFormat2() << endl;
  cout << "Expected: May 15, 2025\n\n";
  cout << "--- Test 3: setDate() with Valid Date ---\n";
  d1.setDate(10, 20, 2023);
  cout << "Result (10/20/2023, Format 3): " << d1.printFormat3() << endl;
  cout << "Expected: 20 October 2023\n\n";
  cout << "--- Test 4: setDate() with Invalid Month/Day (13/45/2018) ---\n";
  d1.setDate(13, 45, 2018); 
  cout << "Result: " << d1.printFormat1() << endl;
  cout << "Expected: 1/1/1900\n\n";
  cout << "--- Test 5: setDate() with Invalid Day for Month (4/31/2000) ---\n";
  d1.setDate(4, 31, 2000); 
  cout << "Result: " << d1.printFormat1() << endl;
  cout << "Expected: 1/1/1900\n\n";
    
  cout << "--- Test 6: setDate() with Invalid Leap Day (2/29/2009) ---\n";
  d1.setDate(2, 29, 2009); 
  cout << "Result: " << d1.printFormat1() << endl;
  cout << "Expected: 1/1/1900\n\n";

  cout << "--- Test 7: Subtraction Operator (-) Same Month (4/18/2014 - "
          "4/10/2014) ---\n";
  d1.setDate(4, 10, 2014); 
  d2.setDate(4, 18, 2014); 
  cout << "Difference: " << d2 - d1 << " days" << endl;
  cout << "Expected: 8 days\n\n";

  cout << "--- Test 8: Subtraction Operator (-) Across Years (2/2/2006 - "
          "11/10/2003) ---\n";
  d1.setDate(2, 2, 2006);
  d2.setDate(11, 10, 2003);
  cout << "Difference: " << d1 - d2 << " days" << endl;
  cout << "Expected: 815 days\n\n";

  cout << "--- Test 9: Prefix ++ and -- (Leap Year Rollover) ---\n";
  d1.setDate(2, 29, 2008);
  cout << "Initial (2/29/2008): " << d1.printFormat1() << endl;
  ++d1;
  cout << "After ++d1: " << d1.printFormat1() << " (Expected: 3/1/2008)"
       << endl;
  --d1;
  cout << "After --d1: " << d1.printFormat1() << " (Expected: 2/29/2008)\n\n";

  cout << "--- Test 10: Postfix ++ and -- (Leap Year Rollover) ---\n";
  d1.setDate(2, 29, 2008);
  cout << "Initial (2/29/2008): " << d1.printFormat1() << endl;
  d1++; 
  cout << "After d1++: " << d1.printFormat1() << " (Expected: 3/1/2008)"
       << endl;
  d1--; 
  cout << "After d1-- (Actual state): " << d1.printFormat1()
       << " (Expected: 2/29/2008)\n\n";

  cout << "--- Test 11: Postfix ++ and -- (Year Rollover) ---\n";
  d1.setDate(12, 31, 2024);
  cout << "Initial (12/31/2024): " << d1.printFormat1() << endl;
  d1++; 
  cout << "After d1++: " << d1.printFormat1() << " (Expected: 1/1/2025)"
       << endl;
  d1--; 
  cout << "After d1-- (Actual state): " << d1.printFormat1()
       << " (Expected: 12/31/2024)\n\n";
    
  cout << "--- Test 12: Prefix ++ and -- (Year Rollover) ---\n";
  d1.setDate(12, 31, 2024);
  cout << "Initial (12/31/2024): " << d1.printFormat1() << endl;
  ++d1;
  cout << "After ++d1: " << d1.printFormat1() << " (Expected: 1/1/2025)"
       << endl;
  --d1;
  cout << "After --d1: " << d1.printFormat1() << " (Expected: 12/31/2024)\n\n";

  cout << "--- Test 13: Stream Extraction Operator (cin >> d1) ---\n";
 
  cin >> d1;

  cout << "\n--- Test 14: Stream Insertion Operator (cout << d1) ---\n";
  cout << "Displaying entered date: " << d1 << endl;
  cout << "Expected Format: Month DD, YYYY (e.g., April 18, 2018)\n\n";

  cout << "========================================================\n";
  cout << "Test program finished.\n";
}

int main() {
  
  cout << fixed << setprecision(0);

  runTests();
  return 0;
}
