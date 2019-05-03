# Compilation étape par étape

1) Pour compiler le projet: make ou make build

2) Pour installer le projet: make install

3) ./server ./install/$(PLAYER1).so ./install/$(PLAYER2).so -n <value> -o

$(PLAYER 1) : nom de la bibliothèque dynamique du premier joueur
$(PLAYER 1) : nom de la bibliothèque dynamique du premier joueur
-n <value>: paramètre optionnel pour choisir la taille de la grille (par défaut n=5)
-o        : lancer la partie en mode SWAP (par défaut le mode est standard)


# Simuler une partie

1) Lancer une partie avec les paramètres par défaut: make play

# Tests

1) Lancer les tests: make test


# Nettoyer les répertoires

1) Nettoyage des binaires et fichiers temporaires: make clean
