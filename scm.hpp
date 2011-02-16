#ifndef scm_hpp
#define scm_hpp

#include <map>
#include <set>
#include <algorithm>
#include <iomanip>

#include "point.hpp"
#include "team.hpp"
#include "learner.hpp"
#include "env.hpp"

#define USE_DISTINCTIONS 0
#define KFACTOR 10

/***********************************************************************************************************/

class scm
{
  void genUniqueLearner(learner *, set < learner * >);
  
  void genTeams(long, team *, team **, vector < learner * > &, long);
  
  template < class ptype, class vtype > void sharingScorePie(map < ptype *, vector < vtype > * > &, map < ptype *, double > &, set < ptype * > &, set < ptype * > &, int);
  
  void cleanup(long, long);
  void finalize();
  void finalfinalize();

  /* Save solved point in _solvedPoints, return true if saved. */
  bool saveTestPoint(long, point *);

  virtual long dim() = 0;

  /* Remove inactive learners. If true is passed check if team size equals _omega first. */
  void pruneLearners(bool);
  
protected:

  /* Number of levels to train for. */
  int _numLevels;

  /* Number of actions, actions 0, 1, ..., _numActions - 1 are assumed. */
  long _numActions;

  /* Populations at the current level begin trained. */

  set < point * > _P; /* Points. */
  set < team * > _M; /* Teams */
  set < learner * > _L; /* Learners. */

  /* 
     Store teams from previous levels here. Element _mLevel[i]
     contains the teams evolved at level i - 1. So, if evolving
     level 1 teams, the actions available to those teams are in
     _mLevel[1].
     
     Also save the point and learners at each level using the same
     convention.

     Hence, teams accessed by level 1 teams are at _mLevel[1], and
     _mLevel[0] should never be accessed.
  */

  vector < vector < point * > * > _pLevel;
  vector < vector < team * > * > _mLevel;
  vector < vector < learner * > * > _lLevel;
  
  /* Point population size. */
  int _Psize;
  /* Team population size. */
  int _Msize;
  /* Probability of learner deletion.*/
  double _pmd;
  /* Probability of learner addition. */
  double _pma;
  /* Probability of learner mutation. */
  double _pmm;
  /* Probability of learner action mutation. */
  double _pmn;
  /* Maximum team size. */
  int _omega;
  /* Maximum number of training epochs. */
  long _t;
  /* Point generation gap. */
  int _Pgap;
  /* Team generation gap. */
  int _Mgap;
  
  /* GP parameters. */
  int _maxProgSize;
  double _pBidDelete;
  double _pBidAdd;
  double _pBidSwap;
  double _pBidMutate;

  /* How often stats are printed out. */
  long _statMod;
  
  /* Random number seed. */
  int _seed;

  /* Thresholds for determining if two values are equal. */
  double _paretoEpsilonPoint; /* Possible that this is not used. */
  double _paretoEpsilonTeam;

  /* Maximum number of time steps per episode. */
  long _maxSteps;
  
  /* Power for calculating sharing score. */
  int _pointPow;
  int _teamPow;
  
  /* Number of individuals pushed out of the population by younger individuals. */
  
  long _mdom;
  long _pdom;
  
  /* Points used to build the learner profiles. */
  vector < point * > _profilePoints;

  /* Unique points solved so far during training. */
  set < point * > _solvedPoints;
  
  void getDistinctions(set < point * > &, map < point *, vector < short > * > &);
  
  /* Number of actions available to the learners at the specified
     level.
     
     Define numActions(0) as number of actions in the environment,
     else, it's the number of teams in the level below that can be
     referenced by the specified level.
  */
  inline long numActions(long level)
  { if(level == 0) return _numActions; return _mLevel[level]->size(); }
  
  void initTeams(long); 
  void genTeams(long, long);
  void selPoints(long);
  void selTeams(long); 
    
  virtual void initPoints() = 0;
  virtual void genPoints(long) = 0;

  virtual void evaluate(team *, point *, string) = 0;
  
  virtual void stats(long, long) = 0;
  virtual void test(long) = 0;

  /* Get distances between all points, return the maximum. */
  double getDistances(set < point * > &, map < point *, map < point *, double > * > &);
  
  virtual double distance(vector < double > &, vector < double > &) = 0;

  virtual string myclass() { return "scm"; }
  
public:

  scm(map < string, string > &);
  virtual ~scm() {};

  void go();
};

/***********************************************************************************************************/

class scmImplicit : public scm
{
  implicitEnv *_env;
  
  /* Between two state vectors. */
  inline double distance(vector < double > &v1, vector < double > &v2)
  { return _env->distance(v1, v2); }

  inline long dim()
  { return _env->dim(); }
  
public:
  
  scmImplicit(map < string, string > &);
  ~scmImplicit();
  
  void initPoints();
  void genPoints(long); 
  
  inline void evaluate(team *m, point *p, string prefix) 
  { double n; m->setOutcome(p, _env->evaluate(m, p, false, n, prefix, true)); m->setMargin(p, n); }

  void stats(long, long);

  void test(long);

  virtual string myclass() { return "scmImplicit"; }
};

class scmExplicit : public scm
{

protected:
  
  explicitEnv *_trainEnv; /* Training data. */
  explicitEnv *_testEnv; /* Test data. */

  /* Effectively, no distance metric defined. */
  inline double distance(vector < double > &v1, vector < double > &v2)
  { if(v1.size() != v2.size()) {}; return 1; }

  inline long dim() 
  { return _trainEnv->dim(); }
  
public:
  
  scmExplicit(map < string, string > &);
  ~scmExplicit();

  void initPoints();
  void genPoints(long);
  
  inline void addTeam(team *m)
  { _M.insert(m); }

  inline void evaluate(team *m, point *p, string prefix) 
  { double n; m->setOutcome(p, _trainEnv->evaluate(m, p, false, n, prefix, true)); m->setMargin(p, n); }

  void stats(long, long);
  void test(long); 
  
  virtual string myclass() { return "scmExplicit"; }
};

/***********************************************************************************************************/

template < class ptype > struct lessThan : public binary_function < ptype *, ptype *, bool >
{
  bool operator() (ptype *lhs, ptype *rhs) { return lhs->key() < rhs->key(); }
};

/***********************************************************************************************************/

#endif
