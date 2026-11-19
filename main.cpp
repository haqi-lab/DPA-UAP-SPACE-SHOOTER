#include <ncurses.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

struct Pos { int x, y; };

int main() {
    srand(time(nullptr));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(50);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    int H, W;
    getmaxyx(stdscr, H, W);
    if (H < 12 || W < 30) {
        endwin();
        puts("Terminal terlalu kecil (minimal 30x12).");
        return 1;
    }

    bool quit = false;
    while (!quit) {
        vector<Pos> bullets, enemies;
        int px = W / 2, score = 0, lives = 3, frame = 0;

        auto checkHits = [&]() {
            for (size_t b = 0; b < bullets.size();) {
                bool hit = false;
                for (size_t e = 0; e < enemies.size(); e++) {
                    if (bullets[b].x == enemies[e].x && bullets[b].y == enemies[e].y) {
                        enemies.erase(enemies.begin() + e);
                        score += 10;
                        hit = true;
                        break;
                    }
                }
                if (hit) bullets.erase(bullets.begin() + b);
                else b++;
            }
        };

        while (lives > 0 && !quit) {
            int ch = getch();
            if (ch == KEY_LEFT || ch == 'a') px--;
            if (ch == KEY_RIGHT || ch == 'd') px++;
            if (ch == ' ') bullets.push_back({px, H - 2});
            if (ch == 'q') quit = true;
            px = max(1, min(W - 2, px));

            for (auto& b : bullets) b.y--;
            for (size_t i = 0; i < bullets.size();)
                if (bullets[i].y < 1) bullets.erase(bullets.begin() + i);
                else i++;
            checkHits();

            int interval = max(2, 6 - score / 100);
            if (frame % interval == 0)
                for (auto& e : enemies) e.y++;
            if (frame % 10 == 0) enemies.push_back({rand() % W, 1});
            checkHits();

            for (size_t i = 0; i < enemies.size();)
                if (enemies[i].y >= H - 1) { enemies.erase(enemies.begin() + i); lives--; }
                else i++;
            frame++;

            erase();
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "Skor: %d  Nyawa: %d  (Q keluar)", score, lives);
            for (auto& b : bullets) mvaddch(b.y, b.x, '|');
            attroff(COLOR_PAIR(3));
            attron(COLOR_PAIR(2));
            for (auto& e : enemies) mvaddch(e.y, e.x, 'V');
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(1));
            mvprintw(H - 1, px - 1, "/A\\");
            attroff(COLOR_PAIR(1));
            refresh();
        }

        if (quit) break;

        timeout(-1);
        erase();
        mvprintw(H / 2 - 1, W / 2 - 5, "GAME OVER");
        mvprintw(H / 2, W / 2 - 8, "Skor akhir: %d", score);
        mvprintw(H / 2 + 1, W / 2 - 10, "R = main lagi, Q = keluar");
        refresh();
        int ch;
        do { ch = getch(); } while (ch != 'r' && ch != 'q' && ch != ERR);
        if (ch != 'r') quit = true;
        timeout(50);
    }

    endwin();
    return 0;
}
