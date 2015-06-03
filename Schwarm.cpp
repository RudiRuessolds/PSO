#include "stdafx.h"


Schwarm::Schwarm(){
	srand((unsigned int)time(0));
}

Schwarm::~Schwarm(){

}

void Schwarm::init(std::vector<Particle>& particles){
	this->particles = &particles;
}

int Schwarm::getRandomNumber(int min, int max){
	return min + (rand() % (max - min + 1));
}

float Schwarm::getRandomNumberFloat(float min, float max){
	return  min + (float)rand() / ((float)RAND_MAX / (max - min));
}

void Schwarm::updateSwarm(sf::Vector2f mousePos, float deltaTime){
#pragma omp parallel
	{
		// für jeden partikel die distance zum target berechnen
		#pragma omp for
		for (int i = 0; i < PARTICLES_COUNT; ++i) 
		{
			particles->at(i).distance = distanceOf(mousePos, particles->at(i).position);
		}

		//für jedes Partikel den besten Nachbar im sozialen Umkreis finden
		#pragma omp for
		for (int i = 0; i < PARTICLES_COUNT; ++i) 
		{
			int compareWithSocialNeighbours = NEIGHBOUR_COUNT;
			//alle nachbarn links des current particle vom nächsten zu weitesten index
			for (int j = i - 1; j >= 0; --j) 
			{
				//Bewertung der eigenen besten Position mit der Position des Nachbars
				if (particles->at(j).distance < particles->at(i).distance)
				{
					particles->at(i).bestNeighborPosition = particles->at(j).position; //neue beste Position
				}

				// damit er nicht alle nachbran überprüft nur bis der NEIGHBOUR_COUNT erreicht ist
				if (--compareWithSocialNeighbours == 0)
					break; 
			}

			// +1, damit er sich nciht selbst checked
			//alle nachbarn rechts des current particle vom nächsten zu weitesten index
			for (int j = i + 1; j < PARTICLES_COUNT; ++j)
			{
				// damit er nicht alle nachbran überprüft nur bis der NEIGHBOUR_COUNT erreicht ist
				if (compareWithSocialNeighbours-- == 0)
					break; 

				//Bewertung der eigenen besten Position mit der Position des Nachbars
				if (particles->at(j).distance < particles->at(i).distance)
				{
					particles->at(i).bestNeighborPosition = particles->at(j).position; //neue beste Position
				}
			}
		}

		//updaten der current position
		#pragma omp for
		for (int i = 0; i < PARTICLES_COUNT; ++i)
		{
			Particle &particle = particles->at(i);

			// Zufallswerte rs und rt aus den Intervallen [0,s] und [u,t] generieren.
			float rs = getRandomNumberFloat(0.0f, S);
			float rt = getRandomNumberFloat(U, T);

			// Richtung zur besten beobachteten Position (die Eigene)
			sf::Vector3f dirToBestPos = normalize(particle.bestPosition - particle.position); //normalize the velocity in the direction to the best found position

			sf::Vector3f dirToBestNeighbor = normalize(particle.bestNeighborPosition - particle.position); //normalize the velocity in the direction to the best neighbour

			// finale bewegungsrichtung berechnen und particle updaten mit zufälligkeiten
			//Formel: vi(t+1) = a * vi(t) + rs * b * ( xi(p) - xi(t) ) + rt * c * ( xj(t) - xi(t))
			particle.velocity = A * particle.velocity + rs * B * dirToBestPos + rt * C * dirToBestNeighbor;

			// current Position updaten
			particle.position = particle.position + particle.velocity * deltaTime;

			//Bewertung der neuen Position wenn neue Position besser als bekannte beste Position, tauschen
			if (distanceOf(mousePos, particle.position) < particle.distance)
			{
				particle.bestPosition = particle.position;
			}
		}
	}
}

sf::Vector3f Schwarm::normalize(sf::Vector3f vector){
	float length = this->length(vector);

	if (length != 0){
		vector.x = vector.x / length;
		vector.y = vector.y / length;
		vector.z = vector.z / length;
	}

	return vector;
}

float Schwarm::distanceOf(sf::Vector2f mousePos, sf::Vector3f position){
	return length(sf::Vector3f(mousePos.x, mousePos.y, 0.0f) - position);
}

// länge des Vectors ermitteln
float Schwarm::length(sf::Vector3f vector){
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}
