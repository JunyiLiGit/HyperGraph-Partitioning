#include <map>
#include "Eigen/Dense"
#include <string>
std::string formatFile(Eigen::MatrixXd inc,Eigen::MatrixXd parser,std::map<int, double> mymap,const char* c);
//void formatFileOld(Eigen::MatrixXd inc,Eigen::MatrixXd parser,std::map<int, double> mymap, const char* f);
