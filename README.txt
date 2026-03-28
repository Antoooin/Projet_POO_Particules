# Simulation de dynamique moléculaire 2D

Simulation de N particules en interaction dans une boîte carrée avec conditions
aux limites périodiques. Deux potentiels disponibles (Lennard-Jones, gaz parfait)
et trois intégrateurs temporels (Velocity Verlet, Euler, RK4).
/!\ Les intégrateurs d'Euler et RK4 n'ont été introduits que dans un but comparatif
avec celui de Velocity Verlet. Par soucis de vitesse de calcul et de conservation
de l'énergie, il ne FAUT PAS les utiliser en pratique pour une simulation efficace /!\

---

## Compilation

g++ Boundary.cpp Config.cpp Energy.cpp Euler.cpp Generator.cpp IdealGas.cpp \
    LennardJones.cpp RDF.cpp RK4.cpp Simulation.cpp VelocityVerlet.cpp main.cpp \
    -O2 -std=c++17 -o main

Options de compilation :
  -O2   optimisation standard (recommandée)
  -O3   optimisation agressive — peut modifier l'ordre des opérations flottantes,
        à éviter pour des tests de conservation de l'énergie

## Utilisation

Modifier config.txt puis lancer :
  ./main          (Linux/Mac)
  main.exe        (Windows)

config.txt doit se trouver dans le même répertoire que l'exécutable.

---

## Paramètres (config.txt)

Clé            | Défaut | Description
---------------|--------|--------------------------------------------
N              | 500    | Nombre de particules. N=100 : gaz, N=300 : liquide, N=400 : solide (LJ)
boxSize        | 20.0   | Côté de la boîte carrée
dt             | 0.0005 | Pas de temps (ne pas dépasser ~0.001 pour LJ)
steps          | 10000  | Nombre de pas de simulation
initVelocity   | 0.2    | Amplitude max des vitesses initiales
potential      | LJ     | "LJ" (Lennard-Jones) ou "NONE" (gaz parfait)
integrator     | VV     | "VV", "Euler" ou "RK4"
epsilon        | 1.0    | Profondeur du puits LJ
sigma          | 1.0    | Diamètre effectif des particules
cutoff         | 2.5    | Rayon de troncature du potentiel (unités absolues)
rdfBins        | 100    | Nombre de bins de la RDF
rdfStart       | 2000   | Step à partir duquel la RDF est échantillonnée

Les commentaires sont supportés en début de ligne (#) et en fin de ligne (//).

---

## Fichiers de sortie

Fichier           | Contenu
------------------|-----------------------------------------------
positions.csv     | Une ligne par step : x0,y0,x1,y1,...,xN,yN
velocities.csv    | Même format pour les vitesses
energies.csv      | step, Ek, Ep, Etot, dE_rel (dérive relative)
rdf.csv           | r, g(r) — fonction de distribution radiale

---

## Structure du code

Fichier                | Rôle
-----------------------|---------------------------------------------
Vec2.h                 | Vecteur 2D (opérateurs, normes, produit scalaire)
Particle.h             | Structure particule (position, vitesse, force, masse)
System.h               | Conteneur de particules + géométrie de la boîte
Config.h / .cpp        | Chargement de config.txt
Generator.h / .cpp     | Initialisation sur grille + vitesses aléatoires
Potential.h            | Interface abstraite pour les potentiels
IdealGas.h / .cpp      | Gaz parfait (forces nulles)
LennardJones.h / .cpp  | Potentiel LJ tronqué (forces + énergie)
Energy.h / .cpp        | Énergie cinétique (+ potentiel LJ autonome)
Integrator.h           | Interface abstraite pour les intégrateurs
Euler.h / .cpp         | Intégrateur Euler explicite (ordre 1)
VelocityVerlet.h / .cpp| Intégrateur Velocity Verlet (ordre 2, symplectique)
RK4.h / .cpp           | Intégrateur Runge-Kutta ordre 4
Boundary.h / .cpp      | Conditions aux limites périodiques
RDF.h / .cpp           | Fonction de distribution radiale g(r)
Simulation.h / .cpp    | Boucle principale (intégration + PBC)
main.cpp               | Point d'entrée, I/O, boucle de simulation

---

## Notes physiques

- Densité réduite recommandée : ρ* = N/L² ∈ [0.3, 1.0] pour LJ 2D
  Au-delà de ρ* ≈ 1.2 (close-packing 2D), des overlaps sévères à l'initialisation
  peuvent provoquer une explosion numérique dès les premiers steps.

- Le premier pic de la RDF doit apparaître autour de r_min = 2^(1/6) σ ≈ 1.122
  (minimum du potentiel LJ isolé). Un décalage vers les petits r indique que
  la densité est trop élevée ou que rdfStart est insuffisant.

- Velocity Verlet est l'intégrateur recommandé : même coût qu'Euler, stable
  indéfiniment grâce à sa structure symplectique. Euler et RK4 divergent
  sur des temps longs ou à haute densité.