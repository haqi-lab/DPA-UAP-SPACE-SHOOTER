#include <ncurses.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <vector>

using namespace std;

struct Object {
    int x;
    int y;
};

int main() {
    
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand(time(0));

    
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    
    if (maxX < 30 || maxY < 15) {
        endwin();
        return 0;
    }

    /
    Object player;
    player.x = maxX / 2;
    player.y = maxY - 3;

    
    vector<Object> bullets;
    vector<Object> enemies;

    
    int score = 0;
    bool gameOver = false;
    bool running = true;

    
    enemies.push_back({rand() % (maxX - 2) + 1, 3});

    
    int frame = 0;

    while (running && !gameOver) {

        clear();

        
        getmaxyx(stdscr, maxY, maxX);

        
        int key = getch();

        

        if (key == KEY_LEFT && player.x > 1) {
            player.x--;
        }

        if (key == KEY_RIGHT && player.x < maxX - 2) {
            player.x++;
        }

        
        if (key == ' ') {
            bullets.push_back({player.x, player.y - 1});
        }

        
        if (key == 'q' || key == 'Q') {
            running = false;
            break;
        }

        

        for (int i = 0; i < (int)bullets.size(); i++) {
            bullets[i].y--;

            // Menghapus peluru yang keluar layar
            if (bullets[i].y <= 1) {
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }

  

        frame++;

        
        if (frame % 5 == 0) {
            for (int i = 0; i < (int)enemies.size(); i++) {
                enemies[i].y++;
            }
        }

        
        if (frame % 20 == 0) {
            enemies.push_back({
                rand() % (maxX - 2) + 1,
                2
            });
        }

        
        for (int i = 0; i < (int)bullets.size(); i++) {
            for (int j = 0; j < (int)enemies.size(); j++) {

                if (bullets[i].x == enemies[j].x &&
                    bullets[i].y == enemies[j].y) {

                    score += 10;

                    bullets.erase(bullets.begin() + i);
                    enemies.erase(enemies.begin() + j);

                    i--;
                    break;
                }
            }
        }

        
        for (int i = 0; i < (int)enemies.size(); i++) {

            if (enemies[i].x == player.x &&
                enemies[i].y == player.y) {

                gameOver = true;
            }

            
            if (enemies[i].y >= player.y) {
                gameOver = true;
            }
        }

        
        mvprintw(0, 2, "SPACE SHOOTER");
        mvprintw(0, maxX - 15, "SCORE: %d", score);

        
        for (int x = 0; x < maxX; x++) {
            mvaddch(1, x, '-');
        }

        
        mvaddch(player.y, player.x, '^');

    
        for (int i = 0; i < (int)bullets.size(); i++) {
            mvaddch(bullets[i].y, bullets[i].x, '|');
        }

        
        for (int i = 0; i < (int)enemies.size(); i++) {
            mvaddch(enemies[i].y, enemies[i].x, 'W');
        }

        mvprintw(maxY - 1, 2,
                 "LEFT/RIGHT: Move | SPACE: Shoot | Q: Quit");

        refresh();

        
        napms(80);
    }

    if (gameOver) {
        clear();

        mvprintw(maxY / 2 - 1,
                 maxX / 2 - 5,
                 "GAME OVER!");

        mvprintw(maxY / 2,
                 maxX / 2 - 7,
                 "FINAL SCORE: %d", score);

        mvprintw(maxY / 2 + 2,
                 maxX / 2 - 11,
                 "Press any key to exit");

        refresh();

        nodelay(stdscr, FALSE);
        getch();
    }

    
    endwin();

    return 0;
}

