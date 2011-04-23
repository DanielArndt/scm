#include "scm.hpp"

/***********************************************************************************************************/
scm::scm(map < string, string > &args)
  : _mdom(0), _pdom(0)
{
  map < string, string > :: iterator maiter;
  
  if((maiter = args.find("numLevels")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg numLevels");
  
  _numLevels = stringToInt(maiter->second);
  
  if((maiter = args.find("Psize")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg Psize");
  
  _Psize = stringToInt(maiter->second);
  
  if((maiter = args.find("Msize")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg Msize");
  
  _Msize = stringToInt(maiter->second);
  
  if((maiter = args.find("pmd")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pmd");
  
  _pmd = stringToDouble(maiter->second);

  if((maiter = args.find("pma")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pma");
  
  _pma = stringToDouble(maiter->second);

  if((maiter = args.find("pmm")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pmm");
  
  _pmm = stringToDouble(maiter->second);

  if((maiter = args.find("pmn")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pmn");
  
  _pmn = stringToDouble(maiter->second);
  
  if((maiter = args.find("omega")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg omega");
  
  _omega = stringToInt(maiter->second);
  
  if((maiter = args.find("t")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg t");
  
  _t = stringToLong(maiter->second);
  
  if((maiter = args.find("Pgap")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg Pgap");
  
  _Pgap = stringToInt(maiter->second);
  
  if((maiter = args.find("Mgap")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg Mgap");
  
  _Mgap = stringToInt(maiter->second);
    
  if((maiter = args.find("maxProgSize")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg maxProgSize");
  
  _maxProgSize = stringToInt(maiter->second);
  
  if((maiter = args.find("pBidDelete")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pBidDelete");
  
  _pBidDelete = stringToDouble(maiter->second);
  
  if((maiter = args.find("pBidAdd")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pBidAdd");
  
  _pBidAdd = stringToDouble(maiter->second);
  
  if((maiter = args.find("pBidSwap")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pBidSwap");
  
  _pBidSwap = stringToDouble(maiter->second);
  
  if((maiter = args.find("pBidMutate")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pBidMutate");
  
  _pBidMutate = stringToDouble(maiter->second);

  if((maiter = args.find("statMod")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg statMod");
  
  _statMod = stringToLong(maiter->second);
  
  if((maiter = args.find("seed")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg seed");
  
  _seed = stringToLong(maiter->second);

  if((maiter = args.find("paretoEpsilonPoint")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg paretoEpsilonPoint");
  
  _paretoEpsilonPoint = stringToDouble(maiter->second);

  if((maiter = args.find("paretoEpsilonTeam")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg paretoEpsilonTeam");
  
  _paretoEpsilonTeam = stringToDouble(maiter->second);
  
  if((maiter = args.find("pointPow")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg pointPow");
  
  _pointPow = stringToLong(maiter->second);
  
  if((maiter = args.find("teamPow")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg teamPow");
  
  _teamPow = stringToLong(maiter->second);

  if((maiter = args.find("maxSteps")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg maxSteps");
  
  _maxSteps = stringToLong(maiter->second);

  cout << "arg numLevels " << _numLevels << endl;
  
  cout << "arg Psize " << _Psize << endl;
  cout << "arg Msize " << _Msize << endl;
  cout << "arg omega " << _omega << endl;
  cout << "arg pmd " << _pmd << endl;
  cout << "arg pma " << _pma << endl;
  cout << "arg pmm " << _pmm << endl;
  cout << "arg pmn " << _pmn << endl;
  cout << "arg t " << _t << endl;
  cout << "arg Pgap " << _Pgap << endl;
  cout << "arg Mgap " << _Mgap << endl;

  cout << "arg maxProgSize " << _maxProgSize << endl;
  cout << "arg pBidDelete " << _pBidDelete << endl;
  cout << "arg pBidAdd " << _pBidAdd << endl;
  cout << "arg pBidSwap " << _pBidSwap << endl;
  cout << "arg pBidMutate " << _pBidMutate << endl;
  
  cout << "arg statMod " << _statMod << endl;
  cout << "arg seed " << _seed << endl;
  
  cout << "arg paretoEpsilonPoint " << _paretoEpsilonPoint << endl;
  cout << "arg paretoEpsilonTeam " << _paretoEpsilonTeam << endl;

  cout << "arg pointPow " << _pointPow << endl;
  cout << "arg teamPow " << _teamPow << endl;

  cout << "arg maxSteps " << _maxSteps<< endl;

  if(_numLevels < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg numLevels < 1");

  if(_Psize < 2)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg Psize < 2");
  
  if(_Msize < 2)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg Msize < 2");
   
  if(_pmd < 0 || _pmd > 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pmd < 0 || pmd > 1");

  if(_pma < 0 || _pma > 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pma < 0 || pma > 1");

  if(_pmm < 0 || _pmm > 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pmm < 0 || pmm > 1");
  
  if(_pmn < 0 || _pmn > 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pmn < 0 || pmn > 1");

  if(_omega < 2)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg omega < 2");  
  
  if(_t < 1) 
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg t < 1");  

  if(_Pgap < 1 || _Pgap >= _Psize)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg Pgap < 1 || Pgap >= Psize");

  if(_Mgap < 1 || _Mgap >= _Msize)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg Mgap < 1 || Mgap >= Msize");
  
  if(_maxProgSize < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg _maxProgSize < 1");  
  
  if(_pBidDelete < 0 || _pBidDelete > 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pBidDelete < 0 || pBidDelete > 1");
  
  if(_pBidAdd < 0 || _pBidAdd > 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pBidAdd < 0 || pBidAdd > 1");
  
  if(_pBidSwap < 0 || _pBidSwap > 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pBidSwap < 0 || pBidSwap > 1");
  
  if(_pBidMutate < 0 || _pBidMutate > 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pBidMutate < 0 || pBidMutate > 1");

  if(_statMod < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg statMod < 1");    

  if(_paretoEpsilonPoint < 0)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg _paretoEpsilonPoint < 0");    
  
  if(_paretoEpsilonTeam < 0)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg _paretoEpsilonTeam < 0");    

  if(_pointPow < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg pointPow < 1");    

  if(_teamPow < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg teamPow < 1");    

  if(_maxSteps < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg maxSteps < 1");    

  /* Placeholder null pointers that should never get accessed.

     Set this way because element i is set to contain individuals
     evolved at level i - 1 which does not make sense for element 0.
   */

  _pLevel.push_back(0);
  _mLevel.push_back(0);
  _lLevel.push_back(0);
}

/***********************************************************************************************************/

void scm::initTeams(long level)
{
  long a1, a2;
  team *m;
  learner *l;

  /* Number of teams to initialize. */
  int keep = _Msize - _Mgap;

  int tc;

  int i, j, k;
  
  set < team * > :: iterator teiter;

  vector < learner * > learnervec;

  int size;

  set < learner * > lused;
  
  int tsize = 10;
  
  for(tc = 0; tc < keep; tc++)
    {
      /* Get two different actions. */
      
      a1 = (long) (drand48() * numActions(level));
      
      do
	{
	  a2 = (long) (drand48() * numActions(level));
	} while (a1 == a2);
      
      /* Create a new team containing two learners. */
      
      m = new team(level, _mLevel[level], -1);
      
      l = new learner(-1, a1, _maxProgSize, dim());
      genUniqueLearner(l, _L); /* Make sure bid unique. */
      m->addLearner(l); 
      l->refInc();
      _L.insert(l);
      
      l = new learner(-1, a2, _maxProgSize, dim());
      genUniqueLearner(l, _L); /* Make sure bid unique. */
      m->addLearner(l);
      l->refInc();
      _L.insert(l);
      
      _M.insert(m);

#ifdef MYDEBUG
      cout << "scm::initTeams added " << *m << endl;
#endif
    }

  /* Mix the learners up. */

  learnervec.insert(learnervec.begin(), _L.begin(), _L.end());
  
  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
    {
      /* Get the learners in the team. */
      lused.clear();
      (*teiter)->members(&lused);
      
      /* Get the final team size, make sure not too big. */
      size = (int) (drand48() * (_omega + 1));
      
      if(size > learnervec.size() - 2)
	size = learnervec.size() - 2;
      
      /* Build the team up.*/
      while((*teiter)->size() < size)
	{	  
	  /* Get first learner, make sure not in the team. */

	  i = (int) (drand48() * learnervec.size());
	  
	  while(lused.find(learnervec[i]) != lused.end())
	    i = (i + 1) % learnervec.size();
	
	  for(k = 0; k < tsize; k++)
	    {
	      /* Get another learner, make sure not in the team. */
	      
	      j = (int) (drand48() * learnervec.size());
	      
	      while(lused.find(learnervec[j]) != lused.end())
		j = (j + 1) % learnervec.size();

	      /* Pick second learner if it has fewer refs. */
	      
	      if(learnervec[j]->refs() < learnervec[i]->refs())
		i = j;
	    }
	  
	  (*teiter)->addLearner(learnervec[i]);
	  lused.insert(learnervec[i]);
	  learnervec[i]->refInc();
	}
    }	
}

/***********************************************************************************************************/

/* Keep altering 'lr' as long as its bid profile is the same as the bid
   profile of one of the learners in 'learners'. */
void scm::genUniqueLearner(learner *lr,
			   set < learner * > learners)
{
  vector < double > profile;
  set < learner * > :: iterator leiter;
  
  vector < double > state;
  
  int i;
  
  bool stop = false;
  
  bool changedL;
  
  double *REG = (double *) alloca(REGISTERS * sizeof(double));

#ifdef MYDEBUG
  for(i = 0, leiter = learners.begin(); leiter != learners.end(); leiter++, i++)
    {
      (*leiter)->getProfile(profile);
      cout << "scm::genUniqueLearner " << lr->id() << " " << i << " " << (*leiter)->id() << " profile" << vecToStr(profile) << endl;
    }
#endif
  
  while(stop == false)
    {
      profile.clear();
      
      /* Create new profile. */
      
      for(i = 0; i < _profilePoints.size(); i++)
	{
	  _profilePoints[i]->state(state);
	  profile.push_back(lr->bid(&state[0], REG));
	}

#ifdef MYDEBUG
      cout << "scm::genUniqueLearner " << lr->id() << " profile" << vecToStr(profile);
#endif
      
      if(profile.size() != _profilePoints.size())
	die(__FILE__, __FUNCTION__, __LINE__, "bad profile size");
      
      for(leiter = learners.begin(); leiter != learners.end(); leiter++)
	if((*leiter)->isProfileEqualTo(profile))
	  break;
      
      if(leiter == learners.end())
	{
	  /* Not a duplicate bidder. */

#ifdef MYDEBUG
	  cout << " is unique" << endl;
#endif

	  stop = true;
	}
      else
	{
	  /* Duplicate bidder. */

#ifdef MYDEBUG
	  cout << " is a duplicate of " << (*leiter)->id() << endl;
#endif
	  
	  do
	    {
	      changedL = lr->muBid(_pBidDelete, _pBidAdd, _pBidSwap, _pBidMutate, _maxProgSize);
	    }
	  while(changedL == false);
	}
    }

  lr->setProfile(profile);  
} 

/***********************************************************************************************************/

double scm::getDistances(set < point * > &P,
			 map < point *, map < point *, double > * > &distances)
{
  map < point *, map < point *, double > * > :: iterator iter1, iter2;

  set < point * > :: iterator poiter1, poiter2;
  
  vector < double > state1;
  vector < double > state2;
  
  double d;
  double max = - HUGE_VAL;
  
  if(distances.empty() == false)
    die(__FILE__, __FUNCTION__, __LINE__, "distance map should be empty");
  
  /* Insert initial map entry for each point. */

  for(poiter1 = P.begin(); poiter1 != P.end(); poiter1++)
    distances.insert(map < point *, map < point *, double > * > :: value_type(*poiter1, new map < point *, double >));
  
  for(poiter1 = P.begin(); poiter1 != P.end(); poiter1++)
    {
      /* First point distance map and state. */
      
      iter1 = distances.find(*poiter1);
      (*poiter1)->state(state1);
      
      poiter2 = poiter1;
      
      for(poiter2++; poiter2 != P.end(); poiter2++)
	{
	  /* Second point distance map and state.*/
	  
	  iter2 = distances.find(*poiter2);
	  (*poiter2)->state(state2);
	  
	  /* Get distance between the two points .*/
	  
	  d = distance(state1, state2);
	  
	  if(d > max)
	    max = d;

	  /* Store distance in both maps. */

	  iter1->second->insert(map < point *, double > :: value_type(*poiter2, d));
	  iter2->second->insert(map < point *, double > :: value_type(*poiter1, d));
	}
    }

#ifdef MYDEBUG
  for(poiter1 = P.begin(); poiter1 != P.end(); poiter1++)
    {
      cout << "scm::getDistances point " << (*poiter1)->id();
      
      iter1 = distances.find(*poiter1);
      
      for(poiter2 = P.begin(); poiter2 != P.end(); poiter2++)
	{	
	  if(poiter1 != poiter2)
	    {
	      d = (iter1->second->find(*poiter2))->second;
	      cout << " " << d;
	    }
	}
      cout << endl;
      
    }
  cout << "scm::getDistance max " << max << endl;
#endif

  return max;
}

/***********************************************************************************************************/

void scm::genTeams(long t,
		   team *pm,
		   team **cm,
		   vector < learner * > &lpop,
		   long level)
{  
  set < learner * > plearners;
  set < learner * > clearners;
  
  set < learner * > :: iterator leiter;
  
  vector < learner * > learnervec;
  
  int i;
  
  double b;
  
  learner *lr;
  
  bool changedL;
  bool changedM;
  
  int lsize;

  lsize = lpop.size();
  
  pm->members(&plearners);
  
  *cm = new team(level, _mLevel[level], t);    
  
  for(leiter = plearners.begin(); leiter != plearners.end(); leiter++)
    (*cm)->addLearner(*leiter);
  
  learnervec.insert(learnervec.begin(), plearners.begin(), plearners.end());
  
  /* Remove learners. */
  
  for(b = 1.0; drand48() < b && (*cm)->size() > 2; b = b * _pmd)
    {
      do
	{
	  i = (int) (drand48() * learnervec.size());
	}
      while(learnervec[i] == 0);
      
      (*cm)->removeLearner(learnervec[i]);
      learnervec[i] = 0;
    }

  /* Add learners. */

  for(b = 1.0; drand48() < b && (*cm)->size() < _omega && (*cm)->size() < lsize; b = b * _pma)
    {      
      do
	{
	  i = (int) (drand48() * lsize);
	}
      while((*cm)->addLearner(lpop[i]) == false);
    }

  /* Mutate learners. */
  
  clearners.clear();
  (*cm)->members(&clearners);
  
  changedM = false;

  do
    {
      for(leiter = clearners.begin(); leiter != clearners.end(); leiter++)
	{
	  if(drand48() < _pmm)
	    {
	      changedM = true;
	      
	      (*cm)->removeLearner(*leiter);

	      lr = new learner(t, **leiter);
		  
	      do
		{
		  changedL = lr->muBid(_pBidDelete, _pBidAdd, _pBidSwap, _pBidMutate, _maxProgSize);
		}
	      while(changedL == false);

	      genUniqueLearner(lr, _L); /* Make sure bid unique. */
	      
	      if(drand48() < _pmn)
		lr->muAction(((long) (drand48() * numActions(level))));
		  
              if (this->myclass().compare("scmImplicit") == 0)
              {
                implicitEnv* env = dynamic_cast<scmImplicit*>(this)->env();
                if (env->myclass().compare("rpropEnv") == 0)
                {
                  rpropEnv* my_rpropEnv = dynamic_cast<rpropEnv*>(env);
                  if ((my_rpropEnv->_weights[(*leiter)->id()] != 0) && 
                    (my_rpropEnv->_weights[lr->id()] == 0))
                  {
                    my_rpropEnv->_weights[lr->id()] = new vector<double>(*(my_rpropEnv->_weights[(*leiter)->id()]));
                    my_rpropEnv->_updateValue[lr->id()] = new vector<double>(*(my_rpropEnv->_updateValue[(*leiter)->id()]));
                    my_rpropEnv->_derivatives[lr->id()] = new vector<double>(*(my_rpropEnv->_derivatives[(*leiter)->id()]));
                  }
                }
              }
              
	      (*cm)->addLearner(lr);
	      _L.insert(lr);
	    }
	}
    }
  while(changedM == false);
  
  /* Increment references. */
  
  clearners.clear();
  (*cm)->members(&clearners);
  
  for(leiter = clearners.begin(); leiter != clearners.end(); leiter++)
    (*leiter)->refInc();
}


/***********************************************************************************************************/

void scm::genTeams(long t, 
		   long level)
{
  vector < team * > parent;
  int psize;
  
  team *pm;
  team *cm;
  
  vector < learner * > lpop;
  
  parent.insert(parent.begin(), _M.begin(), _M.end());
  psize = parent.size();

  lpop.insert(lpop.begin(), _L.begin(), _L.end());
  
  while(_M.size() < _Msize)
    {
      pm = parent[(int) (drand48() * psize)];
      
      genTeams(t, pm, &cm, lpop, level);

      _M.insert(cm);
    }
}

/***********************************************************************************************************/

void scm::getDistinctions(set < point * > &P,
			  map < point *, vector < short > * > &dist)
{
  if(dist.empty() == 0)
    die(__FILE__, __FUNCTION__, __LINE__, "distinction map should be empty");
  
  vector < double > outcomes;
  double out;
  vector < short > *distvec;

  int i, j;
  int size = _M.size();
  
  /* Assuming that iterating through point and team population always in the same order
     (which is true for sets). */
  
  set < point * > :: iterator poiter, poiterend;
  set < team * > :: iterator teiter, teiterbegin, teiterend;
  
  teiterbegin = _M.begin();
  teiterend = _M.end();

  /* Build distinction vector for each point. */
  
  for(poiter = P.begin(), poiterend = P.end(); 
      poiter != poiterend; poiter++)
    {
      /* Collect outcomes. */
      
      outcomes.clear();
      
      for(teiter = teiterbegin; teiter != teiterend; teiter++)
	{
	  if((*teiter)->getOutcome(*poiter, &out) == false)
	    die(__FILE__, __FUNCTION__, __LINE__, "outcome not found");
	  
	  outcomes.push_back(out);
	}

      /* Allocate distinction vector and insert into map. */

      distvec = new vector < short >;
      dist.insert(map < point *, vector < short > * > :: value_type(*poiter, distvec));

#ifdef MYDEBUG
      cout << "scm::getDistinctions outcomes " << outcomes.size() << " = " << _M.size();
      for(i = 0; i < size; i++)
	cout << " " << outcomes[i];
#endif
      
      /* Calculate the distinctions. */
      
      for(i = 0; i < size; i++)
	for(j = 0; j < size; j++)
	  {	    
	    /* See if outcome i > j. */
	    
	    if(outcomes[i] > outcomes[j] && (isEqual(outcomes[i], outcomes[j], _paretoEpsilonTeam) == 0))
	      distvec->push_back(1);
	    else
	      distvec->push_back(0);
	  }
#ifdef MYDEBUG
      cout << " distvec" << vecToStr(*distvec) << endl;
#endif
    }
}

/***********************************************************************************************************/

template <class ptype, class vtype > void scm::sharingScorePie(map < ptype *, vector < vtype > * > &outMap, 
							       map < ptype *, double > &score,
							       set < ptype * > &forThese, /* Calculate score for these. */
							       set < ptype * > &wrtThese, /* Denominator w.r.t. these. */
							       int power)
{  
  if(score.empty() == 0)
    die(__FILE__, __FUNCTION__, __LINE__, "score map should be empty");
  
  typename set < ptype * > :: iterator ptiter, ptiterend;
  
  /* Denominator in each dimension. */
  vector < vtype > nd;
  
  /* Whether the sum of the column is zero. */
  vector < bool > zero;
  
  vector < vtype > *outvec;
  double sc;
  int i;
  
  int outvecsize = ((outMap.begin())->second)->size();
  
  /* Initialize to 0 (it is now possible to divide by 0 if not careful). */
  nd.insert(nd.begin(), outvecsize, 0);

  /* Initialize to true. If the column remains marked as true, will not count it. */
  zero.insert(zero.begin(), outvecsize, true);
  
  /* Calculate denominators in each dimension. */

  for(ptiter = wrtThese.begin(), ptiterend = wrtThese.end();
      ptiter != ptiterend; ptiter++)
    {
      /* Get outcome vector. */
      outvec = (outMap.find(*ptiter))->second;
      
      for(i = 0; i < outvecsize; i++)
	nd[i] += (*outvec)[i];
    }
  
  /* Check if denominators are not zero. */
  
  for(i = 0; i < outvecsize; i++)
    if(nd[i] > NEARZERO)
      zero[i] = false;

  for(ptiter = forThese.begin(), ptiterend = forThese.end(); 
      ptiter != ptiterend; ptiter++)
    {
      outvec = (outMap.find(*ptiter))->second;
      sc = 0;
      
      /* Only consider this test if sum of column is not zero. */
      
      for(i = 0; i < outvecsize; i++)
	if(zero[i] == false)
	  sc += pow(((double) (*outvec)[i] / nd[i]), power);
      
      score.insert(typename map < ptype *, double > :: value_type(*ptiter, sc));
    }

#ifdef MYDEBUG
  typename map < ptype *, double > :: iterator sciter;
  cout << "scm::sharingScore";
  for(sciter = score.begin(); sciter != score.end(); sciter++)
    cout << " " << sciter->first->id() << "/" << sciter->second;
  cout << endl;
#endif
}

/***********************************************************************************************************/
void scm::selPoints(long t)
{ 
  /* Distinction vectors .*/
  map < point *, vector < short > * > distinctions;
  map < point *, vector < short > * > :: iterator dviter;

  /* Distances between points. */
  map < point *, map < point *, double > * > distances;
  map < point *, map < point *, double > * > :: iterator dmiter;

  /* Shared score. */
  map < point *, double > score;
  map < point *, double > :: iterator sciter;
  
  set < point * > :: iterator poiter;

  int i, j;
  
  vector < point * > pointvec;
  double maxDistance = 0;
  map < point *, double > :: iterator miter;
  double penalty;
  vector < double > dvec;
  
  int KNEAREST = 1 + _P.size() / KFACTOR ;
  
  vector < double > state;

  double sc;
  double mean;
  int count;
  double out;
  vector < double > outcomes;
  set < team * > :: iterator teiter;

  if(USE_DISTINCTIONS == true) /* Distinction based coevolution. */
    {
      getDistinctions(_P, distinctions);
      sharingScorePie(distinctions, score, _P, _P, _pointPow);
    }
  else /* Use the unique solver function. */
    {      
      for(poiter = _P.begin(); poiter != _P.end(); poiter++)
	{
	  outcomes.clear();
	  mean = 0;
	  count = 0;
	  
#ifdef MYDEBUG
	  cout << "scm::selPoints " << t << " pt " << (*poiter)->id() << " out";
#endif
	  
	  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
	    {
	      if((*teiter)->getOutcome(*poiter, &out) == false)
		die(__FILE__, __FUNCTION__, __LINE__, "outcome not found");
	      
#ifdef MYDEBUG
	      cout << " " << out;
#endif
	      mean += out;
	      outcomes.push_back(out);
	    }
	  
	  /* Multiply by 0.999999 just in case all 1s, otherwise count would be 0. */
	  
	  mean = 0.999999 * mean / outcomes.size();
	  
	  if(mean > NEARZERO)
	    {
	      for(i = 0; i < outcomes.size(); i++)
		if(outcomes[i] > mean)
		  count++;
	    } /* Else count = 0. */
	  
#ifdef MYDEBUG
	  cout << " mean " << mean << " count " << count;
#endif
	  
	  if(count == 0)
	    sc = 0;
	  else 
	    sc = (double) (1 + outcomes.size() - count) / outcomes.size();
	  
#ifdef MYDEBUG
	  cout << " score " << sc << endl;
#endif
	  
	  score.insert(map < point *, double > :: value_type(*poiter, sc));
	}
    }
  
  /* Get distances between points .*/

  if(myclass() != "scmExplicit")
    maxDistance = getDistances(_P, distances);

  /* Store all the points in a vector. */

  pointvec.insert(pointvec.begin(), _P.begin(), _P.end());

#ifdef MYDEBUG
  for(i = 0; i < pointvec.size(); i++)
    {
      pointvec[i]->state(state);
      cout << "scm::selPoints " << t << " " << i << " point " << pointvec[i]->id();
      cout << " gtime " << pointvec[i]->gtime() << " state" << vecToStr(state) << endl;
    }
#endif
  
  /* Normalize the fitness of each point. */
  
  for(i = 0; i < pointvec.size(); i++)
    {
      /* Set penalty to 1 for scmExplicit since distance always 1. */
      
      penalty = 1;
      
      if(myclass() != "scmExplicit")
	{
	  /* Get distance map for this point. */
	  
	  if((dmiter = distances.find(pointvec[i])) == distances.end())
	    die(__FILE__, __FUNCTION__, __LINE__, "cannot find distance");
	  
	  /* Normalize distances for this point to unity, store in vector. */
	  
	  dvec.clear();
	  
	  for(miter = dmiter->second->begin(); miter != dmiter->second->end(); miter++)
	    dvec.push_back(miter->second / maxDistance);
	  
	  /* Sort distances.*/
	  
	  partial_sort(dvec.begin(), dvec.begin() + KNEAREST, dvec.end());
	  
	  /* Calculate penalty, higher values are preferred as they correspond to
	     nearest neighbours that are farther away. */
	  
	  for(penalty = 0, j = 0; j < KNEAREST; j++)
	    penalty += pow(dvec[j], 2.0);
	  
	  /* At this point penalty is in (0, KNEAREST]. */
	  
	  penalty = pow((double) penalty / KNEAREST, 2.0);	  
	}

      /* Get score and normalize by penalty. */

      if((sciter = score.find(pointvec[i])) == score.end())
	die(__FILE__, __FUNCTION__, __LINE__, "cannot find score");
      
      pointvec[i]->key((double) sciter->second * penalty);
      pointvec[i]->slice(pointvec[i]->key());

#ifdef MYDEBUG
      cout << "scm::selPoints " << _seed << " t " << t << " " << pointvec[i]->id();
      cout << " raw " << sciter->second << " pen " << penalty << " key " << pointvec[i]->key() << endl;
#endif
    }
  
  partial_sort(pointvec.begin(), pointvec.begin() + _Pgap, pointvec.end(), lessThan < point > ());
  
#ifdef MYDEBUG
  for(i = 0; i < pointvec.size(); i++)
    cout << "scm::selPoints " << t << " point " << pointvec[i]->id() << " key " << pointvec[i]->key() << endl;
#endif
  
  /* Mark the worst points for deletion. */

  for(i = 0; i < _Pgap; i++)
    {
      if(pointvec[i]->gtime() != t) _pdom++;
      
      pointvec[i]->mark();

#ifdef MYDEBUG
      cout << "scm::selPoints " << t << " removing " << pointvec[i]->id() << endl;
#endif
    }
  
  /* Free allocated memory. */
  
  for(dviter = distinctions.begin(); dviter != distinctions.end(); dviter++)
    delete dviter->second;

  for(dmiter = distances.begin(); dmiter != distances.end(); dmiter++)
    delete dmiter->second;
}

/***********************************************************************************************************/

void scm::selTeams(long t)
{
  vector < team * > teams;

  set < team * > :: iterator teiter, teiterend;
  set < point * > :: iterator poiter, poiterbegin, poiterend;
  
  double out;
  
  int i;

  /* Team outcome maps. */
  map < team *, vector < double > * > outMap;
  map < team *, vector < double > * > :: iterator omiter, omiterend;
  
  /* Sharing score. */
  map < team *, double > score;
  map < team *, double > :: iterator sciter;

  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
    teams.push_back(*teiter);
  
  for(teiter = _M.begin(), teiterend = _M.end(), poiterbegin = _P.begin(), poiterend = _P.end();
      teiter != teiterend; teiter++)
    {
      omiter = outMap.insert(map < team *, vector < double > * > :: value_type(*teiter, new vector < double >)).first;
      
      for(poiter = poiterbegin; poiter != poiterend; poiter++)
	{
	  if((*teiter)->getOutcome(*poiter, &out) == false)
	    die(__FILE__, __FUNCTION__, __LINE__, "cannot find outcome");
	  
	  omiter->second->push_back(out);
	}

#ifdef MYDEBUG
      cout << "scm::selTeams " << t << " tm " << (*teiter)->id() << " out" << vecToStr(*omiter->second) << endl;
#endif
    }

  sharingScorePie(outMap, score, _M, _M, _teamPow);
  
  /* Selected the top teams using sharing score as the fitness. */
  
  for(i = 0; i < teams.size(); i++)
    {
      if((sciter = score.find(teams[i])) == score.end())
	die(__FILE__, __FUNCTION__, __LINE__, "cannot find score");
      
      teams[i]->key(sciter->second);
    }
  
  /* Sort the teams by their keys. */
  
  partial_sort(teams.begin(), teams.begin() + _Mgap, teams.end(), lessThan < team > ());

#ifdef MYDEBUG
  for(i = 0; i < teams.size(); i++)
    cout << "selTeams " << t << " sorted " << teams[i]->id() << " key " << teams[i]->key() << endl;
#endif
  
  /* At this point, the order of teams no longer matches the order of the other vectors. */

  for(i = 0; i < _Mgap; i++)
    {
      if(teams[i]->gtime() != t) _mdom++; /* The team is old. */
      
#ifdef MYDEBUG
      cout << "scm::selTeams deleting " << teams[i]->id() << endl;
#endif
      
      _M.erase(teams[i]);
      delete teams[i]; /* Learner refs deleted automatically. */
    }

  /* Free outcome vectors. */
  
  for(omiter = outMap.begin(), omiterend = outMap.end();
      omiter != omiterend; omiter++)
    delete omiter->second;
}

/***********************************************************************************************************/

void scm::cleanup(long t,
		  long level)
{
  /* Delete points marked for deletion. */
  /* Delete learners that are no longer referenced. */
  /* Delete cached outcomes/margins for deleted points. */

  set < point * > :: iterator poiter, poiterend;
  set < team * > :: iterator teiter, teiterbegin, teiterend;
  set < learner * > :: iterator leiter, leiterend;
  
  vector < point * > pointvec;
  vector < learner * > learnervec;
  
  int size;
  int i;

  /* Collect points into pointvec. */
  
  for(poiter = _P.begin(), poiterend = _P.end(); 
      poiter != poiterend; poiter++)
    pointvec.push_back(*poiter);
  
  /* Delete marked points. */
  
  size = pointvec.size();
  for(i = 0, teiterbegin = _M.begin(), teiterend = _M.end(); i < size; i++)
    {
      if(pointvec[i]->marked() == true)
	{
	  /* This point is marked for deletion. */
	  
	  /* Delete outcomes and margins for this point. */
	  
	  for(teiter = teiterbegin; teiter != teiterend; teiter++)
	    {
	      (*teiter)->deleteOutcome(pointvec[i]);
	      (*teiter)->deleteMargin(pointvec[i]);
	    }
	  
	  /* Get rid of this point. */
	  
	  _P.erase(pointvec[i]);
	  
	  /* Save point if solved and unique. */
	  
	  if(pointvec[i]->solved() == true)
	    {
	      if(saveTestPoint(level, pointvec[i]) == false)
		delete pointvec[i]; /* Solved but a duplicate. */
	    }
	  else
	    {
	      delete pointvec[i]; /* Not solved, delete. */
	    }
	}
    }
  
  /* Prune the learners, check _omega if this is not the last generation. */
  pruneLearners(t != _t);

  /* Collect learners into learnervec. */
  
  for(leiter = _L.begin(), leiterend = _L.end(); 
      leiter != leiterend; leiter++)
    learnervec.push_back(*leiter);

  /* Delete learners with zero references. */
  
  size = learnervec.size();
  for(i = 0; i < size; i++)
  {
    if(learnervec[i]->refs() == 0)
    {
      /* Zero references, delete this learner. */
      if (this->myclass().compare("scmImplicit") == 0)
      {
        implicitEnv* env = dynamic_cast<scmImplicit*>(this)->env();
        if (env->myclass().compare("rpropEnv") == 0)
        {
          //We are in an rprop environment. Delete rprop specific
          //things.
          rpropEnv* my_rpropEnv = dynamic_cast<rpropEnv*>(env);
          delete my_rpropEnv->_weights[learnervec[i]->id()];
          my_rpropEnv->_weights.erase(learnervec[i]->id());
          delete my_rpropEnv->_updateValue[learnervec[i]->id()];
          my_rpropEnv->_updateValue.erase(learnervec[i]->id());
        }
      }
      _L.erase(learnervec[i]);
      delete learnervec[i];
    }
  }

#ifdef MYDEBUG
  int sumTeamSizes, nrefs, sumNumOutcomes;
  
  cout << "scm::cleanup ";
  cout << _P.size() << " points, " << _L.size() << " learners, " << _M.size() << " teams";
  
  for(teiter = _M.begin(), sumTeamSizes = sumNumOutcomes = 0; teiter != _M.end(); teiter++)
    {
      sumTeamSizes += (*teiter)->size();
      sumNumOutcomes += (*teiter)->numOutcomes();
    }
  
  for(leiter = _L.begin(), nrefs = 0; leiter != _L.end(); leiter++)
    nrefs += (*leiter)->refs();
  
  cout << ", sumTeamSizes " << sumTeamSizes << ", nrefs " << nrefs << ", sumNumOutcomes " << sumNumOutcomes;
  cout << endl;

  if(sumTeamSizes != nrefs || sumNumOutcomes != (_Msize - _Mgap) * (_Psize - _Pgap))
    die(__FILE__, __FUNCTION__, __LINE__, "something messed up during cleanup");
  
#endif
}

/***********************************************************************************************************/

void scm::finalize()
{
  set < point * > :: iterator poiter;
  set < team * > :: iterator teiter;
  
  /* Purge outcomes and margins since they won't be used. */
  
  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
    for(poiter = _P.begin(); poiter != _P.end(); poiter++)
      {
	(*teiter)->deleteOutcome(*poiter);
	(*teiter)->deleteMargin(*poiter);
      }

  /* Save points, teams, and learners and clear the populations for use in next level. */
  
  _pLevel.push_back(new vector < point * >);
  (_pLevel.back())->insert((_pLevel.back())->begin(), _P.begin(), _P.end());
  _P.clear();

  _mLevel.push_back(new vector < team * >);
  (_mLevel.back())->insert((_mLevel.back())->begin(), _M.begin(), _M.end());
  _M.clear();
  
  _lLevel.push_back(new vector < learner * > );
  (_lLevel.back())->insert((_lLevel.back())->begin(), _L.begin(), _L.end());
  _L.clear();
  
  /* Hence _P, _M, and _L are purged but we saved the pointers to the 
     points, teams, and learners. */ 

  _mdom = _pdom = 0;
}

/***********************************************************************************************************/

void scm::finalfinalize()
{
  /* Free any allocated memory before exit. */

  set < point * > :: iterator poiter;
  
  int i, j;

  /* Points. */
  
  for(i = 1; i < _pLevel.size(); i++)
    {
      _solvedPoints.insert(_pLevel[i]->begin(), _pLevel[i]->end());
      delete _pLevel[i];
    }
  
  for(poiter = _solvedPoints.begin(); poiter != _solvedPoints.end(); poiter++)
    delete *poiter;
  
  /* Teams. */

  for(i = 1; i < _mLevel.size(); i++)
    {
      for(j = 0; j < _mLevel[i]->size(); j++)
	delete (*_mLevel[i])[j];
      
      delete _mLevel[i];
    }

  /* Learners. */
  
  for(i = 1; i < _lLevel.size(); i++)
    {
      for(j = 0; j < _lLevel[i]->size(); j++)
	delete (*_lLevel[i])[j];

      delete _lLevel[i];
    }
}

/***********************************************************************************************************/

void scm::pruneLearners(bool checkOmega) /* If true, prune only if the team size equals _omega. */
{
  set < team * > :: iterator teiter;
  
  set < learner * > members, active;
  set < learner * > setdiff;
  set < learner * > :: iterator leiter;
  
  /* Remove inactive team members from the teams. */

  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
    {
#ifdef MYDEBUG
      cout << "scm::pruneLearners check " << checkOmega << " size " << (*teiter)->size();
#endif

      if((checkOmega == false) || ((*teiter)->size() == _omega))
	{
	  members.clear();
	  active.clear();
	  setdiff.clear();
	  
	  (*teiter)->members(&members); /* Get all members. */
	  (*teiter)->activeMembers(&active); /* Get active members. */
	  
	  /* Inactive members are members \ active. */

	  set_difference(members.begin(), members.end(),
			 active.begin(), active.end(),
			 insert_iterator < set < learner * > > (setdiff, setdiff.begin()));

#ifdef MYDEBUG
	  cout << " members";
	  for(leiter = members.begin(); leiter != members.end(); leiter++)
	    cout << " " << (*leiter)->id();

	  cout << " active";
	  for(leiter = active.begin(); leiter != active.end(); leiter++)
	    cout << " " << (*leiter)->id();

	  cout << " inactive";
	  for(leiter = setdiff.begin(); leiter != setdiff.end(); leiter++)
	    cout << " " << (*leiter)->id();
#endif

	  for(leiter = setdiff.begin(); leiter != setdiff.end() && (*teiter)->size() > 2; leiter++)
	    {
	      (*leiter)->refDec();
	      (*teiter)->removeLearner(*leiter);
	    }
	}

#ifdef MYDEBUG
      cout << " finalsize " << (*teiter)->size() << endl;
#endif
    }
}

/***********************************************************************************************************/

bool scm::saveTestPoint(long level,
			point *p)
{
  /* Only at level 0 and for scmImplicit. */
  
  if(level != 0 || myclass() != "scmImplicit")
    return false;

  if(p->isPointUnique(_solvedPoints)) /* Is point unique? */
    {
      _solvedPoints.insert(p); /* Insert and return true. */
      return true;
    }

  /* The point is not unique, was not added. */
  return false;
}

/***********************************************************************************************************/
inline void scm::rprop(){
  if (this->myclass().compare("scmImplicit") == 0)
  {
    implicitEnv* env = dynamic_cast<scmImplicit*>(this)->env();
    if (env->myclass().compare("rpropEnv") == 0)
    {
      rpropEnv* my_rpropEnv = dynamic_cast<rpropEnv*>(env);
      set < team * > :: iterator teiter, teiterend;
      set < point * > :: iterator poiter, poiterend;
      
      
      for(poiter = _P.begin(), poiterend = _P.end(); poiter != poiterend; poiter++)
        for(teiter = _M.begin(), teiterend = _M.end(); teiter != teiterend; teiter++)
        {
          my_rpropEnv->cluster(*teiter, *poiter);
        }
      
      map < pair < long, unsigned long >, set< point* >* >::iterator cliter, cliterend;
      
      for (cliter = my_rpropEnv->_cluster.begin(), cliterend = my_rpropEnv->_cluster.end(); 
           cliter != cliterend; 
           cliter++)
       {
	 //Iterate over each cluster and calculate the error, keep the error
	 //which is lowest stored in the error map with the team that it
	 //scored lowest on.
	 set<point*>* cluster = (*cliter).second;
	 unsigned int learner_id = (*cliter).first.second;
	 vector<double>* weights = my_rpropEnv->_weights[learner_id];
	 double error = my_rpropEnv->calcSSE(cluster, weights);         
         //Check if the error of this (team, learner) pair less than the error 
         //of any previous one. If so, replace it.
	 if (error < (my_rpropEnv->_error[learner_id]).second)
	 {
	   long team_id = (*cliter).first.first;
	   pair <long, double> error_pair = make_pair(team_id, error);
           my_rpropEnv->_error[(*cliter).first.second] = error_pair;
	 }
       }
       for (cliter = my_rpropEnv->_cluster.begin(); cliter != cliterend; cliter++)
       {
         //Preform RPROP
         set<point*>* cluster = (*cliter).second;
         unsigned int learner_id = (*cliter).first.second;
         my_rpropEnv->rprop(cluster, learner_id);
         delete cliter->second;
       }
       //Reset all clusters and errors
      my_rpropEnv->_error.clear();
      my_rpropEnv->_cluster.clear();
    }
  }
}

void scm::go()
{
  double sys1, usr1, sys2, usr2;
  
  long level;
  
  long t;
  
  ostringstream oss;
  
  /* Get starting time. */

  if(getusage(sys1, usr1) == false)
    die(__FILE__, __FUNCTION__, __LINE__, "cannot get rusage");
  
  set < team * > :: iterator teiter, teiterend;
  set < point * > :: iterator poiter, poiterend;
  
  for(level = 0; level < _numLevels; level++)
    {
      oss.str("");
      oss << " lev " << level << " ";

      /* Initialization. */
      
      initPoints();
      initTeams((level));
      //Only preform rprop on level 1
      if (!(level)) rprop();
      /* Evaluate initial set of teams. */
      
      for(poiter = _P.begin(), poiterend = _P.end(); poiter != poiterend; poiter++)
	for(teiter = _M.begin(), teiterend = _M.end(); teiter != teiterend; teiter++)
	  evaluate(*teiter, *poiter, oss.str());      
      
      /* Get some initial stats. */
      stats(-1, level);
      
      /* Main loop. */

      for(t = 0; t <= _t; t++)
	{ 
#ifdef MYDEBUG
	  cout << "scm:go " << t << " points";
	  for(poiter = _P.begin(); poiter != _P.end(); poiter++)
	    cout << " " << **poiter;
	  cout << endl;

	  cout << "scm:go " << t << " teams";
	  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
	    cout << " " << **teiter;
	  cout<< endl;
#endif
      
	  genPoints(t);
	  genTeams(t, level);
          
          //Only preform rprop on level 1
          if (!(level)) rprop();
	  for(poiter = _P.begin(), poiterend = _P.end(); poiter != poiterend; poiter++)	
	    {
	      for(teiter = _M.begin(), teiterend = _M.end(); teiter != teiterend; teiter++)
		{
		  /* Evaluate only if created this generation. */
	      
		  if((*poiter)->gtime() == t || (*teiter)->gtime() == t)
		    evaluate(*teiter, *poiter, oss.str());
		}
	    }
      
	  /* Select members into the next generation. */
      
	  selPoints(t);
	  selTeams(t);

	  cleanup(t, level);
	  
	  /* Record some stats. */
	  
	  if(t % _statMod == 0)
	    stats(t, level);
	}
  
      /* Record final stats. */
      
      stats(t, level);

      /* Record training time and print out time to train up to and including the current level. */
  
      if(getusage(sys2, usr2) == 0)
	die(__FILE__, __FUNCTION__, __LINE__, "cannot get rusage");
  
      cout << "rusage lev " << level;
      cout << " sys1 " << sys1 << " usr1 " << usr1 << " sum1 " << (sys1 + usr1);
      cout << " sys2 " << sys2 << " usr2 " << usr2 << " sum2 " << (sys2 + usr2);
      cout << " traintime = " << (sys2 + usr2) - (sys1 + usr1) << endl;

      /* Test the final population. */
      
      /* Solved points are moved to _solvedPoints when they are removed from _P
	 in the cleanup() method. Otherwise, have to move any solved points into 
	 _solvedPoints here. */

      for(poiter = _P.begin(), poiterend = _P.end(); poiter != poiterend; poiter++)	
	if((*poiter)->solved() == true)
	  saveTestPoint(level, *poiter);
  
      test(level);
  
      /* Cleanup for this level. */
      
      finalize();
    }
  
  /* Final cleanup. */

  finalfinalize();
}

/***********************************************************************************************************/

scmImplicit::scmImplicit(map < string, string > &args)
  : scm(args)
{
  long numTestPoints, numValidPoints;
  bool useBinaryFitness;
  string envType;
  
  map < string, string > :: iterator maiter;

  set < long > usedIds;
  int i;
  
  if((maiter = args.find("numTestPoints")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg numTestPoints");
  
  numTestPoints = stringToLong(maiter->second);

  if((maiter = args.find("numValidPoints")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg numValidPoints");
  
  numValidPoints = stringToLong(maiter->second);
  
  if((maiter = args.find("useBinaryFitness")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg useBinaryFitness");
  
  useBinaryFitness = stringToInt(maiter->second);
  
  if((maiter = args.find("envType")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg envType");
  
  envType = maiter->second;
  
  cout << "arg numTestPoints " << numTestPoints << endl;
  cout << "arg numValidPoints " << numValidPoints << endl;
  cout << "arg useBinaryFitness " << useBinaryFitness << endl;
  cout << "arg envType " << envType << endl;

  if(numTestPoints < 1 || numValidPoints < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg numTestPoints < 1 || numValidPoints < 1");
    
  if(envType == "backupEnv")
    {
      double mux, muy, muthetas, muthetac;

      if((maiter = args.find("mux")) == args.end())
	die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg mux");
      
      mux = stringToDouble(maiter->second);

      if((maiter = args.find("muy")) == args.end())
	die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg muy");
      
      muy = stringToDouble(maiter->second);

      if((maiter = args.find("muthetas")) == args.end())
	die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg muthetas");
      
      muthetas = stringToDouble(maiter->second);

      if((maiter = args.find("muthetac")) == args.end())
	die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg muthetac");
      
      muthetac = stringToDouble(maiter->second);

      _env = new backupEnv(_maxSteps, numTestPoints, numValidPoints, mux, muy, muthetas, muthetac, useBinaryFitness, _seed);
    }
  else if(envType == "rubikEnv")
    {
      int initTwists, mutateTwists;
      
      if((maiter = args.find("rubikInitTwists")) == args.end())
	die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg rubikInitTwists");
      
      initTwists = stringToLong(maiter->second);
      
      if((maiter = args.find("rubikMutateTwists")) == args.end())
	die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg rubikMutateTwists");
      
      mutateTwists = stringToLong(maiter->second);

      _env = new rubikEnv(_maxSteps, numTestPoints, numValidPoints, initTwists, mutateTwists, useBinaryFitness, _seed);
    }
    else if(envType == "rpropEnv")
    {
      
      if((maiter = args.find("lineDim")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg lineDim");
      
      long lineDim = stringToLong(maiter->second);
      
      if((maiter = args.find("maxRpropGens")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg maxRpropGens");
      
      long maxRpropGens = stringToLong(maiter->second);
      
      if((maiter = args.find("interval")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg interval");
      
      double interval = stringToDouble(maiter->second);
      
      if((maiter = args.find("initialWeight")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg initialWeight");
      
      double initialWeight = stringToDouble(maiter->second);
      
      if((maiter = args.find("initialUpdate")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg initialUpdate");
      
      double initialUpdate = stringToDouble(maiter->second);
      
      if((maiter = args.find("maxUpdate")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg maxUpdate");
      
      double maxUpdate = stringToDouble(maiter->second); 
      
      if((maiter = args.find("minUpdate")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg minUpdate");
      
      double minUpdate = stringToDouble(maiter->second);
      
      if((maiter = args.find("increaseFactor")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg increaseFactor");
      
      double increaseFactor = stringToDouble(maiter->second);
      
      if((maiter = args.find("decreaseFactor")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg decreaseFactor");
      
      double decreaseFactor = stringToDouble(maiter->second);
      
      if((maiter = args.find("rpropAccuracy")) == args.end())
        die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg rpropAccuracy");
      
      double rpropAccuracy = stringToDouble(maiter->second);

      _env = new rpropEnv(_maxSteps, lineDim, numTestPoints, numValidPoints, 
                          maxRpropGens, interval, initialWeight, initialUpdate, 
                          maxUpdate, minUpdate, increaseFactor, decreaseFactor, 
                          rpropAccuracy, useBinaryFitness, _seed);
    }
  else
    {
      die(__FILE__, __FUNCTION__, __LINE__, "bad arg envType");
    }
  
  _numActions = _env->numActions();

  /* Bidding behaviour. */

  for(i = 0; i < PROFILE_SIZE; i++)
    _profilePoints.push_back(_env->initUniformPoint(-3, usedIds));
}

/***********************************************************************************************************/

scmImplicit::~scmImplicit()
{
  for(int i = 0; i < _profilePoints.size(); i++)
    delete _profilePoints[i];
  
  delete _env;
}

/***********************************************************************************************************/

void scmImplicit::initPoints()
{
  set < long > usedIds;
  
  while(_P.size() < _Psize - _Pgap)
    _P.insert(_env->initUniformPoint(-1, usedIds));
}

/***********************************************************************************************************/

void scmImplicit::genPoints(long t)
{
  /* Generate points selecting parents using roulette wheel selection. */

#ifdef MYDEBUG
  cout << "scmImplicit::genPoints " << t  << endl;
#endif
  
  vector < point * > points;
  set < point * > :: iterator poiter;
  
  vector < double > wheel;
  double denom;
  double sum;
  double drnd;
  int i;
    
  set < long > usedIds;
      
  if(t != 0) /* Don't do for first generation as slice values are not set. */
    {
      for(poiter = _P.begin(); poiter != _P.end(); poiter++)
	points.push_back(*poiter);
      
      for(denom = 0, i = 0; i < points.size(); i++)
	{
	  wheel.push_back(points[i]->slice());
	  denom += wheel.back();
	}

#ifdef MYDEBUG
      cout << "scmImplicit " << t << " denom " << denom << " slices" << vecToStr(wheel) << endl;
#endif

      if(denom < 1e-5) /* All get 0 reward, let each have the same amount of pie. */
	{
	  wheel.clear();
	  wheel.insert(wheel.begin(), points.size(), 1);
	  denom = points.size();
	}

      for(sum = 0, i = 0; i < points.size(); i++)
	{
	  sum += wheel[i];
	  wheel[i] = sum / denom;
	}

#ifdef MYDEBUG
      cout << "scmImplicit " << t << " thresholds" << vecToStr(wheel) << endl;
#endif
      
      while(_P.size() < _Psize)
	{
	  if(drand48() < 0.9)
	    {
	      drnd = drand48();
	      
	      for(i = 0; i < wheel.size() && drnd > wheel[i]; i++)
		;
	      
	      if(i == wheel.size())
		i--;
	      
	      _P.insert(_env->genPoint(t, points[i], points[i]));
	    }
	  else
	    {
	      _P.insert(_env->initUniformPoint(t, usedIds));
	    }
	}
    }
  else
    {
      while(_P.size() < _Psize)
	_P.insert(_env->initUniformPoint(t, usedIds));
    }
}

/***********************************************************************************************************/

void scmImplicit::test(long level)
{
  // Commented out printout lines since the info is already printed out using operator<<.

  ostringstream oss;
  
  oss << "scmImplicit::test " << _seed << " lev " << level;
  
  set < team * > :: iterator teiter;
  set < learner * > learners;
  set < learner * > :: iterator leiter;
  
  team *maxTeam = *_M.begin();
  double maxScore = - HUGE_VAL;
  double score;

  bool equal;

  set < long > features; /* Unique features across all team members. */
  set < long > :: iterator seiter;
  long fcnt; /* Sum of features across all team members, possibly duplicate. */
  
  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
    {      
      learners.clear();
      (*teiter)->members(&learners);
      
      features.clear();
      (*teiter)->features(features);

      for(fcnt = 0, leiter = learners.begin(); leiter != learners.end(); leiter++)
	fcnt += (*leiter)->numFeatures();
      
//       cout << "team " << _seed << " lev " << level;
//       cout << " id " << (*teiter)->id();
//       cout << " size " << (*teiter)->size();
//       cout << " nodes " << (*teiter)->nodes();
//       cout << " gtime " << (*teiter)->gtime();
//       cout << " feat " << fcnt;
//       cout << " uniqfeat " << features.size();

//       cout << " id";
//       for(leiter = learners.begin(); leiter != learners.end(); leiter++)
// 	cout << " " << (*leiter)->id();
      
//       cout << " size";
//       for(leiter = learners.begin(); leiter != learners.end(); leiter++)
// 	cout << " " << (*leiter)->size();

//       cout << " esize";
//       for(leiter = learners.begin(); leiter != learners.end(); leiter++)
// 	cout << " " << (*leiter)->esize();

//       cout << " gtime";
//       for(leiter = learners.begin(); leiter != learners.end(); leiter++)
// 	cout << " " << (*leiter)->gtime();
      
//       cout << " refs";
//       for(leiter = learners.begin(); leiter != learners.end(); leiter++)
// 	cout << " " << (*leiter)->refs();
      
//       cout << " action";
//       for(leiter = learners.begin(); leiter != learners.end(); leiter++)
// 	cout << " " << (*leiter)->action();

//       cout << " numfeat";
//       for(leiter = learners.begin(); leiter != learners.end(); leiter++)
// 	cout << " " << (*leiter)->numFeatures();
      
//       cout << " featid";
//       for(seiter = features.begin(); seiter != features.end(); seiter++)
// 	cout << " " << *seiter;

//       cout << endl;

      cout << oss.str() << " reprint " << **teiter << endl;
      
      score = _env->rtest(*teiter, oss.str(), false, true);

      _env->test(*teiter, oss.str(), false);
      _env->rtest(*teiter, oss.str(), false, false);
      _env->test2(*teiter, oss.str(), false, _solvedPoints);

      equal = isEqual(score, maxScore, 1e-7);
      
      if(((equal == false) && (score > maxScore)) ||
	 ((equal == true) && ((*teiter)->size() < maxTeam->size())))
	{
	  maxScore = score;
	  maxTeam = *teiter;
	}
    }
  
  cout << maxTeam->toString("maxteam");

  oss << " best";
  cout << oss.str() << " " << *maxTeam << endl;

  _env->test(maxTeam, oss.str(), false);
  _env->rtest(maxTeam, oss.str(), true, false);
  _env->test2(maxTeam, oss.str(), false, _solvedPoints);
}

/***********************************************************************************************************/

void scmImplicit::stats(long t,
			long level)
{
  int sumTeamSizes = 0;
  int nrefs = 0;
  int sumNumOutcomes = 0;

  /* Actions in _L. */
  vector < long > actionCount;

  /* Quartiles for team sizes. */
  vector < int > msize;
  int mq1 = (int) (_M.size() * 0.25);
  int mq2 = (int) (_M.size() * 0.5);
  int mq3 = (int) (_M.size() * 0.75);

  /* Team node counts. */
  vector < int > mnode;

  /* Quartiles for team ref counts, counting references to each learner. */
  vector < int > lrefs;  
  vector < int > lsize;
  vector < int > lesize;
  int lrq1 = (int) (_L.size() * 0.25);
  int lrq2 = (int) (_L.size() * 0.5);
  int lrq3 = (int) (_L.size() * 0.75);
    
  set < point * > :: iterator poiter;
  set < learner * > :: iterator leiter;
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

  vector < double > state;

  vector < double > outcomes;
  map < point *, vector < short > * > dist;
  map < point *, vector < short > * > :: iterator diiter;
  vector < short > *dvec;
  long ndist;

  set < team * > :: iterator teiter1, teiter2;
  set < learner * > lset1, lset2, linter;
  int i1, i2;
  map < int, int > olaphist; /* Overlap histogram. */
   
  actionCount.insert(actionCount.begin(), numActions(level), 0);
  
  for(leiter = _L.begin(); leiter != _L.end(); leiter++)
    {
      actionCount[(*leiter)->action()]++;
      nrefs += (*leiter)->refs();
      lrefs.push_back((*leiter)->refs());
      lsize.push_back((*leiter)->size());
      lesize.push_back((*leiter)->esize());
    }
  
  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
    {
      msize.push_back((*teiter)->size());
      mnode.push_back((*teiter)->nodes());
      sumTeamSizes += (*teiter)->size();
      sumNumOutcomes += (*teiter)->numOutcomes();

      minOutcome = HUGE_VAL;
      meanOutcome = 0;

      for(poiter = _P.begin(); poiter != _P.end(); poiter++)
	{
	  if((*teiter)->getOutcome(*poiter, &outcome) == false)
	    die(__FILE__, __FUNCTION__, __LINE__, "cannot get outcome");

	  sumOutcomes += outcome;
	  meanOutcome += outcome;

	  if(outcome < minOutcome)
	    minOutcome = outcome;
	}

      if(minOutcome > maxMinOutcome)
	maxMinOutcome = minOutcome;

      if(meanOutcome > maxMeanOutcome)
	maxMeanOutcome = meanOutcome;
    }

  sort(msize.begin(), msize.end());
  sort(mnode.begin(), mnode.end());
  sort(lrefs.begin(), lrefs.end());
  sort(lsize.begin(), lsize.end());
  sort(lesize.begin(), lesize.end());
  
  cout << "scm::stats " << _seed << " t " << t << " lev " << level;
  cout << " mdom " << _mdom;
  cout << " pdom " << _pdom;
  cout << " Lsize " << _L.size();
  
  cout << " learners";
  for(int i = 0; i < actionCount.size(); i++)
    cout << " " << actionCount[i];
  
  cout << " msize " << msize[0] << " " << msize[mq1] << " " << msize[mq2] << " " << msize[mq3] << " " << msize.back();
  cout << " mnode " << mnode[0] << " " << mnode[mq1] << " " << mnode[mq2] << " " << mnode[mq3] << " " << mnode.back();
  cout << " lrefs " << lrefs[0] << " " << lrefs[lrq1] << " " << lrefs[lrq2] << " " << lrefs[lrq3] << " " << lrefs.back();
  cout << " lsize " << lsize[0] << " " << lsize[lrq1] << " " << lsize[lrq2] << " " << lsize[lrq3] << " " << lsize.back();
  cout << " lesize " << lesize[0] << " " << lesize[lrq1] << " " << lesize[lrq2] << " " << lesize[lrq3] << " " << lesize.back();

  cout << " meanOut " << sumOutcomes / (_M.size() * _P.size());
  cout << " maxMinOut " << maxMinOutcome;
  cout << " maxMeanOut " << maxMeanOutcome / _P.size();

  cout << " nrefs " << nrefs;

  cout << " tpsize " << _solvedPoints.size();

  cout << endl;

  /* Perform the following check to make sure that the number of
     references recorded in the learners is actually equal to the sum
     of the team sizes and that the outcome counts equal the number of
     points multiplied by the number of teams. */

  if(sumTeamSizes != nrefs || sumNumOutcomes != (_Msize - _Mgap) * (_Psize - _Pgap))
    {
      cout << "sumteamsizes " << sumTeamSizes << " nrefs " << nrefs << endl;
      die(__FILE__, __FUNCTION__, __LINE__, "something does not add up"); 
    }

  if(t % (10 * _statMod) == 0 || t == _t || t == -1 || (t < 0.11 *_t && t % _statMod == 0))
    _env->testAbsolute(_M, _seed, t, level);
  
  /* Dump the points every so often. */
  if(t % (10 * _statMod) == 0 || t == _t || t == -1 || (t < 0.11 *_t && t % _statMod == 0))
    {
      getDistinctions(_P, dist);

      for(poiter = _P.begin(); poiter != _P.end(); poiter++)
	{
	  ndist = 0;
	  outcomes.clear();

	  if((diiter = dist.find(*poiter)) == dist.end())
	    die(__FILE__, __FUNCTION__, __LINE__, "cannot get distinctions");
	  
	  dvec = diiter->second;

	  for(long i = 0; i < dvec->size(); i++)
	    ndist += (*dvec)[i];
	  
	  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
	    {
	      if((*teiter)->getOutcome(*poiter, &outcome) == false)
		die(__FILE__, __FUNCTION__, __LINE__, "outcome not found");
	      
	      outcomes.push_back(outcome);
	    }

	  sort(outcomes.begin(), outcomes.end());
	  
	  (*poiter)->state(state);
	  
	  cout << "scmImplicit::stats pointdump " << _seed << " t " << t << " lev " << level;
	  cout << " id " << (*poiter)->id();
	  cout << " age " << t - (*poiter)->gtime();
	  cout << " ndist " << ndist;
	  cout << " out " << outcomes[0] << " " << outcomes[mq1] << " " << outcomes[mq2] << " " << outcomes[mq3] << " " << outcomes.back();
	  cout << " state" << vecToStr(state);
	  cout << endl;
	}

      /* Free allocated array. */
      
      for(diiter = dist.begin(); diiter != dist.end(); diiter++)
	delete diiter->second;
    }
  
  if(t % (10 * _statMod) == 0 || t == _t || t == -1 || (t < 0.11 *_t && t % _statMod == 0))
    {
      /* Team overlap. */

      /* Maps shared learner count to how many teams share that many learners. */
      
      for(int i = 0; i <= _omega; i++)
	olaphist.insert(map < int, int > :: value_type(i, 0));
      
      cout << "scmImplicit::stats " << _seed << " t " << t << " lev " << level;
      cout << " sharedlearners " << _M.size() << " ids";
      for(teiter1 = _M.begin(); teiter1 != _M.end(); teiter1++)
	cout << " " << (*teiter1)->id();
      cout << " matrix";
      
      for(i1 = 0, teiter1 = _M.begin(); teiter1 != _M.end(); i1++, teiter1++)
	  {
	    for(i2 = 0, teiter2 = _M.begin(); teiter2 != _M.end(); i2++, teiter2++)
	      {
		if(i2 < i1) /* Already considered this pair. */
		  {
		    cout << " 0";
		  }
		else if(i2 > i1) /* Haven't considered this pair yet. */
		  {
		    lset1.clear();
		    lset2.clear();
		    linter.clear();
		    
		    (*teiter1)->members(&lset1);
		    (*teiter2)->members(&lset2);
		    
		    set_intersection(lset1.begin(), lset1.end(),
				     lset2.begin(), lset2.end(),
				     insert_iterator < set < learner * > > (linter, linter.begin()));
		    
		    cout << " " << linter.size();
		    
		    olaphist[linter.size()]++;
		  }
		else /* Same team, i1 == i2. */
		  {
		    cout << " " << (*teiter1)->size();
		  }
	      }
	  }
      cout << endl;
      
      cout << "scmImplicit::stats " << _seed << " t " << t << " lev " << level << " olaphist mat";
      for(int i = 0; i <= _omega; i++)
	cout << " " << olaphist[i];
      cout << endl;
    }
}

/***********************************************************************************************************/

scmExplicit::scmExplicit(map < string, string > &args)
  : scm(args)
{
  string trainSetName, testSetName;
  long trainSetSize, testSetSize;
  long setDim;
  string envType;

  map < string, string > :: iterator maiter;
  
  set < long > usedIds;
  long label;
  point *pt;
  map < long, long > counts;
  int i;

  if((maiter = args.find("trainSetName")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg trainSetName");
  
  trainSetName = maiter->second;

  if((maiter = args.find("testSetName")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg testSetName");
  
  testSetName = maiter->second;

  if((maiter = args.find("trainSetSize")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg trainSetSize");
  
  trainSetSize = stringToLong(maiter->second);
  
  if((maiter = args.find("testSetSize")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg testSetSize");
  
  testSetSize = stringToLong(maiter->second);

  if((maiter = args.find("setDim")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg setDim");
  
  setDim = stringToLong(maiter->second);
  
  if((maiter = args.find("envType")) == args.end())
    die(__FILE__, __FUNCTION__, __LINE__, "cannot find arg envType");
  
  envType = maiter->second;
  
  cout << "arg trainSetName " << trainSetName << endl;
  cout << "arg testSetName " << testSetName << endl;
  cout << "arg trainSetSize " << trainSetSize << endl;
  cout << "arg testSetSize " << testSetSize << endl;
  cout << "arg setDim " << setDim << endl;
  cout << "arg envType " << envType << endl;
  
  if(setDim < 1 || setDim > INPUTS)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg setDim < 1 || setDim > INPUTS");
  
  if(trainSetSize < 1 || testSetSize < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg setSize < 1 || testSetSize < 1");
  
  if(envType == "datasetEnv")
    {
      _trainEnv = new datasetEnv(trainSetName, setDim, trainSetSize, _maxSteps);
      _testEnv = new datasetEnv(testSetName, setDim, testSetSize, _maxSteps);
    }
  else
    {
      die(__FILE__, __FUNCTION__, __LINE__, "bad arg envType");
    }
  
  _numActions = _trainEnv->numLabels();

  if(_numActions > _omega)
    die(__FILE__, __FUNCTION__, __LINE__, "numActions > omega");

  if(_trainEnv->numLabels() != _testEnv->numLabels())
    die(__FILE__, __FUNCTION__, __LINE__, "different number of labels in train/test data");
  
  for(i = 0; i < _numActions; i++)
    if(_trainEnv->getLabel(i) != _testEnv->getLabel(i))
      die(__FILE__, __FUNCTION__, __LINE__, "labels do not match");
  cout << "arg numActions train/test " << _numActions << "/" << _testEnv->numLabels() << endl;
  
  if(_numActions < 2)
    die(__FILE__, __FUNCTION__, __LINE__, "bad arg numActions < 2");  
  
  /* Bidding behaviour, essentially copied from initPoints(). */

  if(_trainEnv->size() < PROFILE_SIZE)
    die(__FILE__, __FUNCTION__, __LINE__, "PROFILE_SIZE too bid");  

  for(i = 0; i < _trainEnv->numLabels(); i++)
    counts.insert(map < long, long > :: value_type (_trainEnv->getLabel(i), 0));
  
  for(i = 0; i < PROFILE_SIZE; i++)
    {
      do 
	{
	  /* Get random label so long as don't already have all points of that label. */
	  label = _trainEnv->getLabel((int) (drand48() * numActions(0)));
	} while(counts[label] == _trainEnv->getLabelCount(label));
      
      pt = _trainEnv->initUniformPointLabel(-3, usedIds, label);
      
      if(pt == 0)
	die(__FILE__, __FUNCTION__, __LINE__, "cannot get point");     
      
      usedIds.insert(pt->id());
      _profilePoints.push_back(pt);
      
      counts[label]++;
    }
}

/***********************************************************************************************************/

scmExplicit::~scmExplicit()
{
  for(int i = 0; i < _profilePoints.size(); i++)
    delete _profilePoints[i];

  delete _trainEnv;
  delete _testEnv;
}
 
/***********************************************************************************************************/

void scmExplicit::initPoints()
{
  set < long > usedIds;
  long label;
  point *pt;

  /* Count the number of points of each label. */
  map < long, long > counts;

  for(int i = 0; i < _trainEnv->numLabels(); i++)
    counts.insert(map < long, long > :: value_type (_trainEnv->getLabel(i), 0));
  
  while(_P.size() < _Psize - _Pgap)
    {
      do
	{
	  /* Get random label so long as don't already have all points of that label. */
	  label = _trainEnv->getLabel((int) (drand48() * numActions(0)));
	} while(counts[label] == _trainEnv->getLabelCount(label));

      pt = _trainEnv->initUniformPointLabel(-1, usedIds, label);
      
      if(pt == 0)
	die(__FILE__, __FUNCTION__, __LINE__, "cannot get point");     
      
      usedIds.insert(pt->id());
      _P.insert(pt);
      
      counts[label]++;
      
#ifdef MYDEBUG
      cout << "scmExplicit::initPoints added " << *pt << " _P.size() " << _P.size() << endl;
#endif
    }
}

/***********************************************************************************************************/

void scmExplicit::genPoints(long t)
{
  set < long > usedIds;
  long label;
  point *pt;
  
  set < point * > :: iterator poiter, poiterend;
  
  /* Count the number of points of each label. */
  map < long, long > counts;
  map < long, long > :: iterator coiter;

  for(int i = 0; i < _trainEnv->numLabels(); i++)
    counts.insert(map < long, long > :: value_type (_trainEnv->getLabel(i), 0));
  
  /* Tabulate label counts and point ids in current population. */
  for(poiter = _P.begin(), poiterend = _P.end(); poiter != poiterend; poiter++)
    {
      usedIds.insert((*poiter)->id());
      
      if((coiter = counts.find((*poiter)->label())) == counts.end())
	die(__FILE__, __FUNCTION__, __LINE__, "bad label");
      
      (coiter->second)++;
    }

#ifdef MYDEBUG
  cout << "scmExplicit::genPoints counts";
  for(coiter = counts.begin(); coiter != counts.end(); coiter++)
    cout << " " << coiter->first << "->" << coiter->second;
  cout << endl;
#endif
  
  while(_P.size() < _Psize)
    {
      do
	{
	  /* Get random label so long as don't already have all points of that label. */
	  label = _trainEnv->getLabel((int) (drand48() * numActions(0)));
	} while(counts[label] == _trainEnv->getLabelCount(label));
      
      pt = _trainEnv->initUniformPointLabel(t, usedIds, label);

      if(pt == 0)
	die(__FILE__, __FUNCTION__, __LINE__, "cannot get point");     
      
      usedIds.insert(pt->id());
      _P.insert(pt);

      counts[label]++;
      
#ifdef MYDEBUG
      cout << "scmExplicit::genPoints added " << *pt << " _P.size() " << _P.size() << endl;
#endif
    }
}

/***********************************************************************************************************/

void scmExplicit::test(long level)
{
  if(_M.size() < 1)
    die(__FILE__, __FUNCTION__, __LINE__, "no teams to test");     
  
  ostringstream oss;
  
  oss << _seed << " lev " << level;
  
  set < team * > :: iterator teiter, maxiter;
  
  double score;
  double maxScore = - HUGE_VAL;
  
  bool equal;

  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
    {
      cout << oss.str() << " " << **teiter << endl;

      score = _trainEnv->test(*teiter, oss.str(), false);
      
      if(score < 0)
	die(__FILE__, __FUNCTION__, __LINE__, "bad score");     
      
      equal = isEqual(score, maxScore, 1e-7);
      
      if(((equal == false) && (score > maxScore)) ||
	 ((equal == true) && ((*teiter)->size() < (*maxiter)->size())))
	{
	  /* Select this team if the score is greater than maxScore or if the scores are
	     the same but the size of this team is smaller. */

	  /* Will reach here on the first iteration since maxScore is very small, so
	     maxIter will be initialized at that point. */
	  
	  maxScore = score;
	  maxiter = teiter;
	}
    }

  cout << (*maxiter)->toString("maxteam");
  
  oss << " bestteam";
  cout << oss.str() << " " << **maxiter << endl;

  _testEnv->test(*maxiter, oss.str(), true);

  cout << (*maxiter)->printBids(oss.str());
}

/***********************************************************************************************************/

void scmExplicit::stats(long t,
			long level)
{
  int sumTeamSizes = 0;
  int nrefs = 0;
  int sumNumOutcomes = 0;

  /* Labels in _P. */
  vector < long > labelCount;
  /* Actions in _L. */
  vector < long > actionCount;

  /* Quartiles for team sizes. */
  vector < int > msize;
  int mq1 = (int) (_M.size() * 0.25);
  int mq2 = (int) (_M.size() * 0.5);
  int mq3 = (int) (_M.size() * 0.75);

  /* Team node counts. */
  vector < int > mnode;
  
  /* Quartiles for team ref counts, counting references to each learner. */
  vector < int > lrefs;  
  vector < int > lsize;
  vector < int > lesize;
  int lrq1 = (int) (_L.size() * 0.25);
  int lrq2 = (int) (_L.size() * 0.5);
  int lrq3 = (int) (_L.size() * 0.75);
  
  set < point * > :: iterator poiter;
  set < learner * > :: iterator leiter;
  set < team * > :: iterator teiter;
  
  labelCount.insert(labelCount.begin(), numActions(0), 0);
  actionCount.insert(actionCount.begin(), numActions(level), 0);
  
  for(poiter = _P.begin(); poiter != _P.end(); poiter++)
    labelCount[_trainEnv->getLabelIndex((*poiter)->label())]++;

  for(leiter = _L.begin(); leiter != _L.end(); leiter++)
    {
      actionCount[(*leiter)->action()]++;
      nrefs += (*leiter)->refs();
      lrefs.push_back((*leiter)->refs());
      lsize.push_back((*leiter)->size());
      lesize.push_back((*leiter)->esize());
    }
  
  for(teiter = _M.begin(); teiter != _M.end(); teiter++)
    {
      msize.push_back((*teiter)->size());
      mnode.push_back((*teiter)->nodes());
      sumTeamSizes += (*teiter)->size();
      sumNumOutcomes += (*teiter)->numOutcomes();
    }
  
  sort(msize.begin(), msize.end());
  sort(mnode.begin(), mnode.end());
  sort(lrefs.begin(), lrefs.end());
  sort(lsize.begin(), lsize.end());
  sort(lesize.begin(), lesize.end());
  
  cout << "scm::stats " << _seed << " " << t << " lev " << level;
  cout << " mdom " << _mdom;
  cout << " pdom " << _pdom;
  cout << " Lsize " << _L.size();

  cout << " labels";
  for(int i = 0; i < numActions(0); i++)
    cout << " " << _trainEnv->getLabel(i);
  
  cout << " points";
  for(int i = 0; i < labelCount.size(); i++)
    cout << " " << labelCount[i];

  cout << " learners";
  for(int i = 0; i < actionCount.size(); i++)
    cout << " " << actionCount[i];
  
  cout << " msize " << msize[0] << " " << msize[mq1] << " " << msize[mq2] << " " << msize[mq3] << " " << msize.back();
  cout << " mnode " << mnode[0] << " " << mnode[mq1] << " " << mnode[mq2] << " " << mnode[mq3] << " " << mnode.back();
  cout << " lrefs " << lrefs[0] << " " << lrefs[lrq1] << " " << lrefs[lrq2] << " " << lrefs[lrq3] << " " << lrefs.back();
  cout << " lsize " << lsize[0] << " " << lsize[lrq1] << " " << lsize[lrq2] << " " << lsize[lrq3] << " " << lsize.back();
  cout << " lesize " << lesize[0] << " " << lesize[lrq1] << " " << lesize[lrq2] << " " << lesize[lrq3] << " " << lesize.back();

  cout << " tpsize " << _solvedPoints.size();
  cout << " nrefs " << nrefs;

  cout << endl;

  /* Preform the following check to make sure that the number of
     references recoreded in the learners is actually equal to the sum
     of the team sizes and that the outcome counts equal the number of
     points by the number of teams. */

  if(sumTeamSizes != nrefs || sumNumOutcomes != (_Msize - _Mgap) * (_Psize - _Pgap))
    die(__FILE__, __FUNCTION__, __LINE__, "something does not add up"); 
}

/***********************************************************************************************************/





