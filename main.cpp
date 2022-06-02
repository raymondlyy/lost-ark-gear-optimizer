#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

struct Item{
    std::string slot;
    std::string rarity;
    int quality;
    std::string stat1;
    int stat1val;
    std::string stat2; // if none put NONE
    int stat2val;
    std::string engraving1;
    int engraving1val;
    std::string engraving2;
    int engraving2val;
    std::string malus;
    int malusval;
    int gold;
    int pheons;
};

struct Build{
    int crit, swift, spec, dom, endurance, expertise, goldCost, pheonCost;
    std::map<std::string, int> engravings;
};

int main(){
    std::fstream fin("dummydata.csv"); // testing csv input
    std::string temp;

    std::vector<Item> items;
    std::vector<Build> builds;

    while (fin.good()){
        std::getline(fin, temp, ',');
        std::cout << temp << "\n";
    }

    return 0;
}

/**
 * @brief 
 * Method to print the final build result.
 * 
 * @param b 
 * Struct that contains build information. 
 */
void printBuild(Build b){
    std::cout << "Crit: " << b.crit << "\n"
              << "Swiftness: " << b.swift << "\n"
              << "Specialization: " << b.spec << "\n"
              << "Domination: " << b.dom << "\n"
              << "Endurance: " << b.endurance << "\n"
              << "Expertise: " << b.expertise << "\n\n";

    for (auto& engraving: b.engravings){
        std::cout << engraving.first << ": +" << engraving.second << "\n";
    }
}