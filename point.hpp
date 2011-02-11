#ifndef point_hpp
#define point_hpp

#include <vector>
#include <map>
#include <set>

#include "misc.hpp"

using namespace std;

class point
{
  long _id;
  long _gtime;
  vector < double > _state;
  long _label;

  /* For sorting. */
  double _key;
  
  bool _marked;

  /* If this point has been solved at some time. */
  bool _solved;

  /* For roulette wheel. */
  double _slice;

  /* Some integer, can be used by the environment as required. */
  int _someint;
  
public:

  point(long, int, long, const double *, long);
  
  inline int dim() 
  { return _state.size(); }
  inline void state(vector < double > &v) 
  { v = _state; }
  inline long label() 
  { return _label; }
  inline long id() 
  { return _id; }
  inline long gtime() 
  { return _gtime; }
  
  inline double key() 
  { return _key; }
  inline void key(double key) 
  { _key = key; }
  
  inline bool marked() 
  { return _marked; }
  inline void mark() 
  { _marked = 1; }
    
  inline void solved(bool s)
  { _solved = s; }
  inline bool solved()
  { return _solved; }

  inline double slice() 
  { return _slice; }
  inline void slice(double slice) 
  { _slice = slice; }

  inline void someint(int i)
  { _someint = i; }
  inline int someint()
  { return _someint; }
  
  /* Return true if the point is unique w.r.t. another set. */
  bool isPointUnique(set < point * > &);
  inline bool isPointUnique(vector < point * > &P)
  { set < point * > PS; PS.insert(P.begin(), P.end()); return isPointUnique(PS); }

  friend ostream & operator<<(ostream &, const point &);
};

#endif
