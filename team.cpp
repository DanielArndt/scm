#include "team.hpp"

long team::_count = 0;

team::~team()
{
  set < learner * > :: iterator leiter, leiterend;

  for(leiter = _members.begin(), leiterend = _members.end(); 
      leiter != leiterend; leiter++)
    (*leiter)->refDec();
}

bool team::addLearner(learner *lr)
{
  bool added;

  added = (_members.insert(lr)).second;
  
  if(added == true) /* Update node count. */
    {
      if(_level == 0) /* Added a learner. */
	_nodes += 1; /* Leaf. */
      else /* Added a team. */
	_nodes += (*_actions)[lr->action()]->nodes(); /* Internal node. */
    }

  return added;
}

void team::removeLearner(learner *lr)
{
  set < learner * > :: iterator leiter;
  
  if((leiter = _members.find(lr)) == _members.end())
    die(__FILE__, __FUNCTION__, __LINE__, "should not remove learner that is not there");

  if(_level == 0)
    _nodes -= 1;
  else
    _nodes -= (*_actions)[lr->action()]->nodes();
  
  _members.erase(leiter);

  if((leiter = _active.find(lr)) != _active.end())
    _active.erase(leiter);
}


bool team::getOutcome(point *pt,
		      double *out)
{
  map < point *, double > :: iterator ouiter;
  
  if((ouiter = _outcomes.find(pt)) == _outcomes.end())
    return false;

  *out = ouiter->second;
  
  return true;
}

void team::setOutcome(point *pt,
		      double out)
{
  if((_outcomes.insert(map < point *, double >::value_type(pt, out))).second == false)
    die(__FILE__, __FUNCTION__, __LINE__, "could not set outcome, duplicate point?");
}

void team::deleteOutcome(point *pt)
{
  map < point *, double > :: iterator ouiter;
  
  if((ouiter = _outcomes.find(pt)) == _outcomes.end())
    die(__FILE__, __FUNCTION__, __LINE__, "should not delete outcome that is not set");

  _outcomes.erase(ouiter);
}

bool team::getMargin(point *pt,
		     double *margin)
{
  map < point *, double > :: iterator maiter;
  
  if((maiter = _margins.find(pt)) == _margins.end())
    return false;
  
  *margin = maiter->second;
  
  return true;
}

void team::setMargin(point *pt,
		     double margin)
{
  if((_margins.insert(map < point *, double >::value_type(pt, margin))).second == false)
    die(__FILE__, __FUNCTION__, __LINE__, "could not set margin, duplicate point?");
}

void team::deleteMargin(point *pt)
{
  map < point *, double > :: iterator maiter;
  
  if((maiter = _margins.find(pt)) == _margins.end())
    die(__FILE__, __FUNCTION__, __LINE__, "should not delete margin that is not set");
  
  _margins.erase(maiter);
}

long team::getAction(vector < double > &state,
		     vector < learner * > &winner,
		     vector < double > &bid1,
		     vector < double > &bid2,
		     bool updateActive)
{
  /* 
     Returns the index of the action to be taken in the environment, i.e., the
     action of the winning bidder in the level 0 team. 
     
     The vector element winner[i] contains the winning learner at level i so the
     return value should be the same as winner[0]->action() (bid1, bid2, analogous).
  */

  if(_members.size() < 2)
    die(__FILE__, __FUNCTION__, __LINE__, "team is too small");
  
  set < learner * > :: iterator leiter, wiiter, leiterend;
  
  double maxBid1; /* Highest bid. */
  double maxBid2 = - HUGE_VAL; /* Second highest bid. */
  double nextBid;
  
  double *REG = (double *) alloca(REGISTERS * sizeof(double));
  
  /* Initialize highest bid to that of first learner. */

  wiiter = leiter = _members.begin();
  maxBid1 = (*wiiter)->bid(&state[0], REG);

#ifdef MYDEBUG
  cout << "team::getAction id " << _id << " lev " << _level;
  cout << " winner"; for(int i = 0; i < winner.size(); i++) cout << " " << winner[i]->id();
  cout << " bid1" << vecToStr(bid1) << " bid2" << vecToStr(bid2);
  cout << " bids " << (*wiiter)->id() << "->" << (*wiiter)->action() << "->" << maxBid1;
#endif
  
  for(leiter++, leiterend = _members.end(); /* Go through the rest of the learners. */
      leiter != leiterend; leiter++)
    {
      nextBid = (*leiter)->bid(&state[0], REG);
      
#ifdef MYDEBUG
      cout << " " << (*leiter)->id() << "->" << (*leiter)->action() << "->" << nextBid;
#endif
      
      if(nextBid > maxBid1) /* Found new highest bidder. */
	{
	  maxBid2 = maxBid1;
	  maxBid1 = nextBid;
	  
	  wiiter = leiter;
	}
      else if(nextBid > maxBid2) /* Found new second highest bidder. */
	{
	  maxBid2 = nextBid;
	}
    }
  
  bid1.insert(bid1.begin(), maxBid1);
  bid2.insert(bid2.begin(), maxBid2);

  winner.insert(winner.begin(), *wiiter);

  /* Mark this learner as active. */
  if(updateActive == true)
    {
      _active.insert(*wiiter);
    }

#ifdef MYDEBUG
  cout << " winner learnerid " << (*wiiter)->id();
  if(_level != 0)
    cout << " teamid " << (*_actions)[(*wiiter)->action()]->id();
  else
    cout << " teamid -1";
  cout << " bid1 " << maxBid1 << " bid2 " << maxBid2;
  cout << endl;
#endif
  
  if(_level == 0) 
    return (*wiiter)->action(); /* Just return the action. */
  
  /* Repeate at the chosen team in the level below (don't update active learners). */
  
  return (*_actions)[(*wiiter)->action()]->getAction(state, winner, bid1, bid2, false);
}

void team::features(set < long > &F) const
{
  set < learner * > :: iterator leiter;
  
  if(F.empty() == false)
    die(__FILE__, __FUNCTION__, __LINE__, "feature set not empty");
  
  for(leiter = _members.begin(); leiter != _members.end(); leiter++)
    (*leiter)->features(F);
}

string team::toString(string prefix)
{
  set < learner * > :: iterator leiter;

  ostringstream oss, osslev;

  osslev << _level;
  prefix = prefix + " " + osslev.str();

  oss << prefix << " team::toString id " << _id;
  oss << " gtime " << _gtime;
  oss << " lev " << _level;
  oss << " nodes " << _nodes;
  oss << " size " << _members.size();
  oss << endl;

  prefix = "   " + prefix;

  if(_level == 0)
    {
      oss << prefix << " learners";
      
      for(leiter = _members.begin(); leiter != _members.end(); leiter++)
	oss << " " << (*leiter)->id() << "->" << (*leiter)->action();
      oss << endl;
    }
  else
    {
      for(leiter = _members.begin(); leiter != _members.end(); leiter++)
	oss << (*_actions)[(*leiter)->action()]->toString(prefix);
    }

  return oss.str();
}

ostream & operator<<(ostream &os, 
		     const team &tm)
{
  set < long > features;
  set < long > :: iterator seiter;
  long sumfeat, sumlsize, sumlesize;

  set < learner * > :: iterator leiter;
  
  tm.features(features);
  
  for(sumfeat = 0, sumlsize = 0, sumlesize = 0,
	leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    {
      sumfeat += (*leiter)->numFeatures();
      sumlsize += (*leiter)->size();
      sumlesize += (*leiter)->esize();
    }

  os << "teamout";
  os << " id " << tm.id();
  os << " size " << tm.size();
  os << " nodes " << tm.nodes();
  os << " gtime " << tm.gtime();
  os << " feat " << sumfeat << " " << (double) sumfeat / tm.size();
  os << " uniqfeat " << features.size() << " " << (double) features.size() / tm.size();
  os << " lsize " << sumlsize << " " << (double) sumlsize / tm.size();
  os << " lesize " << sumlesize << " " << (double) sumlesize / tm.size();
  
  os << " id";
  for(leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    os << " " << (*leiter)->id();

  os << " size";
  for(leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    os << " " << (*leiter)->size();
  
  os << " esize";
  for(leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    os << " " << (*leiter)->esize();
  
  os << " gtime";
  for(leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    os << " " << (*leiter)->gtime();
      
  os << " refs";
  for(leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    os << " " << (*leiter)->refs();
  
  os << " action";
  for(leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    os << " " << (*leiter)->action();
  
  os << " numfeat";
  for(leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    os << " " << (*leiter)->numFeatures();

  os << " lfeat";
  for(leiter = tm._members.begin(); leiter != tm._members.end(); leiter++)
    {
      os << " lact " << (*leiter)->action() << " lfid";
      features.clear();
      (*leiter)->features(features);
      for(seiter = features.begin(); seiter != features.end(); seiter++)
	os << " " << *seiter;
    }

  features.clear();
  tm.features(features);

  os << " featid";
  for(seiter = features.begin(); seiter != features.end(); seiter++)
    os << " " << *seiter;
    
  return os;
}

string team::printBids(string prefix)
{
  set < learner * > :: iterator leiter;

  ostringstream oss, ossout;
  int i;

  for(i = 0, leiter = _members.begin(); leiter != _members.end(); i++, leiter++)
    {
      oss.str("");
      oss << prefix << " " << i << " lid " << (*leiter)->id() << " act " << (*leiter)->action();
      oss << " size " << (*leiter)->size() << " esize " << (*leiter)->esize();
      ossout << (*leiter)->printBid(oss.str());
    }

  return ossout.str();
}
