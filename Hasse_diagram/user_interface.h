#ifndef _MYINT_H
#define _MYINT_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

class Interface {

    protected:
    std::string data;

    public:
    void Print();
    virtual void Input() = 0;
    virtual void Run() = 0;
    
    virtual ~Interface() {}

};

#endif