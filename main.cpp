#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

class GravitySource
{
    sf::Vector2f pos;
    float strength;
    sf::CircleShape s;

public:
    GravitySource(float pos_x, float pos_y, float strength)
    {
        pos.x = pos_x;
        pos.y = pos_y;
        this->strength = strength;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(5);
    }

    void render(sf::RenderWindow& wind)
    {
        wind.draw(s);
    }

    sf::Vector2f get_pos()
    {
        return pos;
    }

    float get_strength()
    {
        return strength;
    }
};

class Particle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape s;

public:
    Particle()
    //Particle(float pos_x, float pos_y, float vel_x, float vel_y)
    {
        int x = rand() % 1600;
        int y = rand() % 1000;
        pos.x = x;
        pos.y = y;
        vel.x = 1;
        vel.y = 1;
        //pos.x = pos_x;
        //pos.y = pos_y;

        //vel.x = vel_x;
        //vel.y = vel_y;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(3);
    }

    void render(sf::RenderWindow& wind)
    {
        s.setPosition(pos);
        wind.draw(s);
    }

    void update_physics(GravitySource& s)
    {
        float distance_x = s.get_pos().x - pos.x;
        float distance_y = s.get_pos().y - pos.y;

        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

        float inverse_distance = 1.f / distance;

        float normalized_x = inverse_distance * distance_x;
        float normalized_y = inverse_distance * distance_y;

        // 1/x * 1/x = 1/x² -> plus on est loin moins la gravité est forte
        float inverse_square_dropoff = inverse_distance * inverse_distance;

        float acceleration_x = normalized_x * s.get_strength() * inverse_square_dropoff;
        float acceleration_y = normalized_y * s.get_strength() * inverse_square_dropoff;

        vel.x += acceleration_x;
        vel.y += acceleration_y;

        pos.x += vel.x;
        pos.y += vel.y;
    }

    //void collision_check(Particle& p, Particle& p2)
    //{
    //    float s = 0.0;
    //    if ((p - p2) <= p2.s.getRadius() + p.s.getRadius())
    //    {

    //    }
    //}
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "My Program");
    window.setFramerateLimit(60);

    GravitySource source(800, 500, 7000);

    Particle particle[1000];

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        window.clear();
        for (int i=0; i < 1000; i++)
        {
            //for (int j = 0; j < 1000; j++)
            //{
            //    particle[i].collision_check(particle[j], particle[i]);
            //}
            particle[i].update_physics(source);
        }


        source.render(window);
        for (int i = 0; i < 1000; i++)
        {
            particle[i].render(window);
        }
        //draw calls
        window.display();
    }

    return 0;
}
