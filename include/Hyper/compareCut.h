#include "Eigen/Sparse"
#include <iostream>
#include <map>

std::map<int,int> createVerticeClusterMap(const char *f);
void printMap(const std::map <int, int>& myMap);
int caculateCut(std::map<int,int> myMap, Eigen::SparseMatrix<int,Eigen::RowMajor> inc);
