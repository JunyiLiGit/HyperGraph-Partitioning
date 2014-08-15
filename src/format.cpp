#include "Hyper/format.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

//formate the output, so hMETIS can use the output file
std::string formatFile(Eigen::MatrixXd inc,Eigen::MatrixXd parser,std::map<int, double> mymap,const char* c)
{

   int rowSize = inc.rows();
   int colSize = inc.cols();
   std::string sRow = std::to_string(rowSize);
   char const *pchar1 = sRow.c_str();
   std::string sCol = std::to_string(colSize);
   char const *pchar2 = sCol.c_str();
   const char *middle = "_";
   const char *last;
   char result[100];
   //please change this path to be your own path
   const char *path = "/home/junyil/hypergraph/newVersion/data/";
   strcpy(result,path);
   strcat(result,pchar1); // copy string one into the result.
   strcat(result,middle);
   strcat(result,pchar2);
   strcat(result,c);
   std::ofstream outfile;

   outfile.open(result);
   outfile<<parser(1,1)<<" "<<parser(1,0)<<" "<<1<<std::endl;
  for (std::map<int,double>::iterator it=mymap.begin(); it!=mymap.end(); ++it){

      outfile<<it->second<<" ";
    for(int j=0; j<inc.rows();++j){
      if(inc(j,it->first)){
      outfile<<j+1<<" ";
    }
    }
     outfile<<std::endl;
  }
  std::string str(result);
  //std::cout<<str<<std::endl;
  return str;
}
