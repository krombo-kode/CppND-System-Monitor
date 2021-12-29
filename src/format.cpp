#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string ZeroPadNumber(int num){
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << num;
    return ss.str();
}

string Format::ElapsedTime(long seconds) { 
    int hh, mm, ss;
    hh = seconds/3600;
    mm = (seconds-(hh*3600))/60;
    ss = seconds % 60;
    return ZeroPadNumber(hh) + ":" + ZeroPadNumber(mm) + ":" + ZeroPadNumber(ss);
    }
    
