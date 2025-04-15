#include <iostream>
#include<fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.

const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows 
const int gameColumns = resolutionY / boxPixelsY; // Total columns 
int gameGrid[gameRows][gameColumns] = {};
const int x = 0;
const int y = 1;
const int exists = 2;



void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void movePlayer(float player[], float speed);

void rotationofcenti(float centi[][4], float& speed, float locationOfmushrm[][2], int noOfmushroms, int Lengthofcenti);
void imageofcenti(sf::RenderWindow& window, float centi[][4], sf::Sprite SpofCenti[], int Lengthofcenti);
void strikingofCenti(int& nextmush, float centi[][4], float bullet[], float& speed, int& score, sf::Sprite SpofCenti[], sf::Texture& centihead, int Lengthofcenti, float locationOfmushrm[][2], int noOfmushroms, int HeathOfmushrm[]);
bool playerandCentiCollide(float player[], float centi[][4], int Lengthofcenti);
void Randmushroom(float locationOfmushrm[][2], int HeathOfmushrm[], int noOfmushroms);
void spritemushroom(sf::RenderWindow& window, sf::Sprite& Spofmushrom, float locationOfmushrm[][2], int noOfmushroms);
void bulletandmushroom(float XcoorditeOfbullet, float YcoorditeOfbullet, float bullet[], int& score, float locationOfmushrm[][2], int HeathOfmushrm[], int noOfmushroms);
void scorpian(float scorp[], float& speed);
void scorptext(sf::RenderWindow& window, float scorp[], sf::Sprite& scorpSprite);
bool playerScorpCollide(const float obj1[], const float obj2[]);
void checkhighest(int sc);
void writeScore(int sc);

int main()
{
    srand(time(0));

    // Declaring RenderWindow.
    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);

    // Used to resize your window if it's too big or too small. Use according to your needs.
    window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
    // window.setSize(sf::Vector2u(980, 980)); // Recommended for 2560x1440 (1440p) displays.
    // window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.

    // Used to position your window on every launch. Use according to your needs.
    window.setPosition(sf::Vector2i(100, 0));

    // Initializing Background Music.
    sf::Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();
    bgMusic.setVolume(50);

    // Initializing Background.
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/background1.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

    // Initializing Player and Player Sprites.
    float player[2] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (gameColumns * 3 / 4) * boxPixelsY;
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing Bullet and Bullet Sprites.
    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = true;
    sf::Clock bulletClock;
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));


    // Here i initiall the centipede texture  and length 
    const int Lengthofcenti = 12;
    float centi[Lengthofcenti][4] = {};
    float centispeed = 4;
    sf::Texture textOfCenti;
    sf::Texture centihead;
    sf::Sprite SpofCenti[Lengthofcenti];
    textOfCenti.loadFromFile("Textures/c_body_left_walk.png");
    centihead.loadFromFile("Textures/c_head_left_walk.png");
    // THis loop is for generating the centipede hread infront of centipede 
    for (int i = 0; i < Lengthofcenti; i++)
    {
        if (i == Lengthofcenti - 1)
        {
            SpofCenti[Lengthofcenti - 1].setTexture(centihead);
            SpofCenti[Lengthofcenti - 1].setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
        }
        else
        {
            SpofCenti[i].setTexture(textOfCenti);
            SpofCenti[i].setTextureRect(sf::IntRect(0, 0, boxPixelsY, boxPixelsY));
        }

    }
    // Initialize centi at random position
    float XCordinate = static_cast<float>(rand() % (resolutionX - (Lengthofcenti * boxPixelsX)));
    float YCordinate = static_cast<float>(rand() % (resolutionY / 2));

    const int startingLength = Lengthofcenti;
    for (int i = 0; i < startingLength; ++i)
    {
        centi[i][x] = XCordinate + i * boxPixelsX; ;
        centi[i][y] = YCordinate;
        centi[i][2] = 2;
        centi[i][3] = 0;
    }


    const int size = 45;
    int  totalmush = rand() % 10 + 20;
    float locationOfmushrm[size][2]; // Each row: {x, y}
    int HeathOfmushrm[size]; // Life of each mushroom




    sf::Texture TextOfMushrom;
    sf::Sprite Spofmushrom;
    TextOfMushrom.loadFromFile("Textures/mushroom.png");
    Spofmushrom.setTexture(TextOfMushrom);
    Spofmushrom.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
    bool bulletExists = false;
    Randmushroom(locationOfmushrm, HeathOfmushrm, totalmush);



    float scorp[2] = { static_cast<float>(rand() % (resolutionX - boxPixelsX)),
                  static_cast<float>(rand() % (resolutionY - boxPixelsY)) };
    float scorpspeed = 0.2f;
    sf::Clock scorpSpawnClock;
    sf::Texture scorpTexture;
    sf::Sprite scorpSprite;
    scorpTexture.loadFromFile("Textures/scorp.png");
    scorpSprite.setTexture(scorpTexture);
    scorpSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    sf::Font font1;
    font1.loadFromFile("font/drag.otf");
    int sc = 0;
    sf::Text score;
    score.setFont(font1);
    score.setCharacterSize(40);
    score.setString("Score: " + to_string(sc));
    score.setFillColor(sf::Color::Magenta);
    sf::Text highScore;
    highScore.setFont(font1);
    highScore.setCharacterSize(40);
    ifstream inputFile("highScore.txt");
    int i = 0;
    inputFile >> i;
    highScore.setString("highScore: " + to_string(i));
    highScore.setFillColor(sf::Color::Red);
    highScore.setPosition(600, 1);


    while (window.isOpen())
    {
        highScore.setString("highScore: " + to_string(i));



        float movespeed = 15; // Speed For Player 


        window.draw(backgroundSprite);
        drawPlayer(window, player, playerSprite);

        if (bullet[exists] == true)
        {
            moveBullet(bullet, bulletClock);
            drawBullet(window, bullet, bulletSprite);
        }
        // 	Calling the functions for drwaing and moving of centi	
        rotationofcenti(centi, centispeed, locationOfmushrm, totalmush, Lengthofcenti);
        imageofcenti(window, centi, SpofCenti, Lengthofcenti);



        // Calling the function for Drawing the mushroom  sprite
        spritemushroom(window, Spofmushrom, locationOfmushrm, totalmush);

        //     Calling THe funtion FOR DRWAING AND CONTROOLING THE SCORPIAN 
        scorpian(scorp, scorpspeed);
        scorptext(window, scorp, scorpSprite);



        bool fireBullet = false;
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)

            {
                return 0;

            }

            // Calling Function for THE MOVEMWNT OF THE PLAYER 	
            movePlayer(player, movespeed);

            // hERE WHEN WE PRESS THE SPACE BAR THE BULLET FIRES 
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space)

            {
                // Start firing bullets continuously
                fireBullet = true;
            }

            if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Space)
            {
                // Stop firing bullets
                fireBullet = false;
            }

            // Fire bullets continuously while the space key is pressed
            if (fireBullet)
            {
                bullet[exists] = true;
                bullet[x] = player[x];
                bullet[y] = player[y] - boxPixelsY;
            }
            if (bullet[exists])
            {
                moveBullet(bullet, bulletClock);
                drawBullet(window, bullet, bulletSprite);
            }

        }

        //  Calling for rhe collision od centipede 
        strikingofCenti(totalmush, centi, bullet, centispeed, sc, SpofCenti, centihead, Lengthofcenti, locationOfmushrm, totalmush, HeathOfmushrm);

        // Handle collisions with mushrooms
        bulletandmushroom(bullet[x], bullet[y], bullet, sc, locationOfmushrm, HeathOfmushrm, totalmush);
        score.setString("Score: " + to_string(sc));


        // Handle collisions with the scorp
        if (playerScorpCollide(player, scorp))
        {

            cout << "Game Over!" << endl;
            window.close();

        }

        if (playerandCentiCollide(player, centi, Lengthofcenti))
        {
            cout << "Game Over!" << endl;
            window.close();

        }


        window.draw(highScore);  // Here i am Drwaing my high score 
        window.draw(score);// here i am drwaing mt score 
        window.display();

        window.clear();
    }

    checkhighest(sc);  // Here i am callling this function to CHeck the hghest score 

}

void movePlayer(float player[], float speed) // Here i am chosing this function becuse it controls the player movement through sfml library and here the sf:: Keyboard is used
{

          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[y] > 700)
          {
                    player[y] -= speed;

          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y] < resolutionY - boxPixelsY)
          {
                 player[y] += speed;
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] > 0)
          {
                 player[x] -= speed;
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < resolutionX - boxPixelsX)
          {
                player[x] += speed;
          }
          else   if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player[y] > 700)
          {
                 player[y] -= speed;
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player[y] < resolutionY - boxPixelsY)
          {
             player[y] += speed;
          }
        
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player[x] > 0)
          {
                    player[x] -= speed;

          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player[x] < resolutionX - boxPixelsX)
          {
                   player[x] += speed;

          }
}

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite)  // this fucntion draws the player  and set its position 
{
            
                      playerSprite.setPosition(player[x], player[y]);
                     window.draw(playerSprite);



}
void moveBullet(float bullet[], sf::Clock& bulletClock) // this funtion is used to moving the bullets
{
        
             if (bulletClock.getElapsedTime().asMilliseconds() < 10)
                   return;

         bulletClock.restart();
            bullet[y] -= 10;

         if (bullet[y] < -boxPixelsY)
         {
             bullet[exists] = false;

         }
}
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) // This funtion drwas the  bullets
{
               bulletSprite.setPosition(bullet[x], bullet[y]);
                 window.draw(bulletSprite);

}
void rotationofcenti(float centi[][4], float& speed, float locationOfmushrm[][2], int noOfmushroms, int Lengthofcenti) // Here i am controlling the centipede movement stuf that it moves segment by segment and when hits with the wall o mushromm it goes to lower line and also in this funtion when the centipede  reaches at the botton then it starts moving only in the player area

{
    int i = 0;

    while (i < Lengthofcenti)
    {


        if (centi[i][x] <= 0 || centi[i][x] >= resolutionX - boxPixelsX)
        {
            centi[i][y] += boxPixelsY;

            if (centi[i][3] == 0)
            {
                centi[i][3] = 1;
            }
            else if (centi[i][3] == 1)
            {
                centi[i][3] = 0;
            }
        }
        if (centi[i][3] == 0)
        {
            centi[i][x] += 0.1 * speed;
        }
        else
        {
            centi[i][x] -= 0.1 * speed;
        }

        for (int j = 0; j < noOfmushroms; ++j)
        {
            if (centi[i][x] < locationOfmushrm[j][0] + boxPixelsX && centi[i][x] + boxPixelsX > locationOfmushrm[j][0] && centi[i][y] < locationOfmushrm[j][1] + boxPixelsY &&
                centi[i][y] + boxPixelsY > locationOfmushrm[j][1])
            {
                // Move to the lower line when hitting a mushroom
                centi[i][y] += boxPixelsY;
                centi[i][2] = -centi[i][2];
            }

        }

        if (centi[i][y] >= resolutionY - boxPixelsY && centi[i][y] >= 0)
        {
            centi[i][y] = 700; // Move to the top of the page
        }
        i++;
    }
}
void imageofcenti(sf::RenderWindow& window, float centi[][4], sf::Sprite SpofCenti[], int Lengthofcenti) // Here we use this funtion for drawing the centi sprite
{
    int k = 0;
    while (k < Lengthofcenti)
    {
        SpofCenti[k].setPosition(centi[k][x], centi[k][y]);
        window.draw(SpofCenti[k]);
        k++;
    }
}
void strikingofCenti(int& totalmush, float centi[][4], float bullet[], float& speed, int& score, sf::Sprite SpofCenti[], sf::Texture& centihead, int Lengthofcenti, float locationOfmushrm[][2], int noOfmushroms, int HeathOfmushrm[]) //In this function all the bullet centipede collisions are handling an in this funtion the centipede start  divinding in its fragments and a new haead appeard on it and also when any segment of centipede destroys then it  increases the score by 10

{
    int f = 0;
    while (f < Lengthofcenti)
    {
        if (bullet[exists] && bullet[x] < centi[f][x] + boxPixelsX && bullet[x] + boxPixelsX > centi[f][x] && bullet[y] < centi[f][y] + boxPixelsY &&
            bullet[y] + boxPixelsY >   centi[f][y])
        {
            bullet[exists] = false;

            // Here the new centi head is drawing against every fragment 
            SpofCenti[f - 1].setTexture(centihead);
            SpofCenti[f - 1].setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));


            for (int j = 0; j < f; ++j)
            {

                centi[j][3] =  1-centi[j][3] ;

            }

             if (centi[f][x] < 960 && centi[f][y] > 700)
             {

                               locationOfmushrm[totalmush][x] = centi[f][x];
                               locationOfmushrm[totalmush][y] = centi[f][y];
                               HeathOfmushrm[totalmush] = 2;
                                totalmush++;

             }

             // Here the fragment which is hitted by the bullet is moed out of  the page

            centi[f][x] = -500;
            centi[f][y] = -600;

            score += 10;

        }
        f++;
    }

}


void Randmushroom(float locationOfmushrm[][2], int HeathOfmushrm[], int noOfmushroms)// In this function the muhrooms are generating on the random location on the game grid and this also assigns the life to every mushroom
{

    int i = 0;
    while (i < noOfmushroms)
    {
        //	This is for  generating the mushrooms in the x location 
        locationOfmushrm[i][x] = rand() % gameColumns * boxPixelsX;
        //	This is for  generating the mushrooms in the x location 
        locationOfmushrm[i][y] = rand() % (gameRows)*boxPixelsY;
        // here every mushroom is assigning the life of two ypu can increase the life and then it take bullets according to life to destroy
        HeathOfmushrm[i] = 2;
        ++i;
    }


}

void spritemushroom(sf::RenderWindow& window, sf::Sprite& Spofmushrom, float locationOfmushrm[][2], int noOfmushroms) // This funtion draws the mushroom sprite 

{
    for (int i = 0; i < noOfmushroms; ++i)
    {
        Spofmushrom.setPosition(locationOfmushrm[i][x], locationOfmushrm[i][y]);
        window.draw(Spofmushrom);
    }
}



void bulletandmushroom(float XcoorditeOfbullet, float YcoorditeOfbullet, float bullet[], int& score, float locationOfmushrm[][2], int HeathOfmushrm[], int noOfmushroms) // THis funtion checks the collision of mushrooms with the bullets . Here every muhsroom take two bullets  to destroy 
{

    for (int i = 0; i < noOfmushroms; ++i)
    {
        if (bullet[exists] && XcoorditeOfbullet < locationOfmushrm[i][x] + boxPixelsX && XcoorditeOfbullet + boxPixelsX > locationOfmushrm[i][x] &&
            YcoorditeOfbullet < locationOfmushrm[i][y] + boxPixelsY && YcoorditeOfbullet + boxPixelsY > locationOfmushrm[i][y])
        {

            HeathOfmushrm[i] -= 1; // this reduce the life of a mushroom by 1

            // Here the bullets is removing when hit to the mushroom
            bullet[exists] = false;


            if (HeathOfmushrm[i] == 0)
            {

                //     This remove the mushroom after hitting by the two bullets
                locationOfmushrm[i][x] = -200;
                locationOfmushrm[i][y] = -100;
                score++;
                //HeathOfmushrm[i] = 0; // Reset mushroom life
            }


            break;

        }
    }
}


void scorpian(float scorp[], float& speed) // THis funtion generate a scorpian which when collide with the wall move to a lower line 

{
    scorp[x] -= speed;

    // it keeps scorp to move out of the page 
    if (scorp[x] > resolutionX - boxPixelsX || scorp[x] < 0)
    {
        speed = -speed;
    }

    // here the scorpian moves to the lower line when hitting with the right wall 
    if (scorp[x] > resolutionX - boxPixelsX)
    {
        scorp[x] = resolutionX - boxPixelsX;
        scorp[y] = scorp[y] + boxPixelsY;
    }

    //     here the scorpian moves to the lower line when hitting with the right wall 
    if (scorp[x] < 0)
    {
        scorp[x] = 0;
        scorp[y] = scorp[y] + boxPixelsY;
    }

    // THIS IS FOR TO MAKE THE SOCRPIN TO MOVE OUT THE PAGE FROM VERTICALL DIRECTION 
    if (scorp[y] >= resolutionY - boxPixelsY)
    {
        scorp[y] = resolutionY - boxPixelsY;
    }

}

void scorptext(sf::RenderWindow& window, float scorp[], sf::Sprite& scorpSprite) // tHIS DRAWS THE SPRITE OF scorp 

{
    scorpSprite.setPosition(scorp[x], scorp[y]);
    window.draw(scorpSprite);

}

bool playerScorpCollide(const float obj1[], const float obj2[])   // tHIS FUNCTION CHECKS THE COLLSION OF SCORPIAN AND THE PLAYER AND I USE IT BECUSE WHEN PLAYER AND SCOTPIAN COLLIDES THEN THE GAME OVERS

{


    return (obj1[x] < obj2[x] + boxPixelsX && obj1[x] + boxPixelsX > obj2[x] && obj1[y] < obj2[y] + boxPixelsY && obj1[y] + boxPixelsY > obj2[y]);


}
bool playerandCentiCollide(float player[], float centi[][4], int Lengthofcenti)// tHIS FUNCTION CHECKS THE COLLSION OF centi AND THE PLAYER AND I USE IT BECUSE WHEN PLAYER AND SCOTPIAN COLLIDES THEN THE GAME OVERS

{
    // HERE IF IT RETUNS TRUE THEN IT MEANS THERE IS A COLLSION OTHERWISE NOT 
    for (int i = 0; i < Lengthofcenti; ++i)
    {
        if (player[x] < centi[i][x] + boxPixelsX && player[x] + boxPixelsX > centi[i][x] && player[y] < centi[i][y] + boxPixelsY && player[y] + boxPixelsY > centi[i][y])
        {
            return true;
        }
    }
    return false;
}




void writeScore(int sc)  // HERE I AM WRITING THE HIGH SCORE IN THE FILE 
{
    ofstream out("highScore.txt");

    out << sc;

}
void checkhighest(int sc) // HERE I AM GETTING THE HIGH SCORE 
{

    ifstream inputFile("highScore.txt");
    int i;
    inputFile >> i;
    if (sc == 0)
    {
        writeScore(sc);
    }
    if (sc > i)
    {
        writeScore(sc);
    }

}