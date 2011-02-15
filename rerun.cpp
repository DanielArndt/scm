#include <iostream>

#include "misc.hpp"
#include "learner.hpp"
#include "team.hpp"
#include "point.hpp"
#include <iterator>

using namespace std;

int _maxProgSize;
int _dim;

vector < string > endOfStreamToVector(ifstream &infile, int offset)
{
  vector < string > lines;
  infile.seekg(-offset, ios_base::end);
  
  /* Throw away first line */
  //string *tmp = new string();
  //getline(infile, *tmp);
  //delete tmp;
  
  while (!infile.eof()){
    string tmp;
    getline(infile, tmp);
    if (tmp.length() > 0)
      lines.insert(lines.end(), tmp);
  }
  return lines;
}

int createLearners(vector < string > fileEnd)
{
  learner *l;
  string line;
  long action = -1;
  long prevAction = -1;
  int learnerNum = -1;
  int prevLearnNum = -1;
  vector < instruction* > program;
  vector < string > tok;
  
  vector < string >::iterator fiter, fiterend;
  for (fiter = fileEnd.begin(), fiterend = fileEnd.end(); fiter != fiterend; fiter++)
  {
    istringstream iss(*fiter);
    
    /* tokenize the line into a vector*/
    copy(istream_iterator < string >(iss), istream_iterator < string >(), 
	 back_inserter < vector < string > >(tok));
    /* Skip team data */
    if (tok[0].compare("explicitEnv::test"))
    {
      learnerNum = stringToInt(tok[4]);
      /* Check if we are on a new learner */
      if ((learnerNum != prevLearnNum))
      {
	if (prevLearnNum != -1){
	    /* Create a learner from all the previous instructions */
	    action = stringToLong(tok[8]);
	    l = new learner(1, action, _maxProgSize, _dim, program);
	    ostringstream oss;
	    oss << " lid " << l->id() << " act " << l->action();
	    oss << " size " << l->size() << " esize " << l->esize() ;
	    cout << l->printBid("prefix");
	}
	prevLearnNum = learnerNum;
      }
      program.push_back(new instruction(tok[17]));
      
    }
    tok.clear();
  }
  
  /* The following are variables we need to set */

  /* Bid program. This is what we need to read from the output file. */
  //  vector < instruction * > _bid;

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
  
  vector < string > lines = endOfStreamToVector(infile, foundAt);
  
  
  createLearners(lines);
  cout << "Lines read: " << lines.size() << endl;
  cout << "First line: " << lines[1] << endl;
  
  return 0;
}

