#include "misc.hpp"

float gasdev()
{
  static int iset = 0;
  static float gset;
  float fac, rsq, v1, v2;

  if(iset == 0)
    {
      /* Extra deviate is not available from previous calculation. */

      do
	{
	  v1 = 2.0 * drand48() - 1.0;
	  v2 = 2.0 * drand48() - 1.0;
	  rsq = v1 * v1 + v2 * v2;
	} while(rsq >= 1.0 || rsq == 0.0);
      
      fac = sqrt(-2.0 * log(rsq) / rsq);
      
      gset = v1 * fac;
      iset = 1;

      return v2 * fac;
    }
  else
    {
      /* Extra deviate is available from previous calculation. */

      iset = 0;

      return gset;
    }
}

void die(const char *file, 
	 const char *func, 
	 const int line, 
	 const char *msg)
{
  cerr << "error in file " << string(file) << " function " << string(func);
  cerr << " line " << line << ": " << string(msg) << "... exiting" << endl;
  abort();
}

double EuclideanDistSqrd(double *x, 
			 double *y, 
			 int dim)
{
  double dist = 0;
  
  for(int i = 0; i < dim; i++)
    dist += (x[i] - y[i]) * (x[i] - y[i]);

  return dist;
}

double EuclideanDistSqrd(vector < double > &x, 
			 vector < double > &y)
{
  double dist = 0;
  vector < double > :: iterator xiter, yiter, enditer;

  for(xiter = x.begin(), yiter = y.begin(), enditer = x.end();
      xiter != enditer; xiter++, yiter++)
    dist += (*xiter - *yiter) * (*xiter - *yiter);

  return dist;
}

int hammingDist(vector < int > &x, 
		vector < int > &y)
{
  int dist = 0;
  vector < int > :: iterator xiter, yiter, enditer;

  for(xiter = x.begin(), yiter = y.begin(), enditer = x.end();
      xiter != enditer; xiter++, yiter++)
    dist += *xiter == *yiter? 0 : 1;

  return dist;
}

bool isEqual(vector < int > &x, 
	     vector < int > &y)
{
  if(x.size() != y.size()) return false;

  vector < int > :: iterator xiter, yiter, enditer;

  for(xiter = x.begin(), yiter = y.begin(), enditer = x.end();
      xiter != enditer; xiter++, yiter++)
    if(*xiter != *yiter) return false;
  
  return true;
}

bool isEqual(vector < double > &x, 
	     vector < double > &y, 
	     double e)
{
  if(x.size() != y.size()) return false;

  vector < double > :: iterator xiter, yiter, enditer;

  for(xiter = x.begin(), yiter = y.begin(), enditer = x.end();
      xiter != enditer; xiter++, yiter++)
    if(isEqual(*xiter, *yiter, e) == false) return false;
  
  return true;
}

int stringToInt(string s)
{
  istringstream buffer(s);

  int i;
  
  buffer >> i;
  
  return i;
}

long stringToLong(string s)
{
  istringstream buffer(s);

  long l;
  
  buffer >> l;
  
  return l;
}

double stringToDouble(string s)
{
  istringstream buffer(s);

  double d;

  buffer >> d;
  
  return d;
}

int readMap(string fileName, 
	    map < string, string > &args)
{
  int pairs = 0;

  ifstream infile(fileName.c_str(), ios::in);
  
  if(infile == 0)
    die(__FILE__, __FUNCTION__, __LINE__, "cannot open map file");
  
  do
    {
      string key, value;
            
      if(infile) infile >> key; else break;
      if(infile) infile >> value; else break;
      
      args.insert(map < string, string > :: value_type(key, value));
      pairs++;

    } while(true);

  infile.close();

  return pairs;
}

bool getusage(double &sys, 
	      double &usr)
{
  struct rusage res;

  if(getrusage(RUSAGE_SELF, &res) == -1)
    return false;

  sys = (double) res.ru_stime.tv_sec + res.ru_stime.tv_usec * 1e-6;
  usr = (double) res.ru_utime.tv_sec + res.ru_utime.tv_usec * 1e-6;
  
  return true;
}

string* findLastOccurrence(string fileName, string findString)
{
  ifstream infile(fileName.c_str(), ios::in);
  int reads = 0;
  int readback = 0;
  
  char tmp[READ_BLOCK_SIZE+1];
  tmp[READ_BLOCK_SIZE] = '\0';
  string* fileEnd = new string();
  string* tmpPtr;
  cout << "In." << endl;
  size_t found = string::npos;
  while (found == string::npos)
  {
    ++reads;
    cout << "Searching back "<< reads*READ_BLOCK_SIZE << " bytes." << endl;
    readback = reads * READ_BLOCK_SIZE;
    infile.seekg(-readback, ios_base::end);
    infile.read(tmp, READ_BLOCK_SIZE);
    *fileEnd = string(tmp).append(*fileEnd);
    found = fileEnd->rfind(findString);
    if (reads > MAX_BLOCK_READS)
      die(__FILE__, __FUNCTION__, __LINE__, "cannot find string");
  }
  tmpPtr = fileEnd;
  fileEnd = new string(fileEnd->substr(found));
  delete tmpPtr;
  //cout << "File end: " << fileend << endl;
  return fileEnd;
}
