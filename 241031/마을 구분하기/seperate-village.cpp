#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define WALL 0
#define HUMAN 1
#define VISITED 2

int n;
int field[26][26];
int offset[4][2]={{1,0},{0,1},{-1,0},{0,-1}};

bool isMoveable(int x, int y){
    return (0<=x && x<n && 0<=y && y<n && field[y][x] == HUMAN);
}

int dfs(int x, int y){
    if (field[y][x] == VISITED) return 0;
    
    field[y][x] = VISITED;
    int cnt = 1;
    
    for (int dir=0; dir<4; dir++){
        int y_ = y + offset[dir][0];
        int x_ = x + offset[dir][1];

        if (isMoveable(x_,y_)){
            cnt += dfs(x_,y_);
        }
    }
    return cnt;
}

int main() {
    cin >> n;
    for (int y=0; y<n; y++){
        for (int x=0; x<n; x++){
            cin >> field[y][x];
        }
    }

    vector<int> village;
    for (int y=0; y<n; y++){
        for (int x=0; x<n; x++){
            int cnt = 0;
            if (field[y][x] == HUMAN){
                cnt += dfs(x,y);
            }
            if (cnt != 0) village.push_back(cnt);
        }
    }

    sort(village.begin(),village.end());

    cout << village.size() << "\n";
    for (int i=0; i<village.size(); i++)
        cout << village[i] << "\n";

    return 0;
}