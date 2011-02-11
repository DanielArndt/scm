#include "point.hpp"

point::point(long id, 
	     int dim,
	     long gtime,
	     const double *state,
	     long label)
  : _id(id), _gtime(gtime), _label(label), _key(0), _marked(0), _solved(0), _slice(0), _someint(0)
{
  _state.reserve(dim);
  _state.resize(dim);

  for(int i = 0; i < dim; i++)
    _state[i] = state[i];
}

bool point::isPointUnique(set < point * > &P)
{
  vector < double > state;  
  set < point * > :: iterator poiter;
  
  for(poiter = P.begin(); poiter != P.end(); poiter++)
    {
      (*poiter)->state(state);
      
      if(isEqual(_state, state, 1e-5))
	return false;
    }
  
  return true;
}

ostream & operator<<(ostream &os, const point &pt)
{
  os << "(" << pt._id << ", " << pt._label << ", " << pt._gtime << ")";
  return os;
}
