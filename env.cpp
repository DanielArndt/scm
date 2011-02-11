#include "env.hpp"

/***********************************************************************************************************/

long implicitEnv::_count = 0;

/* 180 deg = PI * RAD -> 1 deg = PI / 180 RAD */
const double backupEnv::_oneDeg = M_PI / 180; 

const double backupEnv::_r = 1.0; 
const double backupEnv::_dc = 6.0; 
const double backupEnv::_ds = 14.0;

const double backupEnv::_maxX = 100;
const double backupEnv::_minY = -100;
const double backupEnv::_maxY = 100;

const double backupEnv::_xNW = 45;
const double backupEnv::_yNW = 50;
const double backupEnv::_xSE = 55;
const double backupEnv::_ySE = -50;

const long rubikEnv::_dim = 54;
const long backupEnv::_dim = 4;

/***********************************************************************************************************/

double env::evaluate(team *m,
		     point *p,
		     bool showState,
		     double &margin)
{
  string prefix = "nopfx";
  vector < double > state;
  int steps;
  
  return evaluate(m, p, showState, state, margin, prefix, steps);
}

/***********************************************************************************************************/

double env::evaluate(team *m,
		     point *p,
		     bool showState,
		     double &margin,
		     string prefix,
		     bool updateActive)
{
  vector < double > state;
  int steps;
  
  return evaluate(m, p, showState, state, margin, prefix, updateActive, steps);
}

/***********************************************************************************************************/

double env::evaluate(team *m,
		     point *p,
		     bool showState,
		     vector < double > &state,
		     double &margin)
{
  string prefix = "nopfx";
  int steps;
  
  return evaluate(m, p, showState, state, margin, prefix, steps);
}

/***********************************************************************************************************/

double env::evaluate(team *m,
		     point *p,
		     bool showState,
		     vector < double > &state, /* Will reflect the updated state. */
		     double &margin, /* At the current level. */
		     string prefix,
		     int &steps)
{
  return evaluate(m, p, showState, state, margin, prefix, false, steps);
}

/***********************************************************************************************************/

double env::evaluate(team *m,
		     point *p,
		     bool showState,
		     vector < double > &state, /* Will reflect the updated state. */
		     double &margin, /* At the current level. */
		     string prefix,
		     bool updateActive, 
		     int &steps)
{  
  vector < learner * > winner; /* Winner at each level. */

  vector < double > bid1; /* First and second highest bid at each level. */
  vector < double > bid2;

  steps = 0; /* Number of steps. */
  
  double rewardSum = 0; /* Total reward. */
  long action = 0;
  
  bool end = false; /* Is the episode over? */
  
  int i;
  
  /* The margin is defined as the average difference in bids at the current level
     (previous level margins should have been recorded earlier). */

  margin = 0;
  
  /* Get initial state defined by point. */
  p->state(state);

  if(showState == true)
    cout << prefix << " env::evaluate tm " << m->id() << " ptid " << p->id() << " ts " << steps  << " st" << vecToStr(state) << " initstate" << endl;
  
  while(end == false)
    {
      winner.clear();
      bid1.clear();
      bid2.clear();
      
      action = m->getAction(state, winner, bid1, bid2, updateActive);
      rewardSum += act(p, action, state, steps++, end);
      
      margin += fabs(bid1.back() - bid2.back());
      
      if(showState == true && (steps == 0 || end == true || (steps % SHOWSTATE_MOD == 0)))
	{
	  cout << prefix << " env::evaluate tm " << m->id() << " ptid " << p->id() << " ts " << steps << " st" << vecToStr(state);
	  cout << " winacts"; for(i = 0; i < winner.size(); i++) cout << " " << winner[i]->action();
	  cout << " action " << action; /* This is redundant since it should be the same as winner[0]. */
	  cout << " bid1" << vecToStr(bid1);
	  cout << " bid2" << vecToStr(bid2);
	  if(end == true)
	    cout << " endeval";
	  else
	    cout << " nxteval";
	  cout << endl;
	}
    }
  
  margin = margin / steps;
  
#ifdef MYDEBUG
  cout << "scm::evaluateEnv " << m->id() << " on " << p->id();
  cout << " state";
  for(int i = 0; i < state.size(); i++)
    cout << " " << state[i];
  cout << " reward " << rewardSum << endl;
#endif
  
  return rewardSum;
}

/***********************************************************************************************************/

double implicitEnv::rtest(team *tm,
			  string prefix,
			  bool showState,
			  bool validate) /* Whether or not to use _validPoints. */
{
  ostringstream oss;
  
  oss << prefix << " rtest " << validate;
  
  set < point * > testPoints;
  
  if(validate == true) /* Use _validPoints. */
    testPoints.insert(_validPoints.begin(), _validPoints.end());
  else /* Use _testPoints. */
    testPoints.insert(_testPoints.begin(), _testPoints.end());
  
  return test2(tm, oss.str(), showState, testPoints);
}

/***********************************************************************************************************/

void implicitEnv::testAbsolute(set < team * > &teams,
			       int seed,
			       long t,
			       long level)
{
  set < team * > :: iterator teiter;
  
  double outcome;

  /* Sum of outcomes over all teams and points. */
  double sumOutcomes = 0;

  /* Minimum outcome for a team, and its maximum over all teams. */
  double minOutcome;
  double maxMinOutcome = - HUGE_VAL;

  /* Mean outcome for a team, and its maximum over all teams. */
  double meanOutcome;
  double maxMeanOutcome = - HUGE_VAL;

  vector < double > endState;
  double margin;
  
  /* Maximum outcome on each point and the sum over all points. */
  vector < double > maxOutcome;
  double sumMaxOutcome = 0;
  
  int i;
  
  maxOutcome.insert(maxOutcome.begin(), _validPoints.size(), 0);
  
  for(teiter = teams.begin(); teiter != teams.end(); teiter++)
    {
      minOutcome = HUGE_VAL;
      meanOutcome = 0;
      
      for(i = 0; i < _validPoints.size(); i++)
	{
	  outcome = evaluate(*teiter, _validPoints[i], false, endState, margin);
	  
	  sumOutcomes += outcome;
	  meanOutcome += outcome;
	  
	  if(outcome < minOutcome)
	    minOutcome = outcome;
	  
	  if(outcome > maxOutcome[i])
	    maxOutcome[i] = outcome;
	}
      
      if(minOutcome > maxMinOutcome)
	maxMinOutcome = minOutcome;
      
      if(meanOutcome > maxMeanOutcome)
	maxMeanOutcome = meanOutcome;
    }
  
  for(i = 0; i < maxOutcome.size(); i++)
    sumMaxOutcome += maxOutcome[i];

  cout << "scm::testAbsolute " << seed << " t " << t << " lev " << level;
  cout << " meanOut " << sumOutcomes / (teams.size() * _validPoints.size());
  cout << " maxMinOut " << maxMinOutcome;
  cout << " maxMeanOut " << maxMeanOutcome / _validPoints.size();
  cout << " meanMaxOutOnPts " << sumMaxOutcome / _validPoints.size();
  
  cout << endl;
}

/***********************************************************************************************************/

rubikEnv::rubikEnv(long maxSteps,
		   long numTestPoints,
		   long numValidPoints,
		   int initTwists,
		   int mutateTwists,
		   bool useBinaryFitness,
		   int seed)
  : implicitEnv(maxSteps, numTestPoints, numValidPoints, useBinaryFitness, seed), _initTwists(initTwists), _mutateTwists(mutateTwists)
{
  int i;
    
  set < long > usedIds;
  point *p;

  _moveLabels.push_back("F");
  _moveLabels.push_back("F_");
  _moveLabels.push_back("B");
  _moveLabels.push_back("B_");
  _moveLabels.push_back("U");
  _moveLabels.push_back("U_");
  _moveLabels.push_back("D");
  _moveLabels.push_back("D_");
  _moveLabels.push_back("R");
  _moveLabels.push_back("R_");
  _moveLabels.push_back("L");
  _moveLabels.push_back("L_");

  /* Define solved state. */

  _solvedState.insert(_solvedState.end(), 9, 0);
  _solvedState.insert(_solvedState.end(), 9, 1);
  _solvedState.insert(_solvedState.end(), 9, 2);
  _solvedState.insert(_solvedState.end(), 9, 3);
  _solvedState.insert(_solvedState.end(), 9, 4);
  _solvedState.insert(_solvedState.end(), 9, 5);
  
  /* Generate 1, 2, ..., twist points. */

  for(i = 0; i < NUM_TWISTPOINTS; i++)
    _twistPoints.push_back(new set < point * >);

  genMoves(_twistPoints);

  for(i = 0; i < _twistPoints.size(); i++)
    cout << "rubikEnv::rubikEnv " << (i + 1) << " twistPoints " << _twistPoints[i]->size() << endl;
  
  /* Populate test points and validation points. */

  srand48(0);
  
  p = 0;
    
  while(_testPoints.size() < _numTestPoints)
    {
      do
	{
	  if(p != 0)
	    delete p;

	  p = initUniformPoint(-2, usedIds);
	}
      while(p->isPointUnique(_testPoints) == false);
      
      _testPoints.push_back(p);
      p = 0;
    }

  p = 0;

  while(_validPoints.size() < _numValidPoints)
    {
      do
	{
	  if(p != 0)
	    delete p;
	  
	  p = initUniformPoint(-2, usedIds);
	}
      while(p->isPointUnique(_validPoints) == false);
      
      _validPoints.push_back(p);
      p = 0;
    }

  srand48(seed);
  
  testMoves();
  
  cout << "rubikEnv::rubikEnv maxSteps " << _maxSteps << " dim " << _dim;
  cout << " actions " << numActions();
  cout << " numTestPoints " << _numTestPoints << " numValidPoints " << _numValidPoints;
  cout << " initTwists " << _initTwists << " mutateTwists " << _mutateTwists;
  cout << " useBinaryFitness " << _useBinaryFitness << endl;
}

/***********************************************************************************************************/

rubikEnv::~rubikEnv()
{
  set < point * > :: iterator poiter;

  for(int i = 0; i < _twistPoints.size(); i++)
    {
      for(poiter = _twistPoints[i]->begin(); poiter != _twistPoints[i]->end(); poiter++)
	delete *poiter;
      
      delete _twistPoints[i];
    }
}

/***********************************************************************************************************/

double rubikEnv::distance(vector < double > &v1,
			  vector < double > &v2)
{
  if(v1.size() != 54 || v2.size() != 54)
    die(__FILE__, __FUNCTION__, __LINE__, "bad vector sizes"); 

  vector < double > v;

  int i, j;

  if(isEqual(v1, v2, 0.001))
    return 0;
  
  for(i = 0; i < numActions(); i++)
    {
      v = v2;
      makeMove(v, i);
      
      if(isEqual(v1, v, 0.001) == true)
	return 1;
    }
  
  for(i = 0; i < numActions(); i++)
    {
      for(j = 0; j < numActions(); j++)
	{
	  v = v2;
	  makeMove(v, i);
	  makeMove(v, j);
	  
	  if(isEqual(v1, v, 0.001) == true)
	    return 4;
	}
    }

  return 16;
}

// double rubikEnv::distance(vector < double > &v1,
// 			  vector < double > &v2)
// {
//   if(v1.size() != 54 || v2.size() != 54)
//     die(__FILE__, __FUNCTION__, __LINE__, "bad vector sizes"); 
  
//   int count = 0;
  
//   for(int i = 0; i < 54; i++)
//     {
//       if(fabs(v1[i] - v2[i]) > 0.1)
// 	count++;
//     }

//   return (double) count / 48;
// }

/***********************************************************************************************************/

void rubikEnv::initState(vector < double > &state)
{
  int k;
  
  state.clear();

  for(k = 0; k < 54; k++)
    state.push_back((double) _solvedState[k]);
}

/***********************************************************************************************************/

void rubikEnv::genMoves(vector < set < point * > * > &psetvec)
{
  int s = psetvec.size();

  int i, j;
  
  vector < double > state, allState, nextState;
  
  set < point * > :: iterator prevPoiter, nextPoiter, allPoiter;
  
  long numTests = 0;
  
  set < point * > prevP; /* Parent points in the current iteration. */
  set < point * > nextP; /* Where new points are to be added in the current iteration. */
  set < point * > allP; /* All points generated so far, including the parents. */

  point *solvedPoint;
  
  if(s < 0)
    die(__FILE__, __FUNCTION__, __LINE__, "must generate at least 1 move states"); 
    
  /* Start off with solved state. */

  initState(state);
  nextP.insert(solvedPoint = new point(numTests, _dim, -1, &state[0], numTests++));

  /* Add moves one step at a time. */
  
  for(i = 0; i < s; i++)
    {
      /* Parents in this iteration are the points generated in the last iteration. */
      prevP = nextP;

      /* Record parents in set of all points generated so far. */
      allP.insert(prevP.begin(), prevP.end());
      
      /* This set will contains the UNIQUE points generated in this iteration. */
      nextP.clear();
      
      /* For each parent point. */
      for(prevPoiter = prevP.begin(); prevPoiter != prevP.end(); prevPoiter++)
	{
	  /* Apply each of the moves to that parent. */
	  for(j = 0; j < numActions(); j++)
	    {
	      (*prevPoiter)->state(state);
	      makeMove(state, j);
	      
	      /* At this point, state contains the new candidate state. */
	      
	      /* Make sure that state is not the same as one of the states from previous step. */
	      
	      for(allPoiter = allP.begin(); allPoiter != allP.end(); allPoiter++)
		{
		  (*allPoiter)->state(allState);
		  
		  if(isEqual(state, allState, 0.001) == true)
		    break;
		}
	      
	      /* If allPoiter is equal to allP.end(), then it did not match any of the previous states. */
	      
	      if(allPoiter == allP.end()) /* No match in allP. */
		{
		  /* Make sure the state is not the same as one of the states in the current step. */
		  
		  for(nextPoiter = nextP.begin(); nextPoiter != nextP.end(); nextPoiter++)
		    {
		      (*nextPoiter)->state(nextState);
		      
		      if(isEqual(state, nextState, 0.001) == true)
			break;
		    }
		  
		  if(nextPoiter == nextP.end())
		    nextP.insert(new point(numTests, _dim, -1, &state[0], numTests++));
		}
	    }
	}

      /* Set nextP contains all the UNIQUE points generated using i + 1 steps. */

      *psetvec[i] = nextP;
    }

  delete solvedPoint;
}


/***********************************************************************************************************/

void rubikEnv::makeMove(vector < double > &state, 
			int action)
{
  /* Turn the cube to the face that want to rotate. */
  switch(action)
    {
    case F: 
    case F_: 
      break;
    case B:
    case B_:
      X(state); X(state); break;
    case U:
    case U_:
      X(state); X(state); X(state); break;
    case D:
    case D_:
      X(state); break;
    case R:
    case R_:
      Y(state); break;
    case L:
    case L_: 
      Y(state); Y(state); Y(state); break;
    default:
      die(__FILE__, __FUNCTION__, __LINE__, "bad move"); 
      break;
    }
  
  /* Rotate the face. */
  switch(action)
    {
    case F:
    case B:
    case U:
    case D:
    case R:
    case L:
      turn(state); break;
    case F_:
    case B_:
    case U_:
    case D_:
    case R_: 
    case L_:
      turn(state); turn(state); turn(state); break;
    default:
      die(__FILE__, __FUNCTION__, __LINE__, "bad move"); 
      break;
    }
  
  /* Turn the cube back to the original position. */
  switch(action)
    {
    case F: 
    case F_: 
      break;
    case B:
    case B_:
      X(state); X(state); break;
    case U:
    case U_:
      X(state); break;
    case D:
    case D_:
      X(state); X(state); X(state); break;
    case R:
    case R_:
      Y(state); Y(state); Y(state); break;
    case L:
    case L_: 
      Y(state); break;
    default:
      die(__FILE__, __FUNCTION__, __LINE__, "bad move"); 
      break;
    }
}

/***********************************************************************************************************/

double rubikEnv::act(point *p,
		     long action, 
		     vector < double > &state, 
		     long steps,
		     bool &end)
{
  bool solved;
  
  makeMove(state, action);
  
  solved = isSolved(state);
  
  if(steps < _maxSteps && solved == false)
    {
      end = false;
      return 0;
    }
  
  /* Either solved the cube or ran out of steps. */
  
  end = true;
  
  if(solved == true)
    p->solved(true); /* Mark point as solved. */

  if(_useBinaryFitness == true) /* Binary reward function. */
    {
      if(solved == true) 
	return 1;
      
      return 0;
    }
  
  /* Return fraction of correct facets, not counting centers. */
  
  //  return scoreCube2(state);
  return scoreCube3(state);
}

/***********************************************************************************************************/

void rubikEnv::printCube(vector < double > &state, 
			 string prefix)
{
  int i;
  vector < int > s;
  int size = state.size();
  
  for(i = 0; i < size; i++)
    s.push_back(lround(state[i]));
  
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[36], s[37], s[38]);
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[39], s[40], s[41]);
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[42], s[43], s[44]);
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s          +-+-+-++-+-+-++-+-+-+\n", prefix.c_str());
  printf("%s          |%d|%d|%d||%d|%d|%d||%d|%d|%d|\n", prefix.c_str(),
	 s[27], s[28], s[29], s[0], s[1], s[2], s[9], s[10], s[11]);
  printf("%s          +-+-+-++-+-+-++-+-+-+\n", prefix.c_str());
  printf("%s          |%d|%d|%d||%d|%d|%d||%d|%d|%d|\n", prefix.c_str(), 
	 s[30], s[31], s[32], s[3], s[4], s[5], s[12], s[13], s[14]);
  printf("%s          +-+-+-++-+-+-++-+-+-+\n", prefix.c_str());
  printf("%s          |%d|%d|%d||%d|%d|%d||%d|%d|%d|\n", prefix.c_str(), 
	 s[33], s[34], s[35], s[6], s[7], s[8], s[15], s[16], s[17]);
  printf("%s          +-+-+-++-+-+-++-+-+-+\n", prefix.c_str());
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[18], s[19], s[20]);
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[21], s[22], s[23]);
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[24], s[25], s[26]);
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[45], s[46], s[47]);
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[48], s[49], s[50]);
  printf("%s                 +-+-+-+\n", prefix.c_str());
  printf("%s                 |%d|%d|%d|\n", prefix.c_str(), s[51], s[52], s[53]);
  printf("%s                 +-+-+-+\n", prefix.c_str());

}

/***********************************************************************************************************/

void rubikEnv::verifyCube(vector < double > &state)
{
  int i;

  vector < int > colourCounts;
  vector < int > colourCountsSolved;
  
  colourCounts.insert(colourCounts.begin(), 6, 0);
  colourCountsSolved.insert(colourCountsSolved.begin(), 6, 0);

  /* Count number of times each colour appears in the solved state. */
  for(i = 0; i < 54; i++)
    colourCountsSolved[lround(_solvedState[i])]++;
  
  /* Count number of times each colour appears in the test state. */
  for(i = 0; i < 54; i++)
    colourCounts[lround(state[i])]++;
  
  for(i = 0; i < colourCounts.size(); i++)
    if(colourCounts[i] != colourCountsSolved[i])
      die(__FILE__, __FUNCTION__, __LINE__, "bad cube"); 
}

/***********************************************************************************************************/

void rubikEnv::testMoves()
{
  vector < double > state;
  vector < double > v1, v2;

  initState(state);

  cout << "distance between solved states " << distance(state, state) << endl;
  
  cout << endl;
  printCube(state, "initial");

  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  /* Do front twice to test distance. */

  /* Front */
  v1 = state;
  makeMove(state, 0);
  v2 = state;
  cout << "distance between move 0 and previous state " << distance(v1, v2) << endl;
  cout << endl;
  printCube(state, "F");

  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  /* Front */
  //  v1 = state;
  makeMove(state, 0);
  v2 = state;
  cout << "(front twice) distance between move 00 and solved state " << distance(v1, v2) << endl;
  cout << endl;
  printCube(state, "FF");

  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  makeMove(state, 1);
  makeMove(state, 1);
  
  /* Back */
  makeMove(state, 2);
  cout << endl;
  printCube(state, "B");

  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  makeMove(state, 3);
  
  /* Up. */
  makeMove(state, 4);
  cout << endl;
  printCube(state, "U");
  
  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  makeMove(state, 5);

  /* Down. */
  makeMove(state, 6);
  cout << endl;
  printCube(state, "D");
  
  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  makeMove(state, 7);

  /* Right. */
  makeMove(state, 8);
  cout << endl;
  printCube(state, "R");
  
  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  makeMove(state, 9);

  /* Left. */
  makeMove(state, 10);
  cout << endl;
  printCube(state, "L");
  
  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  makeMove(state, 11);

  cout << endl;
  printCube(state, "final");

  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  v1 = state;
  makeMove(state, 0);
  makeMove(state, 2);
  makeMove(state, 4);
  makeMove(state, 6);
  makeMove(state, 8);
  makeMove(state, 10);
  makeMove(state, 1);
  makeMove(state, 7);
  v2 = state;
  cout << "distance between move 0 and previous state " << distance(v1, v2) << endl;

  cout << endl;
  printCube(state, "random");
  
  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state) << " " << scoreCube3(state);
  //  cout << "isSolved " << isSolved(state) << " score " << scoreCube(state) << " " << scoreCube2(state);
  cout << " state" << vecToStr(state);
  cout << endl;

  verifyCube(state);  
}

/***********************************************************************************************************/

void rubikEnv::X(vector < double > &state)
{
  /* Rotate cube 90 degrees about x-axis (south to north).
  
                             +-----+-----+-----+		    		    
                             |36-00|37-01|38-02|
                             +-----+-----+-----+		    		    
                             |39-03|40-04|41-05|		    
                             +-----+-----+-----+		    		    
                             |42-06|43-07|44-08|
                             +-----+-----+-----+		    		    
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |27-29|28-32|29-35||00-18|01-19|02-20||09-15|10-12|11-09|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |30-28|31-31|32-34||03-21|04-22|05-23||12-16|13-13|14-10|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |33-27|34-30|35-33||06-24|07-25|08-26||15-17|16-14|17-11|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
           		     +-----+-----+-----+ 		    
                       	     |18-45|19-46|20-47| 
                       	     +-----+-----+-----+ 		    
                       	     |21-48|22-49|23-50| 
                       	     +-----+-----+-----+ 		    
                       	     |24-51|25-52|26-53| 
                       	     +-----+-----+-----+ 		    
                       	     +-----+-----+-----+ 		    
                       	     |45-36|46-37|47-38| 
                       	     +-----+-----+-----+ 		    
                       	     |48-39|49-40|50-41| 
                       	     +-----+-----+-----+ 		    
                       	     |51-42|52-43|53-44|
                       	     +-----+-----+-----+ 		    

   */


  int stateMap[] = {/*0*/ 36, /*1*/ 37, /*2*/ 38, /*3*/ 39, /*4*/ 40, /*5*/ 41, /*6*/ 42,
		    /*7*/ 43, /*8*/ 44, /*9*/ 11, /*10*/ 14, /*11*/ 17, /*12*/ 10,
		    /*13*/ 13, /*14*/ 16, /*15*/ 9, /*16*/ 12, /*17*/ 15, /*18*/ 0,
		    /*19*/ 1, /*20*/ 2, /*21*/ 3, /*22*/ 4, /*23*/ 5, /*24*/ 6,
		    /*25*/ 7, /*26*/ 8, /*27*/ 33, /*28*/ 30, /*29*/ 27, /*30*/ 34,
		    /*31*/ 31, /*32*/ 28, /*33*/ 35, /*34*/ 32, /*35*/ 29, /*36*/ 45,
		    /*37*/ 46, /*38*/ 47, /*39*/ 48, /*40*/ 49, /*41*/ 50, /*42*/ 51,
		    /*43*/ 52, /*44*/ 53, /*45*/ 18, /*46*/ 19, /*47*/ 20, /*48*/ 21, 
		    /*49*/ 22, /*50*/ 23, /*51*/ 24, /*52*/ 25, /*53*/ 26};

//   int stateMap[] = {/*0*/ 0, /*1*/ 1, /*2*/ 2, /*3*/ 3, /*4*/ 4, /*5*/ 5, /*6*/ 6,
// 		    /*7*/ 7, /*8*/ 8, /*9*/ 9, /*10*/ 10, /*11*/ 11, /*12*/ 12,
// 		    /*13*/ 13, /*14*/ 14, /*15*/ 15, /*16*/ 16, /*17*/ 17, /*18*/ 18,
// 		    /*19*/ 19, /*20*/ 20, /*21*/ 21, /*22*/ 22, /*23*/ 23, /*24*/ 24,
// 		    /*25*/ 25, /*26*/ 26, /*27*/ 27, /*28*/ 28, /*29*/ 29, /*30*/ 30,
// 		    /*31*/ 31, /*32*/ 32, /*33*/ 33, /*34*/ 34, /*35*/ 35, /*36*/ 36,
// 		    /*37*/ 37, /*38*/ 38, /*39*/ 39, /*40*/ 40, /*41*/ 41, /*42*/ 42,
// 		    /*43*/ 43, /*44*/ 44, /*45*/ 45, /*46*/ 46, /*47*/ 47, /*48*/ 48, 
// 		    /*49*/ 49, /*50*/ 50, /*51*/ 51, /*52*/ 52, /*53*/ 53};

  mapState(state, stateMap);
}

/***********************************************************************************************************/

void rubikEnv::Y(vector < double > &state)
{

  /* Rotate cube 90 degress about y-axis (east to west).

                             +-----+-----+-----+		    		    
                             |36-42|37-39|38-36|
                             +-----+-----+-----+		    		    
                             |39-43|40-40|41-37|		    
                             +-----+-----+-----+		    		    
                             |42-44|43-41|44-38|
                             +-----+-----+-----+		    		    
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |27-00|28-01|29-02||00-09|01-10|02-11||09-53|10-52|11-51|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |30-03|31-04|32-05||03-12|04-13|05-14||12-50|13-49|14-48|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |33-06|34-07|35-08||06-15|07-16|08-17||15-47|16-46|17-45|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
           		     +-----+-----+-----+ 		    
                       	     |18-20|19-23|20-26| 
                       	     +-----+-----+-----+ 		    
                       	     |21-19|22-22|23-25| 
                       	     +-----+-----+-----+ 		    
                       	     |24-18|25-21|26-24| 
                       	     +-----+-----+-----+ 		    
                       	     +-----+-----+-----+ 		    
                       	     |45-35|46-34|47-33| 
                       	     +-----+-----+-----+ 		    
                       	     |48-32|49-31|50-30| 
                       	     +-----+-----+-----+ 		    
                       	     |51-29|52-28|53-27|
                       	     +-----+-----+-----+ 		    


  */

  int stateMap[] = {/*0*/ 27, /*1*/ 28, /*2*/ 29, /*3*/ 30, /*4*/ 31, /*5*/ 32, /*6*/ 33,
		    /*7*/ 34, /*8*/ 35, /*9*/ 0, /*10*/ 1, /*11*/ 2, /*12*/ 3,
		    /*13*/ 4, /*14*/ 5, /*15*/ 6, /*16*/ 7, /*17*/ 8, /*18*/ 24,
		    /*19*/ 21, /*20*/ 18, /*21*/ 25, /*22*/ 22, /*23*/ 19, /*24*/ 26,
		    /*25*/ 23, /*26*/ 20, /*27*/ 53, /*28*/ 52, /*29*/ 51, /*30*/ 50,
		    /*31*/ 49, /*32*/ 48, /*33*/ 47, /*34*/ 46, /*35*/ 45, /*36*/ 38,
		    /*37*/ 41, /*38*/ 44, /*39*/ 37, /*40*/ 40, /*41*/ 43, /*42*/ 36,
		    /*43*/ 39, /*44*/ 42, /*45*/ 17, /*46*/ 16, /*47*/ 15, /*48*/ 14, 
		    /*49*/ 13, /*50*/ 12, /*51*/ 11, /*52*/ 10, /*53*/ 9};

  mapState(state, stateMap);
}

/***********************************************************************************************************/

void rubikEnv::turn(vector < double > &state)
{
  /* Turn the front face (000 through 008) clockwise.

                             +-----+-----+-----+		    		    
                             |36-36|37-37|38-38|
                             +-----+-----+-----+		    		    
                             |39-39|40-40|41-41|		    
                             +-----+-----+-----+		    		    
                             |42-35|43-32|44-29|
                             +-----+-----+-----+		    		    
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |27-27|28-28|29-18||00-06|01-03|02-00||09-42|10-10|11-11|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |30-30|31-31|32-19||03-07|04-04|05-01||12-43|13-13|14-14|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |33-33|34-34|35-20||06-08|07-05|08-02||15-44|16-16|17-17|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
           		     +-----+-----+-----+ 		    
                       	     |18-15|19-12|20-09| 
                       	     +-----+-----+-----+ 		    
                       	     |21-21|22-22|23-23| 
                       	     +-----+-----+-----+ 		    
                       	     |24-24|25-25|26-26| 
                       	     +-----+-----+-----+ 		    
                       	     +-----+-----+-----+ 		    
                       	     |45-45|46-46|47-47| 
                       	     +-----+-----+-----+ 		    
                       	     |48-48|49-49|50-50| 
                       	     +-----+-----+-----+ 		    
                       	     |51-51|52-52|53-53|
                       	     +-----+-----+-----+ 		    

    */

  int stateMap[] = {/*0*/ 2, /*1*/ 5, /*2*/ 8, /*3*/ 1, /*4*/ 4, /*5*/ 7, /*6*/ 0,
		    /*7*/ 3, /*8*/ 6, /*9*/ 20, /*10*/ 10, /*11*/ 11, /*12*/ 19,
		    /*13*/ 13, /*14*/ 14, /*15*/ 18, /*16*/ 16, /*17*/ 17, /*18*/ 29,
		    /*19*/ 32, /*20*/ 35, /*21*/ 21, /*22*/ 22, /*23*/ 23, /*24*/ 24,
		    /*25*/ 25, /*26*/ 26, /*27*/ 27, /*28*/ 28, /*29*/ 44, /*30*/ 30,
		    /*31*/ 31, /*32*/ 43, /*33*/ 33, /*34*/ 34, /*35*/ 42, /*36*/ 36,
		    /*37*/ 37, /*38*/ 38, /*39*/ 39, /*40*/ 40, /*41*/ 41, /*42*/ 9,
		    /*43*/ 12, /*44*/ 15, /*45*/ 45, /*46*/ 46, /*47*/ 47, /*48*/ 48, 
		    /*49*/ 49, /*50*/ 50, /*51*/ 51, /*52*/ 52, /*53*/ 53};
  
  mapState(state, stateMap);
}

/***********************************************************************************************************/

void rubikEnv::mapState(vector < double > &state, int stateMap[])
{
  /* Map each location in 'state' according to stateMap where
     stateMap[i] indicates where the ith feature is to be mapped
     (i.e., the value of the ith feature is mapped to the value at
     location stateMap[i]). The state vector is overwriten. */
  
  vector < double > oldState = state;
  
  int i;

  /* Update the colours. */
  for(i = 0; i < 54; i++)
    state[stateMap[i]] = oldState[i];
}

/***********************************************************************************************************/

bool rubikEnv::isSolved(vector < double > &state)
{
  int i;

  for(i = 0; i < 54; i++)
    {
      if(_solvedState[i] != lround(state[i]))
	return false;
    }

  return true;
}

/***********************************************************************************************************/

double rubikEnv::scoreCube3(vector < double > &state)
{
  vector < double > s;

  double d;

  initState(s);

  d = distance(s, state);
  
  return 1 / (1 + pow(d, 2));

}

double rubikEnv::scoreCube(vector < double > &state)
{
  int i, j;
  vector < int > s;
  
  int penalty = 0;
  
  for(i = 0; i < 54; i++)
    s.push_back(lround(state[i]));
  
  for(i = 0; i < 54; i++)
    {
      /* Currently on jth face. */
      j = i / 9;
      
      /* Penalty for each face not matching center face. */
      if(s[j * 9 + 4] != s[i])
	penalty++;
    }

  /* Return number of correct facets, not counting centers. */
  return (double) (48 - penalty) / 48;
}


/***********************************************************************************************************/

double rubikEnv::scoreCube2(vector < double > &state)
{

  /* This fitness function is taken from:


     http://www.bionik.tu-berlin.de/user/giani/esdemos/evo.html


     which is a presentation from:


     International Conference On

     Evolutionary Computation

     The Third Parallel Problem
     
     Solving From Nature (PPSN III) 

     
     and the fitness function (verbatim) is defined as:

     
     The quality function Q=Q1+Q2+Q3 for the evaluation of the state of the
     cube consists of three parts combined by addition. With Q1,
     differences from the color of the middle-cube are penalized for every
     surface of the cube. For every wrong color tile, Q1 is increased by
     1. Q2 and Q3 penalize wrong positioned border- or corner-cubes. A
     border-cube is correctly positioned if it contains the two colors of
     the middle-cubes it touches. For every wrongly positioned border-cube,
     Q3 is increased by 4. Similarly, a corner-cube is correctly positioned
     if it contains the three colors of the middle-cubes it touches. For
     every wrongly positioned corner-cube, Q3 is increased by 6. In the
     case of correctly positioned but still wrongly turned border- or
     corner-cubes, Q2 or Q3 are not increased because these turnings are
     sufficiently taken into account by Q1. Each of the three quality parts
     Q1, Q2 and Q3 can take on a maximum value of 48 which means that Q can
     take on a maximum value of Q = 144.

  */
  
  int i, j;
  vector < int > s;
  
  int penalty = 0;
  
  for(i = 0; i < 54; i++)
    s.push_back(lround(state[i]));
  
  /* Penalty of 1 for each facet not matching center. */
  
  for(i = 0; i < 54; i++)
    {
      /* Currently on jth face. */
      j = i / 9;
      
      /* Penalty for each face not matching center face. */
      if(s[j * 9 + 4] != s[i])
	penalty++;
    }

  /* Penalty of 4 for each border cube not matching the centers it
     touches. 

     Border cubes index these facets and must match these centers:

     01, 43 must match 04, 40
     05, 12 must match 04, 13
     07, 19 must match 04, 22
     03, 32 must match 04, 31
     10, 41 must match 13, 40
     16, 23 must match 13, 22
     28, 39 must match 31, 40
     34, 21 must match 31, 22
     37, 52 must match 40, 49
     25, 46 must match 22, 49
     14, 50 must match 13, 49
     30, 48 must match 31, 49

     and we check this using the array sm below.
  */

  int sm []= {1, 43, 4, 40,
	      5, 12, 4, 13,
	      7, 19, 4, 22,
	      3, 32, 4, 31,
	      10, 41, 13, 40,
	      16, 23, 13, 22,
	      28, 39, 31, 40,
	      34, 21, 31, 22,
	      37, 52, 40, 49,
	      25, 46, 22, 49,
	      14, 50, 13, 49,
	      30, 48, 31, 49};

  for(i = 0; i < 12; i++)
    {
      if((s[sm[i * 4]] == s[sm[i * 4 + 2]] && s[sm[i * 4 + 1]] == s[sm[i * 4 + 3]]) ||
	 (s[sm[i * 4]] == s[sm[i * 4 + 3]] && s[sm[i * 4 + 1]] == s[sm[i * 4 + 2]]))
	continue;
      
      penalty += 4;
    }
  

  /* Penalty of 6 for each corner cube not matching the centers it
     touches. 
     
     Corner cubes index these facets and must match these centers:
     
     00, 29, 42 must match 04, 31, 40
     02, 09, 44 must match 04, 13, 40
     08, 15, 20 must match 04, 13, 22
     06, 18, 35 must match 04, 22, 31
     47, 26, 17 must match 49, 22, 13
     45, 24, 33 must match 49, 22, 31
     51, 36, 27 must match 49, 40, 31
     53, 38, 11 must match 49, 40, 13

     and we check this using the array cm below.
  */
  
  int cm[] = {0, 29, 42, 4, 31, 40,
	      2, 9, 44, 4, 13, 40,
	      8, 15, 20, 4, 13, 22,
	      6, 18, 35, 4, 22, 31,
	      47, 26, 17, 49, 22, 13,
	      45, 24, 33, 49, 22, 31,
	      51, 36, 27, 49, 40, 31,
	      53, 38, 11, 49, 40, 13};

  for(i = 0; i < 8; i++)
    {
      if((s[cm[i * 6]] == s[cm[i * 6 + 3]]   && s[cm[i * 6 + 1]] == s[cm[i * 6 + 4]]   && s[cm[i * 6 + 2]] == s[cm[i * 6 + 5]]) ||
	 (s[cm[i * 6]] == s[cm[i * 6 + 3]]   && s[cm[i * 6 + 1]] == s[cm[i * 6 + 5]]   && s[cm[i * 6 + 2]] == s[cm[i * 6 + 4]]) ||
	 (s[cm[i * 6]] == s[cm[i * 6 + 4]]   && s[cm[i * 6 + 1]] == s[cm[i * 6 + 3]]   && s[cm[i * 6 + 2]] == s[cm[i * 6 + 5]]) ||
	 (s[cm[i * 6]] == s[cm[i * 6 + 5]]   && s[cm[i * 6 + 1]] == s[cm[i * 6 + 3]]   && s[cm[i * 6 + 2]] == s[cm[i * 6 + 4]]) ||
	 (s[cm[i * 6]] == s[cm[i * 6 + 4]]   && s[cm[i * 6 + 1]] == s[cm[i * 6 + 5]]   && s[cm[i * 6 + 2]] == s[cm[i * 6 + 3]]) ||
	 (s[cm[i * 6]] == s[cm[i * 6 + 5]]   && s[cm[i * 6 + 1]] == s[cm[i * 6 + 4]]   && s[cm[i * 6 + 2]] == s[cm[i * 6 + 3]]))
	continue;
      
      penalty += 6;
    }
  
  /* Return number of correct facets, not counting centers. */
  return (double) (144 - penalty) / 144;
}

/***********************************************************************************************************/

point * rubikEnv::initUniformPoint(long gtime, 
				   set < long > &usedIds)
{
  long label = _count++;
  
  /* Twist at least once. */
  int twists = 1 + ((int) (drand48() * _initTwists));
  
  int move;
  int i;
  vector < double > state;

  point *p;

  initState(state);
  
  do
    {
#ifdef CUBEDEBUG
      cout << "rubikEnv::initUniformPoint fr " << vecToStr(state) << endl;
#endif
      
      for(i = 0; i < twists; i++)
	{
	  move = (int) (drand48() * numActions());
	  
	  makeMove(state, move);
	}
      
#ifdef CUBEDEBUG
      cout << "rubikEnv::initUniformPoint to " << vecToStr(state) << endl;
      verifyCube(state);
#endif
    } 
  while(isSolved(state) == true);
  
  //  return new point(label, _dim, gtime, &state[0], label);

  p = new point(label, _dim, gtime, &state[0], label);

  p->someint(twists);

  return p;
}

/***********************************************************************************************************/

point * rubikEnv::genPoint(long gtime,
			   point *p1,
			   point *p2)
{
  long label = _count++;
  
  vector < double > state;
  
  int twists;
  int move;
  
  int i;
  
  /* Get parent point. */

  if(drand48() < 0.5)
    p1->state(state);
  else
    p2->state(state);
  
  /* Get number of twists. */
  twists = 1 + ((int) (_mutateTwists * fabs(gasdev())));

  do
    {
      for(i = 0; i < twists; i++)
	{
	  move = (int) (drand48() * numActions());
	  
	  makeMove(state, move);
	}
    } 
  while(isSolved(state) == true);
  
  return new point(label, _dim, gtime, &state[0], label);
}

/***********************************************************************************************************/

double rubikEnv::test2(team *tm,
		       string prefix,
		       bool showState,
		       set < point * > &tpoints)
{
  vector < point * > testPoints;
  
  vector < double > state;
  vector < double > endState;
  
  double reward;
  double rewardSum = 0;
  double minReward = HUGE_VAL;
  
  double score;
  
  int solved = 0;
  
  double margin;
  double sumMargin = 0;

  int steps;
  
  testPoints.insert(testPoints.begin(), tpoints.begin(), tpoints.end());
  
  cout << setprecision(4);
  
  for(int i = 0; i < testPoints.size(); i++)
    {
      testPoints[i]->state(state);
      
      reward = evaluate(tm, testPoints[i], showState, endState, margin, prefix, steps);
      rewardSum += reward;
      sumMargin += margin;
      
      if(reward < minReward) minReward = reward;
      
      cout << prefix << " rubikEnv::test2 " << tm->id() << " test " << i << " ptid " << testPoints[i]->id();
      cout << " upbnd " << testPoints[i]->someint(); // Print upper bound on the number of steps.
      cout << " steps " << steps;
      cout << " istate" << vecToStr(state);
      
      if(isSolved(endState))
	{
	  solved++;
	  cout << " solved 1";
	}
      else
	{
	  cout << " solved 0";
	}
      
      cout << " reward " << reward;
      cout << " margin " << margin;
      
      cout << endl;
    }
  
  score = rewardSum / testPoints.size();
  
  cout << prefix << " rubikEnv::test2 agg tm " << tm->id();
  cout << " minscore " << minReward << " meanscore " << score << " meanmargin " << sumMargin / testPoints.size();
  cout << " solved " << solved << " of " << testPoints.size();
  cout << endl;

  return score;
}

/***********************************************************************************************************/

double rubikEnv::test(team *tm,
		      string prefix,
		      bool showState)
{
  /* This method tests on the 1, 2, 3, ..., twist points. */

  vector < point * > testPoints;
  
  vector < double > state;
  vector < double > endState;
  
  double reward;
  double rewardSum = 0;
  double minReward = HUGE_VAL;
  
  double score;

  int solved = 0;
  
  double margin;
  double sumMargin;
  
  int i, j;

  ostringstream oss;

  int steps;

  for(j = 0; j < _twistPoints.size(); j++)
    {
      /* Look at points generated using j + 1 twists. */

      oss.str("");
      oss << prefix << " twists " << j;

      rewardSum = 0;
      minReward = HUGE_VAL;
      solved = 0;

      sumMargin = 0;

      testPoints.clear();
      testPoints.insert(testPoints.begin(), _twistPoints[j]->begin(), _twistPoints[j]->end());
      
      cout << setprecision(4);
      
      for(i = 0; i < testPoints.size(); i++)
	{
	  testPoints[i]->state(state);
	  
	  reward = evaluate(tm, testPoints[i], showState, endState, margin, oss.str(), steps);
	  rewardSum += reward;
	  sumMargin += margin;
	  
	  if(reward < minReward) minReward = reward;
	  
	  cout << prefix << " rubikEnv::test twists " << j << " tm " << tm->id() << " test " << i << " ptid " << testPoints[i]->id();
	  cout << " steps " << steps;
	  cout << " istate" << vecToStr(state);
	  
	  if(isSolved(endState))
	    {
	      solved++;
	      cout << " solved 1";
	    }
	  else
	    {
	      cout << " solved 0";
	    }
	  
	  cout << " reward " << reward;
	  cout << " margin " << margin;
	  
	  cout << endl;
	}
      
      score = rewardSum / testPoints.size();
      
      cout << prefix << " test agg twists " << j << " tm " << tm->id();
      cout << " minscore " << minReward << " meanscore " << score << " meanmargin " << sumMargin / testPoints.size();
      cout << " solved " << solved << " of " << testPoints.size();
      
      cout << endl;
    }

  /* Note that 0 is returned here though one could calculate a score value
     by averaging the score for each number of twists. 

     As such, this method should not be used to select a 'best' team. 
  */

  return 0;
}

/***********************************************************************************************************/

/* This method appears to be ok. */
backupEnv::backupEnv(long maxSteps,
		     long numTestPoints,
		     long numValidPoints,
    		     double mux,
		     double muy,
		     double muthetas,
		     double muthetac,
		     bool useBinaryFitness,
		     int seed)
  : implicitEnv(maxSteps, numTestPoints, numValidPoints, useBinaryFitness, seed), _mux(mux), _muy(muy), _muthetas(muthetas), _muthetac(muthetac)
{    
  /* The actions that are possible. */
  
//   _actions.push_back(0);
//   _actions.push_back(35 * _oneDeg);
//   _actions.push_back(-35 * _oneDeg);
//   _actions.push_back(70 * _oneDeg);
//   _actions.push_back(-70 * _oneDeg);

  _actions.push_back(0);
  _actions.push_back(30 * _oneDeg);
  _actions.push_back(-30 * _oneDeg);

  cout << "backupEnv::backupEnv oneDeg " << _oneDeg;
  cout << " maxX minY maxY " << _maxX << " " << _minY << " " << _maxY;
  cout << " maxSteps " << _maxSteps;
  cout << " numTestPoints " << _numTestPoints << " numValidPoints " << _numValidPoints;
  cout << " useBinaryFitness " << _useBinaryFitness;
  cout << " mux muy muthetas muthetac " << _mux << " " << _muy << " " << _muthetas << " " << _muthetac << endl;
  
  /* These are some test cases for the act() method. Note that they depend on
     the action values as set in _actions. Output are the (initial state and
     action) -> (what the next state should be) ?->? (what the calculated
     state was). */
  
  vector < double > s;
  bool end;
  long action;

  point *p;

  /* Save the cout flags. */
  ios_base::fmtflags originalFormat = cout.flags();
  
  cout << showpoint << fixed << setprecision(4);
  
  cout << "backupEnv::backupEnv actions";
  for(int i = 0; i < _actions.size(); i++)
    cout << " " << _actions[i];
  cout << endl;
  
  s.clear();
  action = 0;
  s.push_back(0);
  s.push_back(0);
  s.push_back(0);
  s.push_back(0);
  p =  new point(-3, 4, -3, &s[0], -3);
  act(p, action, s, _maxSteps - 1, end);
  delete p;
  cout << "backupEnv::backupEnv 0.000 0.000 0.000 0.000 " << _actions[action] << " -> -0.200 0.000 0.000 0.000 ?->? ";
  cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3];
  if(end == true) cout << " end" << endl; else cout << endl;

  s.clear();
  action = 0;
  s.push_back(0);
  s.push_back(0);
  s.push_back(M_PI);
  s.push_back(M_PI);
  p =  new point(-3, 4, -3, &s[0], -3);
  act(p, action, s, _maxSteps - 1, end);
  delete p;
  cout << "backupEnv::backupEnv 0.000 0.000 3.142 3.142 " << _actions[action] << " -> 0.200 0.000 3.142 3.142 ?->? ";
  cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3];
  if(end == true) cout << " end" << endl; else cout << endl;

  s.clear();
  action = 0;
  s.push_back(0);
  s.push_back(0);
  s.push_back(- M_PI / 2);
  s.push_back(- M_PI / 2);
  p =  new point(-3, 4, -3, &s[0], -3);
  act(p, action, s, _maxSteps - 1, end);
  delete p;
  cout << "backupEnv::backupEnv 0.000 0.000 -1.571 -1.571 " << _actions[action] << " -> 0.000 0.200 -1.571 -1.571 ?->? ";
  cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3];
  if(end == true) cout << " end" << endl; else cout << endl;

  s.clear();
  action = 1;
  s.push_back(0);
  s.push_back(0);
  s.push_back(0);
  s.push_back(0);
  p =  new point(-3, 4, -3, &s[0], -3);
  act(p, action, s, _maxSteps - 1, end);
  delete p;
  cout << "backupEnv::backupEnv 0.00 0.00 0.00 0.00 " << _actions[action] << "-> -0.164 0.000 0.000 -0.019 ?->? ";
  cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3];
  if(end == true) cout << " end" << endl; else cout << endl;

  s.clear();
  action = 1;
  s.push_back(0);
  s.push_back(0);
  s.push_back(M_PI / 2);
  s.push_back(M_PI / 4);
  p =  new point(-3, 4, -3, &s[0], -3);
  act(p, action, s, _maxSteps - 1, end);
  delete p;
  cout << "backupEnv::backupEnv 0.000 0.000 1.571 0.785 " << _actions[action] << " -> 0.000 -0.116 1.579 0.766 ?->? ";
  cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3];
  if(end == true) cout << " end" << endl; else cout << endl;

  /* Reset the cout flags. */
  cout.flags(originalFormat);    

  /* Populate test points. */
  
  srand48(0);

  p = 0;
  
  while(_testPoints.size() < _numTestPoints)
    {
      do
	{
	  if(p != 0)
	    delete p;
	  
	  p = initUniformPoint(-2);
	}
      while(p->isPointUnique(_testPoints) == false);
      
      _testPoints.push_back(p);
      p = 0;
    }
  
  p = 0;
  
  while(_validPoints.size() < _numValidPoints)
    {
      do
	{
	  if(p != 0)
	    delete p;
	  
	  p = initUniformPoint(-2);
	}
      while(p->isPointUnique(_validPoints) == false);
      
      _validPoints.push_back(p);
      p = 0;
    }

  srand48(seed);
}

/***********************************************************************************************************/

double backupEnv::distance(vector < double > &v1,
			   vector < double > &v2)
{
  if(v1.size() != 4 || v2.size() != 4)
    die(__FILE__, __FUNCTION__, __LINE__, "bad vector sizes"); 

  /* This function does not consider thetac.*/

  double d;

  double delta1;
  double delta2;
    
#ifdef MYDEBUG
  cout << "backupEnv::distance1" << vecToStr(v1) << endl;
  cout << "backupEnv::distance2" << vecToStr(v2) << endl;
#endif

  d = 0;

  /* Normalized x-coordinates. */

  d += pow((v1[0] - v2[0]) / _maxX, 2);
  
  /* Normalized y-coordinates. */
  
  d += pow((v1[1] - v2[1]) / (_maxY - _minY), 2);

  /* Cab and semi angles assumed to be in (-PI, PI). */

  /* Condsider the smaller delta around the unit circle. */

  delta1 = fabs(v1[2] - v2[2]);
  delta2 = 2 * M_PI - delta1;

  if(delta1 < delta2)
    d += pow(delta1 / M_PI, 2);
  else
    d += pow(delta2 / M_PI, 2);
  
#ifdef MYDEBUG
  cout << "backupEnv:distance final " << d << endl;
#endif
  
  return d;
}
/***********************************************************************************************************/

double backupEnv::act(point *p,
		      long action, 
		      vector < double > &state, 
		      long steps,
		      bool &end)
{
  /* State. */
  
  /* The x coordinate of center of rear of trailer. */
  double x = state[0];
  /* The y coordinate of center of rear of trailer. */
  double y = state[1];
  /* Angle of trailer w.r.t. positive x. */
  double thetas = state[2];
  /* Angle of cab w.r.t. positive x. */
  double thetac = state[3];

  double xNext, yNext, thetasNext, thetacNext;
  double numer, denom;

  /* Distance to origin. */
  double dist;

  /* The error. */
  
  double error;
  
  /* Get the steering angle of front wheels. */
  
  double u = _actions[action];
  
  double A, B, C;

  bool hasHitWall;
  
  A = _r * cos(u);
  B = A * cos(thetac - thetas);
  C = A * sin(thetac - thetas);
  
  /* Find cooridnates at time t + 1 . */
  
  xNext = x - B * cos(thetas);
  yNext = y - B * sin(thetas);
  
  /* Find angles at time t + 1. */
  
  numer = _dc * sin (thetac) - _r * cos (thetac) * sin (u);
  denom = _dc * cos (thetac) + _r * sin (thetac) * sin (u);
  
  /* Note that atan2(y, x) returns the principal value of the arc tangent of y/x. */
  
  thetacNext = atan2 (numer, denom);
  
  numer = _ds * sin (thetas) - C * cos (thetas);
  denom = _ds * cos (thetas) + C * sin (thetas);
  
  thetasNext = atan2(numer, denom);
  
  /* Record the state t + 1. */
  
  state[0] = xNext;
  state[1] = yNext;
  state[2] = thetasNext;
  state[3] = thetacNext;
  
  /* Note that thetasNext and thetacNext will always be in (-PI, PI] as
     this is the range of possible values returned by atan2. */

  /* Get minimum distance to origin. */

  dist = sqrt(xNext * xNext + yNext * yNext);
  
  /* We stop the simulation if:
     1) Cross the y-axis.
     2) Take too many steps.
     3) Jackknife beyond PI/2 radians.
     4) Reach a point where won't be able to return in time.
     5) Hit the wall.
  */

  hasHitWall = isInWall(xNext, yNext);

  if(xNext > 0 && 
     steps < _maxSteps &&
     jackknife(thetasNext, thetacNext) == false &&
     dist < _r * (_maxSteps - steps) &&
     hasHitWall == false)
    {
      /* Episode is not over yet. */
      
      end = false;
      return 0;
    }
  
  end = true;

  /* Don't tolerate jackknifing. */

  if(jackknife(thetasNext, thetacNext) == true)
    return 0;

  if(fabs(xNext) < 1 && fabs(yNext) < 1 && fabs(thetasNext) < M_PI / 4)
    p->solved(true); /* Mark the point as solved. */

  if(_useBinaryFitness == true) /* Binary reward function. */
    {
      if(fabs(xNext) < 1 && fabs(yNext) < 1 && fabs(thetasNext) < M_PI / 4)
	return 1;
      
      return 0;
    }
  
  /* Not using binary fitness, calculate error. */
  
  error = xNext * xNext + yNext * yNext + thetasNext * thetasNext + 1;
  
#ifdef MYDEBUG
  cout << "backupEnv::act state " << xNext << " " << yNext << " " << thetasNext << " " << thetacNext;
  cout << " error " << error << endl;
#endif
  
  return 1 / sqrt(error);
}

/***********************************************************************************************************/

double backupEnv::flipAngle(double &theta)
{
  /* Value of theta in -2 * PI to 2 * PI is assumed. */
  
  if(theta > M_PI)
    theta = theta - 2 * M_PI;
  else if(theta < - M_PI)
    theta = theta + 2 * M_PI;

  return theta;
}

/***********************************************************************************************************/

bool backupEnv::jackknife(double &thetas, 
			  double &thetac)
{
  /* Assume angles are in (-PI, PI]. */
  
  if(thetas <= - M_PI || thetas > M_PI ||
     thetac <= - M_PI || thetac > M_PI)
    die(__FILE__, __FUNCTION__, __LINE__, "angles out of range"); 
  
  double thetac1 = thetac - thetas;
  
  if((thetac1 >= 0.5 * M_PI && thetac1 <= 1.5 * M_PI) || /* Positive thetac1. */
     (thetac1 >= - 1.5 * M_PI && thetac1 <= - 0.5 * M_PI)) /* Negative thetac1. */
    return true;

  return false;
}

/***********************************************************************************************************/

bool backupEnv::thetaIsInvalid(double x,
			       double y,
			       double theta)
{
  /* This function tests if the orientation of the angle would move the cab and 
     semi into the block along a straight line. The controller may be able to avoid
     the block given the configuration, though this is difficult to tell, so in
     some situations these configurations are better avoided.
     
     We assume that fabs(theta) < PI.
  */

  bool invalid;
  
  if(x < _xNW) /* Left of block. */
    {
      if(y < _ySE) /* Below block. */
	{
	  /* SW zone. */
	  invalid = theta > - M_PI && theta < - M_PI / 2;
	}
      else if (y < _yNW) /* Level with block. */
	{
	  /* W zone. */
	  invalid = (theta > M_PI / 2 && theta < M_PI) || 
	    (theta > - M_PI && theta < - M_PI / 2);
	}
      else /* Above block. */
	{
	  /* NW zone. */
	  invalid = theta > M_PI / 2 && theta < M_PI;
	}
    }
  else if(x < _xSE) /* Level with block. */
    {
      if(y < _ySE) /* Below block. */
	{
	  /* S zone. */
	  invalid = theta > - M_PI && theta < 0;
	}
      else if(y < _yNW) /* Level with block, bad condition. */
	{
	  invalid = true;
	  die(__FILE__, __FUNCTION__, __LINE__, "bad coordinates");
	}
      else /* Above block. */
	{
	  /* N zone. */
	  invalid = theta > 0 && theta < M_PI;
	}
    }
  else /* Right of block. */
    {
      if(y < _ySE) /* Below block. */
	{
	  /* SE zone. */
	  invalid = theta > - M_PI / 2 && theta < 0;
	}
      else if (y < _yNW) /* Level with block. */
	{
	  /* E zone. */
	  invalid = theta > - M_PI / 2 && theta < M_PI / 2;
	}
      else /* Above block. */
	{
	  /* NE zone. */
	  invalid = theta > 0 && theta < M_PI / 2;
	}
    }
  
  return invalid;
}

/***********************************************************************************************************/

point * backupEnv::initUniformPoint(long gtime)
{
  long label = _count++;
  
  double x, y, thetas, thetac;
  
  vector < double > state;
  
  /* Coordinates in (0, _maxX) X (_minY, _maxY). */
  
  do
    {
      x = drand48() * _maxX;
      y = _minY + drand48() * (_maxY - _minY);
    }
  while(isInWall(x, y) == true);

  /* Angle in (- PI, PI). */
  
  do
    {
      thetas = 0.99 * drand48() * M_PI;
      if(drand48() < 0.5)
	thetas = - thetas;
    }
  while(thetaIsInvalid(x, y, thetas) == true);

  thetac = thetas;
  
  state.push_back(x);
  state.push_back(y);
  state.push_back(thetas);
  state.push_back(thetac);
  
  return new point(label, 4, gtime, &state[0], label);
}

/***********************************************************************************************************/

point * backupEnv::initUniformPoint(long gtime, 
				    set < long > &usedIds)
{
  /* This implementation generates any point with uniform
     probability. Note that the angles of the cab and semi are both
     the same. This should be ok as the path to a solution will have
     to visit many configurations of angles. */

  /* The method has been modified not to generate points in the wall. */

  long label = _count++;
  
  double x, y, thetas, thetac;
  
  vector < double > state;
  
  /* Coordinates in (0, _maxX) X (_minY, _maxY), not in wall. */

  do
    {
      x = drand48() * _maxX;
      y = _minY + drand48() * (_maxY - _minY);
    }
  while(isInWall(x, y) == true);
  
  /* Any thetas, make sure in (-PI, PI). */

  thetas = 0.99 * drand48() * M_PI;
  if(drand48() < 0.5)
    thetas = - thetas;

  thetac = thetas;
  
  state.push_back(x);
  state.push_back(y);
  state.push_back(thetas);
  state.push_back(thetac);
  
#ifdef MYDEBUG
  cout << "backupEnv::initUniformPoint" << vecToStr(state) << endl;
#endif
  
  return new point(label, 4, gtime, &state[0], label);
}

/***********************************************************************************************************/

point * backupEnv::genPoint(long gtime,
			    point *p1,
			    point *p2)
{  
  long label = _count++;
  
  vector < double > state;
  
  double m;

  double x, y;
  double thetas, thetac;

  /* Right now, just copy one of the parents. */
  
  if(drand48() < 0.5)
    p1->state(state);
  else
    p2->state(state);
  
#ifdef MYDEBUG
  cout << "backupEnv::genPoint " << state[0] << " " << state[1] << " " << state[2] << " " << state[3] << " ->";
#endif

  /* Coordinates in (0, _maxX) X (_minY, _maxY). */

  do
    {
      /* Mutate x coordinate, make sure > 0 but < _maxX. */

      do
	{
	  m = _mux * gasdev();
	} 
      while(state[0] + m < 0 || state[0] + m > _maxX);
      
      x = state[0] + m;
      
      /* Mutate y coordinate, make sure < _maxY and > _minY. */
      
      do
	{
	  m = _muy * gasdev();
	}
      while(state[1] + m > _maxY || state[1] + m < _minY);
      
      y = state[1] + m ;
    }
  while(isInWall(x, y) == true);
  
  /* Note that _muthetas and _muthetac are both given in degrees. */
  
  /* Mutate angles. */
  
  m = _muthetas * _oneDeg * gasdev(); /* Original mutation amount. */
  
  thetas = fmod(state[2] + m, 2 * M_PI); /* Get absolute value under 2 * M_PI. */
  
  flipAngle(thetas); /* Get absolute value under M_PI. */
  
  thetac = thetas;

  state[0] = x;
  state[1] = y;
  state[2] = thetas;
  state[3] = thetac;

#ifdef MYDEBUG
  cout << " -> " << state[0] << " " << state[1] << " " << state[2] << " " << state[3];
  if(fmod(fabs(state[2] - state[3]), 2 * M_PI) > M_PI / 2 - _oneDeg)
    cout << " thetadiff " << fmod(fabs(state[2] - state[3]), 2 * M_PI);
  cout << endl;
#endif

  return new point(label, 4, gtime, &state[0], label);
}

/***********************************************************************************************************/

double backupEnv::test2(team *tm,
			string prefix,
			bool showState,
			set < point * > &tpoints)
{
  vector < point * > testPoints;

  vector < double > state;
  
  double reward;
  double rewardSum = 0;
  double minReward = HUGE_VAL;
  
  vector < double > endState;
  vector < double > endStateSum;
  
  double score;

  int solved = 0;
  int almostsolved = 0;
  
  double margin;
  double sumMargin = 0;

  int steps;
  
  testPoints.insert(testPoints.begin(), tpoints.begin(), tpoints.end());
  
  endStateSum.push_back(0);
  endStateSum.push_back(0);
  endStateSum.push_back(0);
  endStateSum.push_back(0);

  cout << setprecision(4);
  
  for(int i = 0; i < testPoints.size(); i++)
    {
      testPoints[i]->state(state);
      
      reward = evaluate(tm, testPoints[i], showState, endState, margin, prefix, steps);
      rewardSum += reward;
      sumMargin += margin;
      
      if(reward < minReward) minReward = reward;

      cout << prefix << " backupEnv::test2 tm " << tm->id() << " test " << i << " ptid " << testPoints[i]->id();
      cout << " steps " << steps;
      cout << " istate" << vecToStr(state) << " estate";
      
      for(int j = 0; j < endState.size(); j++)
	{
	  endStateSum[j] += fabs(endState[j]);

	  cout << " " << endState[j];
	}

      if(fabs(endState[0]) < 1 && fabs(endState[1]) < 1 && fabs(endState[2]) < M_PI / 4 && jackknife(endState[2], endState[3]) == false)
	{
	  solved++;
	  cout << " solved 1";
	}
      else
	{
	  cout << " solved 0";
	}

      if(fabs(endState[0]) < 3 && fabs(endState[1]) < 3 && fabs(endState[2]) < M_PI / 3 && jackknife(endState[2], endState[3]) == false)
	{
	  almostsolved++;
	  cout << " almostsolved 1";
	}
      else
	{
	  cout << " almostsolved 0";
	}
      
      cout << " reward " << reward;

      if(jackknife(endState[2], endState[3]) == true)
	cout << " jack 1";
      else
	cout << " jack 0";
      
      if(isInWall(endState[0], endState[1]) == true)
	cout << " inwall 1";
      else
	cout << " inwall 0";

      cout << " margin " << margin;

      cout << endl;
    }
  
  score = rewardSum / testPoints.size();
  
  cout << prefix << " backupEnv::test2 agg tm " << tm->id();
  cout << " minscore " << minReward << " meanscore " << score << " meanmargin " << sumMargin / testPoints.size();
       
  cout << " meanstate";

  for(int i = 0; i < endStateSum.size(); i++)
    cout << " " << (double) endStateSum[i] / testPoints.size();
  
  cout << " solved " << solved;
  cout << " almostsolved " << almostsolved;
  cout << " of " << testPoints.size();
  
  cout << endl;

  return score;
}

/***********************************************************************************************************/

double backupEnv::test(team *tm,
		       string prefix,
		       bool showState)
{
  vector < point * > testPoints;

  vector < double > state;

  vector < double > xVals;
  vector < double > yVals;
  vector < double > thetasVals;

  double reward;
  double rewardSum = 0;
  double minReward = HUGE_VAL;
  
  vector < double > endState;
  vector < double > endStateSum;
  
  double score;
  
  int solved = 0;
  int almostsolved = 0;

  double margin;
  double sumMargin = 0;
  
  ostringstream oss;

  int steps;
  
  oss << prefix << " koza backupEnv::test";
  prefix = oss.str();

  /* Generate points at the given locations. */

  xVals.push_back(20);
  xVals.push_back(40);

  yVals.push_back(-50);
  yVals.push_back(50);
  
  thetasVals.push_back(- M_PI / 2);
  thetasVals.push_back(M_PI / 2);

  int id = 0;

  for(int ix = 0; ix < xVals.size(); ix++)
    for(int iy = 0; iy < yVals.size(); iy++)
      for(int ithetas = 0; ithetas < thetasVals.size(); ithetas++)
	{
	  state.clear();
	  state.push_back(xVals[ix]);
	  state.push_back(yVals[iy]);
	  state.push_back(thetasVals[ithetas]);
	  state.push_back(thetasVals[ithetas]);
	  
	  testPoints.push_back(new point(id, 4, 0, &state[0], id));
	  
	  id++;
	}
  
  /* Initialize average end state vector. */

  endStateSum.push_back(0);
  endStateSum.push_back(0);
  endStateSum.push_back(0);
  endStateSum.push_back(0);

  cout << setprecision(4);
  
  for(int i = 0; i < testPoints.size(); i++)
    {
      testPoints[i]->state(state);

      reward = evaluate(tm, testPoints[i], showState, endState, margin, oss.str(), steps);
      rewardSum += reward;
      sumMargin += margin;
      
      if(reward < minReward) minReward = reward;
      
      cout << prefix << " backupEnv::test koza tm " << tm->id() << " test " << i << " ptid " << testPoints[i]->id();
      cout << " steps " << steps;
      cout << " istate" << vecToStr(state) << " estate";
      
      for(int j = 0; j < endState.size(); j++)
	{
	  endStateSum[j] += fabs(endState[j]);

	  cout << " " << endState[j];
	}

      if(fabs(endState[0]) < 1 && fabs(endState[1]) < 1 && fabs(endState[2]) < M_PI / 4 && jackknife(endState[2], endState[3]) == false)
	{
	  solved++;
	  cout << " solved 1";
	}
      else
	{
	  cout << " solved 0";
	}

      if(fabs(endState[0]) < 3 && fabs(endState[1]) < 3 && fabs(endState[2]) < M_PI / 3 && jackknife(endState[2], endState[3]) == false)
	{
	  almostsolved++;
	  cout << " almostsolved 1";
	}
      else
	{
	  cout << " almostsolved 0";
	}
      
      cout << " reward " << reward;

      if(jackknife(endState[2], endState[3]) == true)
	cout << " jack 1";
      else
	cout << " jack 0";

      if(isInWall(endState[0], endState[1]) == true)
	cout << " inwall 1";
      else
	cout << " inwall 0";

      cout << " margin " << margin;

      cout << endl;
      
      delete testPoints[i];
    }
  
  score = rewardSum / testPoints.size();
  
  cout << prefix << " backupEnv::test agg tm " << tm->id();
  cout << " minscore " << minReward << " meanscore " << score << " meanmargin " << sumMargin / testPoints.size();

  cout << " meanstate";

  for(int i = 0; i < endStateSum.size(); i++)
    cout << " " << (double) endStateSum[i] / testPoints.size();
  
  cout << " solved " << solved;
  cout << " almostsolved " << almostsolved;
  
  cout << endl;

  return score;
}

/***********************************************************************************************************/

long explicitEnv::getLabelIndex(long c)
{ 
  map < long, long >::iterator iter = _labelMap.find(c); 
  
  if(iter == _labelMap.end()) 
    die(__FILE__, __FUNCTION__, __LINE__, "bad label"); 
  
  return (*iter).second; 
} 

/***********************************************************************************************************/

point * explicitEnv::initUniformPoint(long gtime, 
				      set < long > &usedIds)
{
  long index;
  set < long > :: iterator iditerend = usedIds.end();
  
  do
    {
      index = (long) (drand48() * size());
    }
  while(usedIds.find(index) != iditerend);
  
  return new point(index, _dim, gtime, _pointSet[index]->first, _pointSet[index]->second);  
}

/***********************************************************************************************************/

point * explicitEnv::initUniformPointLabel(long gtime, 
					   set < long > &usedIds,
					   long label)
{
  /* Of the new point in _pointSet (also its id). */
  long index;

  set < long > :: iterator iditerend = usedIds.end();

  long tries = 0;
  
  /* Retrieve appropriate index vector from _index. */
  vector < long > *indexvec = _index[getLabelIndex(label)];
  long size = indexvec->size();
  
  do
    {
      /* Get random point of label 'label'. */
      index = (*indexvec)[(long) (drand48() * size)];      

      if(tries++ > _maxTries) /* Cannot find a point of that label. */
	return 0;
    }
  while(usedIds.find(index) != iditerend);

  return new point(index, _dim, gtime, _pointSet[index]->first, _pointSet[index]->second);  
}

/***********************************************************************************************************/

datasetEnv::datasetEnv(string file, 
		       int dim, 
		       long size,
		       long maxSteps)
  : explicitEnv(dim, file, maxSteps)
{
  set < long > labels;
  set < long > :: iterator laiter;
  
  map < long, long > labelCount;

  _pointSet.reserve(size);
  
  ifstream infile(file.c_str(), ios::in);
  
  if(infile == 0)
    die(__FILE__, __FUNCTION__, __LINE__, "cannot open dataset");
  
  for(long i = 0; i < size; i++)
    {
      pair < double *, long > *p = new pair < double *, long >;
      p->first = new double[dim];
      
      for(int j = 0; j < dim; j++)
	{
	  if(infile == 0)
	    die(__FILE__, __FUNCTION__,  __LINE__, "bad pattern");
	  
	  infile >> p->first[j]; 
	}
      
      if(infile == 0)
	die(__FILE__, __FUNCTION__,  __LINE__, "missing pattern class");
      
      infile >> p->second;
      
      labels.insert(p->second);
      
      if(labelCount.count(p->second) == 0)
	labelCount.insert(map < long, long > :: value_type(p->second, 1));
      else
	labelCount[p->second]++;
      
      _pointSet.push_back(p);
    }
  
  for(laiter = labels.begin(); laiter != labels.end(); laiter++)
    _labels.push_back(*laiter);

  /* Sort labels, then create label->index map. */
  
  sort(_labels.begin(), _labels.end());

  for(long i = 0; i < _labels.size(); i++)
    {
      _labelMap.insert(map < long, long > :: value_type(_labels[i], i));
      _labelCount.push_back(labelCount[_labels[i]]);

      cout << "dataset " << file << " label at index " << i << " is " << _labels[i] ;
      cout << " number of patterns of " << _labels[i] << " is " << _labelCount[i];
      cout << " (=" << getLabelCount(_labels[i]) << ")" << endl;
    }
  
  infile.close();
  
  /* PFAIL = ((size - 1) / (size)) ^ _maxTries
     
     Sample the dataset uniformly. PFAIL represents the probability
     of not sampling a specific point after _maxTries attempts.
     
     Use this in initUniformPointLabel since in theory could always sample
     the same point, and if the point is already in usedIds, then would fail.
  */

  _maxTries = (long) (log10(PFAIL) / log10((double) (size - 1) / size));
  
  cout << "maxTries " << _maxTries << " (PFAIL = " << PFAIL << ")" << endl;

  /* Build index. */
  
  for(int i = 0; i < numLabels(); i++)
    _index.push_back(new vector < long > );
  
  for(long i = 0; i < size; i++)
    _index[getLabelIndex(_pointSet[i]->second)]->push_back(i);

  for(int i = 0; i < numLabels(); i++)
    cout << "index " << i << " label " << _labels[i] << " _index[getLabelIndex(_labels[i])]->size() " << _index[getLabelIndex(_labels[i])]->size() << endl;
  
#ifdef MYDEBUG
  cout << "datasetEnv::datasetEnv pattern first";
  for(int i = 0; i < _dim; i++)
    cout << " " << *(_pointSet[0]->first + i);
  cout << " " << _pointSet[0]->second << endl;
  cout << "datasetEnv::datasetEnv pattern last";
  for(int i = 0; i < _dim; i++)
    cout << " " << *(_pointSet[size - 1]->first + i);
  cout << " " << _pointSet[size - 1]->second << endl;
  
  cout << "datasetEnv::datasetEnv size " << this->size() << " labels";
  for(int i = 0; i < numLabels(); i++)
    {
      cout << " " << getLabelIndex(getLabel(i)) << "/" << getLabel(i) << "/" << getLabelCount(getLabel(i));
      cout << "/" << _index[i]->size();
    }
  cout << endl;  
#endif

}

/***********************************************************************************************************/

datasetEnv::~datasetEnv()
{
  for(long i = 0; i < _pointSet.size(); i++)
    {
      delete [] _pointSet[i]->first;
      delete _pointSet[i];
    }
  
  for(int i = 0; i < numLabels(); i++)
    delete _index[i];
}

/***********************************************************************************************************/

double datasetEnv::act(point *p,
		       long action, 
		       vector < double > &state, 
		       long steps,
		       bool &end)
{
  end = true;

  if(p->label() == getLabel(action))
    {
      p->solved(true); /* Mark the point as solved. */
      return 1;
    }
  else 
    {
      return 0;
    }
}

/***********************************************************************************************************/

double datasetEnv::test(team *tm,
			string prefix,
			bool showState)
{
  set < learner * > members;
  set < learner * > :: iterator leiter;
  map < learner *, long > wincount;
  map < learner *, long > :: iterator maiter;

  vector < learner * > winner;
  vector < double > bid1;
  vector < double > bid2;
  
  /* Confusion matrix. */
  vector < long > cm;
  /* Dimension of confusion  matrix. */
  long matDim = numLabels() * numLabels();

  vector < pair < double *, long > * > :: iterator psiter, psiterend;
  vector < double > state;
  
  double score;
  
  cm.insert(cm.begin(), matDim, 0);

  state.reserve(_dim);
  
  long labelIndex;
  long action;
  
  long hit = 0;
  long mis = 0;

  long partfp;

  int i, j;

  double margin = 0;
  
  vector < double > margins;
  margins.insert(margins.begin(), numLabels(), 0);

  tm->members(&members);

  for(leiter = members.begin(); leiter != members.end(); leiter++)
    wincount.insert(map < learner *, long > :: value_type(*leiter, 0));

  /* Calculate confusion matrix. */

  cout << "explicitEnv::test " << prefix << " " << _setName;
  cout << " id " << tm->id() << " hitlist";
  
  for(psiter = _pointSet.begin(), psiterend = _pointSet.end();
      psiter != psiterend; psiter++)
    { 
      state.clear();
      
      winner.clear();
      bid1.clear();
      bid2.clear();
      
      /* Now, labelIndex can be matched with team action. */
      labelIndex = getLabelIndex((*psiter)->second);
      
      for(i = 0; i < _dim; i++)
	state.push_back(((*psiter)->first)[i]);
      
      action = tm->getAction(state, winner, bid1, bid2, false);
      
      wincount[winner.back()]++;

      margin += fabs(bid1.back() - bid2.back());
      margins[labelIndex] += fabs(bid1.back() - bid2.back());
      
      cm[labelIndex * numLabels() + action]++;
      
      if(labelIndex == action)
	{
	  /* Correct. */
	  hit++;
	  cout << " 1";
	}
      else
	{
	  /* Inorrect. */
	  mis++;
	  cout << " 0";
	}
    }
  cout << endl;

  /* Calculate score. */
  
  score = 0;

  for(i = 0; i < numLabels(); i++)
    score += (double) cm[i * numLabels() + i] / getLabelCount(getLabel(i));
  
  score = (double) score / numLabels();

  /* Calculate margin for each class. */

  for(i = 0; i < numLabels(); i++)
    margins[i] = margins[i] / getLabelCount(getLabel(i));

  /* Print out results. */
  
  cout << "explicitEnv::test " << prefix << " " << _setName;
  cout << " id " << tm->id();
  cout << " margin " << margin / _pointSet.size();
  cout << " hit " << hit << " mis " << mis << " rate " << (double) hit / (hit + mis);
  cout << " score " << score << " labels";
  for(i = 0; i < numLabels(); i++)
    cout << " " << getLabel(i);
  cout << " margins";
  for(i = 0; i < numLabels(); i++)
    cout << " " << margins[i];
  cout << " cm";
  for(i = 0; i < matDim; i++)
    cout << " " << cm[i];
  cout << " dr fr";
  for(i = 0; i < numLabels(); i++)
    {
      cout << " " << (double) cm[i * numLabels() + i] / getLabelCount(getLabel(i));
      
      partfp = 0;
      for(j = 0; j < numLabels(); j++)
	if(i != j)
	  partfp += cm[j * numLabels() + i];

      cout << " " << (double) partfp / (size() - getLabelCount(getLabel(i)));
    }

  cout << " size " << wincount.size() <<  " id act cnt";
  for(maiter = wincount.begin(); maiter != wincount.end(); maiter++)
    cout << " " << (maiter->first)->id();
  for(maiter = wincount.begin(); maiter != wincount.end(); maiter++)
    cout << " " << (maiter->first)->action();
  for(maiter = wincount.begin(); maiter != wincount.end(); maiter++)
    cout << " " << maiter->second;
  cout << endl;

  return score;
}

