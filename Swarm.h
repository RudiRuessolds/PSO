#include "stdafx.h"

//coefficients
#define A 0.95f //Motivation for current velocity.
#define B 1.1f //Motivation for own best position.
#define C 3.0f //Motivation for best position of neighbours.

//Blur factors rs and rt are random values between [0,s] and [u,t].
#define S 1.5 //for rs (influencing B)
#define U 1.5 //lower limit of rt (influencing C)
#define T 2.0 //upper limit of rt

struct Particle
{
	sf::Vector3f position; //the particle's current position
	float distance; //the distance to the target
	sf::Vector3f bestPosition; //the best position the particle has been to so far
	sf::Vector3f bestNeighborPosition; //the best position of a neighbor within range
	sf::Vector3f velocity; //current velocity	
};

class Swarm
{
private:
	typedef std::vector<Particle>::iterator iterator;
	std::vector<Particle>* particles;

public:
	Swarm();
	~Swarm();
	void init(std::vector<Particle>&);
	int getRandomNumber(int min, int max);
	float getRandomNumberFloat(float min, float max);
	void updateSwarm(sf::Vector2f, float deltaTime);
	sf::Vector3f normalize(sf::Vector3f vector);
	float distanceOf(sf::Vector2f mousePos, sf::Vector3f position);
	float length(sf::Vector3f vector);

};