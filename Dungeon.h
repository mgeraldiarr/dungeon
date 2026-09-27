#ifndef DUNGEON_H
#define DUNGEON_H

#include "AStar.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Menampilkan visualisasi peta Dungeon di terminal
inline void renderDungeon(
    const vector<vector<int>>& grid,
    Point enemy,
    Point player,
    const vector<Point>& path = {}
) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    cout << "\n   ";
    for (int c = 0; c < cols; ++c) {
        cout << " " << c << " ";
    }
    cout << "\n";

    for (int r = 0; r < rows; ++r) {
        cout << " " << r << " ";
        for (int c = 0; c < cols; ++c) {
            Point pt{r, c};

            if (pt == enemy && pt == player) {
                cout << "[!]"; // Tertangkap
            } else if (pt == player) {
                cout << "[P]"; // Player
            } else if (pt == enemy) {
                cout << "[E]"; // Enemy
            } else if (grid[r][c] == 1) {
                cout << "###"; // Tembok
            } else {
                // Tampilkan simbol '*' jika titik ini adalah rute A* yang direncanakan
                bool isPathNode = false;
                for (size_t i = 1; i + 1 < path.size(); ++i) {
                    if (path[i] == pt) {
                        isPathNode = true;
                        break;
                    }
                }
                if (isPathNode) {
                    cout << " * ";
                } else {
                    cout << " . ";
                }
            }
        }
        cout << "\n";
    }
    cout << "Keterangan: [P]=Player, [E]=Enemy, ###=Tembok, [.]=Jalan, [*]=Jalur A*\n";
}

// Logika pergerakan Enemy untuk 1 giliran (turn)
inline void enemyTurn(
    const vector<vector<int>>& dungeon,
    Point& enemy,
    Point player,
    int detectionRange
) {
    double dist = getEuclideanDistance(enemy, player);
    cout << "\n--- Giliran Enemy ---\n";
    cout << "Jarak ke Player: " << dist << " (Detection Range: " << detectionRange << ")\n";

    // 1. Cek apakah Player berada dalam jangkauan deteksi
    if (!isPlayerInRange(enemy, player, detectionRange)) {
        cout << "Status: [DI LUAR JANGKAUAN] Player belum terdeteksi. Enemy diam / patroli.\n";
        return;
    }

    cout << "Status: [TERDETEKSI!] Player berada dalam jangkauan!\n";
    cout << "Enemy menjalankan A* Pathfinding menuju Player...\n";

    // 2. Cari jalur terpendek menggunakan A*
    vector<Point> path = aStar(dungeon, enemy, player);

    if (path.empty()) {
        cout << "Jalur menuju player terhalang total oleh tembok.\n";
        return;
    }

    cout << "Path A* terdekat: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << "(" << path[i].x << "," << path[i].y << ")";
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\n";

    // 3. Enemy melangkah 1 petak ke node berikutnya pada path
    if (path.size() >= 2) {
        enemy = path[1];
        cout << ">> Enemy bergerak maju ke (" << enemy.x << ", " << enemy.y << ")\n";
    }
}

// Mode 1: Simulasi Otomatis (Demo Langkah per Langkah)
inline void runAutoSimulation(const vector<vector<int>>& dungeon, int detectionRange) {
    Point enemy = {4, 0};
    Point player = {0, 5};

    // Rute pergerakan otomatis Player di dalam dungeon
    vector<Point> playerMoves = {
        {0, 4}, // Langkah 1
        {1, 4}, // Langkah 2 (Mulai masuk jangkauan deteksi)
        {2, 4}, // Langkah 3
        {2, 3}, // Langkah 4
        {2, 2}, // Langkah 5
        {2, 1}, // Langkah 6
        {2, 0}  // Langkah 7
    };

    cout << "\n============================================\n";
    cout << "         MODE: SIMULASI OTOMATIS            \n";
    cout << "============================================\n";
    cout << "Player akan bergerak mengikuti rute yang ditentukan.\n";
    cout << "Enemy akan mendeteksi, mencari jalur (A*), dan mengejar.\n";
    cout << "Tekan [Enter] untuk menjalankan setiap giliran (turn).\n\n";

    int turn = 0;
    renderDungeon(dungeon, enemy, player);

    string dummy;
    cout << "\nTekan [Enter] untuk memulai langkah pertama...";
    getline(cin, dummy);

    for (Point nextPlayerPos : playerMoves) {
        turn++;
        cout << "\n============================================\n";
        cout << "                 GILIRAN " << turn << "                 \n";
        cout << "============================================\n";

        // 1. Player bergerak
        player = nextPlayerPos;
        cout << "[Player] Bergerak ke posisi (" << player.x << ", " << player.y << ")\n";

        // 2. Enemy merespons
        enemyTurn(dungeon, enemy, player, detectionRange);

        // Render dungeon terkini
        vector<Point> currentPath = {};
        if (isPlayerInRange(enemy, player, detectionRange)) {
            currentPath = aStar(dungeon, enemy, player);
        }
        renderDungeon(dungeon, enemy, player, currentPath);

        // 3. Cek apakah tertangkap
        if (enemy == player) {
            cout << "\n[!] TERTANGKAP! Enemy berhasil mengejar dan menangkap Player!\n";
            return;
        }

        cout << "\nTekan [Enter] untuk lanjut ke giliran berikutnya...";
        getline(cin, dummy);
    }

    // Jika player berhenti bergerak tapi belum tertangkap, enemy terus mengejar
    while (enemy != player) {
        turn++;
        cout << "\n============================================\n";
        cout << "                 GILIRAN " << turn << "                 \n";
        cout << "============================================\n";
        cout << "[Player] Diam di posisi (" << player.x << ", " << player.y << ")\n";

        enemyTurn(dungeon, enemy, player, detectionRange);

        vector<Point> currentPath = {};
        if (isPlayerInRange(enemy, player, detectionRange)) {
            currentPath = aStar(dungeon, enemy, player);
        }
        renderDungeon(dungeon, enemy, player, currentPath);

        if (enemy == player) {
            cout << "\n[!] TERTANGKAP! Enemy berhasil mengejar dan menangkap Player!\n";
            return;
        }

        cout << "\nTekan [Enter] untuk lanjut...";
        getline(cin, dummy);
    }
}

// Mode 2: Interaktif (Kontrol Manual Player WASD)
inline void runInteractiveMode(const vector<vector<int>>& dungeon, int detectionRange) {
    Point enemy = {4, 0};
    Point player = {0, 5};

    cout << "\n============================================\n";
    cout << "          MODE: KONTROL MANUAL (WASD)       \n";
    cout << "============================================\n";
    cout << "Kontrol:\n";
    cout << "  W = Atas, S = Bawah, A = Kiri, D = Kanan\n";
    cout << "  Q = Keluar permainan\n";

    int turn = 0;

    while (true) {
        turn++;
        cout << "\n============================================\n";
        cout << "                 GILIRAN " << turn << "                 \n";
        cout << "============================================\n";

        vector<Point> previewPath = {};
        if (isPlayerInRange(enemy, player, detectionRange)) {
            previewPath = aStar(dungeon, enemy, player);
        }
        renderDungeon(dungeon, enemy, player, previewPath);

        cout << "\nPosisi Player: (" << player.x << ", " << player.y << ")\n";
        cout << "Posisi Enemy : (" << enemy.x << ", " << enemy.y << ")\n";
        cout << "Masukkan gerakan (W/A/S/D atau Q): ";

        char input;
        if (!(cin >> input)) break;
        input = tolower(input);

        if (input == 'q') {
            cout << "Permainan dihentikan.\n";
            break;
        }

        int nx = player.x;
        int ny = player.y;

        if (input == 'w') nx--;
        else if (input == 's') nx++;
        else if (input == 'a') ny--;
        else if (input == 'd') ny++;
        else {
            cout << "[!] Input tidak valid! Gunakan W, A, S, atau D.\n";
            continue;
        }

        // Cek validasi pergerakan Player
        if (!isInside(dungeon, nx, ny)) {
            cout << "[!] Tidak bisa bergerak ke luar area dungeon!\n";
            continue;
        }
        if (dungeon[nx][ny] == 1) {
            cout << "[!] Menabrak tembok! Pilih arah lain.\n";
            continue;
        }

        // 1. Player bergerak
        player = {nx, ny};
        cout << ">> Player berpindah ke (" << player.x << ", " << player.y << ")\n";

        // Cek apakah player sengaja menabrak enemy
        if (player == enemy) {
            renderDungeon(dungeon, enemy, player);
            cout << "\n[!] TERTANGKAP! Player menabrak Enemy!\n";
            break;
        }

        // 2. Giliran Enemy mengejar
        enemyTurn(dungeon, enemy, player, detectionRange);

        // 3. Cek apakah enemy menangkap player
        if (enemy == player) {
            renderDungeon(dungeon, enemy, player);
            cout << "\n[!] GAME OVER! Enemy berhasil menangkap Player!\n";
            break;
        }
    }
}

#endif // DUNGEON_H
