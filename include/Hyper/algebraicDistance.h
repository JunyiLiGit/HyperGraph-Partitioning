#include "Eigen/Dense"
#include "Eigen/Sparse"

Eigen::SparseMatrix<double,Eigen::RowMajor> randomVector(int row, int col);
std::vector<double> getRowSum(const Eigen::SparseMatrix<int,Eigen::RowMajor>& adjacencyMatrix);
void print(std::vector<double> v);
void updateMatrix(Eigen::SparseMatrix<double,Eigen::RowMajor>& randomMatrix,
                  const Eigen::SparseMatrix<int,Eigen::RowMajor>& adjacencyMatrix,
                  int itr);

Eigen::MatrixXd getMaxDiffForeachEdge(const Eigen::MatrixXd& m);
Eigen::MatrixXd caculateAlgebraicDistance(const Eigen::SparseMatrix<int,Eigen::RowMajor>& incidenceMatrix,
                                          const Eigen::SparseMatrix<double,Eigen::RowMajor>& updatedMatrix);
