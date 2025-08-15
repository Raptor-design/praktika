#include <iostream>
#include <string>
#include <locale>
#include <clocale>
#include <io.h>
#include <fcntl.h>

// Установка русской локали для консоли
void setRussianLocale() {
    // Для Windows
#ifdef _WIN32
    // Устанавливаем локаль для консоли
    setlocale(LC_ALL, "Russian");
    // Настраиваем вывод wide-строк в Windows
    _setmode(_fileno(stdout), _O_U16TEXT);
    // Для Linux и других UNIX-подобных систем
#else
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
#endif
}

/**
 * @brief Базовый класс - Транспортное средство
 */
class Transport {
protected:
    std::wstring model;
    int year;

public:
    /**
     * @brief Конструктор базового класса
     * @param model Модель транспортного средства
     * @param year Год выпуска
     */
    Transport(const std::wstring& model, int year) : model(model), year(year) {}

    /**
     * @brief Виртуальный метод для отображения информации
     */
    virtual void displayInfo() const {
        std::wcout << L"Транспортное средство: " << model << std::endl;
        std::wcout << L"Год выпуска: " << year << std::endl;
    }

    /**
     * @brief Виртуальный метод для звука транспортного средства
     */
    virtual void makeSound() const {
        std::wcout << L"Транспортное средство издает звук!" << std::endl;
    }

    /**
     * @brief Виртуальный деструктор
     */
    virtual ~Transport() {
        std::wcout << L"Базовый класс Transport уничтожен" << std::endl;
    }
};

/**
 * @brief Производный класс - Автомобиль
 */
class Car : public Transport {
private:
    int horsepower;
    std::wstring fuelType;

public:
    /**
     * @brief Конструктор производного класса
     * @param model Модель автомобиля
     * @param year Год выпуска
     * @param horsepower Количество лошадиных сил
     * @param fuelType Тип топлива
     */
    Car(const std::wstring& model, int year, int horsepower, const std::wstring& fuelType)
        : Transport(model, year), horsepower(horsepower), fuelType(fuelType) {
    }

    /**
     * @brief Переопределенный метод для отображения информации
     */
    void displayInfo() const override {
        std::wcout << L"\n=== Информация об автомобиле ===" << std::endl;
        Transport::displayInfo();
        std::wcout << L"Лошадиные силы: " << horsepower << std::endl;
        std::wcout << L"Тип топлива: " << fuelType << std::endl;
    }

    /**
     * @brief Переопределенный метод для звука автомобиля
     */
    void makeSound() const override {
        std::wcout << L"Автомобиль сигналит: Бип-бип!" << std::endl;
    }

    /**
     * @brief Уникальный метод для автомобиля
     */
    void startEngine() const {
        std::wcout << L"Двигатель автомобиля запущен. Вррррр..." << std::endl;
    }

    /**
     * @brief Деструктор
     */
    ~Car() override {
        std::wcout << L"Производный класс Car уничтожен" << std::endl;
    }
};

/**
 * @brief Производный класс - Велосипед
 */
class Bicycle : public Transport {
private:
    std::wstring type;
    bool hasBasket;

public:
    /**
     * @brief Конструктор производного класса
     * @param model Модель велосипеда
     * @param year Год выпуска
     * @param type Тип велосипеда
     * @param hasBasket Наличие корзины
     */
    Bicycle(const std::wstring& model, int year, const std::wstring& type, bool hasBasket)
        : Transport(model, year), type(type), hasBasket(hasBasket) {
    }

    /**
     * @brief Переопределенный метод для отображения информации
     */
    void displayInfo() const override {
        std::wcout << L"\n=== Информация о велосипеде ===" << std::endl;
        Transport::displayInfo();
        std::wcout << L"Тип велосипеда: " << type << std::endl;
        std::wcout << L"Корзина: " <<(hasBasket ? L"Да" : L"Нет") << std::endl;
    }


    /**
         * @brief Переопределенный метод для звука велосипеда
         */
    void makeSound() const override {
        std::wcout << L"Велосипед звенит: Дзинь-дзинь!" << std::endl;
    }

    /**
     * @brief Уникальный метод для велосипеда
     */
    void ringBell() const {
        std::wcout << L"Велосипедный звонок звонит!" << std::endl;
    }

    /**
     * @brief Деструктор
     */
    ~Bicycle() override {
        std::wcout << L"Производный класс Bicycle уничтожен" << std::endl;
    }
};

/**
 * @brief Основная функция программы
 * @id739365412 (@return) Код завершения программы
 */
int main() {
    setRussianLocale();

    std::wcout << L"Демонстрация работы базового и производных классов\n" << std::endl;

    // Создаем объекты
    Transport genericTransport(L"Generic Model", 2000);
    Car myCar(L"Toyota Camry", 2020, 180, L"Бензин");
    Bicycle myBike(L"Stels Navigator-500", 2022, L"Горный", true);

    // Демонстрация работы методов базового класса
    std::wcout << L"\n=== Демонстрация базового класса ===" << std::endl;
    genericTransport.displayInfo();
    genericTransport.makeSound();

    // Демонстрация работы методов производного класса Car
    std::wcout << L"\n=== Демонстрация производного класса Car ===" << std::endl;
    myCar.displayInfo();
    myCar.makeSound();
    myCar.startEngine();

    // Демонстрация работы методов производного класса Bicycle
    std::wcout << L"\n=== Демонстрация производного класса Bicycle ===" << std::endl;
    myBike.displayInfo();
    myBike.makeSound();
    myBike.ringBell();

    // Демонстрация полиморфизма через указатель на базовый класс
    std::wcout << L"\n=== Демонстрация полиморфизма ===" << std::endl;
    Transport* transports[3];
    transports[0] = &genericTransport;
    transports[1] = &myCar;
    transports[2] = &myBike;

    for (int i = 0; i < 3; ++i) {
        transports[i]->displayInfo();
        transports[i]->makeSound();
        std::wcout << std::endl;
    }

    std::wcout << L"\nПрограмма завершена." << std::endl;
    return 0;
}