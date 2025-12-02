#include "System.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

string normalizeString(const string& str) {
    string normalized = str;
    size_t start = normalized.find_first_not_of(" \t\n\r");
    size_t end = normalized.find_last_not_of(" \t\n\r");
    if (start != string::npos && end != string::npos) {
        normalized = normalized.substr(start, end - start + 1);
    }
    transform(normalized.begin(), normalized.end(), normalized.begin(),
              [](unsigned char c) { return tolower(c); });
    return normalized;
}

// AJOUTEZ CETTE FONCTION MANQUANTE
void System::ajouter_station(const Station& s) {
    if (findStation(s.getNom()) == nullptr) {
        stations.push_back(s);
    }
}

Station* System::findStation(const string& name) {
    string normalizedName = normalizeString(name);
    for (auto& s : stations) {
        if (normalizeString(s.getNom()) == normalizedName)
            return &s;
    }
    return nullptr;
}

const Station* System::findStation(const string& name) const {
    string normalizedName = normalizeString(name);
    for (const auto& s : stations) {
        if (normalizeString(s.getNom()) == normalizedName)
            return &s;
    }
    return nullptr;
}

void System::afficher_stations() const {
    int i = 1;
    for(const auto& l : stations) {
        cout << "-------" << i << "------" << l.getNom() << endl;
        i++;
    }
}

// ENLEVEZ OU COMMENTEZ CETTE FONCTION SI ELLE EXISTE DÉJÀ AILLEURS
/*
void System::ajouter_destination(const string& stationName, const Destination& dest) {
    Station* s = findStation(stationName);
    if (s == nullptr) {
        cout << "Station " << stationName << " introuvable!" << endl;
        return;
    }

    if (findDestination(dest.getNom(), stationName) != nullptr) {
        cout << "La destination " << dest.getNom() << " est deja ajoutee" << endl;
        return;
    }

    vector<Destination>& dests = s->getDestinations();
    dests.push_back(dest);
}
*/

Destination* System::findDestination(const string& destName, const string& stationName) {
    Station* x = findStation(stationName);
    if (x == nullptr) {
        return nullptr;
    }
    else {
        string normalizedDestName = normalizeString(destName);
        for (auto& s : x->getDestinations()) {
            if (normalizeString(s.getNom()) == normalizedDestName)
                return &s;
        }
        return nullptr;
    }
}

const Destination* System::findDestination(const string& destName, const string& stationName) const {
    const Station* x = findStation(stationName);
    if (x == nullptr) {
        return nullptr;
    }
    else {
        string normalizedDestName = normalizeString(destName);
        for (const auto& s : x->getDestinations()) {
            if (normalizeString(s.getNom()) == normalizedDestName)
                return &s;
        }
        return nullptr;
    }
}

Louage* System::findLouage(const Louage& l, const string& destName, const string& stationName) {
    Destination* d = findDestination(destName, stationName);
    if (d == nullptr) {
        return nullptr;
    }

    for (auto& lou : d->getLouages()) {
        if (lou.getnumero_louage() == l.getnumero_louage()) {
            return &lou;
        }
    }

    return nullptr;
}

void System::ajouter_louage(const string& stationName, const string& destName, const Louage& l) {
    Station* s = findStation(stationName);
    if (s == nullptr) {
        cout << "Station " << stationName << " introuvable!" << endl;
        return;
    }

    Destination* d = findDestination(destName, stationName);
    if (d == nullptr) {
        cout << "destination " << destName << " introuvable!" << endl;
        return;
    }

    Louage* x = findLouage(l, destName, stationName);
    if(x != nullptr) {
        cout << " le louage numero" << l.getnumero_louage() << "est deja pret dans "
             << stationName << "vers la destination" << destName << endl;
    }
    else {
        vector<Louage>& Lous = d->getLouages();
        Lous.push_back(l);
    }
}

void System::remove_louage(const string& stationName, const string& destName, const Louage& l) {
    Station* s = findStation(stationName);
    if (s == nullptr) {
        cout << "Station " << stationName << " introuvable!" << endl;
        return;
    }

    Destination* d = findDestination(destName, stationName);
    if (d == nullptr) {
        cout << "destination " << destName << " introuvable!" << endl;
        return;
    }

    Louage* x = findLouage(l, destName, stationName);
    if(x == nullptr) {
        cout << " le louage numero" << l.getnumero_louage() << "n'est pas deja pret dans " << endl;
    }

    vector<Louage>& L = d->getLouages();
    bool removed = false;

    for (auto it = L.begin(); it != L.end(); it++) {
        if (it->getnumero_louage() == l.getnumero_louage()) {
            L.erase(it);
            removed = true;
            break;
        }
    }

    if (!removed) {
        cout << "Aucun louage avec le numéro " << l.getnumero_louage() << " n'existe." << endl;
    }
}

void System::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Impossible d'ouvrir le fichier pour ecriture: " << filename << endl;
        return;
    }

    for (const auto& s : stations) {
        file << "Station|" << s.getNom() << "|" << s.getNumStation() << "\n";

        for (const auto& d : s.getDestinations()) {
            file << "Destination|" << d.getNom() << "\n";

            for (const auto& l : d.getLouages()) {
                file << "Louage|"
                     << l.getSerieVehicule() << "|"
                     << l.getnumero_louage() << "|"
                     << l.getIdProp() << "|"
                     << l.getDestination() << "|"
                     << l.getDepart() << "\n";
            }
        }
    }

    file.close();
}

void System::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "❌ Impossible d'ouvrir: " << filename << endl;
        return;
    }

    stations.clear();
    string line;
    Station* currentStation = nullptr;
    Destination* currentDestination = nullptr;

    cout << "📖 Chargement de " << filename << "..." << endl;

    while (getline(file, line)) {
        // Ignorer les lignes vides et commentaires
        if (line.empty() || line[0] == '#') {
            continue;
        }

        stringstream ss(line);
        string type;
        getline(ss, type, '|');

        if (type == "Station") {
            string name, numStr;
            getline(ss, name, '|');
            getline(ss, numStr);
            
            if (!numStr.empty()) {
                try {
                    Station s(name, stoi(numStr));
                    stations.push_back(s);
                    currentStation = &stations.back();
                    currentDestination = nullptr;
                    cout << "✅ Station: " << name << " (ID: " << numStr << ")" << endl;
                } catch (...) {
                    cout << "⚠️ Numéro invalide pour: " << name << endl;
                }
            }
        }
        else if (type == "Destination") {
            if (!currentStation) {
                cout << "⚠️ Destination sans station parente: " << line << endl;
                continue;
            }
            
            string destName;
            getline(ss, destName);
            
            if (!destName.empty()) {
                // VÉRIFIER SI LA DESTINATION EXISTE DÉJÀ
                bool destinationExists = false;
                for (auto& dest : currentStation->getDestinations()) {
                    if (normalizeString(dest.getNom()) == normalizeString(destName)) {
                        destinationExists = true;
                        currentDestination = &dest;
                        break;
                    }
                }
                
                // Si la destination n'existe pas, la créer
                if (!destinationExists) {
                    Destination d(destName);
                    currentStation->ajouterDestination(d);
                    currentDestination = &currentStation->getDestinations().back();
                    cout << "  📍 Destination: " << destName << " (dans " << currentStation->getNom() << ")" << endl;
                } else {
                    cout << "  🔁 Destination déjà existante: " << destName << endl;
                }
            }
        }
        else if (type == "Louage") {
            if (!currentDestination) {
                cout << "⚠️ Louage sans destination parente: " << line << endl;
                continue;
            }
            
            string serieStr, numStr, idStr;
            getline(ss, serieStr, '|');
            getline(ss, numStr, '|');
            getline(ss, idStr);
            
            // Certaines lignes peuvent avoir un format supplémentaire, ignorer les champs restants
            string restOfLine;
            if (getline(ss, restOfLine, '|')) {
                // Il y a plus de champs, mais on les ignore
            }
            
            if (!serieStr.empty() && !numStr.empty() && !idStr.empty()) {
                try {
                    // Convertir série (ex: "TN001" -> 1)
                    int sv = 0;
                    if (serieStr.length() > 2) {
                        string numPart = serieStr.substr(2);
                        if (!numPart.empty()) {
                            sv = stoi(numPart);
                        }
                    }
                    
                    int numLouage = stoi(numStr);
                    int idProp = stoi(idStr);
                    string depart = currentStation->getNom();
                    string dest = currentDestination->getNom();
                    
                    // VÉRIFIER SI LE LOUAGE EXISTE DÉJÀ
                    bool louageExists = false;
                    for (const auto& l : currentDestination->getLouages()) {
                        if (l.getnumero_louage() == numLouage) {
                            louageExists = true;
                            break;
                        }
                    }
                    
                    if (!louageExists) {
                        Louage l(sv, numLouage, idProp, dest, depart);
                        currentDestination->ajouter(l);
                        cout << "    🚌 Louage #" << numLouage << " ajouté: " << depart << " → " << dest << endl;
                    } else {
                        cout << "    🔁 Louage #" << numLouage << " déjà existant, ignoré" << endl;
                    }
                    
                } catch (...) {
                    cout << "⚠️ Erreur format louage: " << line << endl;
                }
            }
        }
    }

    file.close();
    
    // Résumé
    cout << "\n📊 CHARGEMENT TERMINÉ:\n";
    int totalStations = stations.size();
    int totalDestinations = 0;
    int totalLouages = 0;
    
    for (const auto& station : stations) {
        int stationDestinations = station.getDestinations().size();
        totalDestinations += stationDestinations;
        
        int stationLouages = 0;
        for (const auto& dest : station.getDestinations()) {
            stationLouages += dest.getLouages().size();
        }
        totalLouages += stationLouages;
        
        cout << "  " << station.getNom() << ": " 
             << stationDestinations << " destinations, "
             << stationLouages << " louages\n";
    }
    
    cout << "✅ TOTAL: " << totalStations << " stations, " 
         << totalDestinations << " destinations, " 
         << totalLouages << " louages\n";
}

// ======= NEW METHODS FOR WEB INTERFACE =======

vector<string> System::getAllStationNames() const {
    vector<string> names;
    for (const auto& station : stations) {
        names.push_back(station.getNom());
    }
    return names;
}

vector<string> System::getDestinationsFromStation(const string& stationName) const {
    vector<string> destinations;
    const Station* station = findStation(stationName);
    if (station) {
        for (const auto& dest : station->getDestinations()) {
            destinations.push_back(dest.getNom());
        }
    }
    return destinations;
}

vector<Louage> System::searchLouages(const string& fromStation, const string& toDestination) {
    vector<Louage> results;
    
    Station* station = findStation(fromStation);
    if (!station) {
        cout << "🔍 Recherche: Station '" << fromStation << "' non trouvée" << endl;
        return results;
    }
    
    Destination* dest = findDestination(toDestination, fromStation);
    if (!dest) {
        cout << "🔍 Recherche: Destination '" << toDestination << "' non trouvée depuis '" << fromStation << "'" << endl;
        return results;
    }
    
    const vector<Louage>& louages = dest->getLouages();
    results.insert(results.end(), louages.begin(), louages.end());
    
    cout << "🔍 Recherche: " << results.size() << " louage(s) trouvé(s) pour " 
         << fromStation << " → " << toDestination << endl;
    
    return results;
}