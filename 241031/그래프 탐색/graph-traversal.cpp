#include <iostream>
#include <vector>

using namespace std;

#define EDGE 1
#define VISITED 2

int N, M;
int graph[1001][1001];
int vertex[1001];

int dfs(int v) {
    if (vertex[v] == VISITED) return 0;

    vertex[v] = VISITED;
    int c = 1;
    for (int i = 1; i <= N; i++) {
        if (graph[v][i] == EDGE) {
            c += dfs(i);
        }
    }
    return c;
}

int main() {
    cin >> N >> M;

    int x, y;
    for (int i = 0; i < M; i++) {
        cin >> x >> y;
        graph[x][y] = EDGE;
        graph[y][x] = EDGE; 
    }

    int result = dfs(1); 
    cout << result - 1; 

    return 0;
}