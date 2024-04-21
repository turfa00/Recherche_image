# Mise en correspondance d'images parallèle

La mise en correspondance d'images est une tâche fondamentale dans de nombreux domaines.
L'objectif de ce projet est de développer un système de mise en correspondance d'images robuste en utilisant l'algorithme de somme de différences au carré (SSD). 
Cependant, la complexité computationnelle de cet algorithme peut être élevée, en particulier pour de grandes images ou un grand nombre de correspondances. 
Donc ce programme est parallélisé avec openmp pour accélérer le calcul en répartissant la charge de travail entre plusieurs threads.

### Éxécution
Pour l'éxécution du programme principale, il suffit d'éxécuter avec l'ajout de flags -fopenmp pour le support d'OpenMp.
Par exemple: gcc main.c -o main -fopenmp
Le programme principale prends deux arguments dont l'image d'éntrée et l'image de recherche. Si il'y a pas d'erreur, en sortie, vous aurez l'image avec une boite rouge qui encercle
là où l'image à été trouvé.
