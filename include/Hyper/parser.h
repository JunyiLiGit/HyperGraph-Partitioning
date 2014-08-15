/*
Author: Junyi Li
Email: junyil@g.clemson.edu
*/

/*
Parser the input file, which is a m*3 matrix.
The format is as following:
verticeNumber  edgeNumber  numberOfNonzeroEntry
verticeIndex   edgeIndex   edgeweigh?

*/


#include "Eigen/Dense"
#include "Eigen/Sparse"
#include <map>
#include <boost/algorithm/string.hpp>

//int getLineNumber(const char* f);
int getNonzeroEntries(const char* f);
Eigen::MatrixXd parser(const char* f);
void printWeight(const std::map <int, double>& myMap);
Eigen::SparseMatrix<int,Eigen::RowMajor> incindenceMatrix(const Eigen::MatrixXd& m);
Eigen::SparseMatrix<int,Eigen::ColMajor> checkIncindenceMatrix(Eigen::SparseMatrix<int,Eigen::RowMajor> m);
std::vector<int> getVerticeDegree(const Eigen::SparseMatrix<int,Eigen::RowMajor>& inc);
Eigen::SparseMatrix<int,Eigen::RowMajor> getDiagonalMatrix(const std::vector<int>& v);
Eigen::SparseMatrix<int,Eigen::RowMajor> adjancentMatrix(const Eigen::SparseMatrix<int,Eigen::RowMajor>& H,
                                                         const Eigen::SparseMatrix<int,Eigen::RowMajor>& transposeOfH,
                                                         const Eigen::SparseMatrix<int,Eigen::RowMajor>& D);
