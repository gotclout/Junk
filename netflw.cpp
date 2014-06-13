#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <map>
#include "FifoQueue.h"

#define NIL NULL

using namespace std;

/** Time **/
static int t = 0;

/** logging string **/
static const char* ln =
  "--------------------------------------------------------------------------------\n";

/*******************************************************************************
 * Structure for expressing a graph vertex
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
           mcap;  //min capacity of path to this vertex
  string   id;    //uid

  /**
   * Default construct
   */
  Vertex()
  {
    adj   = 0;
    pi    = 0;
    color = "UNDEFINED";
    d     = f = -1;
    id    = "DEFAULT";
    mcap  = INT_MAX;
  };

  /**
   * Construct from id
   *
   * @param string pId is the uid for this vertex
   */
  Vertex(string pId)
  {
    id    = pId;
    pi    = 0;
    adj   = 0;
    d     = f = -1;
    mcap  = INT_MAX;
    color = "UNDEFINED";
  };

  /**
   * Adds a vertex to this vertexes adjaceny list
   *
   * @param Vertex* v is the vertex pointer to be added to the adjacency list
   */
  bool add_adj(Vertex* & v){ adj->push_back(v); };

  /**
   * Less than operator overload for container comparators
   *
   * @param Vertex is the right hand side vertex
   */
  bool operator<(const Vertex & rhs) const { return id < rhs.id; };

  /**
   * Equivalence operator overload
   * 
   * @param Vertex rhs is a const reference to the Vertex to be compared to 
   *  this vertex
   * @return bool true if the id of this Vertex is equivalent to the id
   *  of the rhs Vertex id
   */
  bool operator==(const Vertex & rhs) const
  {
    return id == rhs.id;
  };

  /**
   * Inequivalence operator overload
   * 
   * @param Vertex rhs is a const reference to the Vertex to be compared to
   *  this vertex
   * @return bool true if the id of this Vertex is not equivalent to the id
   *  of the rhs Vertex id
   */
  bool operator !=(const Vertex & rhs) const {return !(*this == rhs); };

  /**
   * Renders a vertex
   *
   * @param ostream outputstream for rendering
   * @param Vertex is the vertex to be rendered
   */
  friend ostream& operator<< (ostream & o, const Vertex & pV)
  {
    Vertex v = (Vertex) pV;
    return operator<<(o, v);
  };
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
    else o << "Vertex: " << v.id << " Has No Adjacent Verticies";

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
  string  id;       // uid
  int     flow,     // flow of the current edge
          cap;      // capacity of the edge

  /**
   * Default construct
   */
  Edge() { v1 = v2 = 0; flow = 0; cap = 0; id = ""; };

  /**
   * Construct from verticies
   *
   * @param Vertex* pV1 start vertex, pV2 end vertex
   * @param Vertex* pV2 end vertex
   */
  Edge(Vertex* pV1, Vertex* pV2)
  {
    v1 = &(*pV1);
    v2 = &(*pV2);
    id = v1->id + v2->id;
    flow = cap = 0;
  };

  /**
   * Construct from vertex pointer and value
   *
   * @param Vertex* pV1 is the source vertex of edge e
   * @param Vertex* pV2 is the destination vertex of edge e
   * @param int pCam is the capacity of edge e
   */
  Edge(Vertex* pV1, Vertex* pV2, int pCap)
  {
    v1 = &(*pV1);
    v2 = &(*pV2);
    cap = pCap;
    flow = 0;
    id = v1->id + v2->id;
  };

  /**
   * Less than operator overload
   *
   * @param rhs is the edge to be compared
   * @return true if the id of this edge < rhs false otherwise
   */
  bool operator<(const Edge & rhs) const
  {
    return id < rhs.id;
  };

  /**
   * Retrieves the residual value of the edge
   *
   * @return int is the value of capacity - flow
   */
  int residual() { return cap - flow; };

  /**
   * Renders an edge
   *
   * @param ostream o is the outputstream
   * @param Edge e is the edge to be rendered
   * @return ostream is the same as param o
   */
  friend ostream& operator<< (ostream & o, Edge e)
  {
    o << "Edge: [" << e.v1->id << ", " << e.v2->id << "]"
      << ", Capacity:" << e.cap << ", Flow:" << e.flow << endl;

    return o;
  };
};

/** Typedefs for Vertex Containers **/
typedef map<Vertex, AdjList>  VertexMap;
typedef VertexMap::iterator   VertexMapIt;
typedef VertexMap::value_type VertexMapType;

/** Typedefs for Edge Containers **/
typedef vector<Edge> EdgeVector;
typedef vector<Edge*> EdgePtrVector;

/*******************************************************************************
 * Structure for representing a graph
 ******************************************************************************/
struct Graph
{
  EdgeVector E;  //Collection of Edges
  VertexMap VE;  //Maps a vertex to a list of adjacent verticies
  bool directed; //indicates whether G is a directed or undirected graph

  /**
   * Retrieves the number of verticies |V|
   */
  size_t vsize() { return VE.size(); };

  /**
   * Retrieves the number of edges |E|
   */
  size_t esize() { return E.size(); };

  /**
   * Default construct
   *
   * @param bool pDirected is true if G is directed, false otherwise
   */
  Graph(bool pDirected = false) { directed = pDirected; };

  /**
   * Retrieves a pointer to the vertex v in G
   *
   * @param Vertex v is the vertex to be retrieved
   */
  Vertex* get_vertex(Vertex v) { return (Vertex*) &VE.find(v)->first; };

  /**
   * Sets the parent value of all verticies to nil
   */
  void nilpi()
  {
    for(VertexMapIt i = VE.begin(); i != VE.end(); ++i)
    {
      Vertex* vptr = (Vertex*) &i->first;
      vptr->pi = 0;
    }
  }

  /**
   * Adds an edge to this graph
   *
   * @param int u is the identifier for the first vertex
   * @param int v is the identifier for the second vertex
   */
  void add_edge(string u, string v, int w = -1)
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
    E.push_back(Edge(&(*uvt), &(*vvt), w));
    E.push_back(Edge(&(*vvt), &(*uvt), 0));

  };

  /**
   * Updates an edge flow and its reverse edge by m
   */
  void update_edge(Vertex v1, Vertex v2, int m)
  {
    for(size_t i = 0; i < E.size(); ++i)
    {
      if(E[i].v1->id == v1.id && E[i].v2->id == v2.id)E[i].flow += m;
      else if(E[i].v1->id == v2.id &&  E[i].v2->id == v1.id) E[i].cap -= m;
    }
  };

  /**
   * Retrieves the edges connecting v to e in G
   */
  EdgePtrVector adjacent_edges(Vertex v)
  {
    EdgePtrVector ev;
    for(size_t i = 0; i < E.size(); ++i)
      if(E[i].v1->id == v.id) ev.push_back(&E[i]);
    return ev;
  };

  /**
   * Retrieves the vertex adjacent to v on edge e
   *
   * @param Vertex v is the specified vertex
   * @param Edge e is the specified edge
   * @return Vertex* is the vertex adjacent to v on edge e
   */
  Vertex* adjacent_vertex(Vertex v, Edge e) {return *e.v1 == v ? e.v2 : e.v1;};

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

    for(; vmi != G.VE.end(); ++vmi) cout << vmi->first << endl;
    o << "Rendering Edges\nNum Edges: " << G.E.size() << endl;
    for(i = 0; i < G.E.size(); ++i) o << G.E[i];
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

  for(; vit != G.VE.end(); ++vit)
  {
    u = (Vertex*)&vit->first;
    u->color  = "WHITE";
    u->pi     = NIL;
    cout << "DFS-BRIDGE      : time:0" << ", "
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
 * Performs breadth first search on graph G = (V, E) with source vertex
 * s in V
 *
 * @param graph G is the graph to be searched
 * @param Vertex s is the source
 * @param Vertex t is the sink
 * @return int is the flow along path s->t
 */
int BFS(Graph & G, Vertex & s, Vertex & t)
{
  int r = 0;
  queue<Vertex> q;
  s.mcap = INT_MAX;
  s.pi = &s;
  q.enqueue(s);

  while(!q.empty())
  {
    Vertex* u = (Vertex*) &G.VE.find(q.dequeue())->first;
    EdgePtrVector E = G.adjacent_edges(*u);

    for(size_t i = 0; i < E.size(); ++i) //for each edge adjacent to u
    {
      Edge* e   = E[i];
      Vertex* v = G.adjacent_vertex(*u, *e);

      if(v->pi == NIL && v->id != s.id && e->residual())
      {
        v->pi   = (Vertex*) &(G.VE.find(*u)->first);
        v->mcap = min(u->mcap, e->residual());
        if(v->id != t.id) q.enqueue(*v);
        else return v->mcap;
      }
    }
  }

  return r;
}

/**
 * Renders the resultant BFS path of EDMONDS_KARP
 */
void print_path(map<Vertex, Vertex> & P)
{
  stringstream ss;
  ss << "Rendering Path...\n";
  ss << P.begin()->second.id << " -> ";
  for(map<Vertex, Vertex>::iterator it = P.begin();
      it != P.end(); ++it)
  {
    Vertex v = it->first;
    ss << v.id;
    if(++it == P.end())
      ss << endl;
    else
      ss << " -> ";
    it--;
  }
  cout << ss.str();
}

/**
 * Implementation of the FordFulkerson method for computing the maximum flow
 * in a flow network in O(|V||E^2|)
 *
 * @param graph G is the specified graph
 * @param Vertex s is the source
 * @param Vertex t is the sink
 * @return int f is the max flow of G
 */
int EDMONDS_KARP(Graph & G, Vertex s, Vertex t)
{
  int m = INT_MAX, f = 0;

  while(m)
  {
    m = BFS(G, s, t);           //BFS returns capacity of flow for the path
    if(m == 0)  break;          //if 0 no additional flow can be sent
    cout << "f + m = " << f << " + " <<  m << " = " << f+m << endl;

    f += m;                     //update the flow
    Vertex v = *G.get_vertex(t);
    map<Vertex, Vertex> P;
    while(v.id != s.id)
    {
      Vertex u = *(v.pi);
      P[v] = u;
      G.update_edge(u, v, m);   //update edge and reverse edge flow
      v = u;
    }
    print_path(P);
    G.nilpi();
  }

  cout << "EDMONDS_KARP: " << f << endl;
  return f;
}

/**
 * Finds bridges in a graph using DFS and DFS-VISIT patterns
 *
 * @param argc is the argument count
 * @param argv is the argument vector
 */
int main(int argc, char** argv)
{
  Graph g1;
  g1.add_edge("1","2");
  g1.add_edge("1","4");
  g1.add_edge("2","3");
  g1.add_edge("2","4");
  g1.add_edge("4","5");
  g1.add_edge("5","6");
  g1.add_edge("5","7");
  g1.add_edge("6","7");

  cout << g1 << ln;
  vector<Edge> bridges1 = DFS_BRIDGE(g1);
  PRINT_BRIDGES(bridges1);
  Graph g2;
  g2.add_edge("1","2");
  g2.add_edge("2","4");
  g2.add_edge("4","6");
  g2.add_edge("1","3");
  g2.add_edge("3","2");
  g2.add_edge("2","7");
  g2.add_edge("2","5");
  g2.add_edge("5","6");

  vector<Edge> bridges2 = DFS_BRIDGE(g2);
  cout << g2 << endl;
  PRINT_BRIDGES(bridges2);

  Graph g3;
  g3.add_edge("2","1");
  g3.add_edge("1","4");
  g3.add_edge("2","3");
  g3.add_edge("3","1");
  g3.add_edge("4","5");
  vector<Edge> bridges3 = DFS_BRIDGE(g3);
  cout << g3 << endl;
  PRINT_BRIDGES(bridges3);

  Graph g4;
  g4.add_edge("1","2");
  g4.add_edge("2","3");
  g4.add_edge("3","4");
  vector<Edge> bridges4 = DFS_BRIDGE(g4);
  cout << g4 << endl;
  PRINT_BRIDGES(bridges4);

/******************************************************************************
 ******************************************************************************
G = (V, E), a Directed Graph
V = { A, B, C, D, E, F }
E = { A->B, A->E, B->C, C->D, C->E, C->F, D->B, D->E, E->F }

               8
       +B-----------+C-
      /  |          /|  \
     /   |         / |   \
  7 /    |        /  |    \ 1
   /     |       /   |     \
  /      |      /    |      \
A-       |3    /4    |2      +F                                #: Capacity
  \      |    /      |      /                                  -: Outgoing
   \     |   /       |     /                                   +: Incoming
  2 \    |  /        |    / 8
     \   | /         |   /
      \  |/          |  /
       \ /           | /
        +D-----------+E-
               4
******************************************************************************
******************************************************************************/

  Graph g5(true);
  g5.add_edge("A", "B", 7);
  g5.add_edge("A", "D", 2);
  g5.add_edge("B", "C", 8);
  g5.add_edge("C", "D", 4);
  g5.add_edge("C", "E", 2);
  g5.add_edge("C", "F", 1);
  g5.add_edge("D", "B", 3);
  g5.add_edge("D", "E", 4);
  g5.add_edge("E", "F", 8);
  cout << g5 << ln;

  VertexMapIt src = g5.VE.find(Vertex("A"));
  VertexMapIt snk = g5.VE.find(Vertex("F"));
  EDMONDS_KARP(g5, src->first, snk->first);
  cout << g5 << ln;

  return 0;
}
