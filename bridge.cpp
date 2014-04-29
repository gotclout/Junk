#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

const static int n = 7;
      static int t = 0;

struct vertex
{
  vector<vertex*> adj;
  vector<vertex*> pi;
  string          color;
  int             d,
                  f,
                  id;

  vertex(int pId)
  {
    id = pId;
  };

  friend ostream& operator<< (ostream & o, vertex & v)
  {
    size_t i = 0,
           sz = v.adj.size();

    o << "Rendering Vertex " << v.id << "\nAdjacency List Len: " << sz << endl;

    if(sz > 0)
    {
      o << "\nVertex " << v.id << " Adjacent To The Following Verticies...\n";
      for(; i < sz; ++i)
        o << "Vertex: " << v.adj[i]->id << endl;
    }

    sz = v.pi.size();
    o << "\nPredecessor List Len: " << sz << endl;
    if(sz > 0)
    {
      o << "\nVertex " << v.id << " Has The Following Predecessors...\n";
      for(i = 0; i < sz; ++i)
        o << "         Predecessor: " << v.pi[i]->id << endl;
    }
    else
      o << "\nVertex " << v.id << " Has No Predecessor\n";

    o << "      Discovery Time: " << v.d
      << "\n         Finish Time: " << v.f
      << "\n               Color: " << v.color << endl;

    return o;
  };
};

struct edge
{
  vertex* v1,
        * v2;

         int id;
  static int count;

  edge(vertex* pBegin, vertex* pEnd)
  {
    v1 = pBegin;
    v2 = pEnd;
    id = count;
    edge::count++;
  };

  friend ostream& operator <<(ostream & o, edge & e)
  {
    o << "Edge " << e.id << ": ";
    if(e.v1 && e.v2)
      o << " [" << e.v1->id << ", " << e.v2->id << "]\n";
    return o;
  };
};

int edge::count = 0;

struct graph
{
  vector<vertex*> V;
  vector<edge>    E;

  size_t size() { return V.size(); };

  friend ostream& operator << (ostream & o, graph & g)
  {
    size_t i = 0;
    o << "Rendering Graph...\n"
      << "Num Verticies: " << g.size() << endl;
    for( ; i < g.size(); ++i)
    {
      vertex* v = g.V[i];
      if(v)
        o << *v << endl;
    }

    o << "Rendering Edges\nNum Edges: " << g.E.size() << endl;
    for(i = 0; i < g.E.size(); ++i)
    {
      edge e = g.E[i];
      o << e;
    }
    o << "\nRendering Complete\n";
  };

  ~graph()
  {
    for(size_t i = 0; i < V.size(); ++i)
    {
      delete V[i];
      V[i] = 0;
    }
    V.clear();
  };
};

graph g;

void make_graph(graph & G)
{
  vertex *v1 = new vertex(1),
         *v2 = new vertex(2),
         *v3 = new vertex(3),
         *v4 = new vertex(4),
         *v5 = new vertex(5),
         *v6 = new vertex(6),
         *v7 = new vertex(7);

  v1->adj.push_back(v2);
  v1->adj.push_back(v4);
  v2->adj.push_back(v1);
  v2->adj.push_back(v4);
  v2->adj.push_back(v3);
  v3->adj.push_back(v2);
  v4->adj.push_back(v1);
  v4->adj.push_back(v2);
  v4->adj.push_back(v5);
  v5->adj.push_back(v4);
  v5->adj.push_back(v6);
  v5->adj.push_back(v7);
  v6->adj.push_back(v5);
  v6->adj.push_back(v7);
  v7->adj.push_back(v5);
  v7->adj.push_back(v6);

  edge e1(v1, v2), e2(v1, v4), e3(v5, v6), e4(v2, v4),
       e5(v2, v3), e6(v5, v7), e7(v4, v5), e8(v6, v7);

  G.E.push_back(e1);
  G.E.push_back(e2);
  G.E.push_back(e3);
  G.E.push_back(e4);
  G.E.push_back(e5);
  G.E.push_back(e6);
  G.E.push_back(e7);
  G.E.push_back(e8);

  G.V.push_back(v1);
  G.V.push_back(v2);
  G.V.push_back(v3);
  G.V.push_back(v4);
  G.V.push_back(v5);
  G.V.push_back(v6);
  G.V.push_back(v7);
};

void DFS_VISIT(vertex* & u)
{
  u->color = "GRAY";
  ++t;
  u->d = t;
  for(size_t i = 0; i < u->adj.size(); ++i)
  {
    vertex* v = u->adj[i];
    if(v->color == "WHITE")
    {
      u->pi.push_back(v);
      DFS_VISIT(v);
    }
  }
  u->color = "BLACK";
  t++;
  u->f = t;
}

void DFS(graph & G)
{
  int i = 0;
  vertex* u;

  for(; i < n; ++i)
  {
    u = G.V[i];
    u->color = "WHITE";
    //u->pi[i] = NULL;
  }

  t = 0;

  for(i = 0; i < n; ++i)
  {
    u = G.V[i];
    if(u->color == "WHITE")
      DFS_VISIT(u);
  }
}

void print_graph(graph & G)
{
  cout << "Rendering Graph With " << G.size() << "Verticies...\n";
  for(size_t i = 0; i < G.size(); ++i)
  {
     vertex* v = G.V[i];
     if(v)
       cout << *v << endl;
  }
  cout << "Rendering Complete\n";
}

int main(int argc, char** argv)
{
  make_graph(g);
  DFS(g);
  cout << g << endl;
  return 0;
}
