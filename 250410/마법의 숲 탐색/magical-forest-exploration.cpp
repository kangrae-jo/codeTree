#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int EMPTY = 0;
const int GOLEM = 1;
const int FAIRY = 2;
const int ESCAPE = 3;
const int VISITED = 10;

const int FAIRY_OFFSET[8][2] = {
    {-2, 0}, {-1, -1}, {-1, 1},
    {0, -2}, {0, 2},
    {2, 0}, {1, -1}, {1, 1}
};
const int OFFSET[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;

int R, C;

struct Golem {
    int col, dir;
};

struct Fairy {
    int col, row, escapeX, escapeY;
};

bool isIn(int y, int x) { return 4 <= y && y < R + 4 && 1 <= x && x < C + 1; }

bool canDown(vector<vector<int>> forest, Fairy fairy, int FROM) {
    fairy.row += OFFSET[DOWN][0];
    if (fairy.row + OFFSET[DOWN][0] < R + 4 && forest[fairy.row + OFFSET[DOWN][0]][fairy.col] == EMPTY) {
            if (FROM == DOWN && 
                forest[fairy.row][fairy.col + OFFSET[LEFT][1]] == EMPTY && 
                forest[fairy.row][fairy.col + OFFSET[RIGHT][1]] == EMPTY) return true;
            else if (FROM == LEFT && forest[fairy.row][fairy.col + OFFSET[RIGHT][1]] == EMPTY) return true;
            else if (FROM == RIGHT && forest[fairy.row][fairy.col + OFFSET[LEFT][1]] == EMPTY) return true;
            return false;
    }
    return false;
}

bool canLeft(vector<vector<int>> forest, Fairy fairy) {
    fairy.col += OFFSET[LEFT][1];
    if (fairy.col + OFFSET[LEFT][1] > 0 &&
        forest[fairy.row + OFFSET[UP][0]][fairy.col] == EMPTY &&
        forest[fairy.row][fairy.col + OFFSET[LEFT][1]] == EMPTY &&
        forest[fairy.row + OFFSET[DOWN][0]][fairy.col] == EMPTY)
        return canDown(forest, fairy, RIGHT);
    return false;
}

bool canRight(vector<vector<int>> forest, Fairy fairy) {
    fairy.col += OFFSET[RIGHT][1];
    if (fairy.col + OFFSET[RIGHT][1] < C + 1 &&
        forest[fairy.row + OFFSET[UP][0]][fairy.col] == EMPTY &&
        forest[fairy.row][fairy.col + OFFSET[RIGHT][1]] == EMPTY &&
        forest[fairy.row + OFFSET[DOWN][0]][fairy.col] == EMPTY)
        return canDown(forest, fairy, LEFT);
    return false;
}

void move(vector<vector<int>>& forest, Fairy& fairy, int DIR) {
    // 이동 잔상(?) 지우기
    for (int dir = 0; dir < 4; dir++) {
        forest[fairy.row + OFFSET[dir][0]][fairy.col + OFFSET[dir][1]] = EMPTY;
    }

    // 정령 위치 옮기기
    fairy.row += OFFSET[DIR][0];
    fairy.col += OFFSET[DIR][1];
    forest[fairy.row][fairy.col] = FAIRY;
    for (int dir = 0; dir < 4; dir++) {
        forest[fairy.row + OFFSET[dir][0]][fairy.col + OFFSET[dir][1]] = GOLEM;
    }

    // 정령 출구 옮기기
    fairy.escapeY += OFFSET[DIR][0];
    fairy.escapeX += OFFSET[DIR][1];
    forest[fairy.escapeY][fairy.escapeX] = ESCAPE;
}

void rotate(vector<vector<int>>& forest, Fairy& fairy, int DIR) {
    forest[fairy.escapeY][fairy.escapeX] = GOLEM;
    fairy.escapeY = fairy.row + OFFSET[DIR][0];
    fairy.escapeX = fairy.col + OFFSET[DIR][1];
    forest[fairy.escapeY][fairy.escapeX] = ESCAPE;
}

bool isFairyOut(Fairy fairy) {
    if (fairy.row <= 4) return true;
    else return false;
}

void findNextFairy(vector<vector<int>>& forest, queue<pair<int,int>>& q, int x, int y) {
    for (int dir = 0; dir < 8; dir++) {
        int y_ = y + FAIRY_OFFSET[dir][0];
        int x_ = x + FAIRY_OFFSET[dir][1];
        if (isIn(y_, x_) && forest[y_][x_] == FAIRY) {
            q.push({y_, x_});
        }
    }
}

int getRowOfFairy(vector<vector<int>> forest, Fairy fairy) {
    queue<pair<int, int>> q;
    q.push({fairy.row, fairy.col});
    forest[fairy.row][fairy.col] = VISITED;

    // q에는 fairy의 좌표만 들어감
    int answer = fairy.row;
    while (!q.empty()) {
        auto [curY, curX] = q.front();
        q.pop();

        forest[curY][curX] = VISITED;
        for (int dir = 0; dir < 4; dir++) {
            int nextY = curY + OFFSET[dir][0];
            int nextX = curX + OFFSET[dir][1];
            if (isIn(nextY, nextX)) {
                if (forest[nextY][nextX] == ESCAPE) {
                    answer = max(answer, nextY);
                    // dfs로 다음 fairy를 찾아서 q.push
                    // 다음 fairy는 무조건 2칸 뒤에 있음
                    findNextFairy(forest, q, nextX, nextY);
                }
                else if (forest[nextY][nextX] == GOLEM) {
                    // 가장 낮은 곳인지 파악만
                    answer = max(answer, nextY);
                }
            }
        }
    }
    return answer - 3;
}

void cleanForest(vector<vector<int>>& forest) {
    forest.assign(R + 4, vector<int>(C + 1, EMPTY));
}

int moveGolem(vector<vector<int>>& forest, int gCol, int gDir) {
    // 숲을 이동하는 객체 생성
    Fairy fairy = {gCol, 1, gCol + OFFSET[gDir][1], 1 + OFFSET[gDir][0]};

    // 객체를 숲에 두기
    forest[fairy.row][fairy.col] = FAIRY;
    for (int dir = 0; dir < 4; dir++) {
        if (dir == gDir) forest[fairy.row + OFFSET[dir][0]][fairy.col + OFFSET[dir][1]] = ESCAPE;
        else forest[fairy.row + OFFSET[dir][0]][fairy.col + OFFSET[dir][1]] = GOLEM;
    }

    while (1) {
        // 아래로 이동 가능할 때 까지 이동
        while (canDown(forest, fairy, DOWN)) {
            move(forest, fairy, DOWN);
        }

        // 서쪽 회전 가능?
        if (canLeft(forest, fairy)) {
            move(forest, fairy, LEFT);
            gDir = (gDir + 3) % 4;
            rotate(forest, fairy, gDir);
            move(forest, fairy, DOWN);
            continue;
        }
        // 동쪽 회전 가능?
        if (canRight(forest, fairy)) {
            move(forest, fairy, RIGHT);
            gDir = (gDir + 1) % 4;
            rotate(forest, fairy, gDir);
            move(forest, fairy, DOWN);
            continue;
        }
        if (isFairyOut(fairy)) break;  // 서쪽&동쪽 회전 불가인데 삐져나오기까지 했다? 그럼 다 지워야함
        return getRowOfFairy(forest, fairy);  // 안 삐져나갔으면 정령이 어디까지 내려갈 수 있는지 파악 후 리턴
    }
    // 숲을 비우는 로직 후 0 리턴
    cleanForest(forest);
    return 0;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    // 1) input
    int K;
    cin >> R >> C >> K;

    // 처음에 골렘을 놓기 위해서 높이 + 3
    vector<vector<int>> forest(R + 4, vector<int>(C + 1, 0));

    vector<Golem> golems(K);
    for (int i = 0; i < K; i++) {
        cin >> golems[i].col >> golems[i].dir;
    }

    // 2) solution
    int answer = 0;
    for (int i = 0; i < K; i++) {
        int gCol = golems[i].col;
        int gDir = golems[i].dir;

        answer += moveGolem(forest, gCol, gDir);
    }

    cout << answer << endl;

    return 0;
}
