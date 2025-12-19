#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

/* =========================
   Enums
   ========================= */
enum Job { WARRIOR = 1, MAGE, THIEF };
enum ItemType { POTION, ETHER };

/* =========================
   Item structure
   ========================= */
struct Item {
    string name;
    ItemType type;
    int value;
};

/* =========================
   Player structure
   ========================= */
struct Player {
    string name;
    Job job;
    int level;
    int exp;
    int hp, maxHp;
    int mp, maxMp;
    int atk, def;
    int gold;
    vector<Item> bag;
};

/* =========================
   Monster structure
   ========================= */
struct Monster {
    string name;
    int hp;
    int atk;
    int def;
    int expReward;
    int goldReward;
};

/* =========================
   Utility function
   ========================= */
int randRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/* =========================
   Create player
   ========================= */
Player createPlayer() {
    Player p;

    cout << "Enter your character name: ";
    getline(cin, p.name);

    cout << "\nChoose a job:\n";
    cout << "1. Warrior (High HP & Defense)\n";
    cout << "2. Mage (High Magic Power)\n";
    cout << "3. Thief (High Attack)\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    p.level = 1;
    p.exp = 0;
    p.gold = 50;

    if (choice == WARRIOR) {
        p.job = WARRIOR;
        p.maxHp = 120;
        p.maxMp = 20;
        p.atk = 15;
        p.def = 10;
    }
    else if (choice == MAGE) {
        p.job = MAGE;
        p.maxHp = 80;
        p.maxMp = 60;
        p.atk = 20;
        p.def = 5;
    }
    else {
        p.job = THIEF;
        p.maxHp = 100;
        p.maxMp = 30;
        p.atk = 18;
        p.def = 7;
    }

    p.hp = p.maxHp;
    p.mp = p.maxMp;

    return p;
}

/* =========================
   Show player status
   ========================= */
void showStatus(const Player& p) {
    cout << "\n====== PLAYER STATUS ======\n";
    cout << "Name  : " << p.name << endl;
    cout << "Level : " << p.level << "  EXP: " << p.exp << endl;
    cout << "HP    : " << p.hp << "/" << p.maxHp << endl;
    cout << "MP    : " << p.mp << "/" << p.maxMp << endl;
    cout << "ATK   : " << p.atk << "  DEF: " << p.def << endl;
    cout << "Gold  : " << p.gold << endl;
}

/* =========================
   Generate monster
   ========================= */
Monster generateMonster(int level) {
    Monster m;
    int r = randRange(1, 3);

    if (r == 1) m.name = "Slime";
    else if (r == 2) m.name = "Skeleton";
    else m.name = "Wolf";

    m.hp = 40 + level * 10;
    m.atk = 10 + level * 3;
    m.def = 5 + level * 2;
    m.expReward = 20 + level * 10;
    m.goldReward = randRange(10, 30);

    return m;
}

/* =========================
   Use item
   ========================= */
void useItem(Player& p) {
    if (p.bag.empty()) {
        cout << "Your bag is empty!\n";
        return;
    }

    cout << "\nInventory:\n";
    for (int i = 0; i < p.bag.size(); i++) {
        cout << i + 1 << ". " << p.bag[i].name << endl;
    }

    cout << "Choose item: ";
    int choice;
    cin >> choice;

    if (choice < 1 || choice > p.bag.size()) return;

    Item item = p.bag[choice - 1];

    if (item.type == POTION) {
        p.hp += item.value;
        if (p.hp > p.maxHp) p.hp = p.maxHp;
        cout << "Recovered HP +" << item.value << endl;
    }
    else {
        p.mp += item.value;
        if (p.mp > p.maxMp) p.mp = p.maxMp;
        cout << "Recovered MP +" << item.value << endl;
    }

    p.bag.erase(p.bag.begin() + choice - 1);
}

/* =========================
   Battle system
   ========================= */
bool battle(Player& p) {
    Monster m = generateMonster(p.level);
    cout << "\nA wild " << m.name << " appeared!\n";

    while (p.hp > 0 && m.hp > 0) {
        cout << "\n1. Attack\n2. Skill\n3. Use Item\nChoice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            int damage = p.atk - m.def;
            if (damage < 1) damage = 1;
            m.hp -= damage;
            cout << "You dealt " << damage << " damage.\n";
        }
        else if (choice == 2 && p.mp >= 10) {
            int damage = p.atk * 2;
            p.mp -= 10;
            m.hp -= damage;
            cout << "Skill dealt " << damage << " damage!\n";
        }
        else if (choice == 3) {
            useItem(p);
            continue;
        }

        if (m.hp > 0) {
            int damage = m.atk - p.def;
            if (damage < 1) damage = 1;
            p.hp -= damage;
            cout << m.name << " hits you for " << damage << " damage.\n";
        }
    }

    if (p.hp <= 0) {
        cout << "\nYou were defeated...\n";
        return false;
    }

    cout << "\nYou defeated the " << m.name << "!\n";
    p.exp += m.expReward;
    p.gold += m.goldReward;

    if (p.exp >= 100) {
        p.level++;
        p.exp = 0;
        p.maxHp += 20;
        p.atk += 5;
        p.def += 3;
        p.hp = p.maxHp;
        cout << "LEVEL UP! Now level " << p.level << endl;
    }

    return true;
}

/* =========================
   Shop system
   ========================= */
void shop(Player& p) {
    cout << "\nSHOP\n";
    cout << "1. HP Potion (20 Gold)\n";
    cout << "2. MP Potion (20 Gold)\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;

    if (p.gold < 20) {
        cout << "Not enough gold!\n";
        return;
    }

    if (choice == 1) {
        p.bag.push_back({ "HP Potion", POTION, 30 });
    }
    else if (choice == 2) {
        p.bag.push_back({ "MP Potion", ETHER, 20 });
    }

    p.gold -= 20;
}

/* =========================
   Main function
   ========================= */
int main() {
    srand(time(0));

    Player player = createPlayer();

    while (true) {
        showStatus(player);
        cout << "\n1. Adventure\n2. Shop\n3. Exit\nChoice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            if (!battle(player)) break;
        }
        else if (choice == 2) {
            shop(player);
        }
        else {
            break;
        }
    }

    cout << "\nGame Over\n";
    return 0;
}
