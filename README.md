# LAN Virtuelle  
**Projet tutoré – 1ʳᵉ année de BUT Informatique**

<p align="center">
  <img src="https://github.com/user-attachments/assets/7d16e94a-de34-4da2-ac8b-6fe92e2d71ee" alt="Illustration du projet" width="400">
</p>

<p align="center"><sub>Illustration : Designed by <a href="https://www.freepik.com/author/macrovector">macrovector</a> / <a href="https://www.freepik.com/">Freepik</a></sub></p>


## Description  
Ce projet a pour objectif de simuler le fonctionnement d’un **réseau local (LAN)** en langage C, en reproduisant les interactions entre **stations** et **commutateurs (switchs)**.  

L’application lit un **fichier de configuration** décrivant les équipements et leurs connexions, construit une représentation interne du réseau sous forme de **graphe**, puis permet de **simuler l’envoi de trames Ethernet** entre différentes machines.  
Les commutateurs apprennent automatiquement les adresses MAC des stations pour gérer la commutation de manière dynamique.

Ce projet a été réalisé pour appliquer les notions de :
- Programmation en langage C  
- Structures de données dynamiques  
- Concepts fondamentaux des réseaux informatiques  

---

## Technologies utilisées  
- **Langage C (ANSI C99)**  
- **Compilation avec GCC**  
- **Gestion dynamique de la mémoire (malloc, free)**  
- **Structures : graphes, tableaux, unions, énumérations**  
- **Lecture/écriture de fichiers (`fopen`, `fgets`, `fclose`)**  
- **Interface console interactive (bibliothèque `<termios>`)**

---

## Installation et lancement  

### 1. Cloner le projet  
```bash
git clone https://github.com/thomasbarseghian/LAN-Virtuelle.git
cd LAN-Virtuelle
```

### 2. Compiler le projet  
```bash
make
```

Assurez-vous que tous les fichiers `.c` et `.h` sont présents dans le même répertoire.

### 3. Créer un fichier de configuration réseau  
Exemple de fichier `mylan.lan.txt` :  
```
4 3
2;01:45:23:a6:f7:ab;8;1024
1;54:d6:a6:82:c5:23;130.79.80.21
1;c8:69:72:5e:43:af;130.79.80.27
1;77:ac:d6:82:12:23;130.79.80.42
0;1;4
0;2;19
0;3;4
```

### 4. Exécuter le programme  
```bash
./main mylan.lan.txt
```

---

## Fonctionnalités  

### Lecture et construction du réseau  
- Lecture d’un fichier de configuration décrivant les équipements et les connexions  
- Vérification du format et du nombre d’éléments  
- Allocation dynamique de la structure du graphe  

### Simulation de communication  
- Sélection des machines émettrice et réceptrice via un menu interactif  
- Envoi de trames Ethernet avec apprentissage automatique des adresses MAC  
- Gestion des réponses de type **ACK** pour valider la réception  
- Affichage complet des étapes de la communication dans la console  

### Fonctions utilitaires  
- Conversion et validation des adresses MAC et IP  
- Affichage des tables de commutation des switchs  
- Détection de boucles réseau et protection anti-récursion  

---

## Cahier des charges  

| Objectif | Détails |
|-----------|----------|
| Lecture du fichier de configuration | Vérifier la syntaxe et extraire les équipements et arêtes |
| Représentation du réseau | Graphe : nœuds = équipements, arêtes = connexions |
| Simulation d’échanges Ethernet | Envoi de trames, apprentissage des adresses MAC, gestion ACK |
| Interface utilisateur | Menu interactif (flèches haut/bas, validation par Entrée) |
| Fiabilité | Vérification des erreurs et gestion de la mémoire dynamique |

---

## Auteurs  
Projet réalisé dans le cadre du **BUT Informatique**  

- **Étudiant :** Thomas Barseghian
- **Binôme :** Abdullah Nezami

---

## Licence  
Projet académique – usage pédagogique uniquement.  
Toute réutilisation à des fins éducatives est autorisée avec mention de la source.  
