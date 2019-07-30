#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <list>


using namespace std;

struct Tail
{
    int pos[2];
    int oldPos[2];
};

enum Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

void spawnFood(char grid[][17], size_t gridSize)
{
    int y, x;
    do
    {
        y=rand() % gridSize;
        x=rand() % sizeof(grid[0])/sizeof(char);
    } while(grid[y][x] != ' ');
    grid[y][x] = 'F';
}

int main()
{
    char grid[17][17];
    Direction dir = NORTH;
    float timer=0, delay=0.1;
    list<Tail> tails;
    sf::Clock clock;

    for(int i=0; i<sizeof(grid)/sizeof(grid[0]); i++)
    {
        for(int j=0; j<sizeof(grid[0])/sizeof(char); j++)
        {
            grid[i][j] = ' ';
        }
    }

    spawnFood(grid, sizeof(grid)/sizeof(grid[0]));
    grid[(sizeof(grid)/sizeof(grid[0]))/2][sizeof(grid[0])/2] = 'P';

    sf::RenderWindow window(sf::VideoMode(690, 690), "title");
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer+=time;

		/// event handler ///
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }

                    if (event.key.code == sf::Keyboard::Up && dir != SOUTH)
                    {
                        dir = NORTH;
                    }
                    else if (event.key.code == sf::Keyboard::Right && dir != WEST)
                    {
                        dir = EAST;
                    }
                    else if (event.key.code == sf::Keyboard::Down && dir != NORTH)
                    {
                        dir = SOUTH;
                    }
                    else if (event.key.code == sf::Keyboard::Left && dir != EAST)
                    {
                        dir = WEST;
                    }
                    break;

                default:
                    break;
            }
        }

        /// every tick do this ///
        if (timer>delay)
        {
            /// get player position
            int x, y, a, b;
            for(int i=0; i<sizeof(grid)/sizeof(grid[0]); i++)
            {
                for(int j=0; j<sizeof(grid[0])/sizeof(char); j++)
                {
                    if(grid[i][j] == 'P')
                    {
                        x=j, a=j, y=i, b=i;
                        grid[i][j] = ' ';
                    }
                }
            }

            /// check if snake head collided with tail
            if(!tails.empty())
            {
                if(tails.front().pos[0] == b && tails.front().pos[1] == a)
                {
                    tails.clear();
                    for(int i=0; i<sizeof(grid)/sizeof(grid[0]); i++)
                    {
                        for(int j=0; j<sizeof(grid[0])/sizeof(char); j++)
                        {
                            grid[i][j] = ' ';
                        }
                    }

                    spawnFood(grid, sizeof(grid)/sizeof(grid[0]));
                    grid[(sizeof(grid)/sizeof(grid[0]))/2][sizeof(grid[0])/2] = 'P';
                }

                list<Tail>::iterator it1 = tails.begin();
                advance(it1, 1);
                for(int i=1; i<tails.size(); i++)
                {
                    if((*it1).pos[0] == b && (*it1).pos[1] == a)
                    {
                        tails.clear();
                        for(int i=0; i<sizeof(grid)/sizeof(grid[0]); i++)
                        {
                            for(int j=0; j<sizeof(grid[0])/sizeof(char); j++)
                            {
                                grid[i][j] = ' ';
                            }
                        }

                        spawnFood(grid, sizeof(grid)/sizeof(grid[0]));
                        grid[(sizeof(grid)/sizeof(grid[0]))/2][sizeof(grid[0])/2] = 'P';
                    }
                    advance(it1, 1);
                }
            }

            /// movement of head
            switch(dir)
            {
                case NORTH:
                    if((y-1)<0){ y=sizeof(grid)/sizeof(grid[0]); }
                    if(grid[y-1][x] == 'F')
                    {
                        spawnFood(grid, sizeof(grid)/sizeof(grid[0]));
                        Tail tail;
                        if (tails.empty())
                        {
                            tail.pos[0] = b+1;
                            tail.pos[1] = a;
                        }
                        else
                        {
                            tail.pos[0] = tails.back().oldPos[0];
                            tail.pos[1] = tails.back().oldPos[1];
                        }
                        tails.insert(tails.end(), tail);
                    }
                    grid[y-1][x] = 'P';
                    break;

                case EAST:
                    if((x+1)==sizeof(grid[0])/sizeof(char)){ x=-1; }
                    if(grid[y][x+1] == 'F')
                    {
                        spawnFood(grid, sizeof(grid)/sizeof(grid[0]));
                        Tail tail;
                        if (tails.empty())
                        {
                            tail.pos[0] = b;
                            tail.pos[1] = a-1;
                        }
                        else
                        {
                            tail.pos[0] = tails.back().oldPos[0];
                            tail.pos[1] = tails.back().oldPos[1];
                        }
                        tails.insert(tails.end(), tail);
                    }
                    grid[y][x+1] = 'P';
                    break;

                case SOUTH:
                    if((y+1)==sizeof(grid)/sizeof(grid[0])){ y=-1; }
                    if(grid[y+1][x] == 'F')
                    {
                        spawnFood(grid, sizeof(grid)/sizeof(grid[0]));
                        Tail tail;
                        if (tails.empty())
                        {
                            tail.pos[0] = b-1;
                            tail.pos[1] = a;
                        }
                        else
                        {
                            tail.pos[0] = tails.back().oldPos[0];
                            tail.pos[1] = tails.back().oldPos[1];
                        }
                        tails.insert(tails.end(), tail);
                    }
                    grid[y+1][x] = 'P';
                    break;

                case WEST:
                    if((x-1)<0){ x=sizeof(grid[0])/sizeof(char); }
                    if(grid[y][x-1] == 'F')
                    {
                        spawnFood(grid, sizeof(grid)/sizeof(grid[0]));
                        Tail tail;
                        if (tails.empty())
                        {
                            tail.pos[0] = b;
                            tail.pos[1] = a+1;
                        }
                        else
                        {
                            tail.pos[0] = tails.back().oldPos[0];
                            tail.pos[1] = tails.back().oldPos[1];
                        }
                        tails.insert(tails.end(), tail);
                    }
                    grid[y][x-1] = 'P';
                    break;
            }

            /// movement of tail
            if(!tails.empty())
            {
                tails.front().oldPos[0] = tails.front().pos[0];
                tails.front().oldPos[1] = tails.front().pos[1];
                tails.front().pos[0] = b;
                tails.front().pos[1] = a;
                grid[tails.front().oldPos[0]][tails.front().oldPos[1]] = ' ';

                list<Tail>::iterator it1 = tails.begin();
                list<Tail>::iterator it2 = tails.begin();
                advance(it2, 1);
                for(int i=1; i<tails.size(); i++)
                {
                    (*it2).oldPos[0] = (*it2).pos[0];
                    (*it2).oldPos[1] = (*it2).pos[1];
                    (*it2).pos[0] = (*it1).oldPos[0];
                    (*it2).pos[1] = (*it1).oldPos[1];
                    grid[(*it2).oldPos[0]][(*it2).oldPos[1]] = ' ';
                    advance(it1, 1);
                    advance(it2, 1);
                }
                it1 = tails.begin();
                it2 = tails.begin();
                advance(it1, 1);
                grid[tails.front().pos[0]][tails.front().pos[1]] = 'T';
                for(int i=1; i<tails.size(); i++)
                {
                    grid[(*it1).pos[0]][(*it1).pos[1]] = 'T';
                    advance(it1, 1);
                    advance(it2, 1);
                }
            }
            timer = 0;
        }

        /// draw ///
        window.clear();

        for(int i=0; i<sizeof(grid)/sizeof(grid[0]); i++)
        {
            for(int j=0; j<sizeof(grid[0])/sizeof(char); j++)
            {
                if(grid[i][j] == ' ')
                {
                    sf::CircleShape shape(25.f, 4);
                    shape.setPosition((float)((j+1)*38), (float)(i*38));
                    shape.setFillColor(sf::Color(100, 100, 100));
                    shape.setOutlineThickness(2.f);
                    shape.setOutlineColor(sf::Color(200, 200, 200));
                    shape.rotate(45.f);
                    window.draw(shape);
                }
                else if (grid[i][j] == 'F')
                {
                    sf::CircleShape shape(25.f, 4);
                    shape.setPosition((float)((j+1)*38), (float)(i*38));
                    shape.setFillColor(sf::Color(220, 10, 10));
                    shape.setOutlineThickness(2.f);
                    shape.setOutlineColor(sf::Color(200, 200, 200));
                    shape.rotate(45.f);
                    window.draw(shape);
                }
                else if (grid[i][j] == 'P' || grid[i][j] == 'T')
                {
                    sf::CircleShape shape(25.f, 4);
                    shape.setPosition((float)((j+1)*38), (float)(i*38));
                    shape.setFillColor(sf::Color(10, 220, 10));
                    shape.setOutlineThickness(2.f);
                    shape.setOutlineColor(sf::Color(200, 200, 200));
                    shape.rotate(45.f);
                    window.draw(shape);
                }
            }
        }

        window.display();
    }
    return 0;
}
