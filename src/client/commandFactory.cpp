#include <ctype.h>
#include "commandFactory.hpp"

void _lowercase(string &str) {
   for (auto &c: str) {
      c = tolower(c);
   }
}

CommandFactory::CommandFactory() {
   addCommand();
}

ICommand *CommandFactory::getCommandProcessor(string &cmd) {
   ICommand *pProcessor = NULL;
   _lowercase(cmd);
   COMMAND_MAP::iterator iter;
   iter = _cmdMap.find(cmd);
   if (iter != _cmdMap.end()) {
      pProcessor = iter->second;
   }
   return pProcessor;
}