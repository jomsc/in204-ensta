# in204-ensta
### Projet IN204 de Simon DROUET et Joseph MOUSCADET
Pour exécuter le code, clonez le repo, et exécutez :
```mkdir build
cd build
cmake ..
make && bin/TETRIS_RIVALS```


## To-Do List
- [ ] netcode
- [ ] score
- [ ] niveau
- [ ] clean UI 
- [ ] menu principal :
  - [ ] un seul joueur:
    - [ ] mode classique
  - [ ] multijoueur en LAN :
    - [ ] mode classique 
    - [ ] mode personnage
  - [ ] bouton quit
- [ ] musique

## Partie Joseph
- rendre atomic le nombre de joueurs
- pourquoi le serveur crash quand un joueur en trop cherche a rejoindre ou que le pseudo est identique
[state-synchro](https://gafferongames.com/post/state_synchronization/)

## Partie Simon
- [ ] Finir le jeu solo :
  - [x] mettre le score
  - [x] rotation antihoraire
  - [x] afficher les 3 prochaines pièces
  - [x] améliorer la gestion des inputs
  - [ ] textures
  - [x] augmentation du niveau
  - [ ] condition de perte / ecran de défaite







#### Par Simon Drouet et Joseph Mouscadet
