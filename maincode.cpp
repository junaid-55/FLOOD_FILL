#include <time.h>
#include "iGraphicsnew.h"

// global variable
// DO NOT TOUCH HERE

const int n = 15;
int bg_x = 1500, bg_y = 780;
int board[n][n];
int button_x = 1200, button_y = 500;
int block_size = 40;
int x = 2;
bool page1 = false, page2 = false, page3 = false;
int currentpage = 0;
int colors = 7;
bool status = false;
char str[100];
bool checktext = true;
int len = 0;
int score = 0, life = 10, level = 0;
char rem[10];

// structure for player
typedef struct
{
    char name[100];
    int temp;

} player;
player first, lead[100], high;
// structure for button
typedef struct
{
    int x;
    int y;
    int r;
    int s;
} buttons;
buttons button[n];
buttons play, res, pausb, name, leaderboard;
// struct for color
typedef struct
{
    int r;
    int g;
    int b;
} RGB;
RGB color[n], textc;

void floodFill(int board[n][n], int n, int x, int y, int targetColor, int newColor)
{
    if (x < 0 || x >= n || y < 0 || y >= n)
    {

        return;
    }
    else if (targetColor == newColor)
    {
        score++;
        return;
    }

    else if (board[x][y] != targetColor)
    {
        return;
    }
    else
    {
        board[x][y] = newColor;
        score++;
    }
    // board[0][0] = newColor;

    floodFill(board, n, x + 1, y, targetColor, newColor);
    floodFill(board, n, x - 1, y, targetColor, newColor);
    floodFill(board, n, x, y + 1, targetColor, newColor);
    floodFill(board, n, x, y - 1, targetColor, newColor);
}

void fillinit(int user_in)
{
    int targetColor = board[0][0];
    int newColor = user_in;
    // if (newColor == 5)
    floodFill(board, n, 0, 0, targetColor, newColor);
}

void generateColor()
{
    color[0] = {28, 109, 57};
    color[1] = {150, 155, 150};
    color[2] = {200, 36, 58};
    color[3] = {12, 45, 56};
    color[4] = {249, 207, 87};
    color[5] = {47, 92, 175};
    color[6] = {212, 106, 143};
    textc = {150, 150, 150};
}
// Compare function for QuickSort
int compare(const void *a, const void *b)
{
    player *p1 = (player *)a;
    player *p2 = (player *)b;
    return p2->temp - p1->temp;
}
// file handling
void fileread()
{
    FILE *output = fopen("leaderboard.txt", "r");
    if (output == NULL)
        return;
    int n = 0;
    while (fread(&lead[n], sizeof(player), 1, output))
    {
        n++;
    }
    // Sort the data based on the temp field using QuickSort
    qsort(lead, n, sizeof(player), compare);
    // Open the file for writing
    output = fopen("leaderboard.txt", "w");
    if (output == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    // Write the sorted data back to the file
    for (int i = 0; i < n; i++)
    {
        fwrite(lead[i].name, sizeof(char) * 100, 1, output);
        fwrite(&lead[i].temp, sizeof(int), 1, output);
    }
    fclose(output);
}

void playeradd()
{
    FILE *input = fopen("leaderboard.txt", "a");
    fwrite(first.name, sizeof(char) * 100, 1, input);
    fwrite(&first.temp, sizeof(int), 1, input);
    fclose(input);
}
bool checkfinish(void)
{
    int num = board[0][0];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (board[i][j] != num)
                return false;
        }
    }
    return true;
}
void levelchange(int l)
{

    x = 2 + (l - 1);
    life = 10 + 5 * (l - 1);

    srand(time(NULL));
    board[n][n];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            board[i][j] = rand() % x;
        }
    }
    // level = level+1;
}
void pageZERO()
{

    iSetColor(0, 255, 65);
    name = {bg_x / 2 - 100, bg_y / 2, 250, 40};
    iRectangle(name.x, name.y, name.r, name.s);
    iText(10, 10, "PRESS ENTER TO START", GLUT_BITMAP_9_BY_15);
    iText(name.x - 200, name.y + 15, "TYPE YOUR NAME", GLUT_BITMAP_9_BY_15);
    iText(name.x + 10, name.y + 15, str, GLUT_BITMAP_9_BY_15);
}
void pageONE()
{

    /*int index = 20, pix_size=100;
    int background[index][index];*/

    play = {(bg_x - 350) / 2, (bg_y - 60) / 2, 350, 115};
    res = {play.x, play.y - 100, 300, 97};
    leaderboard = {res.x, res.y - 75, 300, 69};

    iShowBMP(play.x, play.y, "play.bmp");
    iShowBMP2(res.x, res.y, "resume.bmp", 0);
    iShowBMP(leaderboard.x, leaderboard.y, "leaderboard.bmp");
}

void pageTWO()
{
    int grid_x = (bg_x - n * block_size) / 2, grid_y = bg_y * 9 / 11;
    // display the game board design
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int k = board[i][j];
            iSetColor(color[k].r, color[k].g, color[k].b);
            iFilledRectangle(grid_x, grid_y, block_size, block_size);
            grid_x = grid_x + block_size;
        }
        grid_x = (bg_x - n * block_size) / 2;
        grid_y = grid_y - block_size;
    }

    // generating buttons
    for (int i = 0; i < colors; i++)
    {
        button[i].x = (grid_x + i * 2.9 * block_size) - 30, button[i].y = (bg_y / 100) * 4, button[i].r = block_size - 10, button[i].s = 1000;
        iSetColor(color[i].r, color[i].g, color[i].b);
        iFilledCircle(button[i].x, button[i].y, button[i].r, button[i].s);
    }
    pausb.x = bg_x - 300, pausb.y = bg_y - 200, pausb.r = 300, pausb.s = 78;
    iShowBMP2(pausb.x, pausb.y, "pause.bmp", 0);
    char tempo[25];
    sprintf(tempo, "SCORE    %d", score);
    iSetColor(0, 255, 65);
    iText(pausb.x + 5, pausb.y - 100, tempo, GLUT_BITMAP_9_BY_15);

    sprintf(tempo, "LEVEL    %d", level);
    iText(pausb.x + 5, pausb.y - 130, tempo, GLUT_BITMAP_9_BY_15);

    sprintf(tempo, "HIGH SCORE   %d", high.temp);
    iText(pausb.x + 5, pausb.y - 160, tempo, GLUT_BITMAP_9_BY_15);

    sprintf(tempo, "REMAINING MOVE   %d", life);
    status = checkfinish();
    iSetColor(0, 255, 65);
    iText((bg_x / 2) - 160, grid_y / 2 + (n + 2.5) * block_size, tempo, GLUT_BITMAP_9_BY_15);
    if (status == true && level != 7)
    {
        level++;
        levelchange(level + 1);
    }
}
void pageTHREE()
{
    if (status == false)
    {
        iSetColor(0, 0, 0);
        iFilledRectangle(0, 0, bg_x, bg_y);
        iShowBMP(500, 200, "game_over.bmp");
        iSetColor(255, 255, 255);
        pausb.x = bg_x - 300, pausb.y = bg_y - 200, pausb.r = 300, pausb.s = 78;
        iShowBMP2(pausb.x, pausb.y, "pause.bmp", 0);
        // FILE *output =fopen("leaderboard.txt", "r");
    }
    else if (level == 7 && status == true)
    {
        // iFilledRectangle(0, 0, bg_x, bg_y);
        iShowBMP(500, 200, "happy.bmp");
        pausb.x = bg_x - 300, pausb.y = bg_y - 200, pausb.r = 300, pausb.s = 78;
        iShowBMP2(pausb.x, pausb.y, "pause.bmp", 0);
    }
}

/*void pageFOUR()
{
    pausb.x = bg_x-300, pausb.y = bg_y-200,pausb.r = 300, pausb.s =78;
    iShowBMP2(pausb.x,pausb.y, "pause.bmp",0);
    char buffer[20];
    int data;
    char temp[10];
    FILE *output =fopen("leaderboard.bin", "rb");
    iSetColor(0, 0 ,0);
    iFilledRectangle(0, 0, bg_x, bg_y);
    buttons box[5];
    for(int i=0; i<5; i++)
    {

        box[i] ={(bg_x-300)/2, (bg_y-300)-i*80, 300, 70};
        iSetColor(66, 66, 66);
        iFilledRectangle(box[i].x, box[i].y, box[i].r, box[i].s);

        /*fread(buffer, sizeof(buffer), 1, output);
        fread(&data, sizeof(int), 1, output);
        itoa(data, temp, 10);
        iSetColor(255, 255, 255);
        iText(box[i].x+70, box[i].y+55, buffer, GLUT_BITMAP_TIMES_ROMAN_24);
       // iText(box[i].x+150, box[i].y+30, temp, GLUT_BITMAP_TIMES_ROMAN_24);
    }

}*/
void pageFOUR()
{

    iSetColor(0, 0, 0);
    iFilledRectangle(0, 0, bg_x, bg_y);
    pausb.x = bg_x - 300, pausb.y = bg_y - 200, pausb.r = 300, pausb.s = 78;
    iShowBMP2(pausb.x, pausb.y, "pause.bmp", 0);
    buttons box[5];
    for (int i = 0; i < 5; i++)
    {

        box[i] = {(bg_x - 300) / 2, (bg_y - 300) - i * 80, 300, 70};
        iSetColor(120, 120, 120);
        iFilledRectangle(box[i].x, box[i].y, box[i].r, box[i].s);

        char buffer[10];
        itoa(lead[i].temp, buffer, 10);
        iSetColor(255, 255, 255);
        iText(box[i].x + 15, box[i].y + 30, lead[i].name, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(box[i].x + 180, box[i].y + 30, buffer, GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

void iDraw()
{
    iClear();
    // grid color pallette declaration
    generateColor();

    // background work
    iSetColor(0, 0, 0);
    iFilledRectangle(0, 0, bg_x, bg_y);
    switch (currentpage)
    {
    case 0:
        pageZERO();
        break;
    case 1:
        pageONE();
        break;
    case 2:
        pageTWO();
        break;
    case 3:
        pageTHREE();
        break;
    case 4:
        pageFOUR();
    }
}

bool isInsideCircle(buttons btn, int x, int y)
{
    int dx = x - btn.x;
    int dy = y - btn.y;
    // use Pythagorean theorem to check if (x, y) is within circle
    if (dx * dx + dy * dy <= btn.r * btn.r)
        return true;
    else
        return false;
}

bool isInsideRectangle(buttons btn, int x, int y)
{
    if (x >= btn.x && x <= btn.x + btn.r && y >= btn.y && y <= btn.y + btn.s)
        return true;
    else
        return false;
}

int pause()
{
    FILE *input = fopen("stat.bin", "wb");
    if (input == NULL)
    {
        return 1;
    }
    // fwrite(&n, sizeof(n),1, input );
    fwrite(&bg_x, sizeof(int), 1, input);
    fwrite(&bg_y, sizeof(int), 1, input);
    fwrite(board, sizeof(board), 1, input);
    fwrite(&button_x, sizeof(int), 1, input);
    fwrite(&button_y, sizeof(int), 1, input);
    fwrite(&block_size, sizeof(int), 1, input);
    fwrite(&x, sizeof(int), 1, input);
    fwrite(&currentpage, sizeof(int), 1, input);
    fwrite(&life, sizeof(life), 1, input);
    fwrite(&level, sizeof(int), 1, input);
}

int resume()
{
    FILE *input = fopen("stat.bin", "rb");
    //  if(input != NULL)
    //{
    // fread(&n, sizeof(n),1, input );
    fread(&bg_x, sizeof(int), 1, input);
    fread(&bg_y, sizeof(int), 1, input);
    fread(board, sizeof(board), 1, input);
    fread(&button_x, sizeof(int), 1, input);
    fread(&button_y, sizeof(int), 1, input);
    fread(&block_size, sizeof(int), 1, input);
    fread(&x, sizeof(int), 1, input);
    fread(&currentpage, sizeof(int), 1, input);
    fread(&life, sizeof(life), 1, input);
    fread(&level, sizeof(int), 1, input);
    //    }
}

void iMouse(int butt, int state, int mx, int my)
{
    if (butt == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        switch (currentpage)
        {
        case 1:
            //  checking if play buttton is clicked
            if (isInsideRectangle(play, mx, my) == true)
            {
                score = 0;
                level = 1;
                levelchange(1);
                currentpage++;
            }
            // checking if resume button is clicked
            else if (isInsideRectangle(res, mx, my) == true)
            {
                resume();
                currentpage = 2;
            }
            else if (isInsideRectangle(leaderboard, mx, my) == true)
            {
                fileread();
                currentpage = 4;
            }
            break;
        case 2:

            for (int i = 0; i < colors; i++)
            {
                bool check = isInsideCircle(button[i], mx, my);
                if (check == true)
                {
                    life--;
                    fillinit(i);
                }
            }
            if (isInsideRectangle(pausb, mx, my) == true)
            {
                pause();
                currentpage = 1;
            }

            if ((life == 0 && status == false) || (status == true && level == 7))
                currentpage = 3;
            break;
        case 3:
            if (isInsideRectangle(pausb, mx, my) == true)
            {
                first.temp = score;
                playeradd();
                currentpage = 1;
            }
        case 4:
            if (isInsideRectangle(pausb, mx, my) == true)
            {
                currentpage = 1;
            }
        }
    }
}

void iKeyboard(unsigned char key)
{
    if (currentpage == 0)
    {
        if (key == '\r')
        {
            strcpy(first.name, str);
            currentpage = 1;
        }
        else
        {

            if (key != '\b')
            {
                str[len] = key;
                str[len + 1] = '\0';
                str[20] = '\0';
                len++;
            }
            else
            {
                str[len - 1] = '\0';
                len--;
            }
        }
    }
    // place your codes for other keys here
}

void iSpecialKeyboard(unsigned char key)
{
    // void
}
void iMouseMove(int mx, int my)
{
    // void
}

int main()
{
    FILE *output = fopen("leaderboard.txt", "r");
    if (output == NULL)
        return 0;
    fread(&high, sizeof(player), 1, output);
    fclose(output);
    iInitialize(bg_x, bg_y, "FLOOD FILL");
    return 0;
}
