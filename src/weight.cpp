#include "Hyper/weight.h"
#include "Eigen/LU"
#include <iostream>

//get new hypergraph weights, store them into a map
std::map <int, double> getNewWeight(Eigen::MatrixXd m)
{
  std::map<int, double> myMap;
  for(int i=0; i<m.rows();++i){
    int key = i;
    double value = round(1/m(i,0)*1000);
    myMap[key] = value;
  }

  return myMap;
}



//read the third column which is edge weight. store the values into a vactor
std::map <int, double> getOldWeight(Eigen::SparseMatrix<int,Eigen::ColMajor> inc)
{
       Eigen::MatrixXd denseInc = Eigen::MatrixXd(inc);
       std::map<int,double> myMap;
       for(int i=0; i<denseInc.cols();++i){
         int key = i;
         double value = 1;
         myMap[key] = value;
       }



  return myMap;
}
