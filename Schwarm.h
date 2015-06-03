#include "stdafx.h"

//Koeffizienten
#define A 0.95f //Motivation für die aktuelle Geschwindigkeit
#define B 1.1f //Motivation für Richtung zur besten beobachteten Position
#define C 3.0f //Motivation für Richtung zum besten Nachbar

//Die Unschärfefaktoren rs und rt sind Zufallswerte aus den Intervallen [0,s] und [u,t].
#define S 1.5	//für rs (beeinflust B)
#define U 1.5 //untere Schranke von rt (beeinflusst C)
#define T 2.0 //obere Schranke von rt

struct Particle
{
	sf::Vector3f position; //the particle's current position
	float distance; //the distance to the target
	sf::Vector3f bestPosition; //the best position the particle has been to so far
	sf::Vector3f bestNeighborPosition; //the best position of a neighbor within range
	sf::Vector3f velocity; //current velocity	
};

class Schwarm
{
private:
	typedef std::vector<Particle>::iterator iterator;
	std::vector<Particle>* particles;

public:
	Schwarm();
	~Schwarm();
	void init(std::vector<Particle>&);
	int getRandomNumber(int min, int max);
	float getRandomNumberFloat(float min, float max);
	void updateSwarm(sf::Vector2f, float deltaTime);
	sf::Vector3f normalize(sf::Vector3f vector);
	float distanceOf(sf::Vector2f mousePos, sf::Vector3f position);
	float length(sf::Vector3f vector);

};