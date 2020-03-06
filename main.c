#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

struct Point {
    int x;
    int y;
};

int xPos, yPos, score, length, quit, gameOver, add, ch, width, height;
struct Point snake[500], currentFruit;

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
enum direction dir;

void newFruit(){
    score += 10;
    currentFruit.x = (rand() % (width - 2)) + 1;
    currentFruit.y = (rand() % (height - 2)) + 1;

    for (int i = 0; i < length; i++) {
        if(snake[i].x == currentFruit.x && snake[i].y == currentFruit.y){
            newFruit();
        }
    }
}

void init(){
    srand(time(NULL));
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(75);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    width = 100;
    height = 50;
    length = 5;
    xPos = width / 2;
    yPos = height / 2;
    dir = RIGHT;
    quit = 0;
    gameOver = 0;
    add = 0;

    for(int i = 0; i < length; i++){
        snake[i].x = (-i) + xPos;
        snake[i].y = yPos;
    }
    newFruit();

    score = 0;
}

void drawBoard() {
    attrset(COLOR_PAIR(2));

    for(int i = 0; i < width; i++){
        mvprintw(0, i, "#");
        mvprintw(height, i, "#");        
    }
    for(int i = 0; i < height; i++){
        mvprintw(i, 0, "#");
        mvprintw(i, width, "#");        
    }
}

void reset(){
    gameOver = 0;
    length = 5;
    xPos = width / 2;
    yPos = height / 2;
    dir = RIGHT;
    for(int i = 0; i < length; i++){
        snake[i].x = (-i) + xPos;
        snake[i].y = yPos;
    };
    newFruit();
    score = 0;
}

int main() {
    init();
    while (!quit) {
        ch = getch();

        switch (ch) {
        case KEY_UP:
            if(dir != DOWN){dir = UP;}
            break;
        case KEY_DOWN:
            if(dir != UP){dir = DOWN;}
            break;
        case KEY_RIGHT:
            if(dir != LEFT){dir = RIGHT;}
            break;
        case KEY_LEFT:
            if(dir != RIGHT){dir = LEFT;}
            break;
        case 'q':
            quit = 1;
            break;
        case 'r':
            if(gameOver){reset();}
            break;
        }

        erase();
    
        drawBoard();
    
        if(!gameOver){

            if (dir == UP) {
                yPos--;
            } else if (dir == DOWN) {
                yPos++;
            } else if (dir == RIGHT) {
                xPos++;
            } else if (dir == LEFT) {
                xPos--;
            }

            attrset(COLOR_PAIR(1));

            struct Point snake_temp[add == 1 ? length + 1 : length];

            snake_temp[0].x = xPos;
            snake_temp[0].y = yPos;
            
            for(int i = 1; i < length; i++){
                snake_temp[i] = snake[i-1];
            }
            
            if(add == 1){
                snake_temp[length] = snake[length - 1];
                length++;
                add = 0;
            }


            for(int i = 0; i < length; i++){
                snake[i] = snake_temp[i];
                if(i == 0){
                    mvprintw(snake[i].y, snake[i].x, "H");
                } else if(i == 1){
                    mvprintw(snake[i].y, snake[i].x, "I");
                } else {
                    mvprintw(snake[i].y, snake[i].x, "S");
                }
            }

            if(xPos == currentFruit.x && yPos == currentFruit.y){
                newFruit();
                add = 1;
            }

            mvprintw(currentFruit.y, currentFruit.x, "*");

            mvprintw(height+1, width/2, "Score: %d", score);

            if(xPos == 0 || yPos == 0 || xPos == width || yPos == height){
                gameOver = 1;
            }

            for(int i = 1; i < length; i++){
                if(xPos == snake[i].x && yPos == snake[i].y){
                    gameOver = 1;
                }
            }
        }
        else{
            attrset(COLOR_PAIR(1));
            mvprintw((height / 2), (width / 2)-5, "GAME OVER");
        }

    }

    timeout(-1);

    refresh();
    getch();
    endwin();

}