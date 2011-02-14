#include <iostream>

#include "misc.hpp"
#include "learner.hpp"
#include "team.hpp"
#include "point.hpp"

#define READ_BLOCK_SIZE 4096
#define MAX_BLOCK_READS 50

using namespace std;

int _maxProgSize;
int _dim;

/* Find the last occurance of a string in a text file by iteratively searching
 * further back in the file. Currently it is hardcoded to only search back up
 * to 200KB. */
string* findLastOccurance(string fileName, string findString)
{
  ifstream infile(fileName.c_str(), ios::in);
  int reads = 0;
  int readback = 0;
  
  char tmp[READ_BLOCK_SIZE+1];
  tmp[READ_BLOCK_SIZE] = '\0';
  string* fileEnd = new string();
  string* tmpPtr;
  cout << "In." << endl;
  size_t found = string::npos;
  while (found == string::npos)
  {
    ++reads;
    cout << "Searching back "<< reads*READ_BLOCK_SIZE << " bytes." << endl;
    readback = reads * READ_BLOCK_SIZE;
    infile.seekg(-readback, ios_base::end);
    infile.read(tmp, READ_BLOCK_SIZE);
    tmpPtr = fileEnd;
    *fileEnd = string(tmp).append(*tmpPtr);
    delete tmpPtr;
    found = fileEnd->rfind(findString);
    if (reads > MAX_BLOCK_READS)
      die(__FILE__, __FUNCTION__, __LINE__, "cannot find string");
  }
  tmpPtr = fileEnd;
  fileEnd = new string(fileEnd->substr(found));
  delete tmpPtr;
  //cout << "File end: " << fileend << endl;
  return fileEnd;
}

int createLearners()
{
  learner *l;
  long action = 1;
  l = new learner(-1, action, _maxProgSize, _dim);
  
  instruction *i = new instruction();
  
  /* The following are variables we need to set */

  /* Bid program. This is what we need to read from the output file. */
  //  vector < instruction * > _bid;
  
  /* Features indexed by non-introns in this learner, determined in
     markIntrons(). */
  //  set < long > _features;

  /* Number of references by teams. */
  //  int _nrefs;
  
  /* Bid profile. */
  //  vector < double > _profile;

  return 0;
}

int main(int argc, 
	 char **argv)
{
  map < string, string > args;
  map < string, string > :: iterator ariter;
  cout << readMap(argv[1], args) << " args read" << endl;
  
  /* First, lets make sure that we're in a dataSetEnv.. */
  if((ariter = args.find("envType")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg envType");

  if (ariter->second.compare("datasetEnv"))
    die(__FILE__, __FUNCTION__, __LINE__, "this program requires a datasetEnv");
  
  if((ariter = args.find("maxProgSize")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg maxProgSize");
  
  _maxProgSize = stringToInt(ariter->second);
  cout << "_maxProgSize = " << _maxProgSize << endl;
  
  if((ariter = args.find("setDim")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg setDim");
  
  _dim = stringToInt(ariter->second);
  cout << "_dim = " << _dim << endl;
  
  //createLearners();
  string* fileend = findLastOccurance(argv[2], "explicitEnv::test");
  cout << *fileend;
  cout << endl;
 
  return 0;
}

