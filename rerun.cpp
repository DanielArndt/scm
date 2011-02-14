#include <iostream>

#include "misc.hpp"
#include "learner.hpp"
#include "team.hpp"
#include "point.hpp"

using namespace std;

int _maxProgSize;
int _dim;

vector < string* >* endOfStreamToVector(ifstream &infile, int offset)
{
  vector < string* >* lines = new vector < string* >();
  infile.seekg(-offset, ios_base::end);
  
  /* Throw away first line */
  //string *tmp = new string();
  //getline(infile, *tmp);
  //delete tmp;
  
  while (!infile.eof()){
    string *tmp = new string();
    getline(infile, *tmp);
    lines->insert(lines->end(), tmp);
  }
  return lines;
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
  ifstream infile(argv[2], ios::in);
  int foundAt = findLastOccurrence(argv[2], "explicitEnv::test");
  cout << "Found at: " << foundAt << endl;
  
  vector < string* >* lines = endOfStreamToVector(infile, foundAt);
  cout << "Lines read: " << lines->size() << endl;
  
  return 0;
}

