#include "Hyper/oldCut.h"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>

int getOldCut(const char* f)
{


std::ifstream file(f);

std::string line;
int value=0;

  while(std::getline(file, line))

  {
          std::vector <std::string> field;
          split(field,line,boost::is_any_of(" "),boost::token_compress_on);
          if(field[1]=="Hyperedge" && field[2]=="Cut:" ){
             int newValue = atoi(field[3].c_str());
             value = newValue;
          }

  }

return value;
}
