#include "Hyper/parser.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>


int getLineNumber(const char* f)
{
  int i = 0;
  std::ifstream file(f);
  std::string line;

  while(std::getline(file, line)){
    if(strncmp(&line.at(0), "%",1)!=0){
    ++i;}
  }
 return i;
}

Eigen::MatrixXd parser(const char* f)
{
  int lineNumber = getLineNumber(f);

  Eigen::MatrixXd m(lineNumber+1,2);
  std::ifstream file(f);
  std::string line;
  getline(file, line);

  int verticeKey = 0;
  int edgeKey = 0;
  std::map<int,int> verticeMap;
  std::map<int,int> edgeMap;
  int i = 0;
  int j = 0;
  int k = 2;

  while(std::getline(file, line)){
    if(strncmp(&line.at(0), "%",1)!=0){
      break;
    }

  }


  while(std::getline(file, line))

  {

        std::vector <std::string> field;
          split(field,line,boost::is_any_of(" "));
          int verticeIndex = atoi(field[0].c_str())-1;

          int edgeIndex = atoi(field[1].c_str())-1;
          if(edgeMap.find(edgeIndex)==edgeMap.end()){
          edgeMap[edgeIndex] = i;

          ++i;
        }
          if(verticeMap.find(verticeIndex)==verticeMap.end()){
          verticeMap[verticeIndex] = j;

          ++j;
        }
        m(k,1) = edgeMap[edgeIndex];
        m(k,0) = verticeMap[verticeIndex];
        ++k;

  }
  m(1,0) = j;
  m(1,1) = i;
  m(0,0) = lineNumber-1;
  m(0,1) = 2;


  return m;

}


Eigen::SparseMatrix<int,Eigen::RowMajor> incindenceMatrix(const Eigen::MatrixXd& m)
{
   //m(1,0) is the number of vertice, m(1,1) is number of edge;
   typedef Eigen::Triplet<int> T;
   std::vector<T> tripletList;
   tripletList.reserve(0.001*m(1,1));
   int loopCount = m(0,0);
   for (int i=2; i<loopCount+2; ++i){
       tripletList.push_back(T(m(i,0),m(i,1),1));
   }

   Eigen::SparseMatrix<int,Eigen::ColMajor> inc(m(1,0),m(1,1));
   inc.setFromTriplets(tripletList.begin(), tripletList.end());
   inc.makeCompressed();
   return inc;
}

//get each vertice degree
std::vector<int> getVerticeDegree(const Eigen::SparseMatrix<int,Eigen::RowMajor>& inc)
{
    std::vector<int> v;
    v.reserve(inc.rows());
   for (int k=0; k<inc.outerSize(); ++k){

        int i = 0;
      for (Eigen::SparseMatrix<int,Eigen::RowMajor>::InnerIterator it(inc,k); it; ++it)
         {
           ++i;
         }
          v.push_back(i);

     }

  return v;
}


//generate diagonal Matrix from Degree Matrix
Eigen::SparseMatrix<int,Eigen::RowMajor> getDiagonalMatrix(const std::vector<int>& v)
{
  Eigen::SparseMatrix<int,Eigen::RowMajor> diagnal(v.size(),v.size());
  diagnal.reserve(1);
  for(int i=0; i<v.size(); ++i){
     diagnal.insert(i,i) = v[i];
      }
  diagnal.makeCompressed();
  return diagnal;
}

//generate adjancentMatrix. A=H*H'-D, H is the incidence Matrix, H' is the transpose
Eigen::SparseMatrix<int,Eigen::RowMajor> adjancentMatrix(const Eigen::SparseMatrix<int,Eigen::RowMajor>& H,
                                                         const Eigen::SparseMatrix<int,Eigen::RowMajor>& transposeOfH,
                                                         const Eigen::SparseMatrix<int,Eigen::RowMajor>& D)
{
  Eigen::SparseMatrix<int,Eigen::RowMajor> adjancent(D.rows(),D.cols());
  adjancent.reserve(0.5*D.rows());
  adjancent = H*transposeOfH - D;
  adjancent.makeCompressed();
  return adjancent;

}
