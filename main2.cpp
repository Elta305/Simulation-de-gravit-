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
    float strength;
    sf::CircleShape s;


public:
    Particle()
    //Particle(float pos_x, float pos_y, float vel_x, float vel_y, float strength)
    {
        int x = rand() % 1600;
        int y = rand() % 1000;
        pos.x = x;
        pos.y = y;
        vel.x = 0;
        vel.y = 0;
        strength = 1;
        //pos.x = pos_x;
        //pos.y = pos_y;

        //vel.x = vel_x;
        //vel.y = vel_y;

        //this->strength = strength;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(3);
    }

    void render(sf::RenderWindow& wind)
    {
        s.setPosition(pos);
        wind.draw(s);
    }

    sf::Vector2f get_pos()
    {
        return pos;
    }

    float get_radius()
    {
        return s.getRadius();
    }

    float get_strength()
    {
        return strength;
    }

    void update_physics(Particle& s, Particle& p)
    {
        if (&s != &p) {
            float distance_x = s.get_pos().x - p.get_pos().x;
            float distance_y = s.get_pos().y - p.get_pos().y;

            float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

            float inverse_distance = 1.f / distance;

            float normalized_x = inverse_distance * distance_x;
            float normalized_y = inverse_distance * distance_y;

            // 1/x * 1/x = 1/x² -> plus on est loin moins la gravité est forte
            float inverse_square_dropoff = inverse_distance * inverse_distance;

            float acceleration_x = normalized_x * s.get_strength() * inverse_square_dropoff;
            float acceleration_y = normalized_y * s.get_strength() * inverse_square_dropoff;

            p.vel.x += acceleration_x;
            p.vel.y += acceleration_y;

            collision_check(s, p, distance);

            p.pos.x += p.vel.x;
            p.pos.y += p.vel.y;
        }
    }

    void collision_check(Particle& s, Particle& p, float d)
    {
        if (d <= p.get_radius() + s.get_radius())
        {
            p.vel.x = -p.vel.x / 10;
            p.vel.y = -p.vel.y / 10;
            s.vel.x = -s.vel.x / 10;
            s.vel.y = -s.vel.y / 10;
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "My Program");
    window.setFramerateLimit(60);

    //GravitySource source(800, 500, 7000);

    const int nb = 50;
    Particle particle[nb];

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        window.clear();
        for (int i=0; i < nb; i++)
        {
            for (int j = 0; j < nb; j++)
            {
                particle[i].update_physics(particle[j], particle[i]);
            }
        }


        //source.render(window);
        for (int i = 0; i < nb; i++)
        {
            particle[i].render(window);
        }
        //draw calls
        window.display();
    }

    return 0;
}
