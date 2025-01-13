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
      - taille (1 byte) : 21 octets ou 0x15
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
}

paquet classic game client->server {
    - header : 
      - tete (1 byte) : OBLIGATOIRE 0xD4
      - type (1 byte) : 0x03
      - version (1 byte) : 0x01
      - taille (1 byte) : 128+5*nbpieces bytes
    - sequence number (uint32_t : 4 bytes) / frame number
    - input (1 byte)
    - score (uint32_t : 4 bytes)
    - level (1 byte)
    - grid (gridsize / 2 bytes (1 case = uint4_t pour 16 valeurs max))
    - pieces : 5 uint8_t * nb pieces
} ~1200 bits/paquet -> 213 paquets/seconde max

paquet classic game server->client {
    - header : 
      - tete (1 byte) : OBLIGATOIRE 0xD4
      - type (1 byte) : 0x04
      - version (1 byte) : 0x01
      - taille (1 byte) : 4 + i * 115 + (5 * nb_pieces[i]) * i
    - sequence number (uint32_t : 4 bytes) / frame number
    - for each player i : 105+5*nb_pieces 
      - score (uint32_t : 4 bytes)
      - level (1 byte) 
      - grid (gridsize / 2 bytes)
      - pieces : 5*uint8_t * nb_pieces
} ~2000 bits/paquet pour 2 joueurs -> 128 paquets/seconde max

/ ! \ IL FAUT MODIFIER LE CODE POUR INCLURE LE LIFETIME DE LA PIECE 
DANS SON STRUCT POUR POUVOIR LE TRANSMETTRE