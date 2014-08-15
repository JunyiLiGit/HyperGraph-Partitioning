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
          //std::cout<<it.row()<<std::endl;
        }
/*
        for (std::map<int,int>::iterator it=edgeClusterMap.begin(); it!=edgeClusterMap.end(); ++it){
          std::cout << it->first << " => " << it->second << '\n';
        }
*/
//        std::cout<<"size is: "<<edgeClusterMap.size()-1<<std::endl;
        cutSum += edgeClusterMap.size()-1;


    }

    //std::cout<<cutSum<<std::endl;

 return cutSum;

}

/*

int main()
{
  std::map<int,int> myMap = createVerticeClusterMap("782After.part.8");
  printMap(myMap);
  Eigen::SparseMatrix<int, Eigen::ColMajor> A(7,5);
  A.insert(0,0)=1;
  A.insert(0,1)=1;
  A.insert(1,0)=1;
  A.insert(1,3)=1;
  A.insert(2,3)=1;
  A.insert(3,2)=1;
  A.insert(3,3)=1;
  A.insert(4,1)=1;
  A.insert(4,2)=1;
  A.insert(5,1)=1;
  A.insert(5,2)=1;
  A.insert(6,1)=1;
  caculateCut(myMap, A);

}
*/
