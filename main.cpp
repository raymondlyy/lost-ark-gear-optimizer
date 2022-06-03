#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

struct Item{
    std::string slot, rarity;
    int quality, crit, swift, spec, dom, end, exp, goldCost, pheonCost;
    std::map<std::string, int> engravings;
};

struct Build{
    int crit, swift, spec, dom, end, exp, goldCost, pheonCost;
    std::map<std::string, int> engravings;
};

struct BuildFunctor{
    bool operator ()(const Build& l, const Build& r) const {
        return l.goldCost < r.goldCost;
    }
};

void printBuild(Build);
void printItem(Item);
void assignStat(std::string, int, Item&);
void addItem(Build&, Item&);
bool validBuild(Build&, Build&);

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

    std::cout << necks.size() + rings.size() + earrings.size() << " items added. Running algorithm...\n";

    // preset some starting engravings. replace with user input later
    std::map<std::string, int> startingEngravings;
    startingEngravings.insert(std::pair<std::string, int>("Ambush Master", 15));
    startingEngravings.insert(std::pair<std::string, int>("Adrenaline", 10));

    // preset a target build. replace with user input later
    std::map<std::string, int> targetEngravings;
    targetEngravings.insert(std::pair<std::string, int>("Ambush Master", 15));
    targetEngravings.insert(std::pair<std::string, int>("Adrenaline", 15));
    targetEngravings.insert(std::pair<std::string, int>("Remaining Energy", 15));

    Build target = Build();
    target.spec = 1000;
    target.crit = 410;
    target.engravings = targetEngravings;

    // printBuild(target);

    int neckIndex = 0;
    int earringFIndex = 0, earringSIndex = 1, ringFIndex = 0, ringSIndex = 1;
    while (neckIndex < necks.size()){
        Build tempBuild = {};
        tempBuild.engravings = startingEngravings;

        // build the item
        // std::cout << neckIndex << "," << earringFIndex << "," << earringSIndex << "," << ringFIndex << "," << ringSIndex << "\n";

        addItem(tempBuild, necks[neckIndex]);
        addItem(tempBuild, earrings[earringFIndex]);
        addItem(tempBuild, earrings[earringSIndex]);
        addItem(tempBuild, rings[ringFIndex]);
        addItem(tempBuild, rings[ringSIndex]);

        builds.push_back(tempBuild);

        // increment second ring index
        ++ringSIndex;

        // if the end of the ring vector is reached, increment the first ring index and reset the second
        if (ringSIndex >= rings.size() - 1){
            ++ringFIndex;
            ringSIndex = ringFIndex + 1;
        }
        // if the end of the ring vector is reached, it's time to increment the earring indices
        if (ringFIndex >= rings.size() - 1){
            ++earringSIndex;

            ringFIndex = 0;
            ringSIndex = 1;
            // if the end of the earring vector is reached, increment the second earring index and reset the second
            if (earringSIndex >= earrings.size() - 1){
                ++earringFIndex;
                earringSIndex = earringFIndex + 1;
            }
        }

        // if the end of the earring vector is fully reached, it's time to move onto the next neck object
        if (earringFIndex >= earrings.size() - 1){
            ++neckIndex;
            earringFIndex = 0, earringSIndex = 1, ringFIndex = 0, ringSIndex = 1;
        }
    }
    
    std::sort(builds.begin(), builds.end(), BuildFunctor());

    for (Build& b : builds){
        if (validBuild(target, b)){
            printBuild(b);
            std::cout << "\n===========\n";
        }
    }
    return 0;
}

/**
 * @brief Method to print the final build result to console for testing.
 * 
 * @param b Struct that contains build information. 
 */
void printBuild(Build b){
    std::cout << "Gold cost: " << b.goldCost << "\nPheon cost: " << b.pheonCost << "\n\n";

    if (b.crit) std::cout << "Crit: " << b.crit << "\n";
    if (b.swift) std::cout << "Swiftness: " << b.swift << "\n";
    if (b.spec) std::cout << "Specialization: " << b.spec << "\n";
    if (b.dom) std::cout << "Domination: " << b.dom << "\n";
    if (b.end) std::cout << "Endurance: " << b.end << "\n";
    if (b.exp) std::cout << "Expertise: " << b.exp << "\n\n";

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
    if (i.end) std::cout << "Endurance: " << i.end << "\n";
    if (i.exp) std::cout << "Expertise: " << i.exp << "\n\n";

    for (auto& engraving: i.engravings){
        std::cout << engraving.first << ": +" << engraving.second << "\n";
    }
}

/**
 * @brief Method that assigns a given stat value to an item
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
    else if (stat == "Endurance") item.end = value;
    else if (stat == "Expertise") item.exp = value;
}

/**
 * @brief Adds a specified item to a build.
 * 
 * @param b The build to be modified.
 * @param i The new item to be added.
 */
void addItem(Build& b, Item& i){
    b.crit += i.crit;
    b.swift += i.swift;
    b.spec += i.spec;
    b.dom += i.dom;
    b.end += i.end;
    b.exp += i.exp;

    for (std::pair<std::string, int> engraving : i.engravings){
        b.engravings[engraving.first] += engraving.second;
    }

    b.goldCost += i.goldCost;
    b.pheonCost += i.pheonCost;
}

/**
 * @brief Compares two builds to see if the current build meets the target requirements.
 * 
 * @param target The target build.
 * @param current The current build to be tested.
 * @return true if current build meets the requirements
 * @return false if current build doesn't meet the requirements
 */
bool validBuild(Build& target, Build& current){
    // check if stats reach target
    if (current.crit < target.crit || current.swift < target.swift || current.spec < target.spec || current.dom < target.dom || current.end < target.end || current.exp < target.exp) return false;
    
    // check if engravings reach target
    for (std::pair<std::string, int> engraving : target.engravings){
        if (current.engravings[engraving.first] < engraving.second) return false;
    }

    // check no negative engravings
    // for (std::pair<std::string, int> engraving : current.engravings){
    //     if (engraving.first.find("Reduction") != std::string::npos && engraving.second >= 5) return false;
    // }
    return true;
}