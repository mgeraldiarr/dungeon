#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

struct Node {
    int x, y;
    int g, h;

    int f() const {
        return g + h;
    }

    bool operator>(const Node& other) const {
        return f() > other.f();
    }
};

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

int heuristic(Point a, Point b) {
    // Manhattan distance untuk gerakan 4 arah.
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool isInside(const vector<vector<int>>& grid, int x, int y) {
    return x >= 0 &&
           x < static_cast<int>(grid.size()) &&
           y >= 0 &&
           y < static_cast<int>(grid[0].size());
}

vector<Point> aStar(
    const vector<vector<int>>& grid,
    Point start,
    Point goal
) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    const int INF = numeric_limits<int>::max();

    vector<vector<int>> gScore(
        rows, vector<int>(cols, INF)
    );

    vector<vector<Point>> parent(
        rows, vector<Point>(cols, {-1, -1})
    );

    priority_queue<
        Node,
        vector<Node>,
        greater<Node>
    > openSet;

    gScore[start.x][start.y] = 0;

    openSet.push({
        start.x,
        start.y,
        0,
        heuristic(start, goal)
    });

    // Enemy hanya bergerak atas, bawah, kiri, kanan.
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        Point currentPoint{current.x, current.y};

        if (currentPoint == goal) {
            vector<Point> path;
            Point p = goal;

            while (!(p == start)) {
                path.push_back(p);
                p = parent[p.x][p.y];

                if (p.x == -1) {
                    return {};
                }
            }

            path.push_back(start);
            reverse(path.begin(), path.end());

            return path;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (!isInside(grid, nx, ny))
                continue;

            // 1 = tembok.
            if (grid[nx][ny] == 1)
                continue;

            int newG =
                gScore[current.x][current.y] + 1;

            if (newG < gScore[nx][ny]) {
                gScore[nx][ny] = newG;
                parent[nx][ny] = currentPoint;

                int h = heuristic({nx, ny}, goal);

                openSet.push({
                    nx,
                    ny,
                    newG,
                    h
                });
            }
        }
    }

    return {};
}

bool isPlayerInRange(
    Point enemy,
    Point player,
    int detectionRange
) {
    int dx = enemy.x - player.x;
    int dy = enemy.y - player.y;

    // Menggunakan jarak kuadrat agar tidak perlu sqrt().
    int distanceSquared = dx * dx + dy * dy;

    return distanceSquared <=
           detectionRange * detectionRange;
}

int main() {

    // 0 = jalan
    // 1 = tembok
    vector<vector<int>> dungeon = {
        {0, 0, 0, 1, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 1},
        {0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0}
    };

    Point enemy = {4, 0};
    Point player = {0, 5};

    int detectionRange = 6;

    // 1. Deteksi player.
    if (!isPlayerInRange(
            enemy,
            player,
            detectionRange
        )) {

        cout << "Player berada di luar jangkauan.\n";
        return 0;
    }

    cout << "Player terdeteksi!\n";

    // 2. Cari jalur menuju player menggunakan A*.
    vector<Point> path =
        aStar(dungeon, enemy, player);

    if (path.empty()) {
        cout << "Jalur menuju player tidak ditemukan.\n";
        return 0;
    }

    // 3. Enemy mengikuti path.
    cout << "Path ditemukan:\n";

    for (const auto& point : path) {
        cout << "("
             << point.x
             << ", "
             << point.y
             << ")\n";
    }

    cout << "\nEnemy bergerak menuju player...\n";

    return 0;
}
