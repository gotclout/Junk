#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <list>
#include <map>

#define NIL NULL

using namespace std;

/** Time **/
static int t = 0;

/*******************************************************************************
 *
 *
 ******************************************************************************/
struct Vertex
{
  typedef list<Vertex*>     AdjList;
  typedef AdjList::iterator AdjListIt;

  AdjList* adj;   //
  Vertex*  pi;    //
  string   color; //
  int      d,     //
           f,     //
           id;    //

  /**
   *
   */
  Vertex() { adj = 0; pi = 0; color = "UNDEFINED"; d = f = id = -1; };

  /**
   *
   */
  Vertex(int pId)
  {
    id  = pId;
    pi  = 0;
    adj = 0;
    d   = 0;
    f   = 0;
  };

  /**
   *
   */
  bool add_adj(Vertex* & v)
  {
    bool ret = true;
    if(v)
      adj->push_back(v);
    else
      ret = false;

    return ret;
  };

  /**
   *
   */
  bool operator<(const Vertex & rhs) const { return id < rhs.id; };

  /**
   *
   */
  friend ostream& operator<< (ostream & o, Vertex & v)
  {
    size_t sz = v.adj->size();

    o << "Rendering Vertex " << v.id << "\nAdjacency List Len: " << sz << endl;

    if(sz > 0)
    {
      o << "\nVertex " << v.id << " Adjacent To " << v.adj->size() << " Verticies\n";
      AdjListIt it = v.adj->begin();
      for(; it != v.adj->end(); ++it)
        o << "Vertex: " << (*it)->id << endl;
    }
    else
      o << "Vertex: " << v.id << " Has No Adjacent Verticies\n";

    if(v.pi)
      o << "\nPredecessor:    " << v.pi->id << endl;
    else
      o << "\nPredecessor:    NIL\n";

    o << "Discovery Time: " << v.d
      << "\n   Finish Time: " << v.f
      << "\n         Color: " << v.color << endl;

    return o;
  };
};

typedef list<Vertex*>     AdjList;
typedef AdjList::iterator AdjListIt;

/*******************************************************************************
 *
 *
 ******************************************************************************/
struct Edge
{
  Vertex* v1,       //
        * v2;       //
         int id;    //
  static int count; //

  /**
   *
   */
  Edge() {v1 = v2 = 0; id = Edge::count++; };

  /**
   *
   */
  Edge(Vertex* pV1, Vertex* pV2)
  {
    v1 = &(*pV1);
    v2 = &(*pV2);
    id = Edge::count++;
  };

  /**
   *
   */
  friend ostream& operator <<(ostream & o, Edge & e)
  {
    if(e.v1 && e.v2)
      o << "Edge: [" << e.v1->id << ", " << e.v2->id << "]\n";
    return o;
  };
};
int Edge::count = 1;

typedef map<Vertex, AdjList>  VertexMap;
typedef VertexMap::iterator   VertexMapIt;
typedef VertexMap::value_type VertexMapType;

/*******************************************************************************
 *
 *
 ******************************************************************************/
struct Graph
{
  vector<Edge> E; //

  VertexMap VE;

  /**
   *
   */
  size_t vsize() { return VE.size(); };

  /**
   *
   */
  size_t esize() { return E.size(); };

  /**
   *
   */
  Graph() {};

  /**
   *
   */
  void add_edge(int u, int v)
  {
    VertexMapIt uit, vit, beg = VE.begin();
    Vertex* uvt, //u vertex ptr
           *vvt; //v vertex ptr

    uit = VE.find(Vertex(u));
    if(uit == VE.end())
      uit = VE.insert(beg, VertexMapType(Vertex(u), AdjList()));
    uvt = (Vertex*)&(uit->first);

    if(!uvt->adj) uvt->adj = (AdjList*)& uit->second;

    vit = VE.find(Vertex(v));
    if(vit == VE.end())
      vit = VE.insert(beg, VertexMapType(Vertex(v), AdjList()));
    vvt = (Vertex*)&(vit->first);

    if(!vvt->adj) vvt->adj = (AdjList*)& vit->second;

    uvt->add_adj(vvt);
    vvt->add_adj(uvt);
    E.push_back(Edge(&(*uvt), &(*vvt)));
  };

  /**
   *
   */
  friend ostream& operator << (ostream & o, Graph & G)
  {
    size_t i = 0, sz = G.vsize();

    o << "Rendering Graph...\n\n" << "Num Verticies: " << sz << endl;

    VertexMapIt vmi = G.VE.begin();

    for(; vmi != G.VE.end(); ++vmi)
    {
      Vertex* v = (Vertex*)&(vmi->first);
      cout << *v << endl;
    }

    o << "Rendering Edges\nNum Edges: " << G.E.size() << endl;
    for(i = 0; i < G.E.size(); ++i)
    {
      Edge e = G.E[i];
      o << e;
    }
    o << "\nRendering Complete\n";

    return o;
  };
};

/**
 *
 */
int DFS_VISIT(Vertex* & u, vector<Edge> & bridges)
{
  u->color  = "GRAY";
  u->d      = ++t;
  Vertex* v = 0;
  int minDu = t, minDv;

  for(AdjListIt uit = u->adj->begin(); uit != u->adj->end(); ++uit)
  {
    v = (*uit);
    if(v->color == "WHITE")
    {
      v->pi = &(*u);
      minDv = DFS_VISIT(v, bridges);
      minDu = min(minDu, minDv);

      if(minDv > minDu)
      {
        Edge bridge(u, v);
        bridges.push_back(bridge);
      }
    }
    else if(u->pi && v->id != u->pi->id)
      minDu = min(minDu, v->d);
  }

  u->color = "BLACK";
  u->f = ++t;

  return minDu;
}

/**
 *
 */
vector<Edge> DFS_BRIDGE(Graph & G)
{
  vector<Edge> bridges;
  VertexMapIt vit = G.VE.begin();

  for(; vit != G.VE.end(); ++vit)
  {
    Vertex* v = (Vertex*)&vit->first;
    v->color  = "WHITE";
    v->pi     = NIL;
  }

  t = 0;

  for(vit = G.VE.begin(); vit != G.VE.end(); ++vit)
  {
    Vertex*v = (Vertex*)&vit->first;
    if(v->color == "WHITE")
      DFS_VISIT(v, bridges);
  }

  return bridges;
}

/**
 *
 */
void PRINT_BRIDGES(const vector<Edge> & bridges)
{
  size_t i = 0, sz = bridges.size();

  if(sz == 0)
    cout << "No Bridges Found In Graph\n";
  else
  {
    cout << "Rendering " << sz << " Bridges...\n\n";
    for(; i < sz; ++i)
    {
      Edge e = bridges[i];
      cout << "Birdge " << e;
    }
  }
}

/**
 * Finds bridges in a graph using DFS and DFS-VISIT patterns
 */
int main(int argc, char** argv)
{
  Graph g1;
  g1.add_edge(1,2);
  g1.add_edge(1,4);
  g1.add_edge(4,5);
  g1.add_edge(5,6);
  g1.add_edge(6,7);
  g1.add_edge(2,4);
  g1.add_edge(5,7);
  g1.add_edge(2,3);

  vector<Edge> bridges1 = DFS_BRIDGE(g1);
  cout << g1 << endl;
  PRINT_BRIDGES(bridges1);

  return 0;
}
