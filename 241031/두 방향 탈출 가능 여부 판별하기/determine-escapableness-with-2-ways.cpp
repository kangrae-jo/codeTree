#include <iostream>
using namespace std;

#define WALL 0
#define PATH 1
#define VISITED 2

int n,m;
int field[101][101];
int offset[2][2] = {{-1,0},{0,1}};

bool moveable(int x, int y){
    if (0 > x || x <= n || 0 > y || y <=n) return false;
    if (field[y][x] == PATH) return true;
    return false;
}

bool dfs(int x, int y){
    if (field[y][x] == WALL) return false;

    for (int dir=0; dir<2; dir++){
        int y_ = y + offset[dir][0];
        int x_ = x + offset[dir][1];

        if (moveable(x_, y_)){
            field[y][x] = VISITED;
            return dfs(x_,y_);
        }
    }
    return false;
}

int main() {
    cin >> n >> m;

    for (int y=0; y<m; y++){
        for (int x=0; x<n; x++){
            cin >> field[y][x];
        }
    }

    if (dfs(0,0)) cout << '1';
    else cout << '0';


    return 0;
}