paquets jeu envoyés {
- header (tete, type, version, taille)
- cells pour chaque joueur (220 int)
- pieces pour chaque joueur (5 int * nombre de pieces)
- input (1 int)
- vitesse
- etat dse capacites des autres joueurs
- statut des autres joueurs (?)\
} -> 7232 bits / paquet

objectif : 256 kbits/s par client à 10 paquets/s
    -> 25600 bits / paquet max 

verrouiller le simulation rate à 60 fps


sinon, envoyer les CHANGEMENTS

envoyer/recevoir les lignes (mode de jeu classique)

paquet game broadcast {
    - header :
      - tete (1 byte) : OBLIGATOIRE 0xD4
      - type (1 byte) : 0x01
      - version (1 byte) : 0x01
      - taille (1 byte)
    - game name (32 char max)
    - port : uint16
    - nb of players (uint_8)
    - max nb of players (uint_8)
    - is joinable (1 byte)
    - MOTD (64 char max)
}

paquet game join/joined {
    - header :
      - tete (1 byte) : OBLIGATOIRE 0xD4
      - type (1 byte) : 0x02
      - version (1 byte) : 0x01
      - taille (1 byte) : 21 octets ou 0x15 ou 25 octets
    - join request (1 byte): 
      - 0x01 : join requested
      - 0x02 : join accepted 
      - 0x03 : join rejected because of server full
      - 0x04 : join rejected because of pseudo already taken 
      - 0x05 : join rejected because of unspecified reasons
    - player pseudo (char 16 bytes):
      - pseudo wanted on join request
      - pseudo given on join accepted
      - pseudo refused on join refused
    - if join accepted : seed (4 bytes, uint32_t)
}

paquet classic game client->server {
    - header : 
      - tete (1 byte) : OBLIGATOIRE 0xD4
      - type (1 byte) : 0x03
      - version (1 byte) : 0x01
      - taille (1 byte) : 10
    - sequence number (uint32_t : 4 bytes) / frame number
    - input (1 byte)
    - malus (1 byte), pour les autres joueurs, 0 si rien
} 80 bits par paquet

paquet classic game server->client {
    - header : 
      - tete (1 byte) : OBLIGATOIRE 0xD4
      - type (1 byte) : 0x04
      - version (1 byte) : 0x01
      - taille (1 byte) : 233
    - sequence number (uint32_t : 4 bytes) / frame number
    - score (uint32_t : 4 bytes)
    - level (1 byte) 
    - grid (gridsize bytes) 220 
}

paquet classic game server->client je me prends une ligne {
    - header : 
      - tete (1 byte) : OBLIGATOIRE 0xD4
      - type (1 byte) : 0x05
      - version (1 byte) : 0x01
      - taille (1 byte) : 25 octets
    - sequence number (uint32_t : 4 bytes) / frame number
    - malus (1 byte)
    - pseudo du joueur qui envoie (16 octets)
}

paquet game start {
  - header : 
    - tete (1 byte) : OBLIGATOIRE 0xD4
    - type (1 byte) : 0x06
    - version (1 byte) : 0x01
    - taille (1 byte) : 5
  - game start : (1 byte)
    - 1 : game start normal
}

paquet game end {
  - header : 
    - tete (1 byte) : OBLIGATOIRE 0xD4
    - type (1 byte) : 0x07
    - version (1 byte) : 0x01
    - taille (1 byte) : 5
  - game end : (1 byte)
    - 0 : game not ended
    - 1 : you lost due to reaching the top 
    - 2 : you lost for unknown reasons
    - 3 : you WON 
}

/ ! \ IL FAUT MODIFIER LE CODE POUR INCLURE LE LIFETIME DE LA PIECE 
DANS SON STRUCT POUR POUVOIR LE TRANSMETTRE