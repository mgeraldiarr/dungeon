#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

// Struktur koordinat 2D pada dungeon
struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

// Node untuk priority queue pada algoritma A*
struct Node {
    int x, y;
    int g; // Biaya langkah dari start ke node saat ini
    int h; // Estimasi biaya dari node saat ini ke goal (Heuristik)

    int f() const {
        return g + h;
    }

    // Operator perbandingan untuk priority_queue (min-heap berdasarkan nilai f)
    bool operator>(const Node& other) const {
        return f() > other.f();
    }
};

// 1. Fungsi Heuristik: Manhattan Distance (cocok untuk grid 4 arah)
inline int heuristic(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// 2. Cek apakah koordinat berada di dalam batas grid dungeon
inline bool isInside(const vector<vector<int>>& grid, int x, int y) {
    return x >= 0 &&
           x < static_cast<int>(grid.size()) &&
           y >= 0 &&
           y < static_cast<int>(grid[0].size());
}

// 3. Algoritma A* Pathfinding: Mencari rute terpendek menghindari tembok (nilai 1)
inline vector<Point> aStar(
    const vector<vector<int>>& grid,
    Point start,
    Point goal
) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    const int INF = numeric_limits<int>::max();

    vector<vector<int>> gScore(rows, vector<int>(cols, INF));
    vector<vector<Point>> parent(rows, vector<Point>(cols, {-1, -1}));
    priority_queue<Node, vector<Node>, greater<Node>> openSet;

    gScore[start.x][start.y] = 0;
    openSet.push({start.x, start.y, 0, heuristic(start, goal)});

    // Gerakan 4 arah: Atas, Bawah, Kiri, Kanan
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        Point currentPoint{current.x, current.y};

        // Jika sudah mencapai tujuan, rekonstruksi rute dari goal kembali ke start
        if (currentPoint == goal) {
            vector<Point> path;
            Point p = goal;

            while (!(p == start)) {
                path.push_back(p);
                p = parent[p.x][p.y];
                if (p.x == -1) return {};
            }

            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (!isInside(grid, nx, ny)) continue;
            if (grid[nx][ny] == 1) continue; // 1 = Tembok, lewati

            int newG = gScore[current.x][current.y] + 1;

            if (newG < gScore[nx][ny]) {
                gScore[nx][ny] = newG;
                parent[nx][ny] = currentPoint;
                int h = heuristic({nx, ny}, goal);
                openSet.push({nx, ny, newG, h});
            }
        }
    }

    return {}; // Jalur tidak ditemukan
}

// 4. Deteksi Player: Cek apakah jarak Euclidean kuadrat <= range kuadrat
inline bool isPlayerInRange(Point enemy, Point player, int detectionRange) {
    int dx = enemy.x - player.x;
    int dy = enemy.y - player.y;
    return (dx * dx + dy * dy) <= (detectionRange * detectionRange);
}

// 5. Menghitung jarak Euclidean nyata untuk tampilan informasi
inline double getEuclideanDistance(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

#endif // ASTAR_H
