#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include <vector>

const float PI = 3.14159265359;
const float deg_per_rad = 180.0/PI;
const float pixels_per_meter = 32.0;

class Game {
    private:
        sf::RenderWindow window;
        sf::Font font;
        sf::Text text;
        sf::Music music;

        // Mundo de Box2D
        b2World* world;
        // Vector para almacenar los cuadrados
        std::vector<b2Body*> cuerpos;

        void processEvents();

        void render();

        void crearCuadrado(float x, float y, float width, float height);

        void borrarCuadrados();

        void crearSuelo(float x, float y, float width, float height);
    public:
        Game();

        ~Game();

        void run();
};

#endif // GAME_H
