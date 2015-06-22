#include "stdafx.h"
#include "Swarm.h"

Swarm::Swarm(){
	srand((unsigned int)time(0));
}

Swarm::~Swarm(){

}

void Swarm::init(std::vector<Particle>& particles){
	this->particles = &particles;
}

int Swarm::getRandomNumber(int min, int max){
	return min + (rand() % (max - min + 1));
}

float Swarm::getRandomNumberFloat(float min, float max){
	return  min + (float)rand() / ((float)RAND_MAX / (max - min));
}

void Swarm::updateSwarm(sf::Vector2f mousePos, float deltaTime){
#pragma omp parallel
	{
		// calc distance to target for each particle
		#pragma omp for
		for (int i = 0; i < PARTICLES_COUNT; ++i) 
		{
			particles->at(i).distance = distanceOf(mousePos, particles->at(i).position);
		}

		// Calc best neighbour in social neighbourhood that are in range
		#pragma omp for
		for (int i = 0; i < PARTICLES_COUNT; ++i) 
		{
			int compareWithSocialNeighbours = NEIGHBOUR_COUNT;
			// All neighbours left from the particle, beginning with the closest index
			for (int j = i - 1; j >= 0; --j) 
			{
				// Compare own best pos with neighbour pos 
				if (particles->at(j).distance < particles->at(i).distance)
				{
					particles->at(i).bestNeighborPosition = particles->at(j).position; //Assign new best pos
				}

				// Compare only as long as NEIGHBOUR_COUNT is not reached
				if (--compareWithSocialNeighbours == 0)
					break; 
			}

			// +1, to avoid self checking 
			// All neighbours right from current particle, begining with closest index
			for (int j = i + 1; j < PARTICLES_COUNT; ++j)
			{
				// Compare only as long as NEIGHBOUR_COUNT is not reached
				if (compareWithSocialNeighbours-- == 0)
					break; 

				// Compare own best pos with neighbour pos
				if (particles->at(j).distance < particles->at(i).distance)
				{
					particles->at(i).bestNeighborPosition = particles->at(j).position; //Assign new best pos
				}
			}
		}

		//updaten der current position
		#pragma omp for
		for (int i = 0; i < PARTICLES_COUNT; ++i)
		{
			Particle &particle = particles->at(i);

			// Random values for rs and rt generated between [0,s] and [u,t].
			float rs = getRandomNumberFloat(0.0f, S);
			float rt = getRandomNumberFloat(U, T);

			// Direction to the best observed position (own)
			sf::Vector3f dirToBestPos = normalize(particle.bestPosition - particle.position); //normalize the velocity in the direction to the best found position

			sf::Vector3f dirToBestNeighbor = normalize(particle.bestNeighborPosition - particle.position); //normalize the velocity in the direction to the best neighbour

			// Calc final movedirection with random values and updaten particle
			//Formular: vi(t+1) = a * vi(t) + rs * b * ( xi(p) - xi(t) ) + rt * c * ( xj(t) - xi(t))
			particle.velocity = A * particle.velocity + rs * B * dirToBestPos + rt * C * dirToBestNeighbor;

			// current Position updaten
			particle.position = particle.position + particle.velocity * deltaTime;

			// New position evaluation. If new position is better as known best position assign best position to new position
			if (distanceOf(mousePos, particle.position) < particle.distance)
			{
				particle.bestPosition = particle.position;
			}
		}
	}
}

sf::Vector3f Swarm::normalize(sf::Vector3f vector){
	float length = this->length(vector);
	if (length != 0){
		vector.x = vector.x / length;
		vector.y = vector.y / length;
		vector.z = vector.z / length;
	}
	return vector;
}

float Swarm::distanceOf(sf::Vector2f mousePos, sf::Vector3f position){
	return length(sf::Vector3f(mousePos.x, mousePos.y, 0.0f) - position);
}

// Calc vector lenght.
float Swarm::length(sf::Vector3f vector){
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}
