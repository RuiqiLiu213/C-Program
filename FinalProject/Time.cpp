#include "Time.h"

void DatePlusDays(struct tm* date, int days)
{
    const time_t ONE_DAY = 24 * 60 * 60;

    // Seconds since start of epoch
    time_t date_seconds = mktime(date) + (days * ONE_DAY);

    // Update caller's date
    // Use localtime because mktime converts to UTC so may change date
    *date = *localtime(&date_seconds);
}

void ANNDateStandard(string& day) {
    stringstream sin(day);
    string s1, s2, s3;
    getline(sin, s1, '-');//day
    getline(sin, s2, '-');//month
    getline(sin, s3, '-');//year
    s3 = "20" + s3; //adjust s3
    //adjust s2
    if (s2 == "JAN") s2 = "01";
    if (s2 == "FEB") s2 = "02";
    if (s2 == "MAR") s2 = "03";
    if (s2 == "APR") s2 = "04";
    if (s2 == "MAY") s2 = "05";
    if (s2 == "JUN") s2 = "06";
    if (s2 == "JUL") s2 = "07";
    if (s2 == "AUG") s2 = "08";
    if (s2 == "SEP") s2 = "09";
    if (s2 == "OCT") s2 = "10";
    if (s2 == "NOV") s2 = "11";
    if (s2 == "DEC") s2 = "12";
    //adjust s1
    if (s1.size() == 1) s1 = "0" + s1;
    day = s3 + "-" + s2 + "-" + s1;
}

void ANNDateMinusOne(string& day) {
    stringstream sin(day);
    string s1, s2, s3;
    getline(sin, s1, '-');//year
    getline(sin, s2, '-');//month
    getline(sin, s3, '-');//day

    struct tm date = { 0, 0, 12 };  // nominal time midday(arbitrary).

    // Set up the date structure
    date.tm_year = stoi(s1) - 1900; //start from 1900
    date.tm_mon = stoi(s2) - 1; //zerio index
    date.tm_mday = stoi(s3); // no zero index
    DatePlusDays(&date, -1); //minue one day

    //transfer struct back to string
    s1 = to_string(date.tm_year + 1900);
    s2 = to_string(date.tm_mon + 1);
    if (date.tm_mon + 1 < 10) { s2 = "0" + s2; }
    s3 = to_string(date.tm_mday);
    if (date.tm_mday < 10) { s3 = "0" + s3; }
    day = s1 + "-" + s2 + "-" + s3;
}