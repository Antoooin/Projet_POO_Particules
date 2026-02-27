////////////////////////////////////////////////////////
Comment utiliser le programme ?

Commande de compilation : 
g++ Boundary.cpp Config.cpp Energy.cpp Generator.cpp IdealGas.cpp LennardJones.cpp main.cpp RDF.cpp Simulation.cpp VelocityVerlet.cpp main.cpp -O2 -o main.exe

(le -O2 permet d'optimiser la compilation, i.e. supprimer le code inutile, optimiser les boucles/accès mémoire etc)
-> On pourra faire des tests de conservation de l'énergie avec -O3 , qui est une optimisation "plus agressive" donc qui risque de changer l'ordre des opérations flottantes etc

ATTENTION !
à bien avoir le fichier Config.txt au même endroit que le reste, sinon ça ne compilera PAS 
