#include <stdio.h>
#include "graph.h"
#include <iostream>
using namespace std;

int INF;
int X,Y,T;

int conv_inf(int num){
	if(num<0)
		num = INF;
	return num;
}

typedef struct triple_point{
	int x;
	int y;
	int t;
} triple_point;

triple_point decode(int num){
	triple_point p;
	p.t = num%T;
	p.y = ((num-p.t)/T)%Y;
	p.x = (num-p.y*T-p.t)/(Y*T);
	return p;
}

int encode(triple_point p){
	return p.x*Y*T + p.y*T + p.t;
}

int next_point(int num){
	return num+Y*T;
}

int print_triple_point(triple_point p){
	cout << p.x << " " << p.y << " " << p.t << endl;
}

int main()
{
	int V, E, num_tlinks, num_edges;
	cin >> V >> E >> num_tlinks >> num_edges;
	cin >> X >> Y >> T;
	typedef Graph<int,int,int> GraphType;
	GraphType *g = new GraphType(/*estimated # of nodes*/ V, /*estimated # of edges*/ E); 

	INF = 99999;

	for(int i=0; i<V; i++){
		g->add_node();
	}

	int a,b,c,d;

	for(int i=0; i<num_tlinks; i++){
		cin >> a >> b >> c;
		g->add_tweights(a,conv_inf(b),conv_inf(c));
	}

	for(int i=0; i<num_edges; i++){
		cin >> a >> b >> c >> d;
		g->add_edge(a, b, conv_inf(c), conv_inf(d));
	}

	int flow = g -> maxflow();

	int flag = 0;
	for(int i=0; i<V; i++){
		if(g->what_segment(i)==0 && g->what_segment(next_point(i))==1){
			cout << i << " ";
		}
	}

	delete g;

	return 0;
}
