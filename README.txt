Utiliser make pour compiler (make debug pour plus d'informations)

Lancer bin/serveurUNO dans un terminal
Lancer autant de fois que souhaité bin/clientUNO (dans des terminaux différents)

Choisir l'option souhaitée dans le menu client

Une partie publique se lance automatiquement lorsque 2 joueurs ont rejoint.
Une partie privée lorsque le nombre maximum de joueurs est atteint.
Parfois il se peut que tous les clients ne reçoivent pas le signal de démarrage (tous les clients ne se déconnectent pas du côté serveur et partie non lancée). Nous n'avons pas réussi à identifier la raison de ce bug donc il faut relancer LES clients.

Les joueurs choisissent une carte de leur main pour la poser sur la pile de cartes. Il faut que soit la couleur, soit le symbole corresponde pour pouvoir poser. Sinon il faut piocher.
La partie se joue jusqu'à ce qu'un joueur n'a plus de cartes (très long)
