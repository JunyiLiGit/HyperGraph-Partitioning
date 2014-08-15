//To compile:
//clang++ -std=c++11 -I ../include ../src/algebraicDistance.cpp ../src/parser.cpp ../src/newWeight.cpp ../src/format.cpp  main.cpp
/*
argv[1]: file that will parser.
argv[2]: number of random vector.
argv[3]: range of random number
argv[4]: number of iteration
argv[5]: hMetis input file name before caculating algebraic distance
argv[6]: hMetis input file name after caculating algebraic distance
*/

#include "Hyper/parser.h"
#include "Hyper/format.h"
#include "Hyper/algebraicDistance.h"
#include "Hyper/weight.h"
#include "Hyper/compareCut.h"
#include "Hyper/oldCut.h"
#include <iostream>
#include <cstdlib>
#include <time.h>

int main(int argc, char *argv[]){

    int numberOfRandomVector = atoi(argv[2]);
    int numberOfIteration = atoi(argv[3]);
    int numberOfCluster = atoi(argv[4]);
    //char* newInputFIleName = argv[5];

  clock_t parserstart, parserfinish,incstart, incfinish, diagstart, diagfinish,adjstart, adjfinish,
          updatestart, updatefinish, caculatestart, caculatefinish, formatstart, formatfinish;

  char* fileName = argv[1];

  parserstart = clock();
  Eigen::MatrixXd parserOutput=parser(fileName);
  parserfinish = clock();
  //std::cout<<"parser time is ----"<<((float)(parserfinish - parserstart))/CLOCKS_PER_SEC<<std::endl;
  //std::cout<<parserOutput<<std::endl;
//  printWeight(getWeight(fileName));
  int numberOfVertice = parserOutput(1,0);

  incstart = clock();
  Eigen::SparseMatrix<int,Eigen::RowMajor> incBefore = incindenceMatrix(parserOutput);
  Eigen::SparseMatrix<int,Eigen::RowMajor> inc = checkIncindenceMatrix(incBefore);
  //std::cout<<inc<<std::endl;
  incfinish = clock();
  //std::cout<<"inc time is ----"<<((float)(incfinish - incstart))/CLOCKS_PER_SEC<<std::endl;


  std::vector<int> v = getVerticeDegree(inc);

  diagstart = clock();
  Eigen::SparseMatrix<int> diagonal = getDiagonalMatrix(v);
  diagfinish = clock();
  //std::cout<<"diag time is ----"<<((float)(diagfinish - diagstart))/CLOCKS_PER_SEC<<std::endl;


  adjstart = clock();
  Eigen::SparseMatrix<int,Eigen::RowMajor> adjMatrix = adjancentMatrix(inc,inc.transpose(),diagonal);
  //std::cout<<adjMatrix<<std::endl;
  adjfinish = clock();
  //std::cout<<"adj time is ----"<<((float)(adjfinish - adjstart))/CLOCKS_PER_SEC<<std::endl;



  Eigen::SparseMatrix<double,Eigen::RowMajor> randomMatrix = randomVector(numberOfRandomVector,numberOfVertice);

  updatestart=clock();
  updateMatrix(randomMatrix, adjMatrix, numberOfRandomVector);
  updatefinish=clock();
//  std::cout<<"update time is ----"<<((float)(updatefinish - updatestart))/CLOCKS_PER_SEC<<std::endl;

  caculatestart=clock();
  Eigen::MatrixXd algebraicDistance = caculateAlgebraicDistance(inc,randomMatrix);
  caculatefinish=clock();
//  std::cout<<"caculate time is ----"<<((float)(caculatefinish - caculatestart))/CLOCKS_PER_SEC<<std::endl;


  std::map<int,double> newWeight = getNewWeight(algebraicDistance);
  //printWeight(newWeight);
  std::map<int,double> oldWeight = getOldWeight(inc);
  //printWeight(oldWeight);

  formatstart=clock();
  std::string strBefore = formatFile(inc,parserOutput,oldWeight,"before");
  std::string strAfter = formatFile(inc,parserOutput,newWeight,"after");
  formatfinish=clock();
//  std::cout<<"format time is ----"<<((float)(formatfinish - formatstart))/CLOCKS_PER_SEC<<std::endl;

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
  //std::cout<<inc<<std::endl;
  //std::cout<<newClusterFileName<<std::endl;
  std::map<int,int> myEdgeMap = createVerticeClusterMap(newClusterFileName);
  int newCut = caculateCut(myEdgeMap, inc);
  std::string oldFileName = strBefore+"hmetisOut";
  const char *cstr = oldFileName.c_str();
  int oldCut = getOldCut(cstr);
  std::cout<<"Old Hyperedge Cut is : "<<oldCut<<std::endl;
  std::cout<<"New Hyperedge Cut is : "<<newCut<<std::endl;
  std::cout<<"Ratio is ------------> "<<newCut/float(oldCut)<<std::endl;

}
