// rr54.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;


class Equipment {
public:
    string name;
    int attackBonus;
    int defenseBonus;
    int cost;

    Equipment(string n = "Нет", int atk = 0, int def = 0, int c = 0)
        : name(n), attackBonus(atk), defenseBonus(def), cost(c) {
    }

    void show() const {
        cout << "   " << name << " (Атака: +" << attackBonus
            << ", Защита: +" << defenseBonus << ")" << endl;
    }
};

class Monster {
protected:
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int rewardCoins;

public:
    Monster(string n, int hp, int atk, int def, int coins)
        : name(n), health(hp), maxHealth(hp), attack(atk), defense(def), rewardCoins(coins) {
    }

    virtual ~Monster() {}

    virtual int dealDamage(int playerDefense) {
        int damage = max(1, attack - playerDefense);
        return damage;
    }

    virtual void takeDamage(int dmg) {
        health = max(0, health - dmg);
    }

    bool isAlive() const { return health > 0; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    string getName() const { return name; }
    int getRewardCoins() const { return rewardCoins; }

    virtual void showStatus() const {
        cout << "[" << name << "] HP: " << health << "/" << maxHealth << endl;
    }
};


class MagicMonster : public Monster {
public:
    MagicMonster() : Monster("Магический монстр", 60, 12, 3, 25) {}

    int dealDamage(int playerDefense) override {
        
        int effectiveDefense = max(0, playerDefense - 2);
        int damage = max(1, attack - effectiveDefense);
        return damage;
    }
};


class ArmorMonster : public Monster {
public:
    ArmorMonster() : Monster("Бронированный монстр", 80, 8, 8, 30) {}

    int dealDamage(int playerDefense) override {
        int damage = max(1, attack - playerDefense - 2);
        return damage;
    }
};


class BossMonster : public Monster {
private:
    int specialAttackCooldown;

public:
    BossMonster() : Monster("БОСС - Темный Лорд", 150, 18, 5, 100), specialAttackCooldown(3) {}

    int dealDamage(int playerDefense) override {
        if (specialAttackCooldown <= 0) {
           
            int damage = max(1, attack * 2 - playerDefense);
            int healAmount = damage / 3;
            health = min(maxHealth, health + healAmount);
            specialAttackCooldown = 4;
            cout << "   [ОСОБАЯ АТАКА БОССА! Урон: " << damage << ", Отхил: " << healAmount << "]" << endl;
            return damage;
        }
        specialAttackCooldown--;
        return Monster::dealDamage(playerDefense);
    }
};


class Player {
private:
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int coins;
    int potions;
    vector<Equipment> inventory;
    Equipment* currentWeapon;
    Equipment* currentArmor;

public:
    Player(string n) : name(n), health(100), maxHealth(100), attack(10),
        defense(5), coins(0), potions(3), currentWeapon(nullptr), currentArmor(nullptr) {
       
        inventory.push_back(Equipment("Ржавый меч", 2, 0, 0));
        inventory.push_back(Equipment("Кожаная броня", 0, 2, 0));
        currentWeapon = &inventory[0];
        currentArmor = &inventory[1];
    }

    int getTotalAttack() const {
        int weaponAtk = currentWeapon ? currentWeapon->attackBonus : 0;
        return attack + weaponAtk;
    }

    int getTotalDefense() const {
        int armorDef = currentArmor ? currentArmor->defenseBonus : 0;
        return defense + armorDef;
    }

    void takeDamage(int dmg) {
        health = max(0, health - dmg);
    }

    void heal(int amount) {
        health = min(maxHealth, health + amount);
    }

    bool isAlive() const { return health > 0; }

    void addCoins(int amount) {
        coins += amount;
    }

    void usePotion() {
        if (potions > 0 && health < maxHealth) {
            potions--;
            int healAmount = 30;
            heal(healAmount);
            cout << "   Использовано зелье! Восстановлено " << healAmount << " HP." << endl;
        }
        else {
            cout << "   Нет зелий или здоровье полное!" << endl;
        }
    }

    void addPotion() {
        potions++;
        cout << "   Получено зелье!" << endl;
    }

    void equipWeapon(int index) {
        if (index >= 0 && index < inventory.size()) {
            currentWeapon = &inventory[index];
            cout << "   Экипировано: " << currentWeapon->name << endl;
        }
    }

    void equipArmor(int index) {
        if (index >= 0 && index < inventory.size()) {
            currentArmor = &inventory[index];
            cout << "   Экипировано: " << currentArmor->name << endl;
        }
    }

    void buyEquipment(const Equipment& item) {
        if (coins >= item.cost) {
            coins -= item.cost;
            inventory.push_back(item);
            cout << "   Куплено: " << item.name << endl;
        }
        else {
            cout << "   Недостаточно монет!" << endl;
        }
    }

    void showStats() const {
        cout << "\n=== " << name << " ===" << endl;
        cout << "HP: " << health << "/" << maxHealth << endl;
        cout << "Атака: " << getTotalAttack() << endl;
        cout << "Защита: " << getTotalDefense() << endl;
        cout << "Монеты: " << coins << endl;
        cout << "Зелья: " << potions << endl;
        if (currentWeapon) cout << "Оружие: " << currentWeapon->name << endl;
        if (currentArmor) cout << "Броня: " << currentArmor->name << endl;
    }

    void showInventory() const {
        cout << "\n=== Инвентарь ===" << endl;
        for (size_t i = 0; i < inventory.size(); i++) {
            cout << i + 1 << ". ";
            inventory[i].show();
        }
    }

    string getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getCoins() const { return coins; }
};


class Game {
private:
    Player player;
    int difficulty;
    random_device rd;
    mt19937 gen;

public:
    Game(string playerName, int diff) : player(playerName), difficulty(diff), gen(rd()) {}

    vector<Monster*> generateMonsters() {
        vector<Monster*> monsters;
        int monsterCount = 3 + difficulty * 2;

        for (int i = 0; i < monsterCount - 1; i++) {
            int type = gen() % 2;
            if (type == 0) {
                monsters.push_back(new MagicMonster());
            }
            else {
                monsters.push_back(new ArmorMonster());
            }
        }

        
        monsters.push_back(new BossMonster());

        return monsters;
    }

    void battle(Monster* monster) {
        cout << "\n=== БОЙ НАЧАЛСЯ! ===" << endl;
        cout << "Противник: " << monster->getName() << endl;

        while (player.isAlive() && monster->isAlive()) {
            monster->showStatus();
            player.showStats();

            cout << "\nДействия:" << endl;
            cout << "1. Атаковать" << endl;
            cout << "2. Блокировать (снижает урон на 50%)" << endl;
            cout << "3. Контратака (риск промаха, но двойной урон)" << endl;
            cout << "4. Использовать зелье" << endl;
            cout << "Выбор: ";

            int choice;
            cin >> choice;

            bool blocking = false;
            int playerDamage = 0;

           
            switch (choice) {
            case 1: 
                playerDamage = player.getTotalAttack();
                cout << "Вы атакуете и наносите " << playerDamage << " урона!" << endl;
                break;

            case 2: 
                blocking = true;
                cout << "Вы блокируете следующую атаку!" << endl;
                break;

            case 3: 
                if (gen() % 100 < 70) { 
                    playerDamage = player.getTotalAttack() * 2;
                    cout << "Контратака успешна! Нанесено " << playerDamage << " урона!" << endl;
                }
                else {
                    cout << "Контратака промахнулась!" << endl;
                }
                break;

            case 4: 
                player.usePotion();
                break;

            default:
                cout << "Неверный выбор!" << endl;
                continue;
            }

            if (playerDamage > 0) {
                monster->takeDamage(playerDamage);
                if (!monster->isAlive()) {
                    cout << "\n*** " << monster->getName() << " повержен! ***" << endl;
                    int reward = monster->getRewardCoins();
                    player.addCoins(reward);
                    player.addPotion();
                    cout << "Получено: " << reward << " монет и 1 зелье!" << endl;
                    return;
                }
            }

           
            int monsterDamage = monster->dealDamage(player.getTotalDefense());
            if (blocking) {
                monsterDamage /= 2;
                cout << "Блок сработал! Урон снижен до " << monsterDamage << endl;
            }

            if (monsterDamage > 0) {
                player.takeDamage(monsterDamage);
                cout << monster->getName() << " наносит вам " << monsterDamage << " урона!" << endl;
            }

            if (!player.isAlive()) {
                cout << "\n*** ВЫ ПОГИБЛИ! ***" << endl;
                return;
            }
        }
    }

    void shop() {
        cout << "\n=== МАГАЗИН ===" << endl;
        cout << "Ваши монеты: " << player.getCoins() << endl;
        cout << "1. Железный меч (+5 атаки) - 50 монет" << endl;
        cout << "2. Стальная броня (+5 защиты) - 50 монет" << endl;
        cout << "3. Зелье здоровья - 20 монет" << endl;
        cout << "0. Выйти" << endl;
        cout << "Выбор: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            player.buyEquipment(Equipment("Железный меч", 5, 0, 50));
            break;
        case 2:
            player.buyEquipment(Equipment("Стальная броня", 0, 5, 50));
            break;
        case 3:
            if (player.getCoins() >= 20) {
                player.addCoins(-20);
                player.addPotion();
                cout << "Куплено зелье!" << endl;
            }
            else {
                cout << "Недостаточно монет!" << endl;
            }
            break;
        }
    }

    void run() {
        cout << "Добро пожаловать в RPG игру!" << endl;
        cout << "Сложность: " << difficulty << "/3" << endl;

        vector<Monster*> monsters = generateMonsters();

        for (size_t i = 0; i < monsters.size(); i++) {
            cout << "\n--- Монстр " << i + 1 << "/" << monsters.size() << " ---" << endl;
            battle(monsters[i]);

            if (!player.isAlive()) {
                cout << "\nИгра окончена! Вы погибли." << endl;
                
                return;
            }

            if (i < monsters.size() - 1) {
                cout << "\nХотите посетить магазин? (1 - да, 0 - нет): ";
                int shopChoice;
                cin >> shopChoice;
                if (shopChoice == 1) {
                    shop();
                }
            }
        }

        cout << "\n*** ПОЗДРАВЛЯЕМ! ВЫ ПРОШЛИ ИГРУ! ***" << endl;
        player.showStats();

      
        
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    string playerName;
    int difficulty;

    cout << "Введите имя героя: ";
    cin >> playerName;

    cout << "Выберите сложность (1-3): ";
    cin >> difficulty;

    if (difficulty < 1 || difficulty > 3) {
        cout << "Неверная сложность! Установлена сложность 1." << endl;
        difficulty = 1;
    }

    Game game(playerName, difficulty);
    game.run();

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
