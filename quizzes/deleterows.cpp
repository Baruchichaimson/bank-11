#include <iostream>
#include <string>
#include <sstream>

using namespace std;


string solution(string &s)
{
    stringstream ss(s);
    string line;
    string result;

    bool firstline = true;
    while (getline(ss,line , '\n'))
    {
        if(firstline)
        {
            result += line;
            firstline = false;
            continue;
        }

        stringstream sword(line);
        string cell;
        bool defect = false;

        while(getline(sword , cell, ','))
        {
            if(cell == "NULL")
            {
                defect = true;
                break;
            }

        }

        if (!defect)
        {
            result += "\n" + line;
        }
    }
    
    return result;
}