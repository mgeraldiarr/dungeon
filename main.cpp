#include <iostream>
#include <vector>
#include "Dungeon.h"

using namespace std;

int main() {
    // 0 = Jalan, 1 = Tembok
    vector<vector<int>> dungeon = {
        {0, 0, 0, 1, 0, 0},
        {1, 1, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 1},
        {0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0}
    };

    int detectionRange = 6;

    cout << "====================================================\n";
    cout << "   ENEMY AI DUNGEON (DETECTION & A* PATHFINDING)   \n";
    cout << "====================================================\n";
    cout << "Pilih Mode Permainan:\n";
    cout << "1. Simulasi Otomatis (Demo langkah per langkah)\n";
    cout << "2. Kontrol Manual (Gerakkan Player dengan W/A/S/D)\n";
    cout << "Pilihan Anda (1 atau 2): ";

    int choice = 1;
    if (cin >> choice) {
        string dummy;
        getline(cin, dummy); // Membersihkan sisa input newline

        if (choice == 2) {
            runInteractiveMode(dungeon, detectionRange);
        } else {
            runAutoSimulation(dungeon, detectionRange);
        }
    }

    return 0;
}
