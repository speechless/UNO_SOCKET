Utiliser make pour compiler (make debug pour plus d'informations)

Lancer bin/serveurUNO dans un terminal
Lancer autant de fois que souhaité bin/clientUNO (dans des terminaux différents)

Choisir l'option souhaitée dans le menu client (Seule partie publique fonctionne)
La partie se lance automatiquement lorsque 2 joueurs ont rejoint.
Parfois il se peut qu'un seul client reçoive le signal de démarrage (une seule déconnexion visible du côté serveur et partie non lancée). Nous n'avons pas réussi à identifier la raison de ce bug donc il faut relancer LES clients.

Les joueurs choisissent une carte de leur main pour la poser sur la pile de cartes. Il faut que soit la couleur, soit le symbole corresponde pour pouvoir poser. Sinon il faut piocher.
La partie se joue jusqu'à ce qu'un joueur n'a plus de cartes (très long)
