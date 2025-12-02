# Dockerfile - Pour Render.com
FROM ubuntu:22.04

# Mise à jour et installation des dépendances
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Créer le répertoire de travail
WORKDIR /app

# Copier tous les fichiers
COPY . .

# Compiler avec CMake
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Exposer le port (Render utilise 10000)
EXPOSE 10000

# Commande de démarrage
CMD ["./build/server"]