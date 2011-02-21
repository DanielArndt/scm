#ifndef learner_hpp
#define learner_hpp

#include <bitset>
#include <vector>
#include <cmath>
#include <map>
#include <set>

#include "misc.hpp"

#define REGISTERS 8 /* Should match _dstMask. */
#define INPUTS 65536 /* Requires 16 bits. */

/* Used to define unique bidding behaviour. */
#define PROFILE_SIZE 50 /* Compare this many bid values. */
#define BID_EPSILON 1e-5 /* Using this equality threshold. */

using namespace std;

typedef bitset < 16 + 3 + 3 + 1 > instruction;

class learner
{
private:
  
  /* Next id to use. */
  static long _count; 
  
  /* Unique id of learner. */
  unsigned long _id;
  /* Time step at which generated. */
  long _gtime; 
  /* Action index. */
  long _action;

  /* Bid program. */
  vector < instruction * > _bid;

  /* Expected dimension of input feature vector. */
  long _dim;

  /* Mark structural introns (set to TRUE if NOT an intron).

     For example, _introns[0] is TRUE if the first instruction is effective.
   */
  vector < bool > _introns;

  /* Size of program not counting structural introns identified by markIntrons(). */
  int _esize;
  
  /* Features indexed by non-introns in this learner, determined in markIntrons(). */
  set < long > _features;

  /* Number of references by teams. */
  int _nrefs;
  
  /* Bid profile. */
  vector < double > _profile;

  double run(vector < instruction * > &, double *, double *);

  /* Instruction definitions. */
  
  /* Masks. Apply before shifting. */
  
  static const instruction _modeMask;
  static const instruction _opMask;
  static const instruction _dstMask;
  static const instruction _srcMask;
  
  /* Modes. */
  
  static const instruction _mode0;
  static const instruction _mode1;
  
  /* Operations. */
  
  static const instruction _opSum;
  static const instruction _opDiff;
  static const instruction _opProd;
  static const instruction _opDiv;
  static const instruction _opCos;
  static const instruction _opLog;
  static const instruction _opExp;
  static const instruction _opCond;
  
  /* Shift amounts. */
  
  static const short _modeShift;
  static const short _opShift;
  static const short _dstShift;
  static const short _srcShift;

  void markIntrons(vector < instruction * > &);
  
public:
  
  /* Create empty learner. */
  learner();
  void copyin(long, long, long, vector < instruction* >&);

  /* Create arbitrary learner. */
  learner(long, long, int, long);
  
  /* Create learner from another learner. */
  learner(long, learner &);
  
  /* Create learner with specified bidding program. */
  learner(long, long, long, vector < instruction* >&);
  
  ~learner();
  
  double bid(double *, double *);

  inline long dim()
  { return _dim; }
  
  inline int size()
  { return _bid.size(); }
  inline int esize() /* Not counting introns. */
  { return _esize; }
  inline unsigned long id()
  { return _id; }
  inline long action()
  { return _action; }
  inline long gtime()
  { return _gtime; }

  inline long numFeatures() /* Not counting introns. */
  { return _features.size(); }
  inline void features(set < long > &F)
  { F.insert(_features.begin(), _features.end()); }

  /* Mutate action, return true if the action was actually changed. */
  inline bool muAction(long action) 
  { long a = _action; _action = action; return a != action; }
  /* Mutate bid, return true if any changes occured. */
  bool muBid(double, double, double, double, int);

  inline int refInc() 
  { return ++_nrefs; }
  inline int refDec() 
  { return --_nrefs; }
  inline int refs() 
  { return _nrefs; }
  
  inline void setProfile(vector < double > &p)
  { _profile = p; }
  inline void getProfile(vector < double > &p)
  { p = _profile; }
  inline bool isProfileEqualTo(vector < double > &p)
  { return isEqual(_profile, p, BID_EPSILON); }

  string printBid(string);
  
  friend ostream & operator<<(ostream &, const learner &);
};


#endif
