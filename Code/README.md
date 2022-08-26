# Generic Binary Space Partition Tree

In diesem Repository befinden sich zwei Programme. Im Ordner ```Generic BSP Tree/``` befindet sich die generische Implementierung auf welche die Schriftliche Ausarbeitung bezug nimmt. Im Ordner ```Quick Demo/``` befindet sich eine interaktive Visualisierung eines Quadtrees. Dieser ist in Object Pascal implementiert und dient nur zu Demonstartionszwecken und sit nicht teil der geforderten Implementierung der Datenstrucktur.

## Ordnerstruktur

    .
    ├── Generic BSP Tree/
    │   ├── BSPTree_draft3.vcxproj    #Visual Studio 2022 Projektdatei
    │   ├── ...
    │   ├── dList.h                   #Implementierung einer Dynamischen Liste (Hilfsklasse für die eigentliche Datenstruktur)
    │   ├── helpers.h                 #Hilfsfunktionen für die Entwicklung (Nicht Bestandteil der Datenstruktur)
    │   ├── OcTreeTemplate.h          #Implementierung der Benutzerdefinierten Funktionen und Strukturen für einen Oc-Tree.
    │   ├── QuadTreeTemplate          #Implementierung der Benutzerdefinierten Funktionen und Strukturen für einen QuadTree.
    │   └── TreeNode.h                #Implementierung des Generischen BSP-Trees.
    └── Quick Demo/
        ├── quick_demo.exe            #Kompiliertes Demoprogram
        ├── quick_demo.lpi            #Lazarus Projekt Datei (zum öffnen des Projektes mittels Lazarus IDE)
        ├── ...
        ├── unit_quadtree.pas          #Unit welche den Quadtree implementiert.
        └── unitmain.pas               #Unit zur Steuerung der GUI