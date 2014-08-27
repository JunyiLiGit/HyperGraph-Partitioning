#include "Eigen/Dense"
#include "Eigen/Sparse"
#include <map>
#include <boost/algorithm/string.hpp>

int getLineNumber(const char* f);
Eigen::MatrixXd parser(const char* f);
Eigen::SparseMatrix<int,Eigen::RowMajor> incindenceMatrix(const Eigen::MatrixXd& m);
std::vector<int> getVerticeDegree(const Eigen::SparseMatrix<int,Eigen::RowMajor>& inc);
Eigen::SparseMatrix<int,Eigen::RowMajor> getDiagonalMatrix(const std::vector<int>& v);
Eigen::SparseMatrix<int,Eigen::RowMajor> adjancentMatrix(const Eigen::SparseMatrix<int,Eigen::RowMajor>& H,
                                                         const Eigen::SparseMatrix<int,Eigen::RowMajor>& transposeOfH,
                                                         const Eigen::SparseMatrix<int,Eigen::RowMajor>& D);
