#include "Hyper/algebraicDistance.h"
#include <iostream>
#include <cmath>
#include <random>
//#include <chrono>
#include <time.h>
#include <vector>

Eigen::SparseMatrix<double,Eigen::RowMajor> randomVector(int row, int col)
{
  Eigen::SparseMatrix<double,Eigen::RowMajor> p(row,col);
  p.reserve(col);
//  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
 unsigned int seed = static_cast<unsigned int>( time(NULL) ); 
 std::default_random_engine generator(seed);
  std::uniform_real_distribution<double> distribution(-0.5,0.5);
  for(int i=0; i<row;++i){
    for(int j=0;j<col; ++j){
       double number = distribution(generator);
       p.insert(i,j)=number;
    }
  }
  p.makeCompressed();
  return p;

}



std::vector<double> getRowSum(const Eigen::SparseMatrix<int,Eigen::RowMajor>& adjacencyMatrix)
{
  int rowSize = adjacencyMatrix.rows();
  std::vector<double> rowSum;
  rowSum.reserve(rowSize);
  for(int i=0; i<rowSize;++i){
     double value = adjacencyMatrix.innerVector(i).sum();
     rowSum.push_back(value);
  }

  return rowSum;

}


//update random Vectors
void updateMatrix(Eigen::SparseMatrix<double,Eigen::RowMajor>& randomMatrix,
                  const Eigen::SparseMatrix<int,Eigen::RowMajor>& adjacencyMatrix,
                  int itr)
{

  int numberOfRVector = randomMatrix.rows();
  int numberOfVertice = randomMatrix.cols();

  std::vector<double> rowSum = getRowSum(adjacencyMatrix);

  for(int i=0; i<numberOfRVector;++i){
    for(int h=0;h<itr;++h){
      Eigen::SparseMatrix<double,Eigen::RowMajor> middleValueMatrix(numberOfVertice,1);
      middleValueMatrix.reserve(1);
      for(int j = 0; j<numberOfVertice; ++j){
         Eigen::SparseMatrix<double,Eigen::RowMajor> v1(1,numberOfVertice), v2(numberOfVertice,1);
         v1.reserve(numberOfVertice);
         v2.reserve(1);
         v1 = adjacencyMatrix.cast<double>().innerVector(j);
         v2 = randomMatrix.innerVector(h).transpose();
         Eigen::SparseMatrix<double,Eigen::RowMajor> middleValue1 = v1*v2;
         double middleValue = 0;
         if(rowSum[j] != 0){
            middleValue = (*middleValue1.valuePtr())/rowSum[j];
          } else
            {
              middleValue = 1;
              }

         middleValueMatrix.insert(j,0) = middleValue;

      }

        Eigen::SparseMatrix<double,Eigen::RowMajor> right = middleValueMatrix.transpose();
        Eigen::SparseMatrix<double,Eigen::RowMajor> left = randomMatrix.innerVector(i);

        randomMatrix.innerVector(i) = 0.5*(left + right);
    }
  }
}


Eigen::MatrixXd getMaxDiffForeachEdge(const Eigen::MatrixXd& m)
{
  if(m.cols() == 1){
    return m.cwiseAbs();
  }
  else
    {

      Eigen::MatrixXd maxValueForEachRow = m.rowwise().maxCoeff();
      Eigen::MatrixXd minValueForEachRow = m.rowwise().minCoeff();
      Eigen::MatrixXd maxDiffForEachRow = maxValueForEachRow - minValueForEachRow;
      Eigen::MatrixXd maxAbsDiffForEachRow = maxDiffForEachRow.cwiseAbs();
      return maxAbsDiffForEachRow;
  }

}


Eigen::MatrixXd caculateAlgebraicDistance(const Eigen::SparseMatrix<int,Eigen::RowMajor>& incidenceMatrix,
                                          const Eigen::SparseMatrix<double,Eigen::RowMajor>& updatedMatrix)
{
  Eigen::MatrixXd inc = Eigen::MatrixXd(incidenceMatrix);
  Eigen::MatrixXd update = Eigen::MatrixXd(updatedMatrix);
  int edgeNumber = inc.cols();
  int verticeNumber = inc.rows();
  Eigen::MatrixXd algebraicDistanceForEdge(edgeNumber,1);
  Eigen::VectorXd sumOfcolsVector(edgeNumber);
  sumOfcolsVector = inc.colwise().sum();
  for(int i = 0; i<edgeNumber;++i){
    int verticeNumberForEdge = sumOfcolsVector[i];
    Eigen::MatrixXd allEdgeMatrix(update.rows(),verticeNumberForEdge);
    int g=0;
    for(int j=0;j<verticeNumber;++j){
        if(inc(j,i)){
          ++g;
          for(int v = 0; v<update.rows();++v){
              allEdgeMatrix(v,g-1) = update(v,j);
          }
        }
    }

    if(g!=0){
      Eigen::MatrixXd maxValueOfEdge = getMaxDiffForeachEdge(allEdgeMatrix);
      maxValueOfEdge = maxValueOfEdge.transpose() * maxValueOfEdge;
      double edgeDistance = maxValueOfEdge.sum();
      edgeDistance = sqrt(edgeDistance);
      algebraicDistanceForEdge(i,0) = edgeDistance;

    }

  }
  return algebraicDistanceForEdge;
}
