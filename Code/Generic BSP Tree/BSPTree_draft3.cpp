// BSPTree_draft3.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <string>
#include "TreeNode.h"
#include "QuadTreeTemplate.h"
#include "OcTreeTemplate.h"

//for testing
#include "dList.h"

//compareMethod
bool intCompare(PLPackage<int, QTLocation> a, PLPackage<int, QTLocation> b) {
    return a.data == b.data;
}
 
void memTest(unsigned int count, unsigned int repeats) {
    NLPackage<QTLocation, QTOperationBorder> RootLocation;
    RootLocation.opBorder.min_x = 0;
    RootLocation.opBorder.min_y = 0;
    RootLocation.opBorder.max_x = 1000;
    RootLocation.opBorder.max_y = 1000;
    RootLocation.nodeLoc = getQTNodeLocation(RootLocation.opBorder);

    BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* tree = new BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>(
        4,
        RootLocation,
        getQTFPackage()
        );

    dList<PLPackage<int, QTLocation>>* plist = new dList<PLPackage<int, QTLocation>>(count);

    //create test data dataset
    for (unsigned int i = 0; i < count; i++) {
        int number = i;
        PLPackage<int, QTLocation> pl;
        pl.data = i;
        pl.point.x = rand() % 1000;
        pl.point.y = rand() % 1000;
        //std::cout << "generated payload " << pl.data << " at (" << pl.point.x << "|" << pl.point.y << ")" << std::endl;
        plist->addItem(pl);
    }

    
    for (unsigned int j = 0; j < repeats; j++) {
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.get();
        tree = new BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>(
            4,
            RootLocation,
            getQTFPackage()
            );
        for (unsigned int i = 0; i < count; i++) {
            PLPackage<int, QTLocation> pl = plist->getItem(i);
            //std::cout << "generated payload " << pl.data << " at (" << pl.point.x << "|" << pl.point.y << ")" << std::endl;
            tree->addPayload(pl);
        }
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.get();
        delete tree;
    }
}

void deleteAndCleanupTest() {
    NLPackage<QTLocation, QTOperationBorder> RootLocation;
    RootLocation.opBorder.min_x = 0;
    RootLocation.opBorder.min_y = 0;
    RootLocation.opBorder.max_x = 1000;
    RootLocation.opBorder.max_y = 1000;
    RootLocation.nodeLoc = getQTNodeLocation(RootLocation.opBorder);

    BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* tree = new BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>(
        4,
        RootLocation,
        getQTFPackage()
        );

    dList<PLPackage<int, QTLocation>>* plist = new dList<PLPackage<int, QTLocation>>(5);
    dList<PLPackage<int, QTLocation>>* clist = new dList<PLPackage<int, QTLocation>>(5);
    //create custom data dataset
    PLPackage<int, QTLocation> pl;
    pl.data = 0;
    pl.point.x = 200;
    pl.point.y = 200;
    clist->addItem(pl);
    pl.data = 1;
    pl.point.x = 201;
    pl.point.y = 201;
    clist->addItem(pl);
    pl.data = 2;
    pl.point.x = 202;
    pl.point.y = 202;
    clist->addItem(pl);
    pl.data = 3;
    pl.point.x = 203;
    pl.point.y = 203;
    clist->addItem(pl);
    pl.data = 4;
    pl.point.x = 700;
    pl.point.y = 200;
    clist->addItem(pl);
    std::cout << "cList Itemcount: " << clist->getItemCount() << std::endl;
    for (unsigned int i = 0; i < clist->getItemCount(); i++) {
        tree->addPayload(clist->getItem(i));
    }
    for (unsigned int i = 0; i < clist->getItemCount(); i++) {
        BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* node = tree->getNodeToLocation(clist->getItem(i).point);
        std::cout << "PayloadCount in Child " << node->getNodeLocation().childID << " at level " << node->getNodeLocation().level << ": " << node->getNodePayloadCount() << std::endl;
    }
    std::cout << "Items in Tree: " << tree->getTreePayloadCount() << std::endl;
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();
    BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* node = tree->getNodeToLocation(clist->getItem(4).point);
    node->deletePayload(0);
    tree->cleanupTree();

    for (unsigned int i = 0; i < clist->getItemCount(); i++) {
        BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* node = tree->getNodeToLocation(clist->getItem(i).point);
        std::cout << "PayloadCount in Child " << node->getNodeLocation().childID << " at level " << node->getNodeLocation().level << ": " << node->getNodePayloadCount() << std::endl;
    }
    std::cout << "Items in Tree: " << tree->getTreePayloadCount() << std::endl;
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();


    delete tree;
    tree = new BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>(
        4,
        RootLocation,
        getQTFPackage()
        );




    //create test data dataset
    for (unsigned int i = 0; i < 5; i++) {
        int number = i;
        PLPackage<int, QTLocation> pl;
        pl.data = i;
        pl.point.x = rand() % 1000;
        pl.point.y = rand() % 1000;
        //std::cout << "generated payload " << pl.data << " at (" << pl.point.x << "|" << pl.point.y << ")" << std::endl;
        plist->addItem(pl);
        tree->addPayload(pl);
    }
    std::cout << "Items in Tree: " << tree->getTreePayloadCount() << std::endl;
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();

    unsigned int missing = 0;

    for (unsigned int i = 0; i < 5; i++) {
        BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* node = tree->getNodeToLocation(plist->getItem(i).point);
        if (node == nullptr) {
            log("Node to Location not found.");
            missing++;
        }
        else {
            std::cout << "PayloadCount in Child : " << node->getNodePayloadCount() << std::endl;
        }

    }
    std::cout << "Missing: " << missing << std::endl;

    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();

    missing = 0;
    for (unsigned int i = 0; i < 5; i++) {
        BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* node = tree->getNodeToPayload(plist->getItem(i), intCompare);
        if (node == nullptr) {
            log("Node to Payload not found.");
            missing++;
        }
        else {
            std::cout << "PayloadCount in Child : " << node->getNodePayloadCount() << std::endl;
        }

    }
    std::cout << "Missing: " << missing << std::endl;
    delete tree;
}

void OcTreeTest() {
    //setup of the Space and the startlocation
    NLPackage<OCTLocation, OCTOperationBorder> root;
    root.opBorder.max_x = 1000;
    root.opBorder.max_y = 1000;
    root.opBorder.max_z = 1000;
    root.nodeLoc = getOCTNodeLocation(root.opBorder);
    printOCTLocation(root.nodeLoc, root.nodeLoc);

    //initialisation of the root node
    BSPTreeNode<int, OCTLocation, OCTLocation, OCTOperationBorder, 8> *OcTree = new BSPTreeNode<int, OCTLocation, OCTLocation, OCTOperationBorder, 8>(
        2,
        root,
        getOCTFPackage()
        );

    //define Payload Package
    PLPackage<int, OCTLocation> pl;
    /*
    pl.data = 0;
    pl.point.x = 1;
    pl.point.y = 1;
    pl.point.z = 1;
    OcTree->addPayload(pl);
    pl.data = 1;
    pl.point.x = 2;
    pl.point.y = 2;
    pl.point.z = 2;
    OcTree->addPayload(pl);
    pl.data = 2;
    pl.point.x = 3;
    pl.point.y = 3;
    pl.point.z = 3;
    OcTree->addPayload(pl);
    pl.data = 3;
    pl.point.x = 4;
    pl.point.y = 4;
    pl.point.z = 4;
    OcTree->addPayload(pl);
    */
    
    for(unsigned int i=0; i<100;i++){
        pl.data = i;
        pl.point.x = rand() % 1000;
        pl.point.y = rand() % 1000;
        pl.point.z = rand() % 1000;
        std::cout << "generated payload " << pl.data << " at (" << pl.point.x << "|" << pl.point.y << "|" << pl.point.z <<")" << std::endl;
        if (OCTOutOfBoundsFunction(root.opBorder, pl.point)) {
            log("pl out of bounds!");
        }
        
        OcTree->addPayload(pl);
    }
    
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();
    delete OcTree;

}

void QTdemo() {
    NLPackage<QTLocation, QTOperationBorder> RootLocation;
    RootLocation.opBorder.min_x = 0;
    RootLocation.opBorder.min_y = 0;
    RootLocation.opBorder.max_x = 1000;
    RootLocation.opBorder.max_y = 1000;
    RootLocation.nodeLoc = getQTNodeLocation(RootLocation.opBorder);

    BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* tree = new BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>(
        2,
        RootLocation,
        getQTFPackage()
        );

    dList<PLPackage<int, QTLocation>>* plist = new dList<PLPackage<int, QTLocation>>(10);

    for (unsigned int i = 0; i < 10; i++) {
        PLPackage<int, QTLocation> pl;
        pl.data = i;
        pl.point.x = rand() % 1000;
        pl.point.y = rand() % 1000;
        plist->addItem(pl);
        tree->addPayload(pl);
        std::cout << "generated payload " << pl.data << " at (" << pl.point.x << "|" << pl.point.y << ")" << std::endl;
    }
    for (unsigned int i = 0; i < 10; i++) {
        BSPTreeNode<int, QTLocation, QTLocation, QTOperationBorder, 4>* node = tree->getNodeToLocation(plist->getItem(i).point);
        PLPackage<int, QTLocation> pl = plist->getItem(i);
        if (node == nullptr) {
            log("Node to Location not found.");
        }
        else {
            std::cout << "Node to Payload (" << pl.point.x << "|" << pl.point.y <<") found at Level "<< node->getNodeLocation().level << " with ChildID "<< node->getNodeLocation().childID <<". "
                << node->getNodePayloadCount() << " Nodes are stored in this Node" <<std::endl;
        }
    }

}

int main()
{
    

    
    //memTest(10000, 10);
    deleteAndCleanupTest();
    //QTdemo();

    //OcTreeTest();
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();
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
