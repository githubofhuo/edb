#ifndef _EDB_COMMANDFACTORY_HPP__
#define _EDB_COMMANDFACTORY_HPP__

#include "command.hpp"
#define COMMAND_BEGIN void CommandFactory::addCommand() {
#define COMMAND_END }
#define COMMAND_ADD(cmdName, cmdClass) {                 \
   ICommand *pObj = new cmdClass();                      \
   std::string str = cmdName;                            \
   _cmdMap.insert(COMMAND_MAP::value_type(str, pObj));   \ 
}                                                        

class CommandFactory {
   typedef std::map<std::string, ICommand*> COMMAND_MAP;
public:
   CommandFactory();
   ~CommandFactory(){}
   void addCommand();
   ICommand *getCommandProcessor(string &cmd);
private:
   COMMAND_MAP _cmdMap;
};

#endif