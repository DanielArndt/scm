#include <iostream>

#include "misc.hpp"
#include "learner.hpp"
#include "team.hpp"
#include "point.hpp"
#include "env.hpp"
#include "scm.hpp"

using namespace std;

int main(int argc, 
	 char **argv)
{
  /* Argument must specify parameter file. */
  
  if(argc != 2)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arguments");

  scm *model;
  map < string, string > args;
  map < string, string > :: iterator ariter;

  cout << "PFAIL " << PFAIL << endl;
  cout << "NUM_TWISTPOINTS " << NUM_TWISTPOINTS << endl;
  cout << "REGISTERS " << REGISTERS << endl;
  cout << "INPUTS " << INPUTS << endl;
  cout << "PROFILE_SIZE " << PROFILE_SIZE << endl;
  cout << "BID_EPSILON " << BID_EPSILON << endl;
  cout << "NEARZERO " << NEARZERO << endl;
  cout << "EPSILON " << EPSILON << endl;
  cout << "USE_DISTINCTIONS " << USE_DISTINCTIONS << endl;
  cout << "KFACTOR " << KFACTOR << endl;
  
  /* Get arguments. */
  
  cout << readMap(argv[1], args) << " args read" << endl;

  /* Seed random number generator. */
  
  if((ariter = args.find("seed")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg seed");
  
  srand48(stringToInt(ariter->second));
  
  /* Select environment type and run associated scm. */
  
  if((ariter = args.find("envType")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg envType");
  
  if(ariter->second == "datasetEnv")
    {
      model = new scmExplicit(args);
      model->go();
      delete model;
    }
  else if(ariter->second == "backupEnv" || ariter->second == "rubikEnv")
    {
      model = new scmImplicit(args);
      model->go();
      delete model;
    }
  else
    {
      die(__FILE__, __FUNCTION__, __LINE__, "bad arg envType");
    }
 
  return 0; 
}
