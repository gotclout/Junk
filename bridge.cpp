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

/** logging string **/
static const char* ln =
  "--------------------------------------------------------------------------------\n";

/*******************************************************************************
 * Structure for expressing graph vertex
 ******************************************************************************/
struct Vertex
{
  typedef list<Vertex*>     AdjList;
  typedef AdjList::iterator AdjListIt;

  AdjList* adj;   //adjacent verticies
  Vertex*  pi;    //parent
  string   color; //state
  int      d,     //discovery time
           f,     //finish time
           id;    //uid

  /**
   * Default construct
   */
  Vertex() { adj = 0; pi = 0; color = "UNDEFINED"; d = f = id = -1; };

  /**
   * Construct from id
   */
  Vertex(int pId)
  {
    id  = pId;
    pi  = 0;
    adj = 0;
    d   = -1;
    f   = -1;
  };

  /**
   * Adds a vertex to this vertexes adjaceny list
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
   * Less than operator overload for container comparators
   *
   * @param Vertex is the right hand side vertex
   */
  bool operator<(const Vertex & rhs) const { return id < rhs.id; };

  /**
   * Renders a vertex
   *
   * @param ostream outputstream for rendering
   * @param Vertex is the vertex to be rendered
   */
  friend ostream& operator<< (ostream & o, Vertex & v)
  {
    size_t sz = v.adj->size();

    if(sz > 0)
    {
      o << v.id << " Adjacent to Verticies: ";
      AdjListIt it = v.adj->begin();
      for(size_t i =0; it != v.adj->end(); ++it, ++i)
      {
        o << (*it)->id;
        if(i+1 != sz) cout << ", ";
      }
    }
    else
      o << "Vertex: " << v.id << " Has No Adjacent Verticies";

    cout << endl;
    if(v.pi) o << "                  pi[" << v.id << "]:" << v.pi->id;
    else o << "                  pi[" << v.id << "]:" << "NIL";

    if(v.d < 0) o << ", d[" << v.id << "]:" << "UNDEFINED";
    else o << ", d[" << v.id << "]:" << v.d;
    if(v.f < 0) o << ", f[" << v.id << "]:" << "UNDEFINED";
    else o << ", f[" << v.id << "]:" << v.f;
    o << ", Color:" << v.color;

    return o;
  };
};

/** Typedef for Vertex containers **/
typedef list<Vertex*>     AdjList;
typedef AdjList::iterator AdjListIt;

/*******************************************************************************
 * Structure expressing graph edges
 ******************************************************************************/
struct Edge
{
  Vertex* v1,       // start vertex
        * v2;       // end vertex
         int id;    // uid
  static int count; // uid counter

  /**
   * Default construct
   */
  Edge() {v1 = v2 = 0; id = Edge::count++; };

  /**
   * Construct from verticies
   *
   * @param Vertex* pV1 start vertex, pV2 end vertex
   */
  Edge(Vertex* pV1, Vertex* pV2)
  {
    v1 = &(*pV1);
    v2 = &(*pV2);
    id = Edge::count++;
  };

  /**
   * Renders an edge
   */
  friend ostream& operator <<(ostream & o, Edge & e)
  {
    if(e.v1 && e.v2)
      o << "Edge: [" << e.v1->id << ", " << e.v2->id << "]\n";
    return o;
  };
};
int Edge::count = 1; //Identifier for Edges

/** Typedefs for Vertex Containers **/
typedef map<Vertex, AdjList>  VertexMap;
typedef VertexMap::iterator   VertexMapIt;
typedef VertexMap::value_type VertexMapType;

/*******************************************************************************
 * Structure for representing a graph
 ******************************************************************************/
struct Graph
{
  vector<Edge> E; //Collection of Edges

  VertexMap VE;   //Maps a vertex to a list of adjacent verticies

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
   * Adds an edge to this graph
   *
   * @param int u is the identifier for the first vertex
   * @param int v is the identifier for the second vertex
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
   * Renders a graph
   *
   * @param ostream is the output stream for rendering
   * @param Graph G is the graph to be rendered
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
 * Explores adjacent verticies tracking discovery times to detect bridges
 * if a bridge is detected it will be added to a container
 *
 * @param Vertex* u is the vertex to visit
 * @param vector<Edge> is the collection of bridges
 */
int DFS_BRIDGE_VISIT(Vertex* & u, vector<Edge> & bridges)
{
  u->color  = "GRAY";
  u->d      = ++t;
  Vertex* v = 0;
  int minDu = t, minDv;

  cout << "DFS-BRIDGE-VISIT: time:" << t << ", Visiting u: " << *u << endl;

  for(AdjListIt uit = u->adj->begin(); uit != u->adj->end(); ++uit)
  {
    v = (*uit);
    v->pi = &(*u);
    cout << "DFS-BRIDGE-VISIT: time:" << t << ", Exploring Adjacent Vertex v:"
         << *v << endl;

    if(v->color == "WHITE")
    {
      minDv = DFS_BRIDGE_VISIT(v, bridges);
      cout << "DFS-BRIDGE-VISIT: time:" << t << ", minDV:" << minDv << " ";
      if(minDv < minDu) cout << "<";
      else if(minDv == minDu) cout << "=";
      else cout << ">";
      cout << " minDU:" << minDu;

      minDu = min(minDu, minDv);

      if(minDv > minDu)
      {
        cout << ", Discovered Bridge:(" << u->id << ", " << v->id << ")\n";
        Edge bridge(u, v);
        bridges.push_back(bridge);
      }
      else cout << endl;
    }
    else if(v != u->pi && v->d < minDu) minDu = v->d;
  }

  u->color = "BLACK";
  u->f = ++t;
  cout << "DFS-BRIDGE-VISIT: time:" << t << ", Finished u:" << *u << endl << ln;

  return minDu;
}

/**
 * Finds bridges in a graph
 *
 * @param Graph G is the graph for which bridges should be detected
 * @return vector<Edge> is the collection of edges that are bridges
 */
vector<Edge> DFS_BRIDGE(Graph & G)
{
  vector<Edge> bridges;
  VertexMapIt vit = G.VE.begin();
  Vertex* u = 0;

  t = 0;
  for(; vit != G.VE.end(); ++vit)
  {
    u = (Vertex*)&vit->first;
    u->color  = "WHITE";
    u->pi     = NIL;
    cout << "DFS-BRIDGE      : time:" << t << ", "
         << "Initialized u:" << *u << endl;
  }

  cout << ln;
  t = 0;

  for(vit = G.VE.begin(); vit != G.VE.end(); ++vit)
  {
    u = (Vertex*)&vit->first;
    cout << "DFS-BRIDGE      : time:" << t << ", ";
    if(u->color == "WHITE")
    {
      cout << "Visiting u:" << *u << endl;
      DFS_BRIDGE_VISIT(u, bridges);
    }
    else
      cout << "Not Visiting u:" <<  *u << endl;
  }

  cout << ln;

  return bridges;
}

/**
 * Renders a vector of edges
 *
 * @param vector<Edge> is the collection of edges to be rendered
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
      cout << "Bridge " << e;
    }
  }

  cout << ln;
}

/**
 * Finds bridges in a graph using DFS and DFS-VISIT patterns
 */
int main(int argc, char** argv)
{
  Graph g1;
  g1.add_edge(1,2);
  g1.add_edge(1,4);
  g1.add_edge(2,3);
  g1.add_edge(2,4);
  g1.add_edge(4,5);
  g1.add_edge(5,6);
  g1.add_edge(5,7);
  g1.add_edge(6,7);

  cout << g1 << ln;
  vector<Edge> bridges1 = DFS_BRIDGE(g1);
  PRINT_BRIDGES(bridges1);

  return 0;
}
