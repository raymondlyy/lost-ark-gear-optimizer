#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

struct Item{
    std::string slot, rarity;
    int quality, crit, swift, spec, dom, endurance, expertise, goldCost, pheonCost;
    std::map<std::string, int> engravings;
};

struct Build{
    int crit, swift, spec, dom, endurance, expertise, goldCost, pheonCost;
    std::map<std::string, int> engravings;
};

void printBuild(Build);
void printItem(Item);
void assignStat(std::string, int, Item&);

int main(){
    std::fstream fin("dummydata.csv"); // testing csv input
    std::string temp;

    std::vector<Item> necks, earrings, rings;
    std::vector<Build> builds;

    // read csv input
    while (fin.good()){
        // read the item type
        std::getline(fin, temp, ',');
        // std::cout << "[" << temp << "]";

        // create a new item object and populate its data fields
        if (temp == "Necklace"){
            necks.push_back(Item());
            // rarity
            std::getline(fin, temp, ',');
            // quality
            std::getline(fin, temp, ',');
            necks.back().quality = std::stoi(temp);
            // stat 1
            std::string stat;
            std::getline(fin, stat, ',');
            std::getline(fin, temp, ',');
            assignStat(stat, std::stoi(temp), necks.back());
            // stat 2
            std::getline(fin, stat, ',');
            std::getline(fin, temp, ',');
            assignStat(stat, std::stoi(temp), necks.back());
            // engraving 1
            std::string engraving;
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            necks.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // engraving 2
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            necks.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // malus
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            necks.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // gold and pheons
            std::getline(fin, temp, ',');
            necks.back().goldCost = std::stoi(temp);
            std::getline(fin, temp, '\n');
            necks.back().pheonCost = std::stoi(temp);
        } else if (temp == "Earring"){
            earrings.push_back(Item());
            // rarity
            std::getline(fin, temp, ',');
            // quality
            std::getline(fin, temp, ',');
            earrings.back().quality = std::stoi(temp);
            // stat 1
            std::string stat;
            std::getline(fin, stat, ',');
            std::getline(fin, temp, ',');
            assignStat(stat, std::stoi(temp), earrings.back());
            // stat 2. skip over it
            std::getline(fin, stat, ',');
            std::getline(fin, temp, ',');
            // engraving 1
            std::string engraving;
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            earrings.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // engraving 2
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            earrings.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // malus
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            earrings.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // gold and pheons
            std::getline(fin, temp, ',');
            earrings.back().goldCost = std::stoi(temp);
            std::getline(fin, temp, '\n');
            earrings.back().pheonCost = std::stoi(temp);
        } else if (temp == "Ring"){
            rings.push_back(Item());
            // rarity
            std::getline(fin, temp, ',');
            // quality
            std::getline(fin, temp, ',');
            rings.back().quality = std::stoi(temp);
            // stat 1
            std::string stat;
            std::getline(fin, stat, ',');
            std::getline(fin, temp, ',');
            assignStat(stat, std::stoi(temp), rings.back());
            // stat 2. skip over it
            std::getline(fin, stat, ',');
            std::getline(fin, temp, ',');
            // engraving 1
            std::string engraving;
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            rings.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // engraving 2
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            rings.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // malus
            std::getline(fin, engraving, ',');
            std::getline(fin, temp, ',');
            rings.back().engravings.insert(std::pair<std::string, int>(engraving, std::stoi(temp)));
            // gold and pheons
            std::getline(fin, temp, ',');
            rings.back().goldCost = std::stoi(temp);
            std::getline(fin, temp, '\n');
            rings.back().pheonCost = std::stoi(temp);
        } else std::cout << "There was an error with an item. Check the data.\n";
    }

    printItem(necks.back());
    printItem(earrings.back());
    return 0;
}

/**
 * @brief Method to print the final build result to console for testing.
 * 
 * @param b Struct that contains build information. 
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

/**
 * @brief Method to print an item's data for testing.
 * 
 * @param i Struct that contains item information.
 */
void printItem(Item i){
    if (i.crit) std::cout << "Crit: " << i.crit << "\n";
    if (i.swift) std::cout << "Swiftness: " << i.swift << "\n";
    if (i.spec) std::cout << "Specialization: " << i.spec << "\n";
    if (i.dom) std::cout << "Domination: " << i.dom << "\n";
    if (i.endurance) std::cout << "Endurance: " << i.endurance << "\n";
    if (i.expertise) std::cout << "Expertise: " << i.expertise << "\n\n";

    for (auto& engraving: i.engravings){
        std::cout << engraving.first << ": +" << engraving.second << "\n";
    }
}

/**
 * @brief 
 * Method that assigns a given stat value to an item
 * 
 * @param stat The stat to be assigned
 * @param value The value of the stat to assign
 * @param item The item to be modified
 */
void assignStat(std::string stat, int value, Item& item){
    if (stat == "Crit") item.crit = value;
    else if (stat == "Swiftness") item.swift = value;
    else if (stat == "Specialization") item.spec = value;
    else if (stat == "Domination") item.dom = value;
    else if (stat == "Endurance") item.endurance = value;
    else if (stat == "Expertise") item.expertise = value;
}