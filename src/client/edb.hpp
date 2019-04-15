#ifndef _EDB_HPP__
#define _EDB_HPP__

#include "core.hpp"
#include "ossSocket.hpp"
#include "commandFactory.hpp"

const int CMD_BUFFER_SIZE = 512;
class Edb {
public:
   Edb(){}
   ~Edb(){};
   void start();
protected:
   void prompt();
private:
   void _split(const std::string &text, char delim, std::string &command, std::vector< std::string> &optionVec) ;
   int _readInput();
private:
   ossSocket      _socket;
   CommandFactory _cmdFactory;
   char           _cmdBuffer[CMD_BUFFER_SIZE];
};

#endif