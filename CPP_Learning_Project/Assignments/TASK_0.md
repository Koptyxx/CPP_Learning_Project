# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ?
Comment faire pour quitter le programme ?
A quoi sert la touche 'F' ?

####Réponse :

La fonction responsable de gérer les input du programme est create_keystrokes.  
Afin d'ajouter un avion, on appuie sur la touche C.  
Pour quitter le programme, on appuie sur la touche Q.  
La touche F sert à passer en plein écran.

Ajoutez un avion à la simulation et attendez.
Quel est le comportement de l'avion ?
Quelles informations s'affichent dans la console ?

####Réponse : 

L'avion atterrit sur la piste d'atterrissage, roule jusqu'à atteindre un terminal, effectue un arrêt à celui-ci puis repart.  
Dans la console, nous avons les différents états de l'avion qui sont affichés (landing, servicing, done servicing et lift off).


Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?

####Réponse :

Lorsque l'on ajoute 4 avions, les 3 premiers ont un comportement normal tandis que le quatrième
"attends son tour", plus précisément, il attend qu'un terminal se libère puisque les trois présents
dans l'aéroport sont déjà occupés par les trois premiers avions.
Lorsqu'un terminal se libère, celui-ci accueille le dernier avion.

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.

####Réponse :

Dans le dossier src, nous avons les classes suivantes :

-aircraft  
Cette classe permet de représenter un avion avec ses différentes caractéristiques
(son numéro de vol, sa position, sa vitesse, son chemin, etc)

-airport  
Cette classe permet de représenter un aéroport.

-airport type  
Cette classe contient les positions des terminaux.

-tower
Cette classe permet de représenter une tour de contrôle.

-terminal  
Cette classe permet de représenter un terminal à l'aide de ses caractéristiques
(sa position, l'aéroport dans lequel il se trouve et son état (en service ou non)).

-waypoint  
Cette classe permet de représenter les différents points de cheminement d'un avion.


Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes interagissent ensemble.

####Réponse :

Tower :

- void arrived_at_terminal(const Aircraft& aircraft)
- WaypointQueue get_instructions(Aircraft& aircraft)

Aircraft :

- const std::string& get_flight_num()
- float distance_to(const Point3D& p)
- void display()
- void move()

Airport :

- Tower& get_tower()
- void display()
- void move()

Terminal :

- bool in_use()
- bool is_servicing()
- void assign_craft(const Aircraft& aircraft)
- void start_service(const Aircraft& aircraft)
- void finish_service()
- void move()

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez les intérêts de ce choix.

####Réponse :

Le cheminement d'un avion se fait via notre Tower (plus particulièrement get_instructions) ainsi
que la fonction move d'Aircraft.

Le conteneur de la librairie standard pour la représentation du cheminement
est std::deque. Ce choix s'explique par la capacité d'une deque à pouvoir insérer au début et à la fin de notre queue.

## C- Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.

####Réponse : 

Les vitesses maximales et accélérations de chaque avion sont définis à la fin du fichier aircraft_type (par les variables max_air_speed et max_accel).
Nous avons défini la vitesse aérienne du concorde à 0.8 (contre 0.5 pour le reste).

2) Identifiez quelle variable contrôle le framerate de la simulation.
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.

####Réponse :

La variable qui contrôle la framerate est ticks_per_sec se trouvant dans l'interface opengl.
Nos deux inputs se font avec les touches 'k' et 'l' qui permettent respectivement d'augmenter et diminuer la rate.  
Lorsque l'on essaie de mettre en is_paused notre (donc en la diminuant), le programme segfault dès lors
que l'on tombe en dessous d'une certaine rate.  
Pour la fonctionnalité permettant de mettre en pause notre programme, nous avons créé un booléen is_paused qui est initialisé à false.
En appuyant sur la touche 'p', le programme change cette variable en son contraire.
La boucle principale de notre programme se lance uniquement si notre variable est à false.

3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.

####Réponse : 

La variable qui contrôle le temps de débarquement des avions est SERVICE_CYCLE se trouvant le fichier config.hpp.

4) Lorsqu'un avion a décollé, il réatterrit peu de temps après.
Faites en sorte qu'à la place, il soit retiré du programme.\
Indices :\
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?\
Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?
A quel endroit de la callstack pourriez-vous le faire à la place ?\
Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.
Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour `DynamicObject` ?

####Réponse :

Afin de gérer cela, nous mettons notre display_queue en inline static
ce qui permet d'y avoir accès dans les autres classes.  
Cela n'est pas très pertinent d'en faire de même pour un DisplayableObject car nous n'avons ici pas un vector mais un set.

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.

####Réponse :

Ici, afin de ne plus avoir un temps linéaire, nous allons utiliser le conteneur STL map ayant comme couple clé valeur
toujours un avion et son terminal.

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?

####Réponse :

De manière générale, pour que seule une classe ait accès à un objet,
celui-ci doit se trouver dans la partie privée de la classe.
C'est ce que nous faisons ici en mettant la map AircraftToTerminal dans la partie privée de la classe Tower.

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference ?
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?

####Réponse :

Passer par une référence implique qu'elle
peut être modifiée pendant l'exécution du programme, chose que nous ne voulons pas.
La copie ici est un moyen sûr de ne pas changer de direction en pleine exécution, il n'y a donc pas d'autre moyen.


## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.
