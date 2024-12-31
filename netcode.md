paquets envoyés {
- header
- cells pour chaque joueur (220 int)
- pieces pour chaque joueur (5 int * nombre de pieces)
- input (1 int)
- statut des autres joueurs (?)\
} -> 7232 bits / paquet

objectif : 256 kbits/s par client à 10 paquets/s
    -> 25600 bits / paquet max 

verrouiller le simulation rate à 60 fps
