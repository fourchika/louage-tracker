#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstring>
#include "config.h"

// Include YOUR existing classes
#include "../include/System.h"
#include "../include/Station.h"
#include "../include/Destination.h"
#include "../include/Louage.h"

using namespace std;

// Helper function to escape HTML
string escapeHTML(const string& data) {
    string buffer;
    buffer.reserve(data.size());
    for(size_t pos = 0; pos != data.size(); ++pos) {
        switch(data[pos]) {
            case '&':  buffer.append("&amp;");  break;
            case '\"': buffer.append("&quot;"); break;
            case '\'': buffer.append("&apos;"); break;
            case '<':  buffer.append("&lt;");   break;
            case '>':  buffer.append("&gt;");   break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }
    return buffer;
}

// Generate search form HTML that uses C++ data
string generateSearchForm(const System& system) {
    stringstream ss;
    
    // GET REAL DATA FROM C++
    vector<string> stations;
    try {
        stations = system.getAllStationNames();
        // TRIER les stations par ordre alphabétique
        std::sort(stations.begin(), stations.end());
        if (stations.empty()) {
            stations = {"Gabès", "Medenine", "Sfax", "Sousse", "Tunis"};  // Déjà triées
        }
    } catch (const exception& e) {
        stations = {"Gabès", "Medenine", "Sfax", "Sousse", "Tunis"};  // Déjà triées
    }
    
    ss << "    <div class=\"search-container\">\n";
    ss << "        <h1><i class=\"fas fa-bus\"></i> Louage Tracker Tunisie</h1>\n";
    ss << "        <p class=\"subtitle\">Recherchez des louages entre les stations tunisiennes</p>\n";
    ss << "        \n";
    ss << "        <div class=\"search-form-container\">\n";
    ss << "            <form id=\"searchForm\" onsubmit=\"return performSearch()\">\n";
    ss << "                <div class=\"form-row\">\n";
    ss << "                    <div class=\"form-group\">\n";
    ss << "                        <label for=\"fromStation\"><i class=\"fas fa-map-pin\"></i> Station de depart:</label>\n";
    ss << "                        <select id=\"fromStation\" name=\"from\" class=\"form-select\" required>\n";
    ss << "                            <option value=\"\">-- Selectionnez une station --</option>\n";
    
    // Add REAL stations from C++ (déjà triées)
    for (const auto& station : stations) {
        ss << "                            <option value=\"" << escapeHTML(station) << "\">" << escapeHTML(station) << "</option>\n";
    }
    
    ss << "                        </select>\n";
    ss << "                    </div>\n";
    ss << "                    \n";
    ss << "                    <div class=\"form-group\">\n";
    ss << "                        <label for=\"toDestination\"><i class=\"fas fa-flag-checkered\"></i> Destination:</label>\n";
    ss << "                        <select id=\"toDestination\" name=\"to\" class=\"form-select\" required>\n";
    ss << "                            <option value=\"\">-- Selectionnez d'abord le depart --</option>\n";
    ss << "                        </select>\n";
    ss << "                    </div>\n";
    ss << "                    \n";
    ss << "                    <div class=\"form-group\">\n";
    ss << "                        <label>&nbsp;</label>\n";
    ss << "                        <button type=\"submit\" class=\"btn-search\">\n";
    ss << "                            <i class=\"fas fa-search\"></i> Rechercher\n";
    ss << "                        </button>\n";
    ss << "                    </div>\n";
    ss << "                </div>\n";
    ss << "            </form>\n";
    ss << "            \n";
    ss << "            <div id=\"searchResults\" class=\"search-results\">\n";
    ss << "                <div class=\"info-message\">\n";
    ss << "                    <i class=\"fas fa-info-circle\"></i>\n";
    ss << "                    <p>Selectionnez une station et une destination pour rechercher des louages.</p>\n";
    ss << "                </div>\n";
    ss << "            </div>\n";
    ss << "        </div>\n";
    ss << "    </div>\n";
    
    // SIMPLE FIXED JAVASCRIPT
    ss << "    \n";
    ss << "    <script>\n";
    ss << "    // Simple function to update destinations\n";
    ss << "    document.getElementById('fromStation').addEventListener('change', function() {\n";
    ss << "        const fromStation = this.value;\n";
    ss << "        const toSelect = document.getElementById('toDestination');\n";
    ss << "        \n";
    ss << "        if (!fromStation) {\n";
    ss << "            toSelect.innerHTML = '<option value=\"\">-- Selectionnez d\\'abord le depart --</option>';\n";
    ss << "            toSelect.disabled = true;\n";
    ss << "            return;\n";
    ss << "        }\n";
    ss << "        \n";
    ss << "        // Show loading\n";
    ss << "        toSelect.innerHTML = '<option value=\"\">-- Chargement... --</option>';\n";
    ss << "        toSelect.disabled = true;\n";
    ss << "        \n";
    ss << "        // Get destinations from C++ backend\n";
    ss << "        fetch('/api/destinations?station=' + encodeURIComponent(fromStation))\n";
    ss << "            .then(response => {\n";
    ss << "                if (!response.ok) throw new Error('Erreur réseau: ' + response.status);\n";
    ss << "                return response.json();\n";
    ss << "            })\n";
    ss << "            .then(data => {\n";
    ss << "                console.log('Destinations reçues:', data);\n";
    ss << "                toSelect.innerHTML = '<option value=\"\">-- Selectionnez une destination --</option>';\n";
    ss << "                if (data.destinations && data.destinations.length > 0) {\n";
    ss << "                    // Enlever les doublons ET TRIER par ordre alphabétique\n";
    ss << "                    const uniqueDests = [...new Set(data.destinations)].sort();\n";
    ss << "                    uniqueDests.forEach(dest => {\n";
    ss << "                        const option = document.createElement('option');\n";
    ss << "                        option.value = dest;\n";
    ss << "                        option.textContent = dest;\n";
    ss << "                        toSelect.appendChild(option);\n";
    ss << "                    });\n";
    ss << "                    toSelect.disabled = false;\n";
    ss << "                    \n";
    ss << "                    // Update results area\n";
    ss << "                    document.getElementById('searchResults').innerHTML = `\n";
    ss << "                        <div class=\"info-message\">\n";
    ss << "                            <i class=\"fas fa-check\"></i>\n";
    ss << "                            <p>${uniqueDests.length} destinations chargées pour ${fromStation}</p>\n";
    ss << "                            <p><small>Sélectionnez une destination et cliquez sur Rechercher</small></p>\n";
    ss << "                        </div>\n";
    ss << "                    `;\n";
    ss << "                } else {\n";
    ss << "                    toSelect.innerHTML = '<option value=\"\">-- Aucune destination disponible --</option>';\n";
    ss << "                    document.getElementById('searchResults').innerHTML = `\n";
    ss << "                        <div class=\"info-message\">\n";
    ss << "                            <i class=\"fas fa-exclamation-triangle\"></i>\n";
    ss << "                            <p>Aucune destination disponible depuis ${fromStation}</p>\n";
    ss << "                        </div>\n";
    ss << "                    `;\n";
    ss << "                }\n";
    ss << "            })\n";
    ss << "            .catch(error => {\n";
    ss << "                console.error('Error:', error);\n";
    ss << "                toSelect.innerHTML = '<option value=\"\">-- Erreur de chargement --</option>';\n";
    ss << "                document.getElementById('searchResults').innerHTML = `\n";
    ss << "                    <div class=\"info-message\">\n";
    ss << "                        <i class=\"fas fa-exclamation-triangle\"></i>\n";
    ss << "                        <p>Erreur de chargement des destinations: ${error.message}</p>\n";
    ss << "                    </div>\n";
    ss << "                `;\n";
    ss << "            });\n";
    ss << "    });\n";
    ss << "    \n";
    ss << "    // Perform search\n";
    ss << "    function performSearch() {\n";
    ss << "        const fromStation = document.getElementById('fromStation').value;\n";
    ss << "        const toDestination = document.getElementById('toDestination').value;\n";
    ss << "        \n";
    ss << "        if (!fromStation || !toDestination) {\n";
    ss << "            alert('Veuillez sélectionner une station de départ et une destination');\n";
    ss << "            return false;\n";
    ss << "        }\n";
    ss << "        \n";
    ss << "        // Show loading\n";
    ss << "        document.getElementById('searchResults').innerHTML = `\n";
    ss << "            <div class=\"loading\">\n";
    ss << "                <i class=\"fas fa-spinner fa-spin\"></i>\n";
    ss << "                <p>Recherche en cours... ${fromStation} → ${toDestination}</p>\n";
    ss << "            </div>\n";
    ss << "        `;\n";
    ss << "        \n";
    ss << "        // Search\n";
    ss << "        fetch('/api/search?from=' + encodeURIComponent(fromStation) + '&to=' + encodeURIComponent(toDestination))\n";
    ss << "            .then(response => {\n";
    ss << "                if (!response.ok) throw new Error('Erreur réseau: ' + response.status);\n";
    ss << "                return response.json();\n";
    ss << "            })\n";
    ss << "            .then(data => {\n";
    ss << "                console.log('Résultats reçus:', data);\n";
    ss << "                if (data.louages && data.louages.length > 0) {\n";
    ss << "                    let html = `<h3>${data.louages.length} Louage(s) trouvé(s) pour ${fromStation} → ${toDestination}</h3>`;\n";
    ss << "                    data.louages.forEach(louage => {\n";
    ss << "                        html += `\n";
    ss << "                            <div class=\"result-card\">\n";
    ss << "                                <h4><i class=\"fas fa-bus\"></i> Louage #${louage.numero}</h4>\n";
    ss << "                                <p><strong>${louage.depart} → ${louage.destination}</strong></p>\n";
    ss << "                                <p>Série: ${louage.serie} | Propriétaire: #${louage.proprietaire}</p>\n";
    ss << "                            </div>\n";
    ss << "                        `;\n";
    ss << "                    });\n";
    ss << "                    document.getElementById('searchResults').innerHTML = html;\n";
    ss << "                } else {\n";
    ss << "                    document.getElementById('searchResults').innerHTML = `\n";
    ss << "                        <div class=\"info-message\">\n";
    ss << "                            <i class=\"fas fa-info-circle\"></i>\n";
    ss << "                            <p>Aucun louage trouvé pour ${fromStation} → ${toDestination}</p>\n";
    ss << "                        </div>\n";
    ss << "                    `;\n";
    ss << "                }\n";
    ss << "            })\n";
    ss << "            .catch(error => {\n";
    ss << "                document.getElementById('searchResults').innerHTML = `\n";
    ss << "                    <div class=\"info-message\">\n";
    ss << "                        <i class=\"fas fa-exclamation-triangle\"></i>\n";
    ss << "                        <p>Erreur de recherche: ${error.message}</p>\n";
    ss << "                    </div>\n";
    ss << "                `;\n";
    ss << "            });\n";
    ss << "        \n";
    ss << "        return false;\n";
    ss << "    }\n";
    ss << "    </script>\n";
    
    return ss.str();
}

// Main HTML template
string getFullHTML(const System& system) {
    stringstream html;
    
    html << "<!DOCTYPE html>\n";
    html << "<html lang=\"fr\">\n";
    html << "<head>\n";
    html << "    <meta charset=\"UTF-8\">\n";
    html << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "    <title>🚍 Louage Tracker Tunisie</title>\n";
    html << "    <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css\">\n";
    html << "    <style>\n";
    html << "        * {\n";
    html << "            margin: 0;\n";
    html << "            padding: 0;\n";
    html << "            box-sizing: border-box;\n";
    html << "        }\n";
    html << "\n";
    html << "        body {\n";
    html << "            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n";
    html << "            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);\n";
    html << "            color: #333;\n";
    html << "            line-height: 1.6;\n";
    html << "            min-height: 100vh;\n";
    html << "            display: flex;\n";
    html << "            justify-content: center;\n";
    html << "            align-items: center;\n";
    html << "            padding: 20px;\n";
    html << "        }\n";
    html << "\n";
    html << "        .search-container {\n";
    html << "            background: white;\n";
    html << "            border-radius: 15px;\n";
    html << "            padding: 40px;\n";
    html << "            box-shadow: 0 20px 40px rgba(0,0,0,0.1);\n";
    html << "            max-width: 800px;\n";
    html << "            width: 100%;\n";
    html << "        }\n";
    html << "\n";
    html << "        .search-container h1 {\n";
    html << "            text-align: center;\n";
    html << "            color: #333;\n";
    html << "            margin-bottom: 10px;\n";
    html << "            display: flex;\n";
    html << "            align-items: center;\n";
    html << "            justify-content: center;\n";
    html << "            gap: 15px;\n";
    html << "        }\n";
    html << "\n";
    html << "        .search-container h1 i {\n";
    html << "            color: #667eea;\n";
    html << "        }\n";
    html << "\n";
    html << "        .subtitle {\n";
    html << "            text-align: center;\n";
    html << "            color: #666;\n";
    html << "            margin-bottom: 30px;\n";
    html << "            font-size: 1.1rem;\n";
    html << "        }\n";
    html << "\n";
    html << "        /* Search form styles */\n";
    html << "        .search-form-container {\n";
    html << "            margin-top: 20px;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .form-row {\n";
    html << "            display: grid;\n";
    html << "            grid-template-columns: 1fr 1fr auto;\n";
    html << "            gap: 15px;\n";
    html << "            align-items: end;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .form-group {\n";
    html << "            display: flex;\n";
    html << "            flex-direction: column;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .form-group label {\n";
    html << "            margin-bottom: 8px;\n";
    html << "            font-weight: 600;\n";
    html << "            color: #555;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .form-select {\n";
    html << "            padding: 12px 15px;\n";
    html << "            border: 2px solid #e0e0e0;\n";
    html << "            border-radius: 8px;\n";
    html << "            font-size: 16px;\n";
    html << "            background: white;\n";
    html << "            transition: border-color 0.3s ease;\n";
    html << "        }\n";
    html << "\n";
    html << "        .form-select:focus {\n";
    html << "            border-color: #667eea;\n";
    html << "            outline: none;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .btn-search {\n";
    html << "            padding: 14px 30px;\n";
    html << "            background: linear-gradient(135deg, #667eea, #764ba2);\n";
    html << "            color: white;\n";
    html << "            border: none;\n";
    html << "            border-radius: 8px;\n";
    html << "            font-size: 16px;\n";
    html << "            font-weight: 600;\n";
    html << "            cursor: pointer;\n";
    html << "            display: flex;\n";
    html << "            align-items: center;\n";
    html << "            justify-content: center;\n";
    html << "            gap: 8px;\n";
    html << "            transition: all 0.3s ease;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .btn-search:hover {\n";
    html << "            transform: translateY(-2px);\n";
    html << "            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);\n";
    html << "        }\n";
    html << "        \n";
    html << "        .search-results {\n";
    html << "            margin-top: 30px;\n";
    html << "            padding: 20px;\n";
    html << "            background: #f8f9fa;\n";
    html << "            border-radius: 10px;\n";
    html << "            min-height: 100px;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .info-message {\n";
    html << "            text-align: center;\n";
    html << "            color: #666;\n";
    html << "            padding: 20px;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .info-message i {\n";
    html << "            font-size: 2.5rem;\n";
    html << "            color: #667eea;\n";
    html << "            margin-bottom: 15px;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .result-card {\n";
    html << "            background: white;\n";
    html << "            border: 1px solid #e0e0e0;\n";
    html << "            border-radius: 8px;\n";
    html << "            padding: 20px;\n";
    html << "            margin-bottom: 15px;\n";
    html << "            transition: transform 0.3s ease, box-shadow 0.3s ease;\n";
    html << "        }\n";
    html << "\n";
    html << "        .result-card:hover {\n";
    html << "            transform: translateY(-3px);\n";
    html << "            box-shadow: 0 5px 15px rgba(0,0,0,0.1);\n";
    html << "        }\n";
    html << "        \n";
    html << "        .result-card h4 {\n";
    html << "            color: #1976d2;\n";
    html << "            margin-bottom: 10px;\n";
    html << "            display: flex;\n";
    html << "            align-items: center;\n";
    html << "            gap: 10px;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .result-card h4 i {\n";
    html << "            color: #667eea;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .loading {\n";
    html << "            text-align: center;\n";
    html << "            padding: 30px;\n";
    html << "            color: #667eea;\n";
    html << "        }\n";
    html << "        \n";
    html << "        .loading i {\n";
    html << "            font-size: 2.5rem;\n";
    html << "            margin-bottom: 15px;\n";
    html << "        }\n";
    html << "        \n";
    html << "        /* Responsive */\n";
    html << "        @media (max-width: 768px) {\n";
    html << "            body {\n";
    html << "                padding: 10px;\n";
    html << "            }\n";
    html << "            \n";
    html << "            .search-container {\n";
    html << "                padding: 20px;\n";
    html << "            }\n";
    html << "            \n";
    html << "            .form-row {\n";
    html << "                grid-template-columns: 1fr;\n";
    html << "            }\n";
    html << "        }\n";
    html << "    </style>\n";
    html << "</head>\n";
    html << "<body>\n";
    
    // Add only the search form
    html << generateSearchForm(system);
    
    html << "</body>\n";
    html << "</html>\n";
    
    return html.str();
}

// URL decode function
string urlDecode(const string& str) {
    string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int hex = stoi(str.substr(i + 1, 2), nullptr, 16);
            result += static_cast<char>(hex);
            i += 2;
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

int main() {
    // Create and load your system
    System louageSystem;
    
    try {
        louageSystem.loadFromFile("data.txt");
    } catch (const std::exception& e) {
        cerr << "Error loading data.txt: " << e.what() << endl;
        return 1;
    }
    
    // Linux doesn't need WSADATA initialization
    // Remove Windows-specific networking initialization
    
    // Create server socket
    SOCKET_TYPE serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Failed to create socket" << endl;
        return 1;
    }
    
    // Allow socket reuse
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    
    // Bind to port 10000 (for Render)
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(10000);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Failed to bind to port 10000" << endl;
        CLOSE_SOCKET(serverSocket);
        return 1;
    }
    
    // Listen
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        cerr << "Failed to listen on socket" << endl;
        CLOSE_SOCKET(serverSocket);
        return 1;
    }
    
    cout << "Server: http://localhost:10000" << endl;
    
    // Main server loop
    while (true) {
        SOCKET_TYPE clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            continue;
        }
        
        char buffer[4096];
        int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            string request(buffer);
            
            string response;
            
            // Home page
            if (request.find("GET / ") != string::npos || 
                request.find("GET /index.html") != string::npos ||
                request.find("GET /search") != string::npos) {
                
                response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html; charset=UTF-8\r\n"
                           "\r\n" +
                           getFullHTML(louageSystem);
            }
            // API: Get destinations for a station
            else if (request.find("GET /api/destinations") != string::npos) {
                size_t paramStart = request.find("station=");
                if (paramStart != string::npos) {
                    paramStart += 8;
                    size_t paramEnd = request.find(" ", paramStart);
                    if (paramEnd == string::npos) paramEnd = request.find("HTTP", paramStart);
                    string stationName = request.substr(paramStart, paramEnd - paramStart);
                    
                    stationName = urlDecode(stationName);
                    
                    vector<string> destinations = louageSystem.getDestinationsFromStation(stationName);
                    // TRIER les destinations par ordre alphabétique
                    std::sort(destinations.begin(), destinations.end());
                    
                    stringstream json;
                    json << "HTTP/1.1 200 OK\r\n"
                         << "Content-Type: application/json\r\n"
                         << "Access-Control-Allow-Origin: *\r\n"
                         << "\r\n"
                         << "{\"destinations\": [";
                    
                    for (size_t i = 0; i < destinations.size(); i++) {
                        if (i > 0) json << ",";
                        json << "\"" << escapeHTML(destinations[i]) << "\"";
                    }
                    
                    json << "], \"station\": \"" << escapeHTML(stationName) << "\"}";
                    
                    response = json.str();
                } else {
                    response = "HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\n\r\n{\"error\": \"Missing station parameter\"}";
                }
            }
            // API: Search for louages
            else if (request.find("GET /api/search") != string::npos) {
                string fromStation, toDestination;
                
                size_t fromPos = request.find("from=");
                size_t toPos = request.find("to=");
                
                if (fromPos != string::npos && toPos != string::npos) {
                    fromPos += 5;
                    size_t fromEnd = request.find("&", fromPos);
                    if (fromEnd == string::npos) fromEnd = request.find(" ", fromPos);
                    fromStation = request.substr(fromPos, fromEnd - fromPos);
                    
                    toPos += 3;
                    size_t toEnd = request.find(" ", toPos);
                    toDestination = request.substr(toPos, toEnd - toPos);
                    
                    fromStation = urlDecode(fromStation);
                    toDestination = urlDecode(toDestination);
                    
                    vector<Louage> results = louageSystem.searchLouages(fromStation, toDestination);
                    
                    stringstream json;
                    json << "HTTP/1.1 200 OK\r\n"
                         << "Content-Type: application/json\r\n"
                         << "Access-Control-Allow-Origin: *\r\n"
                         << "\r\n"
                         << "{\n"
                         << "  \"from\": \"" << escapeHTML(fromStation) << "\",\n"
                         << "  \"to\": \"" << escapeHTML(toDestination) << "\",\n"
                         << "  \"louages\": [\n";
                    
                    for (size_t i = 0; i < results.size(); i++) {
                        if (i > 0) json << ",\n";
                        json << "    {\n"
                             << "      \"numero\": " << results[i].getnumero_louage() << ",\n"
                             << "      \"serie\": " << results[i].getSerieVehicule() << ",\n"
                             << "      \"proprietaire\": " << results[i].getIdProp() << ",\n"
                             << "      \"depart\": \"" << escapeHTML(results[i].getDepart()) << "\",\n"
                             << "      \"destination\": \"" << escapeHTML(results[i].getDestination()) << "\"\n"
                             << "    }";
                    }
                    
                    json << "\n  ]\n}";
                    
                    response = json.str();
                } else {
                    response = "HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\n\r\n{\"error\": \"Missing parameters\"}";
                }
            }
            // Favicon
            else if (request.find("GET /favicon.ico") != string::npos) {
                response = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: image/x-icon\r\n"
                    "Content-Length: 0\r\n"
                    "\r\n";
            }
            // 404 - Not found
            else {
                response = 
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/html; charset=UTF-8\r\n"
                    "\r\n"
                    "<!DOCTYPE html>"
                    "<html><head><title>404</title>"
                    "<style>body{font-family:Arial;text-align:center;padding:50px;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);color:white;}</style></head>"
                    "<body>"
                    "<h1>404 - Page non trouvee</h1>"
                    "<a href='/' style='color:white;text-decoration:none;padding:10px 20px;background:rgba(255,255,255,0.2);border-radius:5px;'>Retour</a>"
                    "</body></html>";
            }
            
            // Send response
            send(clientSocket, response.c_str(), response.length(), 0);
            CLOSE_SOCKET(clientSocket);
        }
        
        CLOSE_SOCKET(clientSocket);
    }
    
    CLOSE_SOCKET(serverSocket);
    return 0;
}