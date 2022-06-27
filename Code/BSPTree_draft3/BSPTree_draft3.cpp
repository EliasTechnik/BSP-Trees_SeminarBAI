// BSPTree_draft3.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <string>
#include "TreeNode.h"
#include "QuadTreeTemplate.h"

//for testing
#include "dList.h"


int main()
{
    //Testing dList
    /*
    dList<int> * list = new dList<int>(500);

    for (int i = 0; i < 1000000; i++) {
        list->addItem(rand() % 1000);
        //std::cout << "added item " << i << " to the list" << std::endl;
    }

    for (unsigned int i = 0; i < list->getItemCount(); i++) {
        int a = list->getItem(i);
        //std::cout << "got item " << a << " from the list" << std::endl;
    }
    delete list;

    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();
    list = new dList<int>(500);


    for (int i = 0; i < 1000000; i++) {
        list->addItem(rand() % 1000);
        //std::cout << "added item " << i << " to the list" << std::endl;
    }
    delete list;
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();
    */
    
    NLPackage<QTLocation, QTOperationBorder> RootLocation;
    RootLocation.opBorder.min_x = 0;
    RootLocation.opBorder.min_y = 0;
    RootLocation.opBorder.max_x = 1000;
    RootLocation.opBorder.max_y = 1000;
    RootLocation.nodeLoc = getQTNodeLocation(RootLocation.opBorder);

    BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4> * tree = new BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>(
        4,
        RootLocation,
        getQTFPackage()
        );

    
    for (int i = 0; i < 500; i++) {
        int number = i;
        PLPackage<int, QTLocation> pl;
        pl.data = i;
        pl.point.x = rand() % 1000;
        pl.point.y = rand() % 1000;
        std::cout << "generated payload " << pl.data << " at (" << pl.point.x << "|" << pl.point.y << ")" << std::endl;
        tree->addPayload(pl);
    }
    delete tree;
    std::cout << "Deleted Tree\n";
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();
    tree = new BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>(
        4,
        RootLocation,
        getQTFPackage()
        );

    for (int i = 0; i < 500; i++) {
        int number = i;
        PLPackage<int, QTLocation> pl;
        pl.data = i;
        pl.point.x = rand() % 1000;
        pl.point.y = rand() % 1000;
        std::cout << "generated payload " << pl.data << " at (" << pl.point.x << "|" << pl.point.y << ")" << std::endl;
        tree->addPayload(pl);
    }
    

   //QuadTreeNode<char> qtn = new QuadTreeNode<char>()
    std::cout << RootLocation.opBorder.min_x << std::endl;

    std::cout << "Hello World!\n";
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
