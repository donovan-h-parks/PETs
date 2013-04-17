/*
    Copyright 2007-2012 Janez Konc 

    If you use this program, please cite: 
    Janez Konc and Dusanka Janezic. An improved branch and bound algorithm for the 
    maximum clique problem. MATCH Commun. Math. Comput. Chem., 2007, 58, 569-590.

    More information at: http://www.sicmm.org/~konc

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Minor modifications made by Donovan H. Parks (donovan.parks@gmail.com) on April 16, 2013

#include "Precompiled.hpp"

#include "MCQD.hpp"

using namespace std;

Maxclique::Maxclique (const bool* const* conn, const int sz, const float tt) : pk(0), level(1), Tlimit(tt), V(sz), Q(sz), QMAX(sz) 
{
  assert(conn!=0 && sz>0);
  for (int i=0; i < sz; i++) V.push(i);
  e = conn;
  C = new ColorClass[sz + 1];
  for (int i=0; i < sz + 1; i++) C[i].init(sz + 1);
  S = new StepCount[sz + 1];
}

Maxclique::~Maxclique() 
{
  if (C) delete [] C;
  if (S) delete [] S;
  V.dispose();
};

void Maxclique::_mcq(int* &maxclique, int &sz, bool dyn) 
{ 
  V.set_degrees(*this);
  V.sort();
  V.init_colors();
  if (dyn) {
    for (int i=0; i < V.size() + 1; i++) {
      S[i].set_i1(0);
      S[i].set_i2(0);
    }
    expand_dyn(V);
  }
  else
    expand(V);
  maxclique = new int[QMAX.size()]; 
  for (int i=0; i<QMAX.size(); i++) { 
    maxclique[i] = QMAX.at(i);
  }
  sz = QMAX.size();
}

void Maxclique::Vertices::init_colors() 
{ 
  const int max_degree = v[0].get_degree();
  for (int i = 0; i < max_degree; i++)
    v[i].set_degree(i + 1);
  for (int i = max_degree; i < sz; i++)
    v[i].set_degree(max_degree + 1);
}

void Maxclique::Vertices::set_degrees(Maxclique &m) 
{ 
  for (int i=0; i < sz; i++) {
    int d = 0;
    for (int j=0; j < sz; j++)
      if (m.connection(v[i].get_i(), v[j].get_i())) d++;
    v[i].set_degree(d);
  }
}

bool Maxclique::cut1(const int pi, const ColorClass &A) 
{
  for (int i = 0; i < A.size(); i++)
    if (connection(pi, A.at(i)))
      return true;
  return false;
}

void Maxclique::cut2(const Vertices &A, Vertices &B) 
{
  for (int i = 0; i < A.size() - 1; i++) {
    if (connection(A.end().get_i(), A.at(i).get_i()))
      B.push(A.at(i).get_i());
  }
}

void Maxclique::color_sort(Vertices &R) 
{
  int j = 0;
  int maxno = 1;
  int min_k = QMAX.size() - Q.size() + 1;
  C[1].rewind();
  C[2].rewind();
  int k = 1;
  for (int i=0; i < R.size(); i++) {
    int pi = R.at(i).get_i();
    k = 1;
    while (cut1(pi, C[k]))
      k++;
    if (k > maxno) {
      maxno = k;
      C[maxno + 1].rewind();
    }
    C[k].push(pi);
    if (k < min_k) {
      R.at(j++).set_i(pi);
    }
  }
  if (j > 0) R.at(j-1).set_degree(0);
  if (min_k <= 0) min_k = 1;
  for (k = min_k; k <= maxno; k++)
    for (int i = 0; i < C[k].size(); i++) {
      R.at(j).set_i(C[k].at(i));
      R.at(j++).set_degree(k);
    }
}

void Maxclique::expand(Vertices R) 
{
  while (R.size()) {
    if (Q.size() + R.end().get_degree() > QMAX.size()) {
      Q.push(R.end().get_i());
      Vertices Rp(R.size());
      cut2(R, Rp);
      if (Rp.size()) {
        color_sort(Rp);
	pk++;
        expand(Rp);
      }
      else if (Q.size() > QMAX.size()) { 
	QMAX = Q;
      }    
      Rp.dispose();
      Q.pop();
    }
    else {
      return;
    }
    R.pop();
  }
}

void Maxclique::expand_dyn(Vertices R) {
  S[level].set_i1(S[level].get_i1() + S[level - 1].get_i1() - S[level].get_i2());
  S[level].set_i2(S[level - 1].get_i1());
  while (R.size()) {
    if (Q.size() + R.end().get_degree() > QMAX.size()) {
      Q.push(R.end().get_i());
      Vertices Rp(R.size());
      cut2(R, Rp);
      if (Rp.size()) {
        if ((float)S[level].get_i1()/++pk < Tlimit) {
          degree_sort(Rp);
        }
        color_sort(Rp);
	S[level].inc_i1();
	level++;
	expand_dyn(Rp);
	level--;
      }
      else if (Q.size() > QMAX.size()) { 
	QMAX = Q;
      }    
      Rp.dispose();
      Q.pop();
    }
    else {
      return;
    }
    R.pop();
  }
}
   
/*
void read_dimacs(string name, bool** &conn, int &size) {
  ifstream f (name.c_str());
  string buffer;
  assert(f.is_open());
  set<int> v;
  multimap<int,int> e;
  while (!getline(f, buffer).eof()) {
    if (buffer[0] == 'e') {
      int vi, vj;
      sscanf_s(buffer.c_str(), "%*c %d %d", &vi, &vj);
      v.insert(vi);
      v.insert(vj);
      e.insert(make_pair(vi, vj));
    }
  }
//  size = v.size() + 1;
  size = *v.rbegin() + 1;
  conn = new bool*[size];
  for (int i=0; i < size; i++) {
    conn[i] = new bool[size];
    memset(conn[i], 0, size * sizeof(bool));
  }
  for (multimap<int,int>::iterator it = e.begin(); it != e.end(); it++) {
    conn[it->first][it->second] = true;
    conn[it->second][it->first] = true;
  }
  cout << "|E| = " << e.size() << "  |V| = " << v.size() << " p = " << (double) e.size() / (v.size() * (v.size() - 1) / 2) << endl;
  f.close();
}
  
int main(int argc, char *argv[]) {
  assert(argc == 2);
  cout << "args = " << argv[1] << endl;
  bool **conn;
  int size;
  read_dimacs(argv[1], conn, size);
  cout << "---------- Example 1: run max clique with improved coloring ----------------"<<endl;
  clock_t start1 = time(NULL);
  clock_t start2 = clock();
  Maxclique m(conn, size);
  int *qmax;
  int qsize;
  m.mcq(qmax, qsize);  // run max clique with improved coloring
  cout << "Maximum clique: ";
  for (int i = 0; i < qsize; i++) 
    cout << qmax[i] << " ";
  cout << endl;
  cout << "Size = " << qsize << endl;
  cout << "Number of steps = " << m.steps() << endl;
  cout << "Time = " << difftime(time(NULL), start1) << endl;
  cout << "Time (precise) = " << ((double) (clock() - start2)) / CLOCKS_PER_SEC << endl << endl;
  delete [] qmax;
  cout << "---------- Example 2: run max clique with improved coloring and dynamic sorting of vertices ----------------"<<endl;
  start1 = time(NULL);
  start2 = clock();
  Maxclique md(conn, size, 0.025);  //(3rd parameter is optional - default is 0.025 - this heuristics parameter enables you to use dynamic resorting of vertices (time expensive)
  // on the part of the search tree that is close to the root - in this case, approximately 2.5% of the search tree -
  // you can probably find a more optimal value for your graphs
  md.mcqdyn(qmax, qsize);  // run max clique with improved coloring and dynamic sorting of vertices 
  cout << "Maximum clique: ";
  for (int i = 0; i < qsize; i++) 
    cout << qmax[i] << " ";
  cout << endl;
  cout << "Size = " << qsize << endl;
  cout << "Number of steps = " << md.steps() << endl;
  cout << "Time = " << difftime(time(NULL), start1) << endl;
  cout << "Time (precise) = " << ((double) (clock() - start2)) / CLOCKS_PER_SEC << endl << endl;
  delete [] qmax;
  for (int i=0;i<size;i++)
    delete [] conn[i];
  delete [] conn;
  return 0;
}
*/