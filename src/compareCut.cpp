#include "Hyper/compareCut.h"
#include <fstream>

std::map<int,int> createVerticeClusterMap(const char *f)
{
  std::ifstream file(f);
  std::string line;
  std::map<int,int> myMap;
  int i=0;
  while(std::getline(file, line))

  {
      int key = i;
      int value = atoi(line.c_str());
      myMap[key] = value;
      ++i;

  }

  return myMap;
}

void printMap(const std::map <int, int>& myMap)
{
  for (std::map<int,int>::const_iterator it=myMap.begin(); it!=myMap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

}


int caculateCut(std::map<int,int> myMap, Eigen::SparseMatrix<int,Eigen::RowMajor> inc)
{
  int cutSum = 0;
  Eigen::SparseMatrix<int,Eigen::ColMajor> incMatrix = inc;
  for (int k=0; k<inc.cols(); ++k){
    std::map<int,int> edgeClusterMap;
     for (Eigen::SparseMatrix<int,Eigen::ColMajor>::InnerIterator it(incMatrix,k); it; ++it)
        {
          int key = myMap[it.row()];
          edgeClusterMap[key] = k;
        }
        if(edgeClusterMap.size()>1){
          ++ cutSum;
        }
    }
 return cutSum;

}
