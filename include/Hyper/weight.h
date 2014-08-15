#include "Eigen/Dense"
#include "Eigen/Sparse"
#include <map>

std::map <int, double> getNewWeight(Eigen::MatrixXd m);
std::map <int, double> getOldWeight(Eigen::SparseMatrix<int,Eigen::ColMajor> inc);
