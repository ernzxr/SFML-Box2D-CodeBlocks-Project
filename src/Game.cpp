#include "Game.h"

Game::Game() {
    // Crear la ventana de SFML
    window.create(sf::VideoMode(800, 600), "UTN FRGP");
    window.setFramerateLimit(60);

    // Inicializar el mundo de Box2D
    b2Vec2 gravity(0.0f, 1.0f);
    world = new b2World(gravity); // Crear el mundo con gravedad

    // Crear suelo
    crearSuelo(0, 570, 800, 30);

    // Cargar la fuente
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Error al cargar la fuente" << std::endl;
    }

    // Cargar la música
    if (!music.openFromFile("resources/sounds/KODOMOi_Sunny.mp3")) {
        std::cerr << "Error al cargar la música" << std::endl;
    }

    // Reproducir la música
    music.setVolume(5);
    music.setLoop(true);
    music.play();

    // Crear el texto
    text.setFont(font);
    text.setString("Universidad Tecnologica Nacional\nFacultad Regional General Pacheco\nProgramación II\nSFML + Box2D");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // Centrar el texto en la ventana
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text.setPosition(window.getSize().x / 2, window.getSize().y / 2);
}

Game::~Game() {
    delete world; // Eliminar el puntero al mundo
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();

        // Actualizar el mundo de Box2D
        world->Step(1.0f / 60, 8, 3); // Asegúrate de usar el puntero

        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        float x = sf::Mouse::getPosition(window).x;
        float y = sf::Mouse::getPosition(window).y;
        crearCuadrado(x, y, 10, 10);
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        borrarCuadrados();
    }
}

void Game::render() {
    window.clear();

    for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext()) {
        // Acceder al shape desde los datos de usuario
        sf::RectangleShape* shape = reinterpret_cast<sf::RectangleShape*>(body->GetUserData().pointer);
        if (shape) { // Verificar que shape no sea nullptr
            shape->setPosition(body->GetPosition().x * pixels_per_meter, body->GetPosition().y * pixels_per_meter);
            shape->setRotation(body->GetAngle() * deg_per_rad);
            window.draw(*shape);
        }
    }

    // Dibujar el texto
    window.draw(text);

    window.display();
}

void Game::crearCuadrado(float x, float y, float width, float height) {
    // Definir el cuerpo del cuadrado
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set((x + width / 2.0f) / pixels_per_meter, (y + height / 2.0f) / pixels_per_meter);

    // Crear cuerpo
    b2Body* body = world->CreateBody(&bodyDef);

    // Definir la forma del cuadrado
    b2PolygonShape b2Shape;
    b2Shape.SetAsBox(width / pixels_per_meter / 2.0f, height / pixels_per_meter / 2.0f);

    // Definir las propiedades físicas del cuadrado
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &b2Shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;

    // Asignar la forma y propiedades al cuerpo
    body->CreateFixture(&fixtureDef);

    // Se crea la forma a dibujar de SFML
    sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(width, height));
    shape->setOrigin(width / 2.0f, height / 2.0f);
    shape->setPosition(x, y);
    shape->setFillColor(sf::Color::White);

    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(shape);

    cuerpos.push_back(body);
}

void Game::borrarCuadrados() {
    for (b2Body* body : cuerpos) {
        // Obtener el puntero al shape y eliminarlo
        sf::RectangleShape* shape = reinterpret_cast<sf::RectangleShape*>(body->GetUserData().pointer);
        delete shape; // Liberar la memoria del shape

        world->DestroyBody(body); // Destruir el cuerpo en Box2D
    }
    cuerpos.clear(); // Limpiar el vector de cuerpos
}

void Game::crearSuelo(float x, float y, float width, float height) {
    // Definir el cuerpo del suelo
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((x + width / 2.0) / pixels_per_meter, (y + height / 2.0) / pixels_per_meter);

    // Crear cuerpo
    b2Body* body = world->CreateBody(&bodyDef);

    // Definir la forma del suelo
    b2PolygonShape b2Shape;
    b2Shape.SetAsBox(width / pixels_per_meter / 2.0, height / pixels_per_meter / 2.0);

    // Definir las propiedades físicas del suelo
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &b2Shape;
    fixtureDef.density = 0.0f; // La densidad debe ser 0 para cuerpos estáticos
    fixtureDef.friction = 0.4f;

    // Asignar la forma y propiedades al cuerpo
    body->CreateFixture(&fixtureDef);

    // Se crea la forma a dibujar de SFML
    sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(width, height));
    shape->setOrigin(width / 2.0, height / 2.0);
    shape->setPosition(x, y);
    shape->setFillColor(sf::Color::White);

    // Asignar el puntero al shape al usuario de datos
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(shape);
}
