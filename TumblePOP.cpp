#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 

using namespace sf;
using namespace std;

int screen_x = 1136;
int screen_y = 896;


//Checking Collisions
bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}



void display_level(RenderWindow& window, char** lvl, Texture& bgTex, Sprite& bgSprite,
    Texture& blockTexture, Sprite& blockSprite, const int height,
    const int width, const int cell_size)
{
    window.draw(bgSprite);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (lvl[i][j] == '#')
            {
                blockSprite.setPosition(j * cell_size, i * cell_size);
                window.draw(blockSprite);
            }
        }
    }
}


//General function for Animations
void animation(Sprite& sprite, float& timer, float timeSinceLast, float frameSpeed, int& currentFrame, int maxFrames, int frameWidth, int frameHeight, int startingX, int startingY)
{
    timer += timeSinceLast;

    if (timer >= frameSpeed)
    {
        currentFrame++;
        if (currentFrame >= maxFrames)
            currentFrame = 0;

        sprite.setTextureRect(IntRect(startingX + currentFrame * frameWidth, startingY, frameWidth, frameHeight));
        timer = 0;
    }
}


// Player's Gravity
void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround,
    const float gravity, const float terminal_Velocity, float& player_x,
    float& player_y, const int cell_size, const int height, const int width, int& Pheight, int& Pwidth, bool dropDown)
{
    offset_y = player_y + velocityY;

    int tileY = (int)(offset_y + Pheight + 2.0f) / cell_size;
    int leftX = (int)(player_x + 5.0f) / cell_size;
    int midX = (int)(player_x + Pwidth / 2) / cell_size;
    int rightX = (int)(player_x + Pwidth - 5.0f) / cell_size;

    //Clamping
    if (tileY < 0) tileY = 0;
    else if (tileY >= height) tileY = height - 1;

    if (leftX < 0) leftX = 0;
    else if (leftX >= width) leftX = width - 1;

    if (midX < 0) midX = 0;
    else if (midX >= width) midX = width - 1;

    if (rightX < 0) rightX = 0;
    else if (rightX >= width) rightX = width - 1;

    char bottom_left = lvl[tileY][leftX];
    char bottom_mid = lvl[tileY][midX];
    char bottom_right = lvl[tileY][rightX];

    bool hitPlatform = (bottom_left == '#' || bottom_mid == '#' || bottom_right == '#');

    if (tileY == 0)
    {
        if (velocityY < 0)
        {
            velocityY = 0;
            player_y = 0;
            return;
        }
    }

    if (hitPlatform && velocityY >= 0 && tileY != 0 && !dropDown)
    {
        onGround = true;
        velocityY = 0;
        player_y = tileY * cell_size - Pheight;
        return;
    }
    else
    {
        onGround = false;
        player_y = offset_y;
        velocityY += gravity;

        if (velocityY > terminal_Velocity)
            velocityY = terminal_Velocity;
    }
}

//Player's Movement
void handlePlayerMovement(char** lvl, float& player_x, float& player_y,
    const float PlayerWidth, const float PlayerHeight,
    const float finalSpeed, float& velocityY, bool& onGround,
    const float jumpStrength, const int cell_size, RenderWindow& window)
{
    if (Keyboard::isKeyPressed(Keyboard::Escape))
        exit(0);

    const float PADDING = 2.0f;

    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        float next_x = player_x - finalSpeed;
        if (lvl[(int)(player_y + PlayerHeight / 2) / cell_size]
            [(int)(next_x + PADDING) / cell_size] != '#')
        {
            player_x = next_x;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        float next_x = player_x + finalSpeed;
        if (lvl[(int)(player_y + PlayerHeight / 2) / cell_size]
            [(int)(next_x + PlayerWidth - PADDING) / cell_size] != '#')
        {
            player_x = next_x;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Up) && onGround)
    {
        velocityY = jumpStrength;
        onGround = false;
    }
}

// Level 1 Platforms

void platform_lvl1(char** lvl)
{
    lvl[11][1] = '#'; lvl[11][2] = '#'; lvl[11][3] = '#'; lvl[11][4] = '#'; lvl[11][5] = '#'; lvl[11][6] = '#';
    lvl[11][11] = '#'; lvl[11][12] = '#'; lvl[11][13] = '#'; lvl[11][14] = '#'; lvl[11][15] = '#'; lvl[11][16] = '#';

    lvl[8][5] = '#'; lvl[8][6] = '#'; lvl[8][7] = '#'; lvl[8][8] = '#';
    lvl[8][9] = '#'; lvl[8][10] = '#'; lvl[8][11] = '#'; lvl[8][12] = '#';

    lvl[5][7] = '#'; lvl[5][3] = '#'; lvl[5][4] = '#'; lvl[5][5] = '#'; lvl[5][6] = '#';
    lvl[5][11] = '#'; lvl[5][12] = '#'; lvl[5][13] = '#'; lvl[5][14] = '#'; lvl[5][10] = '#';

    lvl[3][5] = '#'; lvl[3][6] = '#'; lvl[3][7] = '#'; lvl[3][8] = '#';
    lvl[3][9] = '#'; lvl[3][10] = '#'; lvl[3][11] = '#'; lvl[3][12] = '#';
}

//Level 2 Platforms with random
void platformLevel2(char** lvl, int lvlHeight, int lvlWidth) {
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 18; j++) {
            lvl[i][j] = ' ';
        }
    }
    for (int i = 0; i < 14; i++) {
        lvl[i][0] = '#';
        lvl[i][lvlWidth - 1] = '#';
    }
    for (int i = 0; i < 18; i++) {
        lvl[0][i] = '#';
        lvl[lvlHeight - 1][i] = '#';
    }

    int made = 0;
    int targetPlatforms = 12;   
    int tries = 0;

    while (made < targetPlatforms && tries < 500)
    {
        tries++;

        int y = 3 + rand() % 8;
        int x = 2 + rand() % 12;
        int len = 3 + rand() % 3;

        bool overlaps = false;
        for (int j = 0; j < len; j++)
        {
            int cx = x + j;

            if (lvl[y][cx] == '#') { overlaps = true; break; }
            if (lvl[y + 1][cx] == '#') { overlaps = true; break; }
            if (lvl[y - 1][cx] == '#') { overlaps = true; break; }
        }

        if (overlaps) continue;

        // Place platform
        for (int j = 0; j < len; j++)
            lvl[y][x + j] = '#';

        made++;
    }
}

// Initializing Skeletons
void init_skeletons(int count,
    float skeleton_x[], float skeleton_y[],
    float skeleton_speed[], bool skeleton_movingRight[],
    int skeletonHeight[], int skeletonWidth[],
    bool skeletonOnGround[], float skeleton_velY[],
    float skeletonDirTimer[], float skeletonDirInterval[],
    float skeletonJumpTimer[], float skeletonJumpInterval[],
    const int cell_size)
{
    int rows[] = { 13, 13, 11, 8 };
    int cols[] = { 2, 15, 3, 10 };

    for (int i = 0; i < count; i++)
    {
        int r = rows[i % 4];
        int c = cols[i % 4];

        skeleton_x[i] = c * cell_size;
        skeleton_y[i] = r * cell_size - skeletonHeight[i];
        skeleton_speed[i] = 3;
        skeleton_movingRight[i] = true;
        skeletonOnGround[i] = true;
        skeleton_velY[i] = 0;
        skeletonDirTimer[i] = 0;
        skeletonDirInterval[i] = (rand() % 7 + 5);
        skeletonJumpTimer[i] = 0;
        skeletonJumpInterval[i] = (rand() % 5) + 4;
    }
}

//Initializing Ghosts
void init_ghosts(int count,
    float ghost_x[], float ghost_y[],
    float ghost_speed[], bool ghost_movingRight[],
    int ghostHeight[], int ghostWidth[],
    bool ghostOnGround[], float ghost_velY[],
    float ghostDirTimer[], float ghostDirInterval[],
    const int cell_size)
{
    int rows[] = { 13, 13, 11, 8, 5, 5, 11, 11 };
    int cols[] = { 6, 11, 13, 6, 4, 13, 5, 15 };

    for (int i = 0; i < count; i++)
    {
        int r = rows[i % 8];
        int c = cols[i % 8];
        ghost_x[i] = c * cell_size;
        ghost_y[i] = r * cell_size - ghostHeight[i];
        ghost_speed[i] = 4;
        ghost_movingRight[i] = true;
        ghostOnGround[i] = true;
        ghost_velY[i] = 0;
        ghostDirTimer[i] = 0;
        ghostDirInterval[i] = (rand() % 4) + 5;
    }
}


// New Level Genertions
void newLevel(char** lvl, int height, int width,
    float skeleton_x[], float skeleton_y[],
    float skeleton_speed[], bool skeleton_movingRight[],
    bool skeletonOnGround[], float skeleton_velY[],
    float skeletonJumpTimer[], float skeletonJumpInter[],
    int skeletonCount, int skeletonHeight[], int skeletonWidth[],

    float ghost_x[], float ghost_y[],
    float ghost_speed[], bool ghost_movingRight[],
    bool ghostOnGround[], float ghost_velY[],
    float ghostDirectionChangeTimer[], float ghostDirectionChangeInter[],
    int ghostCount, int ghostHeight[], int ghostWidth[],

    int cell_size)
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            lvl[i][j] = ' ';

    for (int j = 0; j < width; j++) lvl[0][j] = '#';
    for (int j = 0; j < width; j++) lvl[height - 1][j] = '#';
    for (int i = 0; i < height; i++) lvl[i][0] = '#';
    for (int i = 0; i < height; i++) lvl[i][width - 1] = '#';

    platformLevel2(lvl, height, width);

    for (int i = 0; i < skeletonCount; i++) {
        skeleton_x[i] = 150 + i * 250;
        skeleton_y[i] = 100;
        skeleton_speed[i] = 3;
        skeleton_movingRight[i] = true;
        skeleton_velY[i] = 0;
        skeletonOnGround[i] = false;
        skeletonJumpTimer[i] = 0;
        skeletonJumpInter[i] = 2 + (rand() % 5);
    }

    for (int g = 0; g < ghostCount; g++) {
        ghost_x[g] = 200 + g * 300;
        ghost_y[g] = 100;
        ghost_speed[g] = 3;
        ghost_movingRight[g] = true;
        ghost_velY[g] = 0;
        ghostOnGround[g] = false;
        ghostDirectionChangeTimer[g] = 0;
        ghostDirectionChangeInter[g] = (rand() % 10 + 5);
    }

    for (int i = 0; i < skeletonCount; i++) {
        int col = skeleton_x[i] / cell_size;
        for (int r = 0; r < height - 1; r++) {
            if (lvl[r + 1][col] == '#') {
                skeleton_y[i] = r * cell_size - skeletonHeight[i];
                skeletonOnGround[i] = true;
                break;
            }
        }
    }

    for (int g = 0; g < ghostCount; g++) {
        int col = ghost_x[g] / cell_size;
        for (int r = 0; r < height - 1; r++) {
            if (lvl[r + 1][col] == '#') {
                ghost_y[g] = r * cell_size - ghostHeight[g];
                ghostOnGround[g] = true;
                break;
            }
        }
    }
}


int collectPlatformTiles(char** lvl, int height, int width, int cellSize,
    int platformX[], int platformY[], int maxCount)
{
    int count = 0;
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (lvl[i][j] != '#') continue;
            if (i == height - 1) continue;
            if (lvl[i - 1][j - 1] == '#' || lvl[i + 1][j + 1] == '#' ||
                lvl[i - 1][j + 1] == '#' || lvl[i + 1][j - 1] == '#')
                continue;
            if (lvl[i - 1][j] == ' ') {
                platformX[count] = j * cellSize;
                platformY[count] = i * cellSize;
                count++;
                if (count >= maxCount) return count;
            }
        }
    }
    return count;
}

// Placing on random Platforms
void level2Enemies(
    int enemyCount,
    float ex[], float ey[],
    int eW[], int eH[],
    int platformX[], int platformY[], int platformCount,
    int cellSize)
{
    if (platformCount <= 0) return;
    for (int i = 0; i < enemyCount; i++) {
        int r = rand() % platformCount;
        ex[i] = platformX[r];
        ey[i] = platformY[r] - eH[i];
    }
}

// Enemy movement
void enemy_move(char** lvl, float& enemy_x, float enemy_y,
    const int enemyWidth, const int enemyHeight,
    float& enemy_speed, bool& movingRight,
    const int cell_size, const int height, const int width, bool isFlying)
{
    const float PADDING = 1.0f;
    int midY = (int)((enemy_y + enemyHeight * 0.5f) / cell_size);

    int footY = (int)((enemy_y + enemyHeight + 2.0f) / cell_size);

    if (midY < 0) midY = 0; if (midY >= height) midY = height - 1;
    if (footY < 0) footY = 0; if (footY >= height) footY = height - 1;

    if (movingRight)
    {
        float next_x = enemy_x + enemy_speed;
        int tileRight = (int)((next_x + enemyWidth + PADDING) / cell_size);
        int tileUnderRight = (int)((next_x + enemyWidth) / cell_size);

        if (tileRight >= width) tileRight = width - 1;
        if (tileUnderRight >= width) tileUnderRight = width - 1;

        bool hitWall = lvl[midY][tileRight] == '#';
        // Only check for gaps in the floor if NOT flying
        bool noFloor = !isFlying && (lvl[footY][tileUnderRight] != '#');

        if (hitWall || noFloor) movingRight = false;
        else enemy_x = next_x;
    }
    else
    {
        float next_x = enemy_x - enemy_speed;
        int tileLeft = (int)((next_x - PADDING) / cell_size);
        int tileUnderLeft = (int)(next_x / cell_size);

        if (tileLeft < 0) tileLeft = 0;
        if (tileLeft >= width) tileLeft = width - 1;
        if (tileUnderLeft < 0) tileUnderLeft = 0;
        if (tileUnderLeft >= width) tileUnderLeft = width - 1;

        bool hitWall = lvl[midY][tileLeft] == '#';
        // Only check for gaps in the floor if NOT flying
        bool noFloor = !isFlying && (lvl[footY][tileUnderLeft] != '#');

        if (hitWall || noFloor) movingRight = true;
        else enemy_x = next_x;
    }
}

// Initializing power ups

void init_powerups(float px[], float py[], int ptype[], bool pactive[], Sprite pshape[], Texture powerupTex[], int count, const int cell_size)
{
    int r[] = { 12, 12, 12, 7, 7, 7, 4, 4, 4, 2, 2, 2 };
    int c[] = { 3, 8, 14, 5, 9, 12, 4, 9, 13, 6, 9, 11 };
    int t[] = { 0, 2, 3, 1, 2, 0, 3, 2, 1, 0, 3, 1 };

    for (int i = 0; i < count; i++) {
        pactive[i] = true;
        ptype[i] = t[i];
        px[i] = c[i] * cell_size + 16;
        py[i] = r[i] * cell_size + 16;
        pshape[i].setTexture(powerupTex[ptype[i]]);
        pshape[i].setScale(3, 3);
        pshape[i].setPosition(px[i], py[i] - 20);
    }
}

// Skeleton gravity

void skeletonGravity(char** lvl, float& yAxis, float& velocityYAxis, bool& onGround,
    const float gravity, const float terminalVelocity,
    float xAxis, int width, int height,
    int cellSize, int lvlHeight, int lvlWidth) {

    float newYAxis = yAxis + velocityYAxis;

    int tileYAxis = (int)(newYAxis + height + 2.0f) / cellSize;
    int leftXAxis = (int)(xAxis + 5.0f) / cellSize; 
    int rightXAxis = (int)(xAxis + width - 5.0f) / cellSize; 

    if (tileYAxis >= lvlHeight) tileYAxis = lvlHeight - 1;

    if ((lvl[tileYAxis][leftXAxis] == '#' || lvl[tileYAxis][rightXAxis] == '#') && velocityYAxis >= 0) {
        onGround = true;
        velocityYAxis = 0;
        yAxis = tileYAxis * cellSize - height;
    }
    else {
        onGround = false;
        yAxis = newYAxis;
        velocityYAxis += gravity;
        if (velocityYAxis > terminalVelocity) velocityYAxis = terminalVelocity;
    }
}


//BackGround 
void applyBackgroundMusic(
    int currentLevel,
    Sprite& bgSprite,
    Texture& bgTex1,
    Texture& bgTex2,
    Music& musicLevel1,
    Music& musicLevel2)
{
    if (currentLevel == 1)
    {
        bgSprite.setTexture(bgTex1);

        musicLevel2.stop();
        musicLevel1.play();
    }
    else if (currentLevel == 2)
    {
        bgSprite.setTexture(bgTex2);

        musicLevel1.stop();
        musicLevel2.play();
    }
}
// Main Function

Clock clock1;

int main()
{
    srand(time(NULL));

    //Variables for several things
    const int CELL_SIZE = 64;
    const int LVL_HEIGHT = 14;
    const int LVL_WIDTH = 18;
    const float JUMP_STRENGTH = -20;
    const float GRAVITY = 1;
    const float TERMINAL_VELOCITY = 20;
    const int MAX_SKELETONS = 12;
    const int MAX_GHOSTS = 12;
    const int MAX_INVISIBLE = 3;
    const int MAX_CHELNOV = 4;
    int skeletonCount = 4;
    int ghostCount = 8;
    const int ENEMY_HITBOX_WIDTH = 48;
    const int ENEMY_HITBOX_HEIGHT = 64;
    const int MAX_POWERUPS = 12;
    int currentLevel = 1;
    int totalEnemies = MAX_SKELETONS + MAX_GHOSTS;
    bool levelComplete = false;
    bool dropDown = false;
    int maxEnemiesHeld = 3; 
    const float PI = 3.14159265f; 

    //Variables for Sequencial Spawning 
    float enemySpawnerTimer = 0;
    float enemySpawnDelay = 1.5f;

    int spawnGhosts = 0;
    int spawnSkeletons = 0;
    int spawnInvisible = 0;
    int spawnChelnov = 0;

    int maxGhostsLevel2 = 4;
    int maxSkeletonsLevel2 = 9;
    int maxChelnovLevel2 = 4;
    int maxInvisibleLevel2 = 3;


    cout << "GAME VERSION CHECK: Powerups 5s & Comprehensive Scoring System." << endl;
    cout << "Controls: Arrows to move, WASD to Vacuum (Hold)." << endl;
    cout << "SPACE to Shoot Projectile." << endl;
    cout << "Powerups last 5 seconds." << endl;


    //Player Direction
    int playerDir = -1;
    bool character = true;
    int characterType = 0;

    RenderWindow window(VideoMode(screen_x, screen_y), "Tumble-POP", Style::Resize);
    window.setFramerateLimit(60);

    char** lvl = new char* [LVL_HEIGHT];
    for (int i = 0; i < LVL_HEIGHT; i++)
        lvl[i] = new char[LVL_WIDTH] {' '};

    for (int j = 0; j < LVL_WIDTH; j++) lvl[LVL_HEIGHT - 1][j] = '#';
    for (int i = 0; i < LVL_WIDTH; i++) lvl[0][i] = '#';
    for (int i = 0; i < LVL_HEIGHT; i++) {
        lvl[i][0] = '#'; lvl[i][LVL_WIDTH - 1] = '#';
    }

    platform_lvl1(lvl);

    int lives = 3;
    bool gameOver = false;
    bool gameWon = false;
    bool isPaused = false;
    long long score = 0;

    // Scoring / Level tracking variables
    float levelTimer = 0.0f;
    bool levelDamageTaken = false;
    int ballHitCount = 0; // Track kills per ball for multi-kill bonus

    // Invulnerability Timer
    float damageCooldown = 0.0f;
    float ballSafeTimer = 0.0f; 

    // Powerup Timers
    float timerSpeed = 0.0f;
    float timerWidth = 0.0f;
    float timerLength = 0.0f;
    const float POWERUP_DURATION = 5.0f;

    
    float dropTimer = 0.0f;
    const float dropHoldTimer = 0.0f;

    //Fonts for lives, score etc.
    Font font;
    if (!font.loadFromFile("Assets/Font/roboto.ttf"))
        cout << "Failed to load font\n";

    Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(30);
    livesText.setFillColor(Color::Red);
    livesText.setPosition(10, 10);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::Green);
    scoreText.setPosition(10, 40);

    Text pauseText;
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(80);
    pauseText.setFillColor(Color::Yellow);
    pauseText.setPosition(400, 400);

    Texture bgTex1, bgTex2, blockTexture, PlayerTexture, SkeletonTexture, GhostTexture;
    Sprite bgSprite, blockSprite, PlayerSprite;

    // Load Logo Texture
    Texture logoTexture;
    Sprite logoSprite;
    if (!logoTexture.loadFromFile("Assets/Sprites/graphic.png")) cout << "Failed to load tumble_pop.png\n";
    logoSprite.setTexture(logoTexture);
    logoSprite.setPosition(300, 20); 
    logoSprite.setScale(3, 3);

    if (!bgTex1.loadFromFile("Assets/Sprites/background.jpg")) cout << "Failed to load bg.png\n";
    bgSprite.setTexture(bgTex1);
    if (!bgTex2.loadFromFile("Assets/Sprites/background1.jpg")) cout << "Failed to load bg.png\n";
    bgSprite.setTexture(bgTex2);

    if (!blockTexture.loadFromFile("Assets/Sprites/block1.png")) cout << "Failed to load block1.png\n";
    blockSprite.setTexture(blockTexture);

    Sprite SkeletonSprite[MAX_SKELETONS];
    SkeletonTexture.loadFromFile("Assets/Sprites/skeletons.png");

    Sprite GhostSprite[MAX_GHOSTS];
    GhostTexture.loadFromFile("Assets/Sprites/ghost.png");

    Music lvlMusic1, lvlMusic2;
    if (!lvlMusic1.openFromFile("Assets/Sound/mus.ogg"))
        cout << "Failed to load mus.ogg\n";
    lvlMusic1.setVolume(20);
    lvlMusic1.setLoop(true);

    if (!lvlMusic2.openFromFile("Assets/Sound/mus2.ogg"))
        cout << "Failed to load mus2.ogg\n";
    lvlMusic2.setVolume(20);
    lvlMusic2.setLoop(true);


    //Player's coordinations and dimensions
    float player_x = 500;
    float player_y = 100;
    float speed = 5;
    float velocityY = 0;
    bool onGround = false;
    int PlayerHeight = 102, PlayerWidth = 96;

    //Skeleton's Dimensions, animation variables
    float skeleton_x[MAX_SKELETONS], skeleton_y[MAX_SKELETONS], skeleton_speed[MAX_SKELETONS], skeleton_velY[MAX_SKELETONS];
    bool skeleton_movingRight[MAX_SKELETONS], skeletonOnGround[MAX_SKELETONS];
    int skeletonHeight[MAX_SKELETONS], skeletonWidth[MAX_SKELETONS];
    float skeleton_scale = 2.25f;
    float skeletonDirTimer[MAX_SKELETONS];
    float skeletonDirInterval[MAX_SKELETONS];
    float skeletonJumpTimer[MAX_SKELETONS];
    float skeletonJumpInterval[MAX_SKELETONS];

    for (int i = 0; i < skeletonCount; i++) {
        skeletonWidth[i] = ENEMY_HITBOX_WIDTH;
        skeletonHeight[i] = ENEMY_HITBOX_HEIGHT;
    }
    //Initializing Skeletons
    init_skeletons(skeletonCount, skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight, skeletonHeight, skeletonWidth, skeletonOnGround, skeleton_velY, skeletonDirTimer, skeletonDirInterval, skeletonJumpTimer, skeletonJumpInterval, CELL_SIZE);

    bool skeletonAlive[MAX_SKELETONS];
    for (int i = 0; i < skeletonCount; i++)
        skeletonAlive[i] = true;

    //Skeleton Animation Frames 
    int skelFrameW = 36, skelFrameH = 42, skelMaxFrames = 4;
    float skelAnimTimer[MAX_SKELETONS] = { 0 };
    int skelCurrentFrame[MAX_SKELETONS] = { 0 };


    //Skeleton sprite Initaliazation
    for (int i = 0; i < skeletonCount; i++)
    {
        SkeletonSprite[i].setTexture(SkeletonTexture);
        SkeletonSprite[i].setTextureRect(IntRect(184, 30, skelFrameW, skelFrameH));
        SkeletonSprite[i].setScale(3, 2.25);
        SkeletonSprite[i].setPosition(skeleton_x[i], skeleton_y[i]);
    }

    //Ghost's Dimensions, animation variables

    float ghost_x[MAX_GHOSTS], ghost_y[MAX_GHOSTS], ghost_speed[MAX_GHOSTS], ghost_velY[MAX_GHOSTS];
    bool ghost_movingRight[MAX_GHOSTS], ghostOnGround[MAX_GHOSTS];
    int ghostHeight[MAX_GHOSTS], ghostWidth[MAX_GHOSTS];
    float ghost_scale = 3.0f;
    float ghostDirTimer[MAX_GHOSTS];
    float ghostDirInterval[MAX_GHOSTS];

    for (int i = 0; i < ghostCount; i++)
    {
        ghostWidth[i] = ENEMY_HITBOX_WIDTH;
        ghostHeight[i] = ENEMY_HITBOX_HEIGHT;
    }
    init_ghosts(ghostCount, ghost_x, ghost_y, ghost_speed, ghost_movingRight, ghostHeight, ghostWidth, ghostOnGround, ghost_velY, ghostDirTimer, ghostDirInterval, CELL_SIZE);

    bool ghostAlive[MAX_GHOSTS];
    for (int i = 0; i < ghostCount; i++)
        ghostAlive[i] = true;

    int ghostFrameW = 48, ghostFrameH = 39, ghostMaxFrames = 4;
    float ghostAnimTimer[MAX_GHOSTS] = { 0 };
    int ghostCurrentFrame[MAX_GHOSTS] = { 0 };

    for (int i = 0; i < ghostCount; i++)
    {
        GhostSprite[i].setTexture(GhostTexture);
        GhostSprite[i].setTextureRect(IntRect(7, 7, ghostFrameW, ghostFrameH));
        GhostSprite[i].setScale(3, 3);
        GhostSprite[i].setPosition(ghost_x[i], ghost_y[i]);
    }

    //Invisible Man Dimensions, and aniamtion vraibles.
    float invisible_x[MAX_INVISIBLE], invisible_y[MAX_INVISIBLE], invisible_speed[MAX_INVISIBLE];
    bool invisibleMovingRight[MAX_INVISIBLE], invisibleAlive[MAX_INVISIBLE];

    int invisibleWidth[MAX_INVISIBLE], invisibleHeight[MAX_INVISIBLE];

    int invisibleFrameW = 38, invisibleFrameH = 40;
    float invisibleAnimTimer[MAX_INVISIBLE] = { 0 };
    int invisibleCurrentFrame[MAX_INVISIBLE] = { 0 };

    Texture invisibleTexture;
    Sprite invisibleSprite[MAX_INVISIBLE];

    invisibleTexture.loadFromFile("Assets/Sprites/invisible.png");

    for (int i = 0; i < MAX_INVISIBLE; i++) {
        invisibleSprite[i].setTexture(invisibleTexture);
        invisibleSprite[i].setTextureRect(IntRect(9, 18, 26, 38));
        invisibleSprite[i].setScale(3, 3);
        invisibleWidth[i] = ENEMY_HITBOX_WIDTH;
        invisibleHeight[i] = ENEMY_HITBOX_HEIGHT;
        invisibleAlive[i] = false;
        invisible_x[i] = 0;
        invisible_y[i] = 0;
        invisibleMovingRight[i] = true;
        invisible_speed[i] = 0;
    }


    // Chelnov Dimensions, and aniamtion vraibles.
    float chelnov_x[MAX_CHELNOV], chelnov_y[MAX_CHELNOV], chelnov_speed[MAX_CHELNOV];
    bool chelnovMovingRight[MAX_CHELNOV], chelnovAlive[MAX_CHELNOV];

    int chelnovWidth[MAX_CHELNOV], chelnovHeight[MAX_CHELNOV];

    int chelnovFrameW = 34, chelnovFrameH = 40;
    float chelnovAnimTimer[MAX_CHELNOV] = { 0 };
    int chelnovCurrentFrame[MAX_CHELNOV] = { 0 };

    Texture chelnovTexture;
    Sprite chelnovSprite[MAX_CHELNOV];

    chelnovTexture.loadFromFile("Assets/Sprites/chelnov.png");

    for (int i = 0; i < MAX_CHELNOV; i++) {
        chelnovSprite[i].setTexture(chelnovTexture);
        chelnovSprite[i].setTextureRect(IntRect(9, 18, 26, 38));
        chelnovSprite[i].setScale(3, 3);
        chelnovWidth[i] = ENEMY_HITBOX_WIDTH;
        chelnovHeight[i] = ENEMY_HITBOX_HEIGHT;
        chelnovAlive[i] = false;
        chelnov_x[i] = 0;
        chelnov_y[i] = 0;
        chelnovMovingRight[i] = true;
        chelnov_speed[i] = 0;
    }


    //Player selection area sprites.
    Sprite yellowPlayer, greenPlayer;
    Texture yellowText, greenText;
    int yAxisOfPlayer;

    greenText.loadFromFile("Assets/Sprites/green.png");
    greenPlayer.setTexture(greenText);
    greenPlayer.setScale(4, 4);
    greenPlayer.setPosition(300, 400);

    yellowText.loadFromFile("Assets/Sprites/yellow.png");
    yellowPlayer.setTexture(yellowText);
    yellowPlayer.setScale(4, 4);
    yellowPlayer.setPosition(700, 400);

    PlayerTexture.loadFromFile("Assets/Sprites/players.png");
    PlayerSprite.setTexture(PlayerTexture);
    PlayerSprite.setScale(3, 3);


    //Bag Sprite
    Texture bagText;
    bagText.loadFromFile("Assets/Sprites/players.png");
    Sprite bagSprite;
    bagSprite.setTexture(bagText);
    bagSprite.setTextureRect(IntRect(13, 160, 17, 24));
    bagSprite.setScale(3, 3);
    bagSprite.setPosition(player_x + 20, player_y + 7);


    //Player Animation Variables
    int PlayerMoveFrameHeight = 41, PlayerMoveFrameWidth = 34;
    int currentPlayer_move = 0, maxPlayers_move = 4;
    float animationTimer_move = 0, speed_move = 0.1f;
    int PlayerJumpFrameHeight = 50, PlayerJumpFrameWidth = 34;


    //Rainbow and shooting effect.
    bool shooting = false;
    float rainbowTimer = 0;
    int crrentRainbowFrame = 0, rainbowMax = 2;
    int rainbowWidth = 32, rainbowHeight = 33;
    int rainbowX = 400, rainbowY = 173;

    Sprite rainbow;
    Texture rainbowTex;
    rainbowTex.loadFromFile("Assets/Sprites/players.png");
    rainbow.setTexture(rainbowTex);
    rainbow.setTextureRect(IntRect(400, 177, 32, 24));
    rainbow.setScale(3, 3);
    rainbow.setOrigin(32, 12);

    //Shooting and suction variables
    int enemiesHeld = 0;
    bool wasShootingPressed = false;
    int lastShootDir = 0;
    int bulletCount = 0;
    int bulletType[4];
    int ballAnimIdx = 0;


    //Bullet Animations
    int currentBulletFrame = 0, maxBullets = 8;
    float bulletTimer = 0, bulletSpeed = 0.1f;


    //Power up Variables 
    float powerup_x[MAX_POWERUPS], powerup_y[MAX_POWERUPS];
    int powerup_type[MAX_POWERUPS];
    bool powerup_active[MAX_POWERUPS];
    Sprite powerup_shape[MAX_POWERUPS];
    Texture powerupTex[4];

    powerupTex[0].loadFromFile("Assets/Sprites/speed.png");
    powerupTex[2].loadFromFile("Assets/Sprites/power.png");
    powerupTex[1].loadFromFile("Assets/Sprites/shot.png");
    powerupTex[3].loadFromFile("Assets/Sprites/life.png");

    init_powerups(powerup_x, powerup_y, powerup_type, powerup_active, powerup_shape, powerupTex, MAX_POWERUPS, CELL_SIZE);

    float speedMultiplier = 1.0f;
    float vacScaleLen = 3.0f;
    float vacScaleThick = 3.0f;


    //Projectile Variables
    float ball_x, ball_y, ball_vx, ball_vy;
    bool ball_active = false;
    float ball_size = 40.f;
    int ballPrevPlatfrom = -1;


    Sprite ballSprite;
    Texture ballText;
    ballSprite.setScale(3, 3);
    int enemyHeldType[5];
    int enemyHeldIdx[5];

    // Character 2 Rotation Variables
    float yellowAngle = 180.0f; // Default Left
    float yellowTargetAngle = 180.0f;
    bool wasSpacePressed = false;

    Event ev;
    applyBackgroundMusic(currentLevel, bgSprite, bgTex1, bgTex2,
        lvlMusic1, lvlMusic2);


    //Main GameLoop
    while (window.isOpen())
    {
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed) window.close();
            if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Escape) window.close();

            //Pausing the game
            if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::P) {
                if (!character && !gameOver) {
                    isPaused = !isPaused;
                    if (isPaused) {
                        if (currentLevel == 1)
                            lvlMusic1.pause();
                        else if (currentLevel == 2)
                            lvlMusic2.pause();
                    }

                    else {
                        if (currentLevel == 1)
                            lvlMusic1.play();
                        else if (currentLevel == 2)
                            lvlMusic2.play();

                    }
                }
            }

            //Sprite Selection
            if (character && ev.type == Event::KeyPressed)
            {
                if (ev.key.code == Keyboard::Num1 || ev.key.code == Keyboard::Numpad1) {
                    characterType = 1; character = false;
                }
                if (ev.key.code == Keyboard::Num2 || ev.key.code == Keyboard::Numpad2) {
                    characterType = 2; character = false;
                }
            }
        }

        if (character)
        {
            window.clear(Color::Black);


            logoSprite.setPosition(150, 50);
            window.draw(logoSprite);

            // Main Title
            Text title; title.setFont(font);
            title.setString("CHOOSE YOUR CHARACTER");
            title.setCharacterSize(40);
            title.setFillColor(Color::White);
            title.setPosition(300, 250); // Below logo
            window.draw(title);

            //Green Sprite
            float leftX = screen_x * 0.3;
            float slotY = 400;

            greenPlayer.setPosition(leftX - 50, slotY); 
            window.draw(greenPlayer);


            Text greenName; greenName.setFont(font);
            greenName.setString("Green Tumblepopper");
            greenName.setCharacterSize(24);
            greenName.setFillColor(Color::Green);
            greenName.setPosition(leftX - 100, slotY + 170);
            window.draw(greenName);

            Text greenDesc; greenDesc.setFont(font);
            greenDesc.setString("Moves around 1.5 times faster than normal.\n\nPress [1]");
            greenDesc.setCharacterSize(18);
            greenDesc.setFillColor(Color::White);
            greenDesc.setPosition(leftX - 100, slotY + 210);
            window.draw(greenDesc);

            //Yellow Sprite
            float rightX = screen_x * 0.7;

            yellowPlayer.setPosition(rightX - 50, slotY);
            window.draw(yellowPlayer);

            Text yellowName; yellowName.setFont(font);
            yellowName.setString("Yellow Tumblepopper");
            yellowName.setCharacterSize(24);
            yellowName.setFillColor(Color::Yellow);

            yellowName.setPosition(rightX - 100, slotY + 170);
            window.draw(yellowName);

            Text yellowDesc; yellowDesc.setFont(font);
            yellowDesc.setString("Wields a 1.2 times stronger and wider vacuum.\n\nPress [2]");
            yellowDesc.setCharacterSize(18);
            yellowDesc.setFillColor(Color::White);

            yellowDesc.setPosition(rightX - 100, slotY + 210);
            window.draw(yellowDesc);

            window.display();
            continue;
        }


        //Checking if the game is not paused or over.
        if (!isPaused && !gameOver)
        {
            //Yaxis is for Animation purpose 
            if (characterType == 1) yAxisOfPlayer = 40;
            else yAxisOfPlayer = 230;

            float timeSinceLast = clock1.restart().asSeconds();
            levelTimer += timeSinceLast;

            //Sequential Spawner
            if (currentLevel == 2)
            {
                enemySpawnerTimer += timeSinceLast;

                if (enemySpawnerTimer >= enemySpawnDelay)
                {
                    enemySpawnerTimer = 0; // Reset timer

                    
                    if (spawnGhosts < maxGhostsLevel2)
                    {
                        ghostAlive[spawnGhosts] = true;
                        spawnGhosts++;
                    }
                    
                    else if (spawnSkeletons < maxSkeletonsLevel2)
                    {
                        skeletonAlive[spawnSkeletons] = true;
                        spawnSkeletons++;
                    }
                    
                    else if (spawnInvisible < maxInvisibleLevel2)
                    {
                        invisibleAlive[spawnInvisible] = true;
                        spawnInvisible++;
                    }
                    
                    else if (spawnChelnov < maxChelnovLevel2)
                    {
                        chelnovAlive[spawnChelnov] = true;
                        spawnChelnov++;
                    }
                }
            }

            // Damage Cooldown Logic
            if (damageCooldown > 0) damageCooldown -= timeSinceLast;
            if (ballSafeTimer > 0) ballSafeTimer -= timeSinceLast; 

            //Power up timer 
            if (timerSpeed > 0) {
                timerSpeed -= timeSinceLast;
                if (timerSpeed <= 0) {
                    timerSpeed = 0;
                    speedMultiplier = 1.0f; // Reset Speed
                }
            }

            if (timerWidth > 0) {
                timerWidth -= timeSinceLast;
                if (timerWidth <= 0) {
                    timerWidth = 0;
                    vacScaleThick = 3.0f; // Reset Width/Thick
                }
            }

            if (timerLength > 0) {
                timerLength -= timeSinceLast;
                if (timerLength <= 0) {
                    timerLength = 0;
                    vacScaleLen = 3.0f; // Reset Length
                }
            }

            float finalSpeed = speed * speedMultiplier;
            if (characterType == 1) {
                finalSpeed = speed * 1.5f * speedMultiplier;
            }

            //Imput Handling
            // WASD for Shooting/Vacuuming
            bool isW = Keyboard::isKeyPressed(Keyboard::W);
            bool isS = Keyboard::isKeyPressed(Keyboard::S);
            bool isA = Keyboard::isKeyPressed(Keyboard::A);
            bool isD = Keyboard::isKeyPressed(Keyboard::D);
            bool isShootingInput = isW || isS || isA || isD;

            //For dropping below the platform
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                dropTimer += timeSinceLast;
            }
            else {
                dropTimer = 0;
            }
            bool canDrop = (!((int)((player_y + PlayerHeight + 2) / CELL_SIZE) >= LVL_HEIGHT - 1) && (dropTimer > dropHoldTimer));
            int shootDir = -1;

            if (isW) shootDir = 2;
            else if (isS) shootDir = 3;
            else if (isA) shootDir = 1;
            else if (isD) shootDir = 0;


            if (shootDir != -1) lastShootDir = shootDir;

            //360 for yellow 
            if (characterType == 2) {
                float ix = 0, iy = 0;
                if (isW) iy = -1;
                if (isS) iy = 1;
                if (isA) ix = -1;
                if (isD) ix = 1;

                if (ix != 0 || iy != 0) {
                    yellowTargetAngle = atan2(iy, ix) * 180.f / PI;
                }

                // Smooth rotation of the vacuum
                float diff = yellowTargetAngle - yellowAngle;
                // Handle wrap around
                while (diff <= -180) diff += 360;
                while (diff > 180) diff -= 360;

                // Rotation speed
                if (abs(diff) < 2.0f) yellowAngle = yellowTargetAngle;
                else yellowAngle += diff * 0.15f;
            }


            if (playerDir == 1) {
                bagSprite.setScale(3, 3);
                bagSprite.setPosition(player_x - 30, player_y + 7);
            }
            else {
                bagSprite.setScale(-3, 3);
                bagSprite.setPosition(player_x + PlayerMoveFrameWidth * 3 + 30, player_y + 7);
            }

            handlePlayerMovement(lvl, player_x, player_y, PlayerWidth, PlayerHeight, finalSpeed, velocityY, onGround, JUMP_STRENGTH, CELL_SIZE, window);

            //Shooting and vaccum
            if (isShootingInput)
            {
                shooting = true;
            }
            else {
                shooting = false;
            }

            //Space bar for shooting
            bool isSpace = Keyboard::isKeyPressed(Keyboard::Space);
            if (isSpace && !wasSpacePressed && enemiesHeld > 0)
            {
                // for scoring 
                if (enemiesHeld >= 3) {
                    score += 300;
                }

                bulletCount = enemiesHeld;
                ballHitCount = 0; // Reset multi kill tracker for this throw
                for (int i = 0; i < enemiesHeld; i++)
                {
                    bulletType[i] = enemyHeldType[i];
                }
                ballAnimIdx = 0;

                ball_active = true;
                ball_x = player_x + 48;
                ball_y = player_y + 40;
                ballPrevPlatfrom = -1;
                ballSafeTimer = 0.2f; // Set grace period on launch

                //For ball animations
                if (enemyHeldType[enemiesHeld - 1] == 0) {
                    ballText.loadFromFile("Assets/Sprites/skeletons.png");
                    ballSprite.setTexture(ballText);
                    ballSprite.setTextureRect(IntRect(1040, 40, 34, 34));
                }
                else if (enemyHeldType[enemiesHeld - 1] == 1) {
                    ballText.loadFromFile("Assets/Sprites/ghost.png");
                    ballSprite.setTexture(ballText);
                    ballSprite.setTextureRect(IntRect(940, 10, 34, 34));

                }
                else if (enemyHeldType[enemiesHeld - 1] == 2) {
                    ballText.loadFromFile("Assets/Sprites/invisible.png");
                    ballSprite.setTexture(ballText);
                    ballSprite.setTextureRect(IntRect(814, 26, 30, 30));

                }
                else if (enemyHeldType[enemiesHeld - 1] == 3) {
                    ballText.loadFromFile("Assets/Sprites/chelnov.png");
                    ballSprite.setTexture(ballText);
                    ballSprite.setTextureRect(IntRect(3, 134, 38, 38));

                }

                float throwPower = 15.0f;
                ball_vx = 0; ball_vy = 0;

                //Yellow Player 360 throw
                if (characterType == 2) {
                    float rad = yellowAngle * PI / 180.0f;
                    ball_vx = cos(rad) * throwPower;
                    ball_vy = sin(rad) * throwPower;
                }
                else {
                    if (lastShootDir == 0) { // Right
                        ball_vx = throwPower;
                        ball_vy = -5.0f;
                    }
                    else if (lastShootDir == 1) { // Left
                        ball_vx = -throwPower;
                        ball_vy = -5.0f;
                    }
                    else if (lastShootDir == 2) { // Up
                        ball_vx = 0;
                        ball_vy = -throwPower;
                    }
                    else if (lastShootDir == 3) { // Down
                        ball_vx = 0;
                        ball_vy = throwPower;
                    }
                }

                enemiesHeld = 0;
                bagSprite.setTextureRect(IntRect(13, 160, 17, 24));
            }
            wasSpacePressed = isSpace;

            animationTimer_move += timeSinceLast;
            if (velocityY != 0)
            {
                PlayerSprite.setTextureRect(IntRect(522, yAxisOfPlayer - 13, PlayerJumpFrameWidth, PlayerJumpFrameHeight));
                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    PlayerSprite.setScale(-3, 3);
                }
                else if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    PlayerSprite.setScale(3, 3);
                }
            }
            else
            {
                if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    animation(PlayerSprite, animationTimer_move, timeSinceLast, speed_move, currentPlayer_move, maxPlayers_move, PlayerMoveFrameWidth, PlayerMoveFrameHeight, 14, yAxisOfPlayer);
                    PlayerSprite.setScale(3, 3);
                }
                else if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    animation(PlayerSprite, animationTimer_move, timeSinceLast, speed_move, currentPlayer_move, maxPlayers_move, PlayerMoveFrameWidth, PlayerMoveFrameHeight, 14, yAxisOfPlayer);
                    PlayerSprite.setScale(-3, 3);
                }
                else {
                    currentPlayer_move = 0;
                    PlayerSprite.setTextureRect(IntRect(14, yAxisOfPlayer, PlayerMoveFrameWidth, PlayerMoveFrameHeight));
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Left)) playerDir = -1;
            if (Keyboard::isKeyPressed(Keyboard::Right)) playerDir = 1;

            if (ball_active)
            {
                ball_vy += 0.5f;

                float next_y = ball_y + ball_vy;
                int gridX = (int)(ball_x / CELL_SIZE);
                int gridY_bottom = (int)((next_y + ball_size) / CELL_SIZE);

                bool onGround = false;

                if (gridX >= 0 && gridX < LVL_WIDTH && gridY_bottom >= 0 && gridY_bottom < LVL_HEIGHT)
                {
                    if (lvl[gridY_bottom][gridX] == '#')
                    {
                        if (ball_vy > 0)
                        {
                            ball_y = gridY_bottom * CELL_SIZE - ball_size;

                            if (ballPrevPlatfrom != 1 && gridY_bottom > ballPrevPlatfrom) {
                                float speed = (fabs(ball_vx) > 0.01f) ? ball_vx : 6.0f;
                                ball_vx = (rand() % 2 == 0) ? speed : -speed;
                            }
                            else {
                                if (ball_vy > 1.0f && fabs(ball_vx) < 0.01f) {
                                    ball_vx = (rand() % 2 == 0) ? 6.0f : -6.0f;
                                }
                            }
                            ball_vy = 0;
                            onGround = true;

                            ballPrevPlatfrom = gridY_bottom;
                        }
                        else ball_y = next_y;
                    }
                    else ball_y = next_y;
                }
                else ball_y = next_y;

                if (onGround)
                {
                    if (ball_vx == 0) ball_vx = (rand() % 2 == 0) ? 6.0f : -6.0f;
                }
                else
                {
                    if (rand() % 10 < 2) ball_vx += (rand() % 5) - 2;
                    if (ball_vx > 8.0f) ball_vx = 8.0f;
                    if (ball_vx < -8.0f) ball_vx = -8.0f;
                }

                float next_x = ball_x + ball_vx;

                int gridY_center = (int)((ball_y) / CELL_SIZE);
                int gridX_side = (int)((next_x + (ball_vx > 0 ? ball_size : 0)) / CELL_SIZE);

                if (gridX_side >= 0 && gridX_side < LVL_WIDTH && gridY_center >= 0 && gridY_center < LVL_HEIGHT)
                {
                    if (lvl[gridY_center][gridX_side] == '#')
                    {
                        ball_vx = -ball_vx;
                    }
                    else ball_x = next_x;
                }
                else
                {
                    if (next_x < 0 || next_x > screen_x) ball_vx = -ball_vx;
                    else ball_x = next_x;
                }

                ballSprite.setPosition(ball_x, ball_y - 30);

                if (bulletCount > 0)
                {

                    if (bulletType[ballAnimIdx] == 0)
                    {
                        // Skeleton animation
                        animation(ballSprite, bulletTimer, timeSinceLast, bulletSpeed, currentBulletFrame, maxBullets, 34, 34, 1036, 40);
                    }
                    else if (bulletType[ballAnimIdx] == 1)
                    {
                        // Ghost animation
                        animation(ballSprite, bulletTimer, timeSinceLast, bulletSpeed, currentBulletFrame, maxBullets, 34, 34, 936, 10);
                    }
                    else if (bulletType[ballAnimIdx] == 2)
                    {
                        // Invisible Man animation
                        animation(ballSprite, bulletTimer, timeSinceLast, bulletSpeed, currentBulletFrame, maxBullets, 36, 30, 811, 26);
                    }
                    else if (bulletType[ballAnimIdx] == 3)
                    {
                        // Chelnov animation
                        animation(ballSprite, bulletTimer, timeSinceLast, bulletSpeed, currentBulletFrame, maxBullets, 36, 34, 4, 134);
                    }

                    if (currentBulletFrame == 0)
                    {
                        ballAnimIdx++;
                        if (ballAnimIdx >= bulletCount)
                            ballAnimIdx = 0;
                    }
                }

                bool isBottom = (ball_y > screen_y - (CELL_SIZE * 2));
                bool isLeftCorner = (ball_x < (CELL_SIZE * 3));
                bool isRightCorner = (ball_x > screen_x - (CELL_SIZE * 3));

                if (isBottom && (isLeftCorner || isRightCorner))
                {
                    ball_active = false;
                }
                if (ball_y > screen_y + 50) ball_active = false;

                // Manual Collision Check for Ball vs Enemies
                for (int i = 0; i < skeletonCount; i++)
                {
                    if (skeletonAlive[i] && checkCollision(ball_x, ball_y, ball_size, ball_size, skeleton_x[i], skeleton_y[i], skeletonWidth[i], skeletonHeight[i])) {
                        skeletonAlive[i] = false;

                        //2x score
                        score += 150;

                        ballHitCount++;
                        // Scoring, Multi Kill Bonus
                        if (ballHitCount == 2) score += 200;
                        else if (ballHitCount >= 3) score += 500;

                    }
                }
                if (currentLevel == 2) {
                    for (int i = 0; i < MAX_INVISIBLE; i++)
                    {
                        if (invisibleAlive[i] && checkCollision(ball_x, ball_y, ball_size, ball_size, invisible_x[i], invisible_y[i], invisibleWidth[i], invisibleHeight[i])) {
                            invisibleAlive[i] = false;

                            //2x score
                            score += 300;

                            ballHitCount++;
                            // Scorin,: Multi-Kill Bonus
                            if (ballHitCount == 2) score += 200;
                            else if (ballHitCount >= 3) score += 500;

                             
                        }
                    }
                    for (int i = 0; i < MAX_CHELNOV; i++)
                    {
                        if (chelnovAlive[i] && checkCollision(ball_x, ball_y, ball_size, ball_size, chelnov_x[i], chelnov_y[i], chelnovWidth[i], chelnovHeight[i])) {
                            chelnovAlive[i] = false;

                            //2x scoring
                            score += 400;

                            ballHitCount++;
                            // scoring, Multi-Kill Bonus
                            if (ballHitCount == 2) score += 200;
                            else if (ballHitCount >= 3) score += 500;


                        }
                    }
                }
                for (int i = 0; i < ghostCount; i++)
                {
                    if (ghostAlive[i] && checkCollision(ball_x, ball_y, ball_size, ball_size, ghost_x[i], ghost_y[i], ghostWidth[i], ghostHeight[i])) {
                        ghostAlive[i] = false;

                        //2x score
                        score += 100;

                        ballHitCount++;
                        // scoring, Multi-Kill Bonus
                        if (ballHitCount == 2) score += 200;
                        else if (ballHitCount >= 3) score += 500;

                    }
                }

                // Manual Collision Check for Ball vs Player
                if (damageCooldown <= 0 && ballSafeTimer <= 0 && checkCollision(ball_x, ball_y, ball_size, ball_size, player_x, player_y, PlayerWidth, PlayerHeight)) {
                    // Player takes damage and projectile bounces
                    lives--;
                    score -= 50;
                    levelDamageTaken = true;
                    if (lives <= 0) gameOver = true;

                    damageCooldown = 1.0f;

                    // Respawn Player
                    if (currentLevel == 1) {
                        player_x = 500; player_y = 100;
                    }
                    else {
                        player_x = 100;
                        player_y = 13 * CELL_SIZE - PlayerHeight;
                        velocityY = 0;
                        onGround = true;
                    }
                    enemiesHeld = 0;
                }
            }

            if (shooting)
            {
                rainbow.setScale(vacScaleLen, vacScaleThick);

                float vacX, vacY, vacW, vacH;
                float playerCX = player_x + 48; 
                float playerCY = player_y + 50; 

                //Yellow Sprite 360 vacuum.
                if (characterType == 2) {
                    // sprite rotation: math angle + 180 because 0 is left for this sprite texture
                    rainbow.setRotation(yellowAngle + 180.0f);
                    rainbow.setPosition(playerCX, playerCY);

                
                    float rad = yellowAngle * PI / 180.0f;
                    float dist = (rainbowWidth * vacScaleLen);

                    // Offset hitbox to be centered at tip
                    vacW = 48; // Approx tip size
                    vacH = 48;
                    vacX = playerCX + cos(rad) * dist - vacW / 2;
                    vacY = playerCY + sin(rad) * dist - vacH / 2;
                }
                // Green sprite classic 4 direction vacuum.
                else {

                    if (lastShootDir == 0) { // Right
                        rainbow.setRotation(180);
                        rainbow.setPosition(playerCX + 35, playerCY - 5);

                        vacX = (playerCX + 35);
                        vacY = (playerCY - 5) - (rainbowHeight * vacScaleThick / 2);
                        vacW = rainbowWidth * vacScaleLen;
                        vacH = rainbowHeight * vacScaleThick;
                    }
                    else if (lastShootDir == 1) { // Left
                        rainbow.setRotation(0);
                        rainbow.setPosition(playerCX - 35, playerCY - 5);

                        vacX = (playerCX - 35) - (rainbowWidth * vacScaleLen);
                        vacY = (playerCY - 5) - (rainbowHeight * vacScaleThick / 2);
                        vacW = rainbowWidth * vacScaleLen;
                        vacH = rainbowHeight * vacScaleThick;
                    }
                    else if (lastShootDir == 2) { // Up
                        rainbow.setRotation(90);
                        rainbow.setPosition(playerCX + 4, playerCY - 30); // Higher anchor for Up

                        vacX = (playerCX + 4) - (rainbowHeight * vacScaleThick / 2);
                        vacY = (playerCY - 30) - (rainbowWidth * vacScaleLen);
                        vacW = rainbowHeight * vacScaleThick;
                        vacH = rainbowWidth * vacScaleLen;
                    }
                    else if (lastShootDir == 3) { // Down
                        rainbow.setRotation(270);
                        rainbow.setPosition(playerCX - 4, playerCY + 10); // Lower anchor for Down

                       
                        vacX = (playerCX - 4) - (rainbowHeight * vacScaleThick / 2);
                        vacY = (playerCY + 10);
                        vacW = rainbowHeight * vacScaleThick;
                        vacH = rainbowWidth * vacScaleLen;
                    }
                }

                animation(rainbow, rainbowTimer, timeSinceLast, 0.08f, crrentRainbowFrame, rainbowMax, rainbowWidth, rainbowHeight, rainbowX, rainbowY);

                for (int i = 0; i < skeletonCount; i++)

                {
                    if (skeletonAlive[i])
                    {
                        if (checkCollision(vacX, vacY, vacW, vacH, skeleton_x[i], skeleton_y[i], skeletonWidth[i], skeletonHeight[i]))
                        {
                            if (enemiesHeld < maxEnemiesHeld) {
                                skeletonAlive[i] = false;

                                // Scoring Capture Skeleton
                                score += 75;

                                enemyHeldType[enemiesHeld] = 0;
                                enemyHeldIdx[enemiesHeld] = i;
                                enemiesHeld++;
                            }
                            if (enemiesHeld > 0) bagSprite.setTextureRect(IntRect(30, 160, 17, 24));
                        }
                    }
                }
                for (int i = 0; i < ghostCount; i++)

                {
                    if (ghostAlive[i])
                    {
                        if (checkCollision(vacX, vacY, vacW, vacH, ghost_x[i], ghost_y[i], ghostWidth[i], ghostHeight[i]))
                        {
                            if (enemiesHeld < maxEnemiesHeld) {
                                ghostAlive[i] = false;

                                // scoring: Capture Ghost
                                score += 50;

                                enemyHeldType[enemiesHeld] = 1;
                                enemyHeldIdx[enemiesHeld] = i;

                                enemiesHeld++;
                            }
                        }
                    }
                }

                for (int i = 0; i < MAX_INVISIBLE; i++)

                {
                    if (invisibleAlive[i])
                    {
                        if (checkCollision(vacX, vacY, vacW, vacH, invisible_x[i], invisible_y[i], invisibleWidth[i], invisibleHeight[i]))
                        {
                            if (enemiesHeld < maxEnemiesHeld) {
                                invisibleAlive[i] = false;

                                // scoring: Capture Invisible Man
                                score += 150;

                                enemyHeldType[enemiesHeld] = 2;
                                enemyHeldIdx[enemiesHeld] = i;
                                enemiesHeld++;
                            }
                            if (enemiesHeld > 0) bagSprite.setTextureRect(IntRect(30, 160, 17, 24));
                        }
                    }
                }

                for (int i = 0; i < MAX_CHELNOV; i++)

                {
                    if (chelnovAlive[i])
                    {
                        if (checkCollision(vacX, vacY, vacW, vacH, chelnov_x[i], chelnov_y[i], chelnovWidth[i], chelnovHeight[i]))
                        {
                            if (enemiesHeld < maxEnemiesHeld) {
                                chelnovAlive[i] = false;

                                // scoring: Capture Chelnox
                                score += 200;

                                enemyHeldType[enemiesHeld] = 3;
                                enemyHeldIdx[enemiesHeld] = i;
                                enemiesHeld++;
                            }
                            if (enemiesHeld > 0) bagSprite.setTextureRect(IntRect(30, 160, 17, 24));
                        }
                    }
                }
            }
            levelComplete = true;
            for (int i = 0; i < skeletonCount; i++)

                if (skeletonAlive[i]) levelComplete = false;
            for (int i = 0; i < ghostCount; i++)
                if (ghostAlive[i]) levelComplete = false;

            if (currentLevel == 2) {
                for (int i = 0; i < MAX_INVISIBLE; i++)

                    if (invisibleAlive[i]) levelComplete = false;
                for (int i = 0; i < MAX_CHELNOV; i++)
                    if (chelnovAlive[i]) levelComplete = false;
                if (spawnGhosts < 4 || spawnSkeletons < 9 || spawnInvisible < 3 || spawnChelnov < 4) {
                    levelComplete = false;
                }
            }

            if (levelComplete)
            {
                // scoring: Level Clear Bonuses
                if (currentLevel == 1) {
                    score += 1000;
                    if (!levelDamageTaken) score += 1500;
                    if (levelTimer < 30.0f) score += 2000;
                    else if (levelTimer < 45.0f) score += 1000;
                    else if (levelTimer < 60.0f) score += 500;
                }
                else {
                    score += 2000;
                    if (!levelDamageTaken) score += 2500;
                    if (levelTimer < 60.0f) score += 3000;
                    else if (levelTimer < 90.0f) score += 1500;
                    else if (levelTimer < 120.0f) score += 750;
                }
                if (currentLevel == 2) {
                    player_x = 100;
                    player_y = 13 * CELL_SIZE - PlayerHeight;
                    onGround = true;
                    velocityY = 0;
                }

                currentLevel++;
                if (currentLevel == 2)
                {
                    maxEnemiesHeld = 5; // Capacity level 2
                    skeletonCount = 9;
                    ghostCount = 4;
                }

                //Sequential spawner.
                enemySpawnerTimer = 0;

                for (int i = 0; i < maxGhostsLevel2; i++) ghostAlive[i] = false;
                for (int i = 0; i < maxSkeletonsLevel2; i++) skeletonAlive[i] = false;
                for (int i = 0; i < maxInvisibleLevel2; i++) invisibleAlive[i] = false;
                for (int i = 0; i < maxChelnovLevel2; i++) chelnovAlive[i] = false;


                applyBackgroundMusic(currentLevel, bgSprite, bgTex1, bgTex2,
                    lvlMusic1, lvlMusic2);
                if (currentLevel > 2) {
                    currentLevel = 2;
                    gameWon = true;
                }
                else {
                    player_x = 100;
                    player_y = 13 * CELL_SIZE - PlayerHeight;
                    velocityY = 0;
                    onGround = true;
                }
                if (gameWon)
                {
                    logoSprite.setTexture(logoTexture);
                    logoSprite.setPosition(150, 50);
                    window.draw(logoSprite);

                    window.clear(Color::Black);
                    Text winText;
                    winText.setFont(font);
                    winText.setString(" YOU WIN!\nPress ENTER to Exit");
                    winText.setCharacterSize(60);
                    winText.setFillColor(Color::White);
                    winText.setPosition(250, 350);

                    window.draw(winText);
                    window.display();

                    if (Keyboard::isKeyPressed(Keyboard::Enter))
                        window.close();

                    continue; // Skip rest of the game loop
                }

                for (int i = 0; i < skeletonCount; ++i) {
                    skeletonWidth[i] = ENEMY_HITBOX_WIDTH;
                    skeletonHeight[i] = ENEMY_HITBOX_HEIGHT;
                }

                init_skeletons(skeletonCount, skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight,
                    skeletonHeight, skeletonWidth, skeletonOnGround, skeleton_velY,
                    skeletonDirTimer, skeletonDirInterval, skeletonJumpTimer, skeletonJumpInterval,
                    CELL_SIZE);


                for (int i = 0; i < ghostCount; ++i) {
                    ghostWidth[i] = ENEMY_HITBOX_WIDTH;
                    ghostHeight[i] = ENEMY_HITBOX_HEIGHT;
                }
                init_ghosts(ghostCount, ghost_x, ghost_y, ghost_speed, ghost_movingRight,
                    ghostHeight, ghostWidth, ghostOnGround, ghost_velY,
                    ghostDirTimer, ghostDirInterval, CELL_SIZE);


                for (int i = 0; i < skeletonCount; ++i) {
                    SkeletonSprite[i].setTexture(SkeletonTexture);
                    SkeletonSprite[i].setTextureRect(IntRect(184, 30, skelFrameW, skelFrameH));
                    SkeletonSprite[i].setScale(3, 2.25);
                    SkeletonSprite[i].setPosition(skeleton_x[i], skeleton_y[i]);
                    skelAnimTimer[i] = 0.0f; skelCurrentFrame[i] = 0;
                }

                for (int i = 0; i < ghostCount; ++i) {
                    GhostSprite[i].setTexture(GhostTexture);
                    GhostSprite[i].setTextureRect(IntRect(7, 7, ghostFrameW, ghostFrameH));
                    GhostSprite[i].setScale(3, 3);
                    GhostSprite[i].setPosition(ghost_x[i], ghost_y[i]);
                    ghostAnimTimer[i] = 0.0f; ghostCurrentFrame[i] = 0;
                }

                // RESET Level State
                levelTimer = 0.0f;
                levelDamageTaken = false;
                lives = 3; // Health reset per level
                enemiesHeld = 0;
                ball_active = false;


                newLevel(lvl, LVL_HEIGHT, LVL_WIDTH,
                    skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight,
                    skeletonOnGround, skeleton_velY, skeletonJumpTimer, skeletonJumpInterval,
                    skeletonCount, skeletonHeight, skeletonWidth,
                    ghost_x, ghost_y, ghost_speed, ghost_movingRight,
                    ghostOnGround, ghost_velY,
                    ghostDirTimer, ghostDirInterval,
                    ghostCount, ghostHeight, ghostWidth,
                    CELL_SIZE);

                int platformX[300], platformY[300];
                int platformCount = collectPlatformTiles(lvl, LVL_HEIGHT, LVL_WIDTH, CELL_SIZE, platformX, platformY, 300);

                if (currentLevel == 2) {
                    for (int i = 0; i < MAX_INVISIBLE; i++) {
                        if (platformCount <= 0) {
                            invisibleAlive[i] = false;
                            continue;
                        }
                        int r = rand() % platformCount;
                        invisible_x[i] = platformX[r];
                        invisible_y[i] = platformY[r] - invisibleHeight[i];
                        invisibleAlive[i] = true;

                        // start moving left or right randomly
                        invisible_speed[i] = (rand() % 2 == 0) ? 2.0f : 2.5f;
                        invisibleMovingRight[i] = (rand() % 2 == 0);


                        platformX[r] = platformX[platformCount - 1];
                        platformY[r] = platformY[platformCount - 1];
                        platformCount--;
                        if (platformCount <= 0) break;
                    }

                    for (int i = 0; i < MAX_CHELNOV; i++) {
                        if (platformCount <= 0) {
                            chelnovAlive[i] = false;
                            continue;
                        }
                        int r = rand() % platformCount;
                        chelnov_x[i] = platformX[r];
                        chelnov_y[i] = platformY[r] - chelnovHeight[i];
                        chelnovAlive[i] = true;

                        // start moving left or right randomly
                        chelnov_speed[i] = (rand() % 2 == 0) ? 2.0f : 2.5f;
                        chelnovMovingRight[i] = (rand() % 2 == 0);

                        platformX[r] = platformX[platformCount - 1];
                        platformY[r] = platformY[platformCount - 1];
                        platformCount--;
                        if (platformCount <= 0) break;
                    }
                }

                level2Enemies(skeletonCount, skeleton_x, skeleton_y, skeletonWidth, skeletonHeight, platformX, platformY, platformCount, CELL_SIZE);
                level2Enemies(ghostCount, ghost_x, ghost_y, ghostWidth, ghostHeight, platformX, platformY, platformCount, CELL_SIZE);

                for (int i = 0; i < MAX_SKELETONS; ++i) skeletonAlive[i] = false;
                for (int i = 0; i < MAX_GHOSTS; ++i)    ghostAlive[i] = false;
                for (int i = 0; i < MAX_INVISIBLE; ++i) invisibleAlive[i] = false;
                for (int i = 0; i < MAX_CHELNOV; ++i)   chelnovAlive[i] = false;

                levelComplete = false;
            }

            float offset_y = 0;
            player_gravity(lvl, offset_y, velocityY, onGround, GRAVITY, TERMINAL_VELOCITY,
                player_x, player_y, CELL_SIZE, LVL_HEIGHT, LVL_WIDTH, PlayerHeight, PlayerWidth, canDrop);

            for (int i = 0; i < skeletonCount; i++)
            {
                if (!skeletonAlive[i]) continue;

                skeletonDirTimer[i] += timeSinceLast;
                if (skeletonDirTimer[i] >= skeletonDirInterval[i] && skeletonOnGround[i])
                {
                    skeleton_movingRight[i] = !skeleton_movingRight[i];
                    skeletonDirTimer[i] = 0;
                    skeletonDirInterval[i] = (rand() % 5 + 3);
                }

                skeletonJumpTimer[i] += timeSinceLast;
                if (skeletonJumpTimer[i] >= skeletonJumpInterval[i] && skeletonOnGround[i]) {
                    skeletonJumpTimer[i] = 0;
                    skeletonJumpInterval[i] = rand() % 5 + 3;
                    int headTile = (int)(skeleton_y[i] - 10) / CELL_SIZE;
                    int feetTile = (int)(skeleton_y[i] + skeletonHeight[i] + 1) / CELL_SIZE;
                    int belowTile = feetTile + 1;

                    int leftTile = (int)(skeleton_x[i]) / CELL_SIZE;
                    int midTile = (int)(skeleton_y[i] + skeletonHeight[i] + 1) / CELL_SIZE;
                    int rightTile = (int)(skeleton_x[i] + skeletonWidth[i] - 1) / CELL_SIZE;




                    bool jump = rand() % 2;
                    if (jump) {
                        if (lvl[headTile][leftTile] == '#' || lvl[headTile][rightTile] == '#') {
                            skeleton_velY[i] = -22;
                            skeletonOnGround[i] = false;
                        }
                    }
                    else {
                        if (belowTile < LVL_HEIGHT) {
                            if (lvl[feetTile][leftTile] == '#' && lvl[feetTile][rightTile] == '#') {
                                skeleton_velY[i] = 22;
                                skeleton_y[i] += 64;
                                skeletonOnGround[i] = false;
                            }
                        }
                    }
                }

                if (skeletonOnGround[i]) {
                    enemy_move(lvl, skeleton_x[i], skeleton_y[i],
                        skeletonWidth[i], skeletonHeight[i],
                        skeleton_speed[i], skeleton_movingRight[i],
                        CELL_SIZE, LVL_HEIGHT, LVL_WIDTH, false);
                }
                else {
                    skeletonGravity(lvl, skeleton_y[i], skeleton_velY[i], skeletonOnGround[i], GRAVITY, TERMINAL_VELOCITY, skeleton_x[i], skeletonWidth[i], skeletonHeight[i], CELL_SIZE, LVL_HEIGHT, LVL_WIDTH);
                }

                if (skeleton_movingRight[i]) {
                    SkeletonSprite[i].setScale(-3.0f, skeleton_scale);
                    SkeletonSprite[i].setPosition(skeleton_x[i] + 108 - 30, skeleton_y[i] - 20);
                }
                else {
                    SkeletonSprite[i].setScale(3.0f, skeleton_scale);
                    SkeletonSprite[i].setPosition(skeleton_x[i] - 30, skeleton_y[i] - 20);
                }

                if (!isPaused && !gameOver) animation(SkeletonSprite[i], skelAnimTimer[i], timeSinceLast, 0.1f, skelCurrentFrame[i], skelMaxFrames, skelFrameW, skelFrameH, 184, 30);
            }

            for (int i = 0; i < ghostCount; i++)

            {
                if (!ghostAlive[i]) continue;

                ghostDirTimer[i] += timeSinceLast;
                if (ghostDirTimer[i] >= ghostDirInterval[i]) {
                    ghost_movingRight[i] = !ghost_movingRight[i];
                    ghostDirTimer[i] = 0;
                    ghostDirInterval[i] = (rand() % 4) + 5;
                }

                enemy_move(lvl, ghost_x[i], ghost_y[i],
                    ghostWidth[i], ghostHeight[i],
                    ghost_speed[i], ghost_movingRight[i],
                    CELL_SIZE, LVL_HEIGHT, LVL_WIDTH, false); 

                if (ghost_movingRight[i]) {
                    GhostSprite[i].setScale(-3.0f, ghost_scale);
                    GhostSprite[i].setPosition(ghost_x[i] + 144 - 48, ghost_y[i] - 50);
                }
                else {
                    GhostSprite[i].setScale(3.0f, ghost_scale);
                    GhostSprite[i].setPosition(ghost_x[i] - 48, ghost_y[i] - 50);
                }

                if (!isPaused && !gameOver) animation(GhostSprite[i], ghostAnimTimer[i], timeSinceLast, 0.12f, ghostCurrentFrame[i], ghostMaxFrames, ghostFrameW, ghostFrameH, 0, 0);
            }
            if (currentLevel == 2) {
                for (int i = 0; i < MAX_INVISIBLE; i++) {
                    if (!invisibleAlive[i]) {
                        continue;
                    }

                    enemy_move(lvl, invisible_x[i], invisible_y[i], invisibleWidth[i], invisibleHeight[i], invisible_speed[i], invisibleMovingRight[i], CELL_SIZE, LVL_HEIGHT, LVL_WIDTH, false);

                    if (invisibleMovingRight[i]) {
                        invisibleSprite[i].setScale(-3, 3);
                        invisibleSprite[i].setPosition(invisible_x[i] + 144 - 48, invisible_y[i] - 55);
                    }
                    else {
                        invisibleSprite[i].setScale(3, 3);
                        invisibleSprite[i].setPosition(invisible_x[i], invisible_y[i] - 50);
                    }
                    if (!isPaused && !gameOver) animation(invisibleSprite[i], invisibleAnimTimer[i], timeSinceLast, 0.12f, invisibleCurrentFrame[i], 4, invisibleFrameW, invisibleFrameH, 152, 17);
                }

                for (int i = 0; i < MAX_CHELNOV; i++) {
                    if (!chelnovAlive[i]) {
                        continue;
                    }

                    enemy_move(lvl, chelnov_x[i], chelnov_y[i], chelnovWidth[i], chelnovHeight[i], chelnov_speed[i], chelnovMovingRight[i], CELL_SIZE, LVL_HEIGHT, LVL_WIDTH, false);

                    if (chelnovMovingRight[i]) {
                        chelnovSprite[i].setScale(-3, 3);
                        chelnovSprite[i].setPosition(chelnov_x[i] + 144 - 48, chelnov_y[i] - 55);
                    }
                    else {
                        chelnovSprite[i].setScale(3, 3);
                        chelnovSprite[i].setPosition(chelnov_x[i], chelnov_y[i] - 50);
                    }
                    if (!isPaused && !gameOver) animation(chelnovSprite[i], chelnovAnimTimer[i], timeSinceLast, 0.12f, chelnovCurrentFrame[i], 4, chelnovFrameW, chelnovFrameH, 38, 4);
                }
            }
            for (int i = 0; i < skeletonCount; i++)

            {
                if (!skeletonAlive[i]) continue;
                if (damageCooldown <= 0 && checkCollision(player_x, player_y, PlayerWidth, PlayerHeight, skeleton_x[i], skeleton_y[i], skeletonWidth[i], skeletonHeight[i]))
                {
                    lives--;
                    if (currentLevel == 1) {
                        player_x = 500; player_y = 100;
                    }
                    else {
                        player_x = 100;
                        player_y = 13 * CELL_SIZE - PlayerHeight;
                        velocityY = 0;
                        onGround = true;
                    }

                    enemiesHeld = 0;

                    // Scoring: Penalty for Taking Damage
                    score -= 50;
                    levelDamageTaken = true;
                    damageCooldown = 1.0f; // Added safety cooldown after respawn

                    if (lives <= 0) {
                        gameOver = true;
                        // Scoring: Penalty for Death
                        score -= 200;
                    }
                }
            }
            if (currentLevel == 2) {
                for (int i = 0; i < MAX_INVISIBLE; i++) {
                    if (!invisibleAlive[i]) {
                        continue;
                    }

                    if (damageCooldown <= 0 && checkCollision(player_x, player_y, PlayerWidth, PlayerHeight, invisible_x[i], invisible_y[i], invisibleWidth[i], invisibleHeight[i])) {
                        lives--;
                        score -= 50;
                        levelDamageTaken = true;
                        damageCooldown = 1.0f;

                        player_x = 100;
                        player_y = 13 * CELL_SIZE - PlayerHeight;
                        if (lives <= 0) {
                            gameOver = true;
                        }
                    }
                }

                for (int i = 0; i < MAX_CHELNOV; i++) {
                    if (!chelnovAlive[i]) {
                        continue;
                    }

                    if (damageCooldown <= 0 && checkCollision(player_x, player_y, PlayerWidth, PlayerHeight, chelnov_x[i], chelnov_y[i], chelnovWidth[i], chelnovHeight[i])) {
                        lives--;
                        score -= 50;
                        levelDamageTaken = true;
                        damageCooldown = 1.0f;

                        player_x = 100;
                        player_y = 13 * CELL_SIZE - PlayerHeight;
                        if (lives <= 0) {
                            gameOver = true;
                        }
                    }
                }
            }
            for (int i = 0; i < ghostCount; i++)

            {
                if (!ghostAlive[i]) continue;
                // Added damageCooldown check
                if (damageCooldown <= 0 && checkCollision(player_x, player_y, PlayerWidth, PlayerHeight, ghost_x[i], ghost_y[i], ghostWidth[i], ghostHeight[i]))
                {
                    lives--;
                    if (currentLevel == 1) {
                        player_x = 500; player_y = 100;
                    }
                    else {
                        player_x = 100;
                        player_y = 13 * CELL_SIZE - PlayerHeight;
                        velocityY = 0;
                        onGround = true;
                    }
                    enemiesHeld = 0;

                    // Scoring: Penalty for Taking Damage
                    score -= 50;
                    levelDamageTaken = true;
                    damageCooldown = 1.0f; // Added safety cooldown after respawn

                    if (lives <= 0) {
                        gameOver = true;
                        // Scoring: Penalty for Death
                        score -= 200;
                    }
                }
            }
        }

        window.clear();
        display_level(window, lvl, bgTex1, bgSprite, blockTexture, blockSprite, LVL_HEIGHT, LVL_WIDTH, CELL_SIZE);

        for (int i = 0; i < skeletonCount; i++)
        {
            if (skeletonAlive[i]) window.draw(SkeletonSprite[i]);
        }
        for (int i = 0; i < ghostCount; i++)
        {
            if (ghostAlive[i]) window.draw(GhostSprite[i]);
        }
        for (int i = 0; i < MAX_INVISIBLE; i++) {
            if (invisibleAlive[i] && currentLevel == 2) {
                window.draw(invisibleSprite[i]);
            }
        }
        for (int i = 0; i < MAX_CHELNOV; i++) {
            if (chelnovAlive[i] && currentLevel == 2) {
                window.draw(chelnovSprite[i]);
            }
        }
        for (int i = 0; i < MAX_POWERUPS; i++)
        {
            if (powerup_active[i])
            {
                window.draw(powerup_shape[i]);
                if (!isPaused && !gameOver && checkCollision(powerup_x[i], powerup_y[i], 48, 48, player_x, player_y, PlayerWidth, PlayerHeight))
                {
                    powerup_active[i] = false;
                    if (powerup_type[i] == 0) {
                        speedMultiplier = 2.0f;
                        timerSpeed = POWERUP_DURATION; // 5s Speed
                    }
                    else if (powerup_type[i] == 1) {
                        vacScaleThick = 4.5f; // Increased thick
                        timerWidth = POWERUP_DURATION; // 5s Width
                    }
                    else if (powerup_type[i] == 2) {
                        vacScaleLen = 4.5f; // Increased Len
                        timerLength = POWERUP_DURATION; // 5s Len
                    }
                    else if (powerup_type[i] == 3) lives++; // Instant Life
                }
            }
        }

        livesText.setString("Lives: " + to_string(lives) + " | Held: " + to_string(enemiesHeld));
        window.draw(livesText);

        // Update Score Text with Time
        scoreText.setString("Score: " + to_string(score) + " | Time: " + to_string((int)levelTimer) + "s");
        window.draw(scoreText);

        if (ball_active) {

            window.draw(ballSprite);
        }

        if (shooting && !isPaused && !gameOver) window.draw(rainbow);

        // Correct player draw based on direction
        if (playerDir == 1) {
            PlayerSprite.setScale(-3, 3);
            PlayerSprite.setPosition(player_x + PlayerMoveFrameWidth * 3, player_y - 20);
        }
        else {
            PlayerSprite.setScale(3, 3);
            PlayerSprite.setPosition(player_x, player_y - 20);
        }

        window.draw(bagSprite);
        window.draw(PlayerSprite);

        if (isPaused) {
            window.draw(pauseText);
        }

        if (gameOver) {
            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER");
            gameOverText.setCharacterSize(80);
            gameOverText.setFillColor(Color::White);
            gameOverText.setPosition(400, 400);
            window.draw(gameOverText);
        }

        window.display();
    }
    if (currentLevel == 1)
        lvlMusic1.stop();
    else
        lvlMusic2.stop();
    for (int i = 0; i < LVL_HEIGHT; i++)
        delete[] lvl[i];
    delete[] lvl;

    return 0;
}