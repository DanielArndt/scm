#ifndef team_hpp
#define team_hpp

#include <set>
#include <map>

#include "point.hpp"
#include "learner.hpp"
#include "misc.hpp"

using namespace std;

class team
{
  /* Next id to use. */
  static long _count;

  /* 
     If the team is at level 0, the action of each learner corresponds to an action
     in the environment.
     
     If the team is at any other level, the action of each learner is an index into
     _actions.
  */
  
  /* The level the team is at. */
  long _level;
  /* Teams in the level directly below. */
  vector < team * > *_actions;

  /* Unique id of team. */
  long _id;
  /* Time step at which generated. */
  long _gtime;
  
  /* The member learners. */
  set < learner * > _members;
  
  /* Active member learners, a subset of _members, activated in getAction(). */
  set < learner * > _active;
  
  /* Maps point->outcome. */
  map < point *, double > _outcomes;
  
  /* Maps point->margin. */
  map < point *, double > _margins;

  /* For sorting. */
  double _key;

  /* Number of nodes at all levels assuming this team is the root. */
  int _nodes;

public:
  
  team(long level, vector < team * > *actions, long gtime) 
    : _level(level), _actions(actions), _id(_count++), _gtime(gtime), _key(0), _nodes(1) {};
  ~team(); /* Affects learner refs, unlike addLearner() and removeLearner(). */
  
  inline long level()
  { return _level; }
  
  inline int size() const /* This is the team size at the current level. */
  { return _members.size(); }
  inline int nodes() const /* This is the number of nodes at all levels. */
  { return _nodes; }
  inline int asize() /* The number of active learners in this team. */
  { return _active.size(); }
  
  inline long gtime() const
  { return _gtime; }
  inline long id() const
  { return _id; }
  
  inline double key() 
  { return _key; }
  inline void key(double key) 
  { _key = key; }

  bool addLearner(learner *);
  void removeLearner(learner *);
  
  inline void members(set < learner * > *m) 
  { m->insert(_members.begin(), _members.end()); }

  inline void activeMembers(set < learner * > *m)
  { m->insert(_active.begin(), _active.end()); }
  
  /* Get outcome, return true if found. */
  bool getOutcome(point *, double *);
  /* Set outcome. */
  void setOutcome(point *, double);
  /* Delete outcome. */
  void deleteOutcome(point *);
  /* Number of outcomes. */
  inline long numOutcomes() { return _outcomes.size(); }
  
  /* Get margin, return true if found. */
  bool getMargin(point *, double *);
  /* Set margin. */
  void setMargin(point *, double);
  /* Delete margin. */
  void deleteMargin(point *);
  /* Number of margins. */
  inline long numMargins() { return _margins.size(); }

  void features(set < long > &) const;

  long getAction(vector < double > &, vector < learner * > &, vector < double > &, vector < double > &, bool);

  string toString(string);
  string printBids(string);
  
  friend ostream & operator<<(ostream &, const team &);
};

#endif
