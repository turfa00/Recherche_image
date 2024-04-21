# Mise en correspondance d'images parallèle

La mise en correspondance d'images est une tâche fondamentale dans de nombreux domaines.\
L'objectif de ce projet est de développer un système de mise en correspondance d'images robuste en utilisant l'algorithme de somme de différences au carré (SSD).\
Cependant, la complexité computationnelle de cet algorithme peut être élevée, en particulier pour de grandes images ou un grand nombre de correspondances.\
Donc ce programme est parallélisé avec OpenMp pour accélérer le calcul en répartissant la charge de travail entre plusieurs threads.

### Exécution
Pour l'éxécution du programme principale, il suffit d'exécuter avec l'ajout de flags -fopenmp pour le support d'OpenMp.\
Par exemple: gcc main.c -o main -fopenmp\
Le programme principal prends deux arguments dont l'image d'entrée et l'image de recherche. S'il n'y a pas d'erreur, en sortie, vous auriez l'image avec une boite rouge qui encercle\
là où l'image à été trouvé.
