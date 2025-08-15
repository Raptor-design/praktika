#include <iostream>
#include <string>
#include <locale>
#include <clocale>
#include <io.h>
#include <fcntl.h>

// Óñòàíîâêà ðóññêîé ëîêàëè äëÿ êîíñîëè
void setRussianLocale() {
    // Äëÿ Windows
#ifdef _WIN32
    // Óñòàíàâëèâàåì ëîêàëü äëÿ êîíñîëè
    setlocale(LC_ALL, "Russian");
    // Íàñòðàèâàåì âûâîä wide-ñòðîê â Windows
    _setmode(_fileno(stdout), _O_U16TEXT);
    // Äëÿ Linux è äðóãèõ UNIX-ïîäîáíûõ ñèñòåì
#else
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
#endif
}

/**
 * @brief Áàçîâûé êëàññ - Òðàíñïîðòíîå ñðåäñòâî
 */
class Transport {
protected:
    std::wstring model;
    int year;

public:
    /**
     * @brief Êîíñòðóêòîð áàçîâîãî êëàññà
     * @param model Ìîäåëü òðàíñïîðòíîãî ñðåäñòâà
     * @param year Ãîä âûïóñêà
     */
    Transport(const std::wstring& model, int year) : model(model), year(year) {}

    /**
     * @brief Âèðòóàëüíûé ìåòîä äëÿ îòîáðàæåíèÿ èíôîðìàöèè
     */
    virtual void displayInfo() const {
        std::wcout << L"Òðàíñïîðòíîå ñðåäñòâî: " << model << std::endl;
        std::wcout << L"Ãîä âûïóñêà: " << year << std::endl;
    }

    /**
     * @brief Âèðòóàëüíûé ìåòîä äëÿ çâóêà òðàíñïîðòíîãî ñðåäñòâà
     */
    virtual void makeSound() const {
        std::wcout << L"Òðàíñïîðòíîå ñðåäñòâî èçäàåò çâóê!" << std::endl;
    }

    /**
     * @brief Âèðòóàëüíûé äåñòðóêòîð
     */
    virtual ~Transport() {
        std::wcout << L"Áàçîâûé êëàññ Transport óíè÷òîæåí" << std::endl;
    }
};

/**
 * @brief Ïðîèçâîäíûé êëàññ - Àâòîìîáèëü
 */
class Car : public Transport {
private:
    int horsepower;
    std::wstring fuelType;

public:
    /**
     * @brief Êîíñòðóêòîð ïðîèçâîäíîãî êëàññà
     * @param model Ìîäåëü àâòîìîáèëÿ
     * @param year Ãîä âûïóñêà
     * @param horsepower Êîëè÷åñòâî ëîøàäèíûõ ñèë
     * @param fuelType Òèï òîïëèâà
     */
    Car(const std::wstring& model, int year, int horsepower, const std::wstring& fuelType)
        : Transport(model, year), horsepower(horsepower), fuelType(fuelType) {
    }

    /**
     * @brief Ïåðåîïðåäåëåííûé ìåòîä äëÿ îòîáðàæåíèÿ èíôîðìàöèè
     */
    void displayInfo() const override {
        std::wcout << L"\n=== Èíôîðìàöèÿ îá àâòîìîáèëå ===" << std::endl;
        Transport::displayInfo();
        std::wcout << L"Ëîøàäèíûå ñèëû: " << horsepower << std::endl;
        std::wcout << L"Òèï òîïëèâà: " << fuelType << std::endl;
    }

    /**
     * @brief Ïåðåîïðåäåëåííûé ìåòîä äëÿ çâóêà àâòîìîáèëÿ
     */
    void makeSound() const override {
        std::wcout << L"Àâòîìîáèëü ñèãíàëèò: Áèï-áèï!" << std::endl;
    }

    /**
     * @brief Óíèêàëüíûé ìåòîä äëÿ àâòîìîáèëÿ
     */
    void startEngine() const {
        std::wcout << L"Äâèãàòåëü àâòîìîáèëÿ çàïóùåí. Âððððð..." << std::endl;
    }

    /**
     * @brief Äåñòðóêòîð
     */
    ~Car() override {
        std::wcout << L"Ïðîèçâîäíûé êëàññ Car óíè÷òîæåí" << std::endl;
    }
};

/**
 * @brief Ïðîèçâîäíûé êëàññ - Âåëîñèïåä
 */
class Bicycle : public Transport {
private:
    std::wstring type;
    bool hasBasket;

public:
    /**
     * @brief Êîíñòðóêòîð ïðîèçâîäíîãî êëàññà
     * @param model Ìîäåëü âåëîñèïåäà
     * @param year Ãîä âûïóñêà
     * @param type Òèï âåëîñèïåäà
     * @param hasBasket Íàëè÷èå êîðçèíû
     */
    Bicycle(const std::wstring& model, int year, const std::wstring& type, bool hasBasket)
        : Transport(model, year), type(type), hasBasket(hasBasket) {
    }

    /**
     * @brief Ïåðåîïðåäåëåííûé ìåòîä äëÿ îòîáðàæåíèÿ èíôîðìàöèè
     */
    void displayInfo() const override {
        std::wcout << L"\n=== Èíôîðìàöèÿ î âåëîñèïåäå ===" << std::endl;
        Transport::displayInfo();
        std::wcout << L"Òèï âåëîñèïåäà: " << type << std::endl;
        std::wcout << L"Êîðçèíà: " <<(hasBasket ? L"Äà" : L"Íåò") << std::endl;
    }


    /**
         * @brief Ïåðåîïðåäåëåííûé ìåòîä äëÿ çâóêà âåëîñèïåäà
         */
    void makeSound() const override {
        std::wcout << L"Âåëîñèïåä çâåíèò: Äçèíü-äçèíü!" << std::endl;
    }

    /**
     * @brief Óíèêàëüíûé ìåòîä äëÿ âåëîñèïåäà
     */
    void ringBell() const {
        std::wcout << L"Âåëîñèïåäíûé çâîíîê çâîíèò!" << std::endl;
    }

    /**
     * @brief Äåñòðóêòîð
     */
    ~Bicycle() override {
        std::wcout << L"Ïðîèçâîäíûé êëàññ Bicycle óíè÷òîæåí" << std::endl;
    }
};

/**
 * @brief Îñíîâíàÿ ôóíêöèÿ ïðîãðàììû
 * @id739365412 (@return) Êîä çàâåðøåíèÿ ïðîãðàììû
 */
int main() {
    setRussianLocale();

    std::wcout << L"Äåìîíñòðàöèÿ ðàáîòû áàçîâîãî è ïðîèçâîäíûõ êëàññîâ\n" << std::endl;

    // Ñîçäàåì îáúåêòû
    Transport genericTransport(L"Generic Model", 2000);
    Car myCar(L"Toyota Camry", 2020, 180, L"Áåíçèí");
    Bicycle myBike(L"Stels Navigator-500", 2022, L"Ãîðíûé", true);

    // Äåìîíñòðàöèÿ ðàáîòû ìåòîäîâ áàçîâîãî êëàññà
    std::wcout << L"\n=== Äåìîíñòðàöèÿ áàçîâîãî êëàññà ===" << std::endl;
    genericTransport.displayInfo();
    genericTransport.makeSound();

    // Äåìîíñòðàöèÿ ðàáîòû ìåòîäîâ ïðîèçâîäíîãî êëàññà Car
    std::wcout << L"\n=== Äåìîíñòðàöèÿ ïðîèçâîäíîãî êëàññà Car ===" << std::endl;
    myCar.displayInfo();
    myCar.makeSound();
    myCar.startEngine();

    // Äåìîíñòðàöèÿ ðàáîòû ìåòîäîâ ïðîèçâîäíîãî êëàññà Bicycle
    std::wcout << L"\n=== Äåìîíñòðàöèÿ ïðîèçâîäíîãî êëàññà Bicycle ===" << std::endl;
    myBike.displayInfo();
    myBike.makeSound();
    myBike.ringBell();

    // Äåìîíñòðàöèÿ ïîëèìîðôèçìà ÷åðåç óêàçàòåëü íà áàçîâûé êëàññ
    std::wcout << L"\n=== Äåìîíñòðàöèÿ ïîëèìîðôèçìà ===" << std::endl;
    Transport* transports[3];
    transports[0] = &genericTransport;
    transports[1] = &myCar;
    transports[2] = &myBike;

    for (int i = 0; i < 3; ++i) {
        transports[i]->displayInfo();
        transports[i]->makeSound();
        std::wcout << std::endl;
    }

    std::wcout << L"\nÏðîãðàììà çàâåðøåíà." << std::endl;
    return 0;
}
