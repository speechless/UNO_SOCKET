Jeu UNO, Thomas VANDERBAUWEDE & Alexis CICHORACKI

- Initialisation
Utiliser make pour compiler (make debug pour plus d'informations)

Lancer bin/serveurUNO dans un terminal
Lancer autant de fois que souhaité bin/clientUNO (dans des terminaux différents)

- Fonctionnement du programme
Choisir l'option souhaitée dans le menu client
La partie publique se lance automatiquement lorsque 2 joueurs ont rejoint.
La partie privée se crée avec un hébergeur et des joueurs.
La partie privée se lance quand le nombre entré initialement par l'hébergeur est atteint.

Dans des cas rares, il se peut qu'un seul client reçoive le signal de démarrage (une seule déconnexion visible du côté serveur et partie non lancée). Cela arrive quand l'un des clients reçoit sa requête avant de se mettre à écouter. Cela se produit souvent avec des PC + lents.

- Déroulé du jeu
Les joueurs choisissent une carte de leur main pour la poser sur la pile de cartes. Il faut que soit la couleur, soit le symbole corresponde pour pouvoir poser. Sinon il faut piocher.
La carte changement de couleur est un joker qui peut être posé sur n'importe quelle carte.
La partie se joue jusqu'à ce qu'un joueur n'a plus de cartes.

- Quelques aspects techniques
Lors de la création d'une partie, le joueur qui se connecte en premier crée un salon chez le serveur.
Une fois les conditions remplies pour lancer la partie, le serveur donne au premier joueur arrivé le rôle de host.
L'host est l'hébergeur de la partie et cela ne nécessite donc plus de communiquer avec le serveur lors du déroulement de la partie.
Cela évite de surcharger le serveur.