#ifndef env_hpp
#define env_hpp

#include <vector>
#include <utility>
#include <set>
#include <map>
#include <cmath>
#include <iomanip>
#include <algorithm>

#include "point.hpp"
#include "team.hpp"

/* Probability of not randomly selecting a specific exemplar. */
#define PFAIL 1e-31

#define NUM_TWISTPOINTS 3

/* How often to print out state in trace. */
#define SHOWSTATE_MOD 1

/***********************************************************************************************************/

class env
{
protected:
  
  /* Maximum number of steps per episode. */
  long _maxSteps;
  
  /* Act given the (starting) point, action, the (current) state, and
     the step number. Return reward and write whether or not end
     state has been reached. The state vector that is passed to this
     function is updated to reflect the effect of the action. */
  virtual double act(point *, long, vector < double > &, long, bool &) = 0;  

public:
  
  env(long maxSteps)
    : _maxSteps(maxSteps) {};
  virtual ~env()
  {};
  
  /* Select a point in the environment with uniform probability. */
  virtual point * initUniformPoint(long, set < long > &) = 0;  
  
  /* Shorter declarations are wrappers. */
  double evaluate(team *, point *, bool, double &);
  double evaluate(team *, point *, bool, double &, string, bool);
  double evaluate(team *, point *, bool, vector < double > &, double &);
  double evaluate(team *, point *, bool, vector < double > &, double &, string, int &);
  double evaluate(team *, point *, bool, vector < double > &, double &, string, bool, int &);
  
  /* Test a team in an environment specific manner. */
  virtual double test(team *, string, bool) = 0;

  virtual long dim() = 0;
};

/***********************************************************************************************************/

class implicitEnv : public env
{ 
protected:
  
  /* Label for the next point in this environment. */
  static long _count;
  
  /* The actions (used if an index references a continuous action). */
  vector < double > _actions; 

  /* Points for testing. */
  long _numTestPoints;
  vector < point * > _testPoints;

  /* Validation points. */
  long _numValidPoints;
  vector < point * > _validPoints;

  /* Whether or not to use binary fitness function. */
  bool _useBinaryFitness;

  int _seed;

public:

  implicitEnv(long maxSteps, long numTestPoints, long numValidPoints, bool useBinaryFitness, int seed)
    : env(maxSteps), _numTestPoints(numTestPoints), _numValidPoints(numValidPoints), _useBinaryFitness(useBinaryFitness), _seed(seed) {} ;

  virtual ~implicitEnv()
  { 
    while(_testPoints.size() > 0) { delete _testPoints.back(); _testPoints.pop_back(); }
    while(_validPoints.size() > 0) { delete _validPoints.back(); _validPoints.pop_back(); }
  };
  
  /* This function must be overwritten if the descendant does not use _actions (e.g., rubikEnv). */
  virtual long numActions()
  { return _actions.size(); }

  /* Generate an offspring point from two parents. */
  virtual point * genPoint(long, point *, point *) = 0;
  
  double rtest(team *, string, bool, bool);
  virtual double test2(team *, string, bool, set < point * > &) = 0;

  /* Test on _validPoints. */
  void testAbsolute(set < team * > &, int, long, long);

  /* Distance between two state vectors in this environment. */
  virtual double distance(vector < double > &, vector < double > &) = 0;
};

/***********************************************************************************************************/

class rubikEnv : public implicitEnv
{
  /* The state of the cube is a vector with 54 dimensions. The value
     of each of the features is in {0, 1, ..., 5}, one for each of the
     colours.

     The cube locations are indexed as such:

                    +--+--+--+		 
                    |36|37|38|		 
                    +--+--+--+		 
                    |39|40|41|		 
                    +--+--+--+		 
                    |42|43|44|		 
                    +--+--+--+		 
          +--+--+--++--+--+--++--+--+--+
          |27|28|29||00|01|02||09|10|11|
          +--+--+--++--+--+--++--+--+--+
          |30|31|32||03|04|05||12|13|14|
          +--+--+--++--+--+--++--+--+--+
          |33|34|35||06|07|08||15|16|17|
          +--+--+--++--+--+--++--+--+--+
                    +--+--+--+		 
                    |18|19|20|		 
                    +--+--+--+		 
                    |21|22|23|		 
                    +--+--+--+		 
                    |24|25|26|		 
                    +--+--+--+		 
                    +--+--+--+		 
                    |45|46|47|		 
                    +--+--+--+		 
                    |48|49|50|		 
                    +--+--+--+		 
                    |51|52|53|		 
                    +--+--+--+              


     The initial configuration is (colours are 0 through 5):  

                    +--+--+--+
                    |04|04|04|
                    +--+--+--+
                    |04|04|04|
                    +--+--+--+
                    |04|04|04|
                    +--+--+--+
          +--+--+--++--+--+--++--+--+--+
          |03|03|03||00|00|00||01|01|01|
          +--+--+--++--+--+--++--+--+--+
          |03|03|03||00|00|00||01|01|01|
          +--+--+--++--+--+--++--+--+--+
          |03|03|03||00|00|00||01|01|01|
          +--+--+--++--+--+--++--+--+--+
                    +--+--+--+
                    |02|02|02|
                    +--+--+--+
                    |02|02|02|
                    +--+--+--+
                    |02|02|02|
                    +--+--+--+
                    +--+--+--+
                    |05|05|05|
                    +--+--+--+
                    |05|05|05|
                    +--+--+--+
                    |05|05|05|
                    +--+--+--+

     And here is a template of a blank transformation cube. Each xx-yy pair
     indicates that face at location yy has been moved to location xx.


                             +-----+-----+-----+		    		    
                             |36-00|37-00|38-00|
                             +-----+-----+-----+		    		    
                             |39-00|40-00|41-00|		    
                             +-----+-----+-----+		    		    
                             |42-00|43-00|44-00|
                             +-----+-----+-----+		    		    
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |27-00|28-00|29-00||00-00|01-00|02-00||09-00|10-00|11-00|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |30-00|31-00|32-00||03-00|04-00|05-00||12-00|13-00|14-00|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
          |33-00|34-00|35-00||06-00|07-00|08-00||15-00|16-00|17-00|     
          +-----+-----+-----++-----+-----+-----++-----+-----+-----+
           		     +-----+-----+-----+ 		    
                       	     |18-00|19-00|20-00| 
                       	     +-----+-----+-----+ 		    
                       	     |21-00|22-00|23-00| 
                       	     +-----+-----+-----+ 		    
                       	     |24-00|25-00|26-00| 
                       	     +-----+-----+-----+ 		    
                       	     +-----+-----+-----+ 		    
                       	     |45-00|46-00|47-00| 
                       	     +-----+-----+-----+ 		    
                       	     |48-00|49-00|50-00| 
                       	     +-----+-----+-----+ 		    
                       	     |51-00|52-00|53-00|
                       	     +-----+-----+-----+ 		    

   */
  
  static const long _dim;

  enum move {F = 0, F_, /* 0, 1 */
 	     B, B_, /* 2, 3 */
 	     U, U_, /* 4, 5 */
 	     D, D_, /* 6, 7 */
 	     R, R_, /* 8, 9 */
 	     L, L_}; /* 10, 11 */
  
  vector < string > _moveLabels;

  /* Defines the colours at each location that represent a solved cube. */
  vector < int > _solvedState;

  /* Used for testing, sets of points after 1 twists (_twistPoints[0]), 
     2 twists (_twistsPoints[1]), etc. */
  vector < set < point * > * > _twistPoints;

  int _initTwists; /* Number of twists used during initialization. */
  int _mutateTwists; /* Number of twists used during mutation. */
  
  /* These are some test functions. */
  void verifyCube(vector < double > &);
  void testMoves();
  
  /* Rotate cube 90 degrees about x-axis (south to north). */
  void X(vector < double > &);
  
  /* Rotate cube 90 degress about y-axis (east to west). */
  void Y(vector < double > &);
  
  /* Rotate front face 90 degrees clockwise. */
  void turn(vector < double > &);
  
  void mapState(vector < double > &, int []);
  
  /* Whether or not the cube is solved. */
  bool isSolved(vector < double > &);
  
  /* Reward for a certain state. */
  double scoreCube(vector < double > &);
  double scoreCube2(vector < double > &);
  double scoreCube3(vector < double > &);
    
  void initState(vector < double > &);
    
public:
  
  rubikEnv(long, long, long, int, int, bool, int);
  ~rubikEnv();

  long numActions()
  { return _moveLabels.size(); }
  
  double act(point *, long, vector < double > &, long, bool &);
  
  point * initUniformPoint(long, set < long > &);

  double test(team *, string, bool);
  double test2(team *, string, bool, set < point * > &);
  
  point * genPoint(long, point *, point *);  
  
  void makeMove(vector < double > &, int action);
  void printCube(vector < double > &, string);
  
  void genMoves(vector < set < point * > * > &);

  inline string getMoveLabel(int i)
  { return _moveLabels[i]; }

  double distance(vector < double > &, vector < double > &);

  inline long dim()
  { return _dim; }
};

/***********************************************************************************************************/

class backupEnv : public implicitEnv
{
  static const long _dim;

  /* How many radians is one degree. */
  static const double _oneDeg;

  /* Parameters. */
  
  /* Distance the front tires move in one time step (metres). */
  static const double _r;
  /* Length of cab from pivot to front axle (metres). */
  static const double _dc; 
  /* Length of trailer from rear to pivot (metres). */
  static const double _ds;

  /* Coordinate boundaries for initialization. */
  static const double _maxX;
  static const double _minY;
  static const double _maxY;
  
  /* North-west and south-east coordinates of the block. */
  static const double _xNW, _yNW, _xSE, _ySE;
  
  /* Mutation sizes in metres and degrees. */

  /* Coordinate x. */
  double _mux;
  /* Coordinate y. */
  double _muy;
  /* Angle of trailer. */
  double _muthetas;
  /* Angle of cab. */
  double _muthetac;

  /* Get angle between -PI and PI assuming already between -2 * PI and 2 * PI. */
  double flipAngle(double &);

  /* Whether or not the cab and semi have jackknifed. */
  bool jackknife(double &, double &);

  /* Whether the angle is bad, assuming the x,y coordinates (used with wall). */
  bool thetaIsInvalid(double, double, double);

  /* Generate point where don't back into wall (use thetaIsInvalid here). */
  point * initUniformPoint(long);
  
  /* Check if (x, y) is in the wall. */
  inline bool isInWall(double x, double y)
  { return x > _xNW && x < _xSE && y < _yNW && y > _ySE; }
  
public:

  backupEnv(long, long, long, double, double, double, double, bool, int);
  ~backupEnv()
  {};
  
  double act(point *, long, vector < double > &, long, bool &);
  point * initUniformPoint(long, set < long > &);
  double test(team *, string, bool);
  double test2(team *, string, bool, set < point * > &);
  point * genPoint(long, point *, point *);
  double distance(vector < double > &, vector < double > &);

  inline long dim()
  { return _dim; }
};

/***********************************************************************************************************/

class explicitEnv : public env
{
protected:
  
  string _setName;
  long _dim;
  
  /* The set of points defining the environment. The point id should be the 
     index of the point in this vector (and the underlying dataset). */
  vector < pair < double *, long > * > _pointSet;
  
  /* Record index of points in _pointSet grouped by label. The dimension
     of _index should be the number of labels. The dimension of *_index[i]
     should be the number of points of label _labels[i]. This is done to
     speed up sampling of minority classes. */
  vector < vector < long > * > _index;
  
  /* Labels sorted into ascending order. */
  vector < long > _labels;
  /* Count the number of points of each label at each index (order matching _labels) */
  vector < long > _labelCount;
  /* Map label->index to find a label in _labels and _labelCount and _index. */
  map < long, long > _labelMap;

  long _maxTries;
  
public:
  
  explicitEnv(long dim, string setName, long maxSteps) 
    : env(maxSteps), _setName(setName), _dim(dim), _maxTries(0) {};

  virtual ~explicitEnv()
  {};
  
  inline long numLabels() 
  { return _labels.size(); }
  
  /* Get label at some index in _labels. */
  inline long getLabel(long i) 
  { return _labels[i]; }
  /* Get index in _labels and _labelCount and _index of some label. */
  long getLabelIndex(long);
  /* Get the number of points of a given label. */
  inline long getLabelCount(long label) 
  { return _labelCount[getLabelIndex(label)]; }
  inline long size() 
  { return _pointSet.size(); }
  
  point * initUniformPoint(long, set < long > &);
  point * initUniformPointLabel(long, set < long > &, long); /* Point must match label. */

  inline long dim()
  { return _dim; }
};

/***********************************************************************************************************/

class datasetEnv : public explicitEnv
{
protected:
  
public:
  
  datasetEnv(string, int, long, long);
  ~datasetEnv();
  
  double act(point *, long, vector <double > &, long, bool &);
  
  double test(team *, string, bool);
};

#endif
