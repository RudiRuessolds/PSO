#include "stdafx.h"
#include "Swarm.h"

int main(){
	sf::RenderWindow window(sf::VideoMode(WIDTH, Height, 32), "Swarm");
	window.setFramerateLimit(60); // Set frame rate limit
	window.setMouseCursorVisible(false);

	sf::Clock deltaClock;
	Swarm* swarm = new Swarm();
	std::vector<Particle> particles;
	particles.reserve(PARTICLES_COUNT);

	//initialize particles
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
	
	//Game loop
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
		//measure deltatime
		sf::Time deltaTime = deltaClock.restart();

		// create mouse objekt
		sf::CircleShape mouse(4.0f);
		mouse.setFillColor(sf::Color::Black);
		mouse.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

		//update swarm with new mouse position
		swarm->updateSwarm(mouse.getPosition(),deltaTime.asSeconds());

		//clear window before rendering
		window.clear(sf::Color(100, 100, 200));

		//draw all Partikel 
		for (int i = 0; i < PARTICLES_COUNT; ++i)
		{
			sf::CircleShape bird(2.0f);
			bird.setPointCount(15);
			bird.setFillColor(sf::Color::White);
			bird.setPosition(particles.at(i).position.x, particles.at(i).position.y);
			window.draw(bird);
		}
		// draw mouse 
		window.draw(mouse);
		
		//render
		window.display();

	}
	return 0;

}