#include "Hyper/algebraicDistance.h"
#include <iostream>
#include <cmath>
#include <random>
#include <vector>

Eigen::SparseMatrix<double,Eigen::RowMajor> randomVector(int row, int col)
{
  Eigen::SparseMatrix<double,Eigen::RowMajor> p(row,col);
  p.reserve(col);
  std::default_random_engine generator;
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

  //loop each random vector
  for(int i=0; i<numberOfRVector;++i){
    //for each random vector, do h iterations
    for(int h=0;h<itr;++h){
      //in each iteration, loop every vertice,update value for each vertice
      Eigen::SparseMatrix<double,Eigen::RowMajor> middleValueMatrix(numberOfVertice,1);
      middleValueMatrix.reserve(1);
      for(int j = 0; j<numberOfVertice; ++j){
         Eigen::SparseMatrix<double,Eigen::RowMajor> v1(1,numberOfVertice), v2(numberOfVertice,1);
         //v1: row vector of adjancyMatrix; V2: row vector of random matrix
         v1.reserve(numberOfVertice);
         v2.reserve(1);
         v1 = adjacencyMatrix.cast<double>().innerVector(j);
         v2 = randomMatrix.innerVector(h).transpose();

         Eigen::SparseMatrix<double,Eigen::RowMajor> middleValue1 = v1*v2;

         double middleValue = (*middleValue1.valuePtr())/rowSum[j];
         middleValueMatrix.insert(j,0) = middleValue;

      }

        Eigen::SparseMatrix<double,Eigen::RowMajor> left = middleValueMatrix.transpose();
        Eigen::SparseMatrix<double,Eigen::RowMajor> right = randomMatrix.innerVector(i);

        randomMatrix.innerVector(i) = 0.5*(left + right);

    }
  }
}


//for each edge,pass the matrix: column number = number of vertice in that edge
//                               row number    = number of random vector
//get the max difference matrix: column number = 1
//                               row number = number of random vector
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



//for each edge, find the whole vertices that are in this edge
//for each vertice which is in the edge, find the correspond value in the
//updated random vectors,store them into a new matrix.
//caculate the max difference between those vertices that are in the same edge
//for each random vector
//sum the max difference for each edge
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
    //loop every vertice, find the vertice indexs which belong to that edge
    for(int j=0;j<verticeNumber;++j){
        if(inc(j,i)){
          //if the vertice belong to that edge, g++
          ++g;
          for(int v = 0; v<update.rows();++v){
              allEdgeMatrix(v,g-1) = update(v,j);
          }
        }
    }

    if(g!=0){
      Eigen::MatrixXd maxValueOfEdge = getMaxDiffForeachEdge(allEdgeMatrix);
      double edgeWeight = maxValueOfEdge.sum();
      algebraicDistanceForEdge(i,0) = edgeWeight;
    } 

  }
  return algebraicDistanceForEdge;
}
