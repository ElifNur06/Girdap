#include <stdio.h>
#include <stdlib.h>

// Bükücünün şifreyi çözmek için arayacağı anahtar
int g_hyper_seed = 42;

// Prosedürel Grid Üretici
// Dışarıdan veri çekmeden tamamen yerel algoritmalarla seviye inşası
void generate_procedural_grid(int grid[10][10], unsigned int seed) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // LCG (Linear Congruential Generator) ile sahte rastgele hücre üretimi
            seed = (seed * 1103515245 + 12345) & 0x7fffffff;
            grid[i][j] = seed % 100;
        }
    }
}

// Dinamik Grid Çözücü
// Oluşturulan harita üzerinde anlık olarak mantıksal yol (pathfinding) bulur
int solve_logic_grid(int grid[10][10]) {
    int score = 0;
    int x = 0, y = 0;
    
    // Matrisin sağ alt köşesine ulaşana kadar ilerle
    while (x < 9 && y < 9) {
        score += grid[x][y];
        
        // Hücrenin içeriğine göre yön belirle (Hiperbolik bükücü burayı parçalayacak)
        if (grid[x][y] % 2 == 0) {
            x++; // Sağa git
        } else if (grid[x][y] % 3 == 0) {
            y++; // Aşağı git
        } else {
            x++; 
            y++; // Çapraz git
        }
    }
    return score;
}

int main() {
    printf("[*] Prosedurel Matris (Offline Grid) Testi Basliyor...\n");
    
    int logic_grid[10][10];
    // 1337 tohumuyla deterministik bir seviye üret
    generate_procedural_grid(logic_grid, 1337);
    
    // Üretilen bölümü çöz
    int final_score = solve_logic_grid(logic_grid);
    
    printf("[*] Prosedurel Harita Basariyla Olusturuldu ve Cozuldu!\n");
    printf("[*] OMEGA Matris Skoru: %d\n", final_score);
    
    return 0;
}