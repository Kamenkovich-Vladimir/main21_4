#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>

struct coordinates {
    int x = 0;
    int y = 0;
};

struct characters {
    bool side = false; //Флаг принадлежности к команде, по умолчанию - Е (ложь - тёмная сторона силы)
    std::string name = "Unknown";
    int health = 0;
    int armor = 0;
    int damage = 0;
    coordinates location;
};

void take_damage(const characters& attacker, characters& defending) {
    std::cout << defending.name << " took damage - " << attacker.damage << " from " << attacker.name << std::endl;
    defending.armor -= attacker.damage;

    if (defending.armor < 0) {
        defending.health += defending.armor;
        defending.armor = 0;
    }
    std::cout << defending.name << "'s health: " << defending.health << std::endl;
}

void show_battlefield(const std::string field[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            std::cout << field[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

int main()
{
    std::cout << "************** Turn-based role-playing game *************\n\n";

    srand(time(0));
    std::string field[20][20];  //Поле боя
    char contin;                //Вспомогательная переменная для продолжения игры

    //Инициализация персонажей
    characters character[6];
    for (int i = 0; i < 5; i++) {
        character[i].name = "Enemy #" + std::to_string(i);
        character[i].health = (std::rand() % 101) + 50;
        character[i].armor = std::rand() % 51;
        character[i].damage = (std::rand() % 16) + 15;
    }

    character[5].side = true;
    std::cout << "Enter name of your character: ";
    std::cin >> character[5].name;

    std::cout << "Enter the value of your character's health: ";
    std::cin >> character[5].health;

    std::cout << "Enter the value of your character's armor: ";
    std::cin >> character[5].armor;

    std::cout << "Enter the value of your character's damage: ";
    std::cin >> character[5].damage;

    //Инициализация игрового поля
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            field[i][j] = ".";
        }
    }
    //Инициализация локации персонажей
    int x, y;

    for (int i = 0; i < 6; i++) {

        do {
            x = std::rand() % 21;
            y = std::rand() % 21;
        } while (field[x][y] != ".");

        field[x][y] = (i < 5) ? "E" : "P";

        character[i].location.x = x;
        character[i].location.y = y;
    }

    //Выводим игровое поле на экран
    show_battlefield(field);
    std::cout << "To continue the game, enter any simbol:  ";
    std::cin >> contin;

    //Игровой цикл пока не умрут все противники.Проверка здоровья персонажа Игрока внутри цикла
    while (character[0].health >= 0 || character[1].health >= 0 || character[2].health >= 0 || character[3].health >= 0 ||
        character[4].health >= 0) {
        //Последовательно каждый персонаж, если живой, делает ход. Последним делает ход персонаж Игрока
        for (int i = 0; i < 5; i++) {
            if (character[i].health < 0) {
                continue;
            }

            //int dir = std::rand() % 4;
            int dir, delta_x, delta_y;            //Определим упрощённо направление движения противника в сторону персонажа Игрока
            delta_x = character[i].location.x - character[5].location.x;
            if (delta_x > 0)
                dir = 2;
            else if (delta_x < 0)
                dir = 3;
            else
            {
                delta_y = character[i].location.y - character[5].location.y;
                if (delta_y > 0)
                    dir = 0;
                else
                    dir = 1;
            }

            switch (dir) {
            case 0:     //Left
                if (character[i].location.y > 0 && field[character[i].location.x][character[i].location.y - 1] == ".") {
                    field[character[i].location.x][character[i].location.y - 1] = "E";

                    field[character[i].location.x][character[i].location.y] = ".";
                    character[i].location.y -= 1;
                    std::cout << character[i].name << " moved left\n\n";
                }
                else if (field[character[i].location.x][character[i].location.y - 1] == "P")
                    take_damage(character[i], character[5]);
                break;
            case 1:     //Right
                if (character[i].location.y < 19 && field[character[i].location.x][character[i].location.y + 1] == ".") {
                    field[character[i].location.x][character[i].location.y + 1] = "E";

                    field[character[i].location.x][character[i].location.y] = ".";
                    character[i].location.y += 1;
                    std::cout << character[i].name << " moved right\n\n";
                }
                else if (field[character[i].location.x][character[i].location.y + 1] == "P")
                    take_damage(character[i], character[5]);
                break;
            case 2:     //Up
                if (character[i].location.x > 0 && field[character[i].location.x - 1][character[i].location.y] == ".") {
                    field[character[i].location.x - 1][character[i].location.y] = "E";

                    field[character[i].location.x][character[i].location.y] = ".";
                    character[i].location.x -= 1;
                    std::cout << character[i].name << " moved up\n\n";
                }
                else if (field[character[i].location.x - 1][character[i].location.y] == "P")
                    take_damage(character[i], character[5]);
                break;
            case 3:     //Down
                if (character[i].location.x < 19 && field[character[i].location.x + 1][character[i].location.y] == ".") {
                    field[character[i].location.x + 1][character[i].location.y] = "E";

                    field[character[i].location.x][character[i].location.y] = ".";
                    character[i].location.x += 1;
                    std::cout << character[i].name << " moved down\n\n";
                }
                else if (field[character[i].location.x + 1][character[i].location.y] == "P")
                    take_damage(character[i], character[5]);
                break;
            }
            if (character[5].health < 0) {
                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.x = 0; //Выводим персонаж Игрока за поле
                character[5].location.y = 0;
                break;
            }
        }

        //Выводим игровое поле на экран
        show_battlefield(field);
        if (character[5].health < 0) { //Если персонаж Игрока испустил дух, то заканчиваем битву
            break;
        }
        //Ход делает Игрок
        std::cout << "Specify the command to select the direction of movement (impact)\n";
        std::cout << "('l' - left, 'r' - right, 'u' - up, 'd' - down,)\n";
        std::cout << "or 's' - to save Game, or 'z' to load saved Game, another simbol - skips a turn\n";

        char ch_dir;
        std::cin >> ch_dir;
        if (ch_dir == 'l') {
            if (character[5].location.y > 0 && field[character[5].location.x][character[5].location.y - 1] == ".") {
                field[character[5].location.x][character[5].location.y - 1] = "P";

                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.y -= 1;
                std::cout << character[5].name << " moved left\n\n";
            }
            else if (field[character[5].location.x][character[5].location.y - 1] == "E") {
                //Определим врага, которого персонаж Игрока атакует
                for (int i = 0; i < 5; i++) {
                    if ((character[i].location.x == character[5].location.x) &&
                        (character[i].location.y == character[5].location.y - 1)) {

                        take_damage(character[5], character[i]);
                        if (character[i].health < 0) {
                            field[character[i].location.x][character[i].location.y] = ".";
                            character[i].location.x = 21; //Выводим персонаж противника за поле
                            character[i].location.y = 21;
                        }
                        break;
                    }
                }
            }
        }
        else if (ch_dir == 'r') {
            if (character[5].location.y < 19 && field[character[5].location.x][character[5].location.y + 1] == ".") {
                field[character[5].location.x][character[5].location.y + 1] = "P";

                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.y += 1;
                std::cout << character[5].name << " moved right\n\n";
            }
            else if (field[character[5].location.x][character[5].location.y + 1] == "E") {
                //Определим врага, которого персонаж Игрока атакует
                for (int i = 0; i < 5; i++) {
                    if ((character[i].location.x == character[5].location.x) &&
                        (character[i].location.y == character[5].location.y + 1)) {

                        take_damage(character[5], character[i]);
                        if (character[i].health < 0) {
                            field[character[i].location.x][character[i].location.y] = ".";
                            character[i].location.x = 21; //Выводим персонаж противника за поле
                            character[i].location.y = 21;
                        }
                        break;
                    }
                }
            }
        }
        else if (ch_dir == 'u') {
            if (character[5].location.x > 0 && field[character[5].location.x - 1][character[5].location.y] == ".") {
                field[character[5].location.x - 1][character[5].location.y] = "P";

                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.x -= 1;
                std::cout << character[5].name << " moved up\n\n";
            }
            else if (field[character[5].location.x - 1][character[5].location.y] == "E") {
                //Определим врага, которого персонаж Игрока атакует
                for (int i = 0; i < 5; i++) {
                    if ((character[i].location.x == character[5].location.x - 1) &&
                        (character[i].location.y == character[5].location.y)) {

                        take_damage(character[5], character[i]);
                        if (character[i].health < 0) {
                            field[character[i].location.x][character[i].location.y] = ".";
                            character[i].location.x = 21; //Выводим персонаж противника за поле
                            character[i].location.y = 21;
                        }
                        break;
                    }
                }
            }
        }
        else if (ch_dir == 'd') {
            if (character[5].location.x < 19 && field[character[5].location.x + 1][character[5].location.y] == ".") {
                field[character[5].location.x + 1][character[5].location.y] = "P";

                field[character[5].location.x][character[5].location.y] = ".";
                character[5].location.x += 1;
                std::cout << character[5].name << " moved down\n\n";
            }
            else if (field[character[5].location.x + 1][character[5].location.y] == "E") {
                //Определим врага, которого персонаж Игрока атакует
                for (int i = 0; i < 5; i++) {
                    if ((character[i].location.x == character[5].location.x + 1) &&
                        (character[i].location.y == character[5].location.y)) {

                        take_damage(character[5], character[i]);
                        if (character[i].health < 0) {
                            field[character[i].location.x][character[i].location.y] = ".";
                            character[i].location.x = 21; //Выводим персонаж противника за поле
                            character[i].location.y = 21;
                        }
                        break;
                    }
                }
            }
        }
        else if (ch_dir == 's') {   //Сохраним игру в файл
            std::ofstream file_strim;
            file_strim.open("game.bin", std::ios::binary);
            if (!file_strim.is_open()) {
                std::cerr << "The file 'in the specified path'game.bin cannot be opened\n";
                return 0;
            }
            for (int i = 0; i < 6; i++) {
                file_strim.write((char*)&character[i], sizeof(character[i]));
            }
            std::cout << "The game data has been successfully written to the 'game.bin' file \n";
            file_strim.close();
            return 0;
        }
        else if (ch_dir == 'z') {   //Загрузим игру из файла
            //Проверка наличия файла game.bin в папке проекта
            if (std::ifstream("game.bin")) {
                std::ifstream file_strim;
                file_strim.open("game.bin", std::ios::binary);
                if (!file_strim.is_open()) {
                    std::cerr << "The file 'game.bin' cannot be opened\n";
                    return 0;
                }
                for (int i = 0; i < 6; i++) {
                    file_strim.read((char*)&character[i], sizeof(character[i]));
                }
                std::cout << "The game data has been successfully readen from the 'game.bin' file \n";
                file_strim.close();
                //Инициализация игрового поля
                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 20; j++) {
                        field[i][j] = ".";
                    }
                }
                //Инициализация локации персонажей

                for (int i = 0; i < 6; i++) {
                    if (character[i].location.x > 19)
                        continue;
                    else if (i < 5)
                        field[character[i].location.x][character[i].location.y] = "E";
                    else if (i == 5)
                        field[character[i].location.x][character[i].location.y] = "P";
                    else
                    {
                        std::cerr << "Error in characters location\n\n";
                        return 0;
                    }

                }
            }
        }
        else
            std::cout << character[5].name << " skips a turn\n\n";

        //Выводим игровое поле на экран
        show_battlefield(field);

        std::cout << "To continue the game, enter any simbol:  ";
        std::cin >> contin;
    }

    //Вывод результата
    if (character[5].health < 0)
        std::cout << "Enemy win!!!\n\n";
    else
        std::cout << "Player win!!!\n\n";

}