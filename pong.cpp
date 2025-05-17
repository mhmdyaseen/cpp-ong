#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>

const int WIDTH = 80;
const int HEIGHT = 24;
const int PADDLE_HEIGHT = 4;
const char PADDLE_CHAR = '|';
const char BALL_CHAR = 'O';

class Paddle {
public:
    int x, y;
    Paddle(int startX, int startY) : x(startX), y(startY) {}
    void moveUp() { if (y > 1) y--; }
    void moveDown() { if (y < HEIGHT - PADDLE_HEIGHT - 1) y++; }
    void draw() {
        for (int i = 0; i < PADDLE_HEIGHT; i++) {
            mvaddch(y + i, x, PADDLE_CHAR);
        }
    }
};

class Ball {
public:
    int x, y;
    int dx, dy;
    Ball(int startX, int startY) : x(startX), y(startY) {
        dx = (rand() % 2) * 2 - 1;
        dy = (rand() % 2) * 2 - 1;
    }
    void move() {
        x += dx;
        y += dy;
        if (y <= 0 || y >= HEIGHT - 1) dy = -dy;
    }
    void draw() {
        mvaddch(y, x, BALL_CHAR);
    }
};

class Game {
private:
    Paddle leftPaddle, rightPaddle;
    Ball ball;
    int leftScore, rightScore;

public:
    Game() : leftPaddle(1, HEIGHT / 2 - PADDLE_HEIGHT / 2),
             rightPaddle(WIDTH - 2, HEIGHT / 2 - PADDLE_HEIGHT / 2),
             ball(WIDTH / 2, HEIGHT / 2),
             leftScore(0), rightScore(0) {}

    void run() {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);
        timeout(100);

        while (true) {
            clear();

            for (int i = 0; i < WIDTH; i++) {
                mvaddch(0, i, '-');
                mvaddch(HEIGHT - 1, i, '-');
            }

            leftPaddle.draw();
            rightPaddle.draw();

            ball.move();
            ball.draw();

            //check for collisions with paddles
            if (ball.x == leftPaddle.x + 1 && 
                ball.y >= leftPaddle.y && 
                ball.y < leftPaddle.y + PADDLE_HEIGHT) {
                ball.dx = -ball.dx;
            }
            if (ball.x == rightPaddle.x - 1 && 
                ball.y >= rightPaddle.y && 
                ball.y < rightPaddle.y + PADDLE_HEIGHT) {
                ball.dx = -ball.dx;
            }

            if (ball.x <= 0) {
                rightScore++;
                ball = Ball(WIDTH / 2, HEIGHT / 2);
            }
            if (ball.x >= WIDTH - 1) {
                leftScore++;
                ball = Ball(WIDTH / 2, HEIGHT / 2);
            }

            //display scores
            mvprintw(1, WIDTH / 2 - 5, "Score: %d - %d", leftScore, rightScore);

            // Handle input
            int ch = getch();
            if (ch == 'q' || ch == 'Q') break;
            if (ch == 'w' || ch == 'W') leftPaddle.moveUp();
            if (ch == 's' || ch == 'S') leftPaddle.moveDown();
            if (ch == KEY_UP) rightPaddle.moveUp();
            if (ch == KEY_DOWN) rightPaddle.moveDown();

            refresh();
        }

        endwin();
    }
};

int main() {
    srand(time(0));
    Game game;
    game.run();
    return 0;
}
