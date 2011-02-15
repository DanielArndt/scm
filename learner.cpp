#include "learner.hpp"

/* Biases towards certain registers/features may be introduced due to
   the number of dst and src bits. For example if there are 6 src bits
   and 30 features, then if the src field is initialized setting each
   bit with uniform probability features 0 through 3 are more likely
   to be indexed (since feature n is indexed if the value of the src
   field is 30 x i + n for i a non-negative integer). */

/* 000 0000 0000 0000 0000 0001 */
const instruction learner::_modeMask(0x1);   
/* 000 0000 0000 0000 0000 1110 */
const instruction learner::_opMask(0xE);     
/* 000 0000 0000 0000 0111 0000 */
const instruction learner::_dstMask(0x70);   
/* 111 1111 1111 1111 1000 0000 */
const instruction learner::_srcMask(0x7FFF80); 

/* Rx <- op Rx Ry */
const instruction learner::_mode0(0x0);
/* Rx <- op Rx Iy */
const instruction learner::_mode1(0x1);

/* IMPORTANT!!!
   
   Note that learner::markIntrons() depends on these instruction so if they
   change that procedure needs to be updated.
*/

const instruction learner::_opSum(0x0);
const instruction learner::_opDiff(0x1);
const instruction learner::_opProd(0x2);
const instruction learner::_opDiv(0x3);
const instruction learner::_opCos(0x4);
const instruction learner::_opLog(0x5);
const instruction learner::_opExp(0x6);
const instruction learner::_opCond(0x7);

const short learner::_modeShift = 0;
const short learner::_opShift = learner::_modeMask.count();
const short learner::_dstShift = learner::_modeMask.count() + learner::_opMask.count();
const short learner::_srcShift = learner::_modeMask.count() + learner::_opMask.count() + learner::_dstMask.count();

long learner::_count = 0;

learner::learner(long gtime, 
		 long action,
		 int maxProgSize,
		 long dim)
  : _id(_count++), _gtime(gtime), _action(action), _dim(dim), _nrefs(0)
{  
  instruction *in;
  
  int progSize = 1 + ((int) (drand48() * maxProgSize));
  
  for(int i = 0; i < progSize; i++)
    {
      in = new instruction();
      
      for(int j = 0; j < in->size(); j++)
	if(drand48() < 0.5) in->flip(j);
      
      _bid.push_back(in);
    }
  
  markIntrons(_bid);
}

learner::learner(long gtime, 
		 long action,
		 int maxProgSize,
		 long dim,
		 vector < instruction* >& program)
  : _id(_count++), _gtime(gtime), _action(action), _dim(dim), _nrefs(0)
{
  int size = program.size();
  /* Copy in program */
  for(int i = 0; i < size; i++)
    {
      _bid.push_back(program[i]);
    }
  markIntrons(_bid);
}

learner::learner(long gtime,
		 learner &plr)
  : _id(_count++), _gtime(gtime), _action(plr.action()), _dim(plr._dim), _nrefs(0)
{  
  vector < instruction * > :: iterator initer, initerend;
  
  for(initer = plr._bid.begin(), initerend = plr._bid.end(); 
      initer != initerend; initer++)
    _bid.push_back(new instruction(**initer));
  
  markIntrons(_bid);
}

bool learner::muBid(double pBidDelete,
		    double pBidAdd,
		    double pBidSwap,
		    double pBidMutate,
		    int maxProgSize)
{
  bool changed = false;
  
#ifdef MYDEBUG
  cout << "learner:muBid before";
  for(int i = 0; i < _bid.size(); i++)
    cout << " " << i << ":" << _bid[i] << ":" << *_bid[i];
  cout << endl;
#endif

  /* Remove random instruction. */
  if(_bid.size() > 1 && drand48() < pBidDelete)
    {
      int i = (int) (drand48() * _bid.size());

      delete *(_bid.begin() + i);
      _bid.erase(_bid.begin() + i);

      changed = true;

#ifdef MYDEBUG
      cout << "learner:muBid delete " << i << endl;
#endif
    }
  
  /* Insert random instruction. */
  if(_bid.size() < maxProgSize && drand48() < pBidAdd)
    {
      instruction *instr = new instruction();
      
      for(int j = 0; j < instr->size(); j++)
        if(drand48() < 0.5) instr->flip(j);
      
      int i = (int) (drand48() * (_bid.size() + 1));
      
      _bid.insert(_bid.begin() + i, instr);
      
      changed = true;
      
#ifdef MYDEBUG
      cout << "learner:muBid add " << i << " " << instr << ":" << *instr << endl;
#endif
    }
  
  /* Flip single bit of random instruction. */
  if(drand48() < pBidMutate)
    {
      int i = (int) (drand48() * _bid.size());
      int j = (int) (drand48() * _bid[0]->size());
      
      _bid[i]->flip(j);
      
      changed = true;

#ifdef MYDEBUG
      cout << "learner:muBid mutate " << i << ", " << j << endl;
#endif
    }

  /* Swap positions of two instructions. */
  if(_bid.size() > 1 && drand48() < pBidSwap)
    {
      int i = (int) (drand48() * _bid.size());
      
      int j;

      do
	{
	  j = (int) (drand48() * _bid.size());
	} while(i == j);
      
      instruction *tmp;
      
      tmp = _bid[i];
      _bid[i] = _bid[j];
      _bid[j] = tmp;

      changed = true;

#ifdef MYDEBUG
      cout << "learner:muBid swap " << i << ", " << j << endl;
#endif
    }

#ifdef MYDEBUG
  cout << "learner:muBid after";
  for(int i = 0; i < _bid.size(); i++)
    cout << " " << i << ":" << _bid[i] << ":" << *_bid[i];
  cout << endl;
#endif

  if(changed == true) /* Need to mark introns if a change has occured. */
    markIntrons(_bid);

  return changed;
}
		 
learner::~learner()
{
  for(int i = 0; i < _bid.size(); i++)
    delete _bid[i];
}

double learner::bid(double *feature, 
		    double *REG)
{
  /* Zero registers. */
  memset(REG, 0, REGISTERS * sizeof(double));
  
  return 1 / (1 + exp(-run(_bid, feature, REG)));
}

double learner::run(vector < instruction * > &prog, 
		    double *feature, 
		    double *REG)
{  
  vector < instruction * > :: iterator initer, initerend;
  
  instruction mode;
  instruction op;
  
  int dstReg;
  double srcVal;

  int k;
  
#ifdef SHOWEXEC
  int i, j = 0;
  cout << *this << endl << "FEATURE";
  for(i = 0; i < _dim; i++)
    cout << " " << feature[i];
  cout << endl << "REG";
  for(i = 0; i < REGISTERS; i++)
    cout << " " << REG[i];
  cout << endl;
#endif
  
  for(k = 0, initer = prog.begin(), initerend = prog.end(); 
      initer != initerend; initer++, k++)
    {
      if(_introns[k] == false) /* Skip introns. */
	continue;
      
      mode = (**initer & _modeMask) >> _modeShift; 
      op = (**initer & _opMask) >> _opShift;
      
      /* Should be between 0 and REGISTERS - 1. */

      dstReg = ((**initer & _dstMask) >> _dstShift).to_ulong();
            
#ifdef SHOWEXEC
      cout << _introns[j++] << " " << **initer << " R[" << dstReg << "] <- R[" << dstReg << "] ";
#endif

      if(mode == _mode0) /* Rx <- op Rx Ry */
	{
	  srcVal = REG[((**initer & _srcMask) >> _srcShift).to_ulong() % REGISTERS];
#ifdef SHOWEXEC
	  cout << "R[" << ((**initer & _srcMask) >> _srcShift).to_ulong() % REGISTERS << "] ";
#endif
	}
      else /* Rx <- op Rx Iy */
	{
	  srcVal = feature[((**initer & _srcMask) >> _srcShift).to_ulong() % _dim];
#ifdef SHOWEXEC
	  cout << "I[" << ((**initer & _srcMask) >> _srcShift).to_ulong() % _dim << "] ";
#endif
	}
      
      if(op == _opSum)
	{
	  REG[dstReg] = REG[dstReg] + srcVal;
#ifdef SHOWEXEC
	  cout << "sum ";
#endif
	}
      else if(op == _opDiff)
	{
	  REG[dstReg] = REG[dstReg] - srcVal;
#ifdef SHOWEXEC
	  cout << "diff ";
#endif
	}
      else if(op == _opProd)
	{
	  REG[dstReg] = REG[dstReg] * srcVal;
#ifdef SHOWEXEC
	  cout << "prod ";
#endif
	}
      else if(op == _opDiv)
	{
	  REG[dstReg] = REG[dstReg] / srcVal;
#ifdef SHOWEXEC
	  cout << "div ";
#endif
	}
      else if(op == _opCos)
	{
	  REG[dstReg] = cos(srcVal);
#ifdef SHOWEXEC
	  cout << "cos ";
#endif
	}
      else if(op == _opLog)
	{
	  REG[dstReg] = log(fabs(srcVal));
#ifdef SHOWEXEC
	  cout << "log ";
#endif
	}
      else if(op == _opExp)
	{
	  REG[dstReg] = exp(srcVal);
#ifdef SHOWEXEC
	  cout << "exp ";
#endif
	}
      else if(op == _opCond)
	{
	  if(REG[dstReg] < srcVal)
	    REG[dstReg] = - REG[dstReg];
#ifdef SHOWEXEC
	  cout << "cond ";
#endif
	}
      else
	{
	  die(__FILE__, __FUNCTION__, __LINE__, "bad operation");
	}      
      
      if(isfinite(REG[dstReg]) == 0)
	REG[dstReg] = 0;
      
#ifdef SHOWEXEC
      cout << "REG";
      for(i = 0; i < REGISTERS; i++)
	cout << " " << REG[i];
      cout << endl;
#endif
    }
  
  return REG[0];
}

string learner::printBid(string prefix)
{  
  ostringstream oss;

  vector < instruction * > :: iterator initer, initerend;
  
  instruction mode;
  instruction op;
  
  int dstReg;
  
  int k;
  
  for(k = 0, initer = _bid.begin(), initerend = _bid.end(); initer != initerend; initer++, k++)
    {
      mode = (**initer & _modeMask) >> _modeShift; 
      op = (**initer & _opMask) >> _opShift;
      
      /* Should be between 0 and REGISTERS - 1. */

      dstReg = ((**initer & _dstMask) >> _dstShift).to_ulong();
      
      oss << prefix << " showexec " << k << " eff " << _introns[k] << " " << **initer << " " << dstReg;

      if(mode == _mode0) /* Rx <- op Rx Ry */
	{
	  oss << " " << ((**initer & _srcMask) >> _srcShift).to_ulong() % REGISTERS;
	  oss << " RR ";
	}
      else /* Rx <- op Rx Iy */
	{
	  oss << " " << ((**initer & _srcMask) >> _srcShift).to_ulong() % _dim;
	  oss << " RI ";
	}
      
      if(op == _opSum)
	{
	  oss << "sum ";
	}
      else if(op == _opDiff)
	{
	  oss << "diff ";
	}
      else if(op == _opProd)
	{
	  oss << "prod ";
	}
      else if(op == _opDiv)
	{
	  oss << "div ";
	}
      else if(op == _opCos)
	{
	  oss << "cos ";
	}
      else if(op == _opLog)
	{
	  oss << "log ";
	}
      else if(op == _opExp)
	{
	  oss << "exp ";
	}
      else if(op == _opCond)
	{
	  oss << "cond ";
	}
      else
	{
	  die(__FILE__, __FUNCTION__, __LINE__, "bad operation");
	}      

      oss << endl;
    }

  return oss.str();
}

void learner::markIntrons(vector < instruction * > &prog)
{  
  vector < instruction * > :: reverse_iterator riter;
  
  instruction mode;
  instruction op;
  
  int reg;
  long feat;
  
  bitset < REGISTERS > target;
  
  _introns.clear();
  _esize = 0;
  _features.clear();
  
  /* Mark the first register. */
  target.reset();
  target.set(0, 1);
  
  for(riter = prog.rbegin(); riter != prog.rend(); riter++) /* From last to first instruction. */
    {
      reg = ((**riter & _dstMask) >> _dstShift).to_ulong(); /* Get destination register. */

#ifdef MYDEBUG
      cout << "learner::markIntrons target " << target << " " << reg;
#endif
      
      if(target.test(reg) == true) /* Destination register is a target. */
	{
	  _introns.insert(_introns.begin(), true);
	  _esize++;

	  op = (**riter & _opMask) >> _opShift;

	  /* If the operation is unary, remove destination register from target set. */
	  if(op == _opCos || op == _opLog || op == _opExp)
	    target.set(reg, 0);
	  
	  mode = (**riter & _modeMask) >> _modeShift; 
	  
	  if(mode == _mode0) /* Rx <- op Rx Ry, need to target Ry. */
	    {
	      reg = ((**riter & _srcMask) >> _srcShift).to_ulong() % REGISTERS;
	      target.set(reg, 1);

#ifdef MYDEBUG
	      cout << " " << reg << " RR " << _esize << endl;
#endif
	    }
	  else /* Rx <- op Rx Iy, get feature index y. */
	    {
	      feat = ((**riter & _srcMask) >> _srcShift).to_ulong() % _dim;
	      _features.insert(feat);

#ifdef MYDEBUG
	      cout << " " << feat << " RI " << _esize << endl;
#endif
	    }
	}
      else /* Not a target, mark as intron. */
	{
	  _introns.insert(_introns.begin(), false);

#ifdef MYDEBUG
	  cout << " intron " << _esize << endl;
#endif
	}
    }

#ifdef MYDEBUG
  set < long > :: iterator seiter;
  cout << "learner::markIntrons features";
  for(seiter = _features.begin(); seiter != _features.end(); seiter++)
    cout << " " << *seiter;
  cout << endl;
#endif
}

ostream & operator<<(ostream &os, 
		     const learner &lr)
{
  os << "[" << lr._id << " " << lr._action << "]";

  return os;
}

