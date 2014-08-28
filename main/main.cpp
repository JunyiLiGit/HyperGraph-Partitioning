#include "Hyper/parser.h"
#include "Hyper/format.h"
#include "Hyper/algebraicDistance.h"
#include "Hyper/weight.h"
#include "Hyper/compareCut.h"
#include "Hyper/oldCut.h"
#include <cstdlib>
#include <time.h>

int main(int argc, char *argv[]){

    int numberOfRandomVector = atoi(argv[2]);
    int numberOfIteration = atoi(argv[3]);
    int numberOfCluster = atoi(argv[4]);

   char* fileName = argv[1];


  Eigen::MatrixXd parserOutput=parser(fileName);

  int numberOfVertice = parserOutput(1,0);


  Eigen::SparseMatrix<int,Eigen::RowMajor> inc = incindenceMatrix(parserOutput);

  std::vector<int> v = getVerticeDegree(inc);


  Eigen::SparseMatrix<int> diagonal = getDiagonalMatrix(v);

  Eigen::SparseMatrix<int,Eigen::RowMajor> adjMatrix = adjancentMatrix(inc,inc.transpose(),diagonal);

  Eigen::SparseMatrix<double,Eigen::RowMajor> randomMatrix = randomVector(numberOfRandomVector,numberOfVertice);

  updateMatrix(randomMatrix, adjMatrix, numberOfRandomVector);

  Eigen::MatrixXd algebraicDistance = caculateAlgebraicDistance(inc,randomMatrix);


  std::map<int,double> newWeight = getNewWeight(algebraicDistance);

  std::map<int,double> oldWeight = getOldWeight(inc);

  std::string strBefore = formatFile(inc,parserOutput,oldWeight,"before");
  std::string strAfter = formatFile(inc,parserOutput,newWeight,"after");

  std::stringstream stream;
  std::stringstream ss;
  ss << numberOfCluster;
  std::string strNumber = ss.str();
	std::string program1 = "./hmetis2.0pre1 "+strBefore+" "+strNumber+" > "+strBefore+"hmetisOut";
  std::string program2 = "./hmetis2.0pre1 "+strAfter+" "+strNumber+" >"+strAfter+"hmetisOut";

	stream << program1 << std::endl;
	system(stream.str().c_str());

  stream << program2 << std::endl;
  system(stream.str().c_str());

  const char * sizeName = strAfter.c_str();
  const char *middleName = ".part.";
  char const *charNumber = strNumber.c_str();


  char newClusterFileName[100];
  strcpy(newClusterFileName,sizeName); // copy string one into the result.
  strcat(newClusterFileName,middleName);
  strcat(newClusterFileName,charNumber);

  std::map<int,int> myEdgeMap = createVerticeClusterMap(newClusterFileName);
  int newCut = caculateCut(myEdgeMap, inc);
  std::string oldFileName = strBefore+"hmetisOut";
  const char *cstr = oldFileName.c_str();
  int oldCut = getOldCut(cstr);

  std::cout<<"Old Hyperedge Cut is : "<<oldCut<<std::endl;
  std::cout<<"New Hyperedge Cut is : "<<newCut<<std::endl;
  std::cout<<"Ratio is ------------> "<<newCut/float(oldCut)<<std::endl;


}
