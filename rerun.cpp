#include <iostream>

#include "misc.hpp"
#include "learner.hpp"
#include "team.hpp"
#include "point.hpp"
#include "env.hpp"
#include "scm.hpp"
#include <iterator>

using namespace std;

int _maxProgSize;
int _dim;
int _omega;

vector < string > endOfStreamToVector(ifstream &infile, int offset)
{
  vector < string > lines;
  infile.seekg(-offset, ios_base::end);
  
  while (!infile.eof()){
    string tmp;
    getline(infile, tmp);
    if (tmp.length() > 0)
      lines.insert(lines.end(), tmp);
  }
  return lines;
}

team* makeTeam(vector < string > fileEnd)
{
  vector < team* >* mlev = new vector < team *>();
  mlev->push_back(0);
  team *t = new team(0, mlev, -1);
  string line;
  long action = -1;
  int learnerNum = -1;
  int prevLearnNum = -1;
  vector < instruction* > program;
  vector < string > tok;
  /* An array of learner pointers the size of the biggest possible team. This
   * allows us to ensure ordering in the team's set of learners. Ordering is
   * important because some evaluations default to the first "high bid"
   * if all high bids are close enough. See macro BID_EPSILON */
  learner* lptrs = new learner[_omega];
  /* Learner count for moving throuhg the above pointer array. */
  int count=0;
  
  vector < string >::iterator fiter, fiterend;
  for (fiter = fileEnd.begin(), fiterend = fileEnd.end(); fiter != fiterend; fiter++)
    {
      istringstream iss(*fiter);

      /* tokenize the line into a vector*/
      tok.clear();
      copy(istream_iterator < string >(iss), istream_iterator < string >(),
	  back_inserter < vector < string > >(tok));
      /* Skip team data */
      if (tok[0].compare("explicitEnv::test"))
	{
	  learnerNum = stringToInt(tok[4]);
	  /* Check if we are constructing a new learner. If yes, create a new
	   * learner from the previous instructions, if not, just push the new
	   * instruction to the program. */
	  if ((learnerNum != prevLearnNum))
	    {
	      /* TODO: We are breaking the 3 indent rule here.
	       * We should reconstruct. */
	      if (prevLearnNum != -1)
		{
		    /* Create a learner from all the previous instructions */
		    lptrs[count].copyin(1, action, _dim, program);
		    t->addLearner(&lptrs[count++]);
		    program.clear();
		}
	      action = stringToLong(tok[8]);
	      prevLearnNum = learnerNum;
	    }
	  program.push_back(new instruction(tok[17]));
	}
    }
  /* Create a final learner from all the previous instructions */
  lptrs[count].copyin(1, action, _dim, program);
  t->addLearner(&lptrs[count]);

  return t;
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
  
  if((ariter = args.find("omega")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg omega");
  
  _omega = stringToInt(ariter->second);
  
  if((ariter = args.find("setDim")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg setDim");
  
  _dim = stringToInt(ariter->second);
  cout << "_dim = " << _dim << endl;
  
  //createLearners();
  ifstream infile(argv[2], ios::in);
  int foundAt = findLastOccurrence(argv[2], "explicitEnv::test");
  cout << "Found at: " << foundAt << endl;
  
  vector < string > bestteam = endOfStreamToVector(infile, foundAt);
  
  scmExplicit *model = new scmExplicit(args);
  team* t = makeTeam(bestteam);
  model->addTeam(t);
  model->test(0);
  cout << "Team output: " << endl;
  cout << t->printBids("");
  
  delete t;
  delete model;
  return 0;
}

