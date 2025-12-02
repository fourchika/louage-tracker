#ifndef STATION_H
#define STATION_H

#include <vector>
#include <string>
#include "Destination.h"
using namespace std;

class Station {
private:
    string nom_station;
    int num_station;
    vector<Destination> destinations;

public:
    Station(string nom, int num);
    void ajouterDestination(const Destination &des);
    void afficher() const;

    string getNom() const;
    int getNumStation() const;

    vector<Destination>& getDestinations();             // modifiable
    const vector<Destination>& getDestinations() const; // read-only
};

#endif
