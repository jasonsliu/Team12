#ifndef LOGGING
#define LOGGING
#include <iostream>
#include <string>
#include <map>


class Logger{
public:
   static Logger* Instance();
   void add2handlerMap(std::string k, std::string v) {uri2handlerName[k] = v;}
   void incNumOfReq() {numOfReq += 1; }
   std::string get_statusPage();

private:
   Logger(){ numOfReq = 0; }  // Private so that it can  not be called
   Logger(Logger const&){};             // copy constructor is private
   Logger& operator=(Logger const&){};  // assignment operator is private
   static Logger* m_pInstance;

   std::map<std::string, std::string> uri2handlerName;
   int numOfReq;

};

#endif