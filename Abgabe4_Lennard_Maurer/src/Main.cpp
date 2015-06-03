#include "stdafx.h"





int main(){
	sf::RenderWindow window(sf::VideoMode(WIDTH, Height, 32), "Swarm");
	window.setFramerateLimit(60); // Set a framrate limit to reduce the CPU load
	window.setMouseCursorVisible(false);

	sf::Clock deltaClock;
	Schwarm* swarm = new Schwarm();
	std::vector<Particle> particles;

	for (int i = 0; i < PARTICLES_COUNT; ++i)
	{
		Particle particle;
		//initialize particle
		particle.position = sf::Vector3f(swarm->getRandomNumber(0, WIDTH - 1), swarm->getRandomNumber(0, Height - 1), 0);
		particle.bestPosition = particle.position;
		particle.distance = 0.0f;
		particle.bestNeighborPosition = sf::Vector3f(0.0f, 0.0f, 0.0f);
		particle.velocity = sf::Vector3f(swarm->getRandomNumberFloat(-3.0f, 3.0f), swarm->getRandomNumberFloat(-3.0f, 3.0f), 0.0f);

		particles.push_back(particle);
	}
	swarm->init(particles);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
		sf::Time deltaTime = deltaClock.restart();

		// mouse objekt erstellen
		sf::CircleShape mouse(4.0f);
		mouse.setFillColor(sf::Color::Black);
		mouse.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

		swarm->updateSwarm(mouse.getPosition(),deltaTime.asSeconds());

		window.clear(sf::Color(100, 100, 200));

		//alle Partikel zeichnen 
		for (int i = 0; i < PARTICLES_COUNT; ++i)
		{
			sf::CircleShape bird(2.0f);
			bird.setPointCount(15);
			bird.setFillColor(sf::Color::White);
			bird.setPosition(particles.at(i).position.x, particles.at(i).position.y);
			window.draw(bird);
		}
		// mouse Zeichen 
		window.draw(mouse);
		
		window.display();

	}
	return 0;

}