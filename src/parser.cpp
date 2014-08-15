#include "Hyper/parser.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <vector>


//get non-zero entries, which equals line number of the file - 1
int getNonzeroEntries(const char* f)
{
   std::ifstream file(f);

   std::string line;
   getline(file, line);
   std::vector <std::string> field;
   split(field,line,boost::is_any_of(" "));
   int nonzeroEntries = atoi(field[3].c_str());
   return nonzeroEntries;

}


//parser the file, read the first two column(vertice index ad edge index)
//change string to int, store them into a matrix
Eigen::MatrixXd parser(const char* f)
{
  std::ifstream file(f);
  std::string line;
  int lineNumber=getNonzeroEntries(f);

  Eigen::MatrixXd m(lineNumber+2,2);
  m(0,0) = lineNumber;
  m(0,1) = 2;

  getline(file, line);
  std::vector <std::string> field;
  split(field,line,boost::is_any_of(" "));
  int verticeNumber = atoi(field[2].c_str());
  int edgeNumber = atoi(field[2].c_str());

  m(1,0) = verticeNumber;
  m(1,1) = edgeNumber;

  int i = 2;

  while(std::getline(file, line))

  {
        std::vector <std::string> field;
          split(field,line,boost::is_any_of(" "));
          int verticeIndex = atoi(field[1].c_str());
            //the vertice index and edge index begin with 0;
          int edgeIndex = atoi(field[2].c_str());
          m(i,0)=verticeIndex-1;
          m(i,1)=edgeIndex-1;

          ++i;

  }

  return m;

}







//print weight
void printWeight(const std::map <int, double>& myMap)
{
  for (std::map<int,double>::const_iterator it=myMap.begin(); it!=myMap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

}


//generate incidenceMatrix, edge--vertice Matrix
Eigen::SparseMatrix<int,Eigen::RowMajor> incindenceMatrix(const Eigen::MatrixXd& m)
{
   //m(1,0) is the number of vertice, m(1,1) is number of edge;
   typedef Eigen::Triplet<int> T;
   std::vector<T> tripletList;
   tripletList.reserve(0.001*m(1,1));
   int loopCount = m(0,0);
   for (int i=2; i<loopCount+2; ++i){
     //std::cout<<m(i,0)<<"  "<<m(i,1)<<std::endl;
       tripletList.push_back(T(m(i,0),m(i,1),1));
   }

   Eigen::SparseMatrix<int,Eigen::RowMajor> inc(m(1,0),m(1,1));
   inc.setFromTriplets(tripletList.begin(), tripletList.end());
   //inc = inc*(inc.transpose());
   inc.makeCompressed();
   return inc;
}


Eigen::SparseMatrix<int,Eigen::ColMajor> checkIncindenceMatrix(Eigen::SparseMatrix<int,Eigen::RowMajor> m)
{
  Eigen::SparseMatrix<int,Eigen::ColMajor> n(m);

  for (int k=0; k<n.outerSize(); ++k){
    if(n.innerVector(k).sum() == 0){

       n.insert(k,k) = 1;
       }
    }

return n;


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
