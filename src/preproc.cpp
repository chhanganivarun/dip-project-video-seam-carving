#include <bits/stdc++.h>
#include <cmath>
using namespace std;

// Uses energy function
// Creates edge based graph
// Output of this file is passed on to graphcut.exe

// ENERGY_FUNCTIONS:
// 0: Difference
// 1: Entropy
#define ENERGY_FUNCTION 1

#define MAX_EDGES 27000000
#define MAX_TLINKS 160000
#define MAX_X 300
#define MAX_Y 300
#define MAX_T 100

int I[MAX_X][MAX_Y][MAX_T];
int edges[MAX_EDGES][4];
int tlinks[MAX_TLINKS][3];

int X, Y, T, NUM_VERTICES;
int edge_count, tlink_count;
int INF;

typedef struct triple_point
{
	int x;
	int y;
	int t;
} triple_point;

triple_point decode(int num)
{
	triple_point p;
	p.t = num % T;
	p.y = ((num - p.t) / T) % Y;
	p.x = (num - p.y * T - p.t) / (Y * T);
	return p;
}

int encode(triple_point p)
{
	return p.x * Y * T + p.y * T + p.t;
}

triple_point make_point(int x, int y, int t)
{
	triple_point p;
	p.x = x;
	p.y = y;
	p.t = t;
	return p;
}

triple_point add_point(triple_point p1, triple_point p2)
{
	triple_point p3;
	p3.x = p1.x + p2.x;
	p3.y = p1.y + p2.y;
	p3.t = p1.t + p2.t;
	return p3;
}

bool in_bounds(triple_point p)
{
	return ((p.x >= 0) & (p.x < X) & (p.y >= 0) & (p.y < Y) & (p.t >= 0) & (p.t < T));
}

int get_val(triple_point p)
{
	if (!in_bounds(p))
		return INF;
	return I[p.x][p.y][p.t];
}

template <typename T>
static float ShannonEntropy(vector<T> data, int n)
{
	float entropy = 0;
	int elements = n * n * n;
	std::map<T, long> counts;
	typename std::map<T, long>::iterator it;
	//
	for (int dataIndex = 0; dataIndex < elements; ++dataIndex)
	{
		counts[data[dataIndex]]++;
	}
	//
	it = counts.begin();
	while (it != counts.end())
	{
		float p_x = (float)it->second / elements;
		if (p_x > 0)
			entropy -= p_x * log(p_x) / log(2);
		it++;
	}
	return entropy;
}

vector<int> neighborhood(triple_point p, int n)
{
	vector<int> arr(n * n * n);
	int ctr = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; i <= 1; i++)
		{
			for (int k = -1; i <= 1; i++)
			{
				arr[ctr] = get_val({p.x + i, p.y + j, p.t + k});
			}
		}
	}
	return arr;
}

int calc_weight(triple_point p1, triple_point p2, int type = 0)
{
	int ret_val = abs(get_val(p1) - get_val(p2));
	if (type == 0)
	{
		ret_val = abs(get_val(p1) - get_val(p2));
	}
	else if (type == 1)
	{
		int n = 3;
		vector<int> arr = neighborhood(p1, n);
		ret_val = ShannonEntropy(arr, n);
	}

	return ret_val;
}

triple_point parity_point(triple_point p)
{
	return add_point(p, make_point(-1, 0, 0));
}

void add_edge(int src, int dst, int w1, int w2)
{
	edges[edge_count][0] = src;
	edges[edge_count][1] = dst;
	edges[edge_count][2] = w1;
	edges[edge_count][3] = w2;
	edge_count++;
}

void add_tlink(int src, int w1, int w2)
{
	tlinks[tlink_count][0] = src;
	tlinks[tlink_count][1] = w1;
	tlinks[tlink_count][2] = w2;
	tlink_count++;
}

int safe_str(int num)
{
	if (num >= INF / 2)
		num = -1;
	return num;
}

int main()
{

	triple_point neighbours[] = {
		make_point(0, 1, 0),
		make_point(0, -1, 0),
		make_point(1, 0, 0),
		make_point(0, 0, 1),
		make_point(0, 0, -1),
		make_point(-1, 0, 0),
		make_point(-1, -1, -1),
		make_point(-1, -1, 0),
		make_point(-1, -1, 1),
		make_point(-1, 0, -1),
		make_point(-1, 0, 1),
		make_point(-1, 1, -1),
		make_point(-1, 1, 0),
		make_point(-1, 1, 1)};

	bool isgoodneighbour[] = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int NUM_NEIGHBOURS = 5 + 9;

	char _;
	cin >> _ >> X >> Y >> T;

	NUM_VERTICES = X * Y * T;
	// MAX_EDGES = 27*X*Y*T;
	// MAX_TLINKS = 2*Y*T;
	INF = NUM_VERTICES * 10 + 99999;

	for (int x = 0; x < X; x++)
	{
		for (int y = 0; y < Y; y++)
		{
			for (int t = 0; t < T; t++)
			{
				cin >> I[x][y][t];
			}
		}
	}

	for (int y = 0; y < Y; y++)
		for (int t = 0; t < T; t++)
			add_tlink(encode(make_point(0, y, t)), INF, 0);

	for (int y = 0; y < Y; y++)
		for (int t = 0; t < T; t++)
			add_tlink(encode(make_point(X - 1, y, t)), 0, INF);

	int src, dst, w;
	triple_point p, q;
	for (int x = 0; x < X; x++)
	{
		for (int y = 0; y < Y; y++)
		{
			for (int t = 0; t < T; t++)
			{
				src = encode(make_point(x, y, t));
				p = make_point(x, y, t);
				for (int i = 0; i < NUM_NEIGHBOURS; i++)
				{
					q = add_point(p, neighbours[i]);
					dst = encode(q);
					if (src < dst && in_bounds(q))
					{
						w = calc_weight(parity_point(p), q, ENERGY_FUNCTION);
						if (isgoodneighbour[i])
							add_edge(src, dst, w, INF);
						else
							add_edge(src, dst, INF, w);
					}
				}
			}
		}
	}

	cout << NUM_VERTICES << ' ' << 2 * (edge_count + tlink_count) << ' ' << tlink_count << ' ' << edge_count << endl;
	cout << X << ' ' << Y << ' ' << T << endl;
	for (int i = 0; i < tlink_count; i++)
	{
		cout << tlinks[i][0] << ' ';
		cout << safe_str(tlinks[i][1]) << ' ';
		cout << safe_str(tlinks[i][2]) << ' ';
		cout << endl;
	}
	for (int i = 0; i < edge_count; i++)
	{
		cout << edges[i][0] << ' ';
		cout << edges[i][1] << ' ';
		cout << safe_str(edges[i][2]) << ' ';
		cout << safe_str(edges[i][3]) << ' ';
		cout << endl;
	}
}