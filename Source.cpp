#include <iostream>
#include <string>
#include <locale>
#include <clocale>
#include <io.h>
#include <fcntl.h>

// ��������� ������� ������ ��� �������
void setRussianLocale() {
    // ��� Windows
#ifdef _WIN32
    // ������������� ������ ��� �������
    setlocale(LC_ALL, "Russian");
    // ����������� ����� wide-����� � Windows
    _setmode(_fileno(stdout), _O_U16TEXT);
    // ��� Linux � ������ UNIX-�������� ������
#else
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
#endif
}

/**
 * @brief ������� ����� - ������������ ��������
 */
class Transport {
protected:
    std::wstring model;
    int year;

public:
    /**
     * @brief ����������� �������� ������
     * @param model ������ ������������� ��������
     * @param year ��� �������
     */
    Transport(const std::wstring& model, int year) : model(model), year(year) {}

    /**
     * @brief ����������� ����� ��� ����������� ����������
     */
    virtual void displayInfo() const {
        std::wcout << L"������������ ��������: " << model << std::endl;
        std::wcout << L"��� �������: " << year << std::endl;
    }

    /**
     * @brief ����������� ����� ��� ����� ������������� ��������
     */
    virtual void makeSound() const {
        std::wcout << L"������������ �������� ������ ����!" << std::endl;
    }

    /**
     * @brief ����������� ����������
     */
    virtual ~Transport() {
        std::wcout << L"������� ����� Transport ���������" << std::endl;
    }
};

/**
 * @brief ����������� ����� - ����������
 */
class Car : public Transport {
private:
    int horsepower;
    std::wstring fuelType;

public:
    /**
     * @brief ����������� ������������ ������
     * @param model ������ ����������
     * @param year ��� �������
     * @param horsepower ���������� ��������� ���
     * @param fuelType ��� �������
     */
    Car(const std::wstring& model, int year, int horsepower, const std::wstring& fuelType)
        : Transport(model, year), horsepower(horsepower), fuelType(fuelType) {
    }

    /**
     * @brief ���������������� ����� ��� ����������� ����������
     */
    void displayInfo() const override {
        std::wcout << L"\n=== ���������� �� ���������� ===" << std::endl;
        Transport::displayInfo();
        std::wcout << L"��������� ����: " << horsepower << std::endl;
        std::wcout << L"��� �������: " << fuelType << std::endl;
    }

    /**
     * @brief ���������������� ����� ��� ����� ����������
     */
    void makeSound() const override {
        std::wcout << L"���������� ��������: ���-���!" << std::endl;
    }

    /**
     * @brief ���������� ����� ��� ����������
     */
    void startEngine() const {
        std::wcout << L"��������� ���������� �������. ������..." << std::endl;
    }

    /**
     * @brief ����������
     */
    ~Car() override {
        std::wcout << L"����������� ����� Car ���������" << std::endl;
    }
};

/**
 * @brief ����������� ����� - ���������
 */
class Bicycle : public Transport {
private:
    std::wstring type;
    bool hasBasket;

public:
    /**
     * @brief ����������� ������������ ������
     * @param model ������ ����������
     * @param year ��� �������
     * @param type ��� ����������
     * @param hasBasket ������� �������
     */
    Bicycle(const std::wstring& model, int year, const std::wstring& type, bool hasBasket)
        : Transport(model, year), type(type), hasBasket(hasBasket) {
    }

    /**
     * @brief ���������������� ����� ��� ����������� ����������
     */
    void displayInfo() const override {
        std::wcout << L"\n=== ���������� � ���������� ===" << std::endl;
        Transport::displayInfo();
        std::wcout << L"��� ����������: " << type << std::endl;
        std::wcout << L"�������: " <<(hasBasket ? L"��" : L"���") << std::endl;
    }


    /**
         * @brief ���������������� ����� ��� ����� ����������
         */
    void makeSound() const override {
        std::wcout << L"��������� ������: �����-�����!" << std::endl;
    }

    /**
     * @brief ���������� ����� ��� ����������
     */
    void ringBell() const {
        std::wcout << L"������������ ������ ������!" << std::endl;
    }

    /**
     * @brief ����������
     */
    ~Bicycle() override {
        std::wcout << L"����������� ����� Bicycle ���������" << std::endl;
    }
};

/**
 * @brief �������� ������� ���������
 * @id739365412 (@return) ��� ���������� ���������
 */
int main() {
    setRussianLocale();

    std::wcout << L"������������ ������ �������� � ����������� �������\n" << std::endl;

    // ������� �������
    Transport genericTransport(L"Generic Model", 2000);
    Car myCar(L"Toyota Camry", 2020, 180, L"������");
    Bicycle myBike(L"Stels Navigator-500", 2022, L"������", true);

    // ������������ ������ ������� �������� ������
    std::wcout << L"\n=== ������������ �������� ������ ===" << std::endl;
    genericTransport.displayInfo();
    genericTransport.makeSound();

    // ������������ ������ ������� ������������ ������ Car
    std::wcout << L"\n=== ������������ ������������ ������ Car ===" << std::endl;
    myCar.displayInfo();
    myCar.makeSound();
    myCar.startEngine();

    // ������������ ������ ������� ������������ ������ Bicycle
    std::wcout << L"\n=== ������������ ������������ ������ Bicycle ===" << std::endl;
    myBike.displayInfo();
    myBike.makeSound();
    myBike.ringBell();

    // ������������ ������������ ����� ��������� �� ������� �����
    std::wcout << L"\n=== ������������ ������������ ===" << std::endl;
    Transport* transports[3];
    transports[0] = &genericTransport;
    transports[1] = &myCar;
    transports[2] = &myBike;

    for (int i = 0; i < 3; ++i) {
        transports[i]->displayInfo();
        transports[i]->makeSound();
        std::wcout << std::endl;
    }

    std::wcout << L"\n��������� ���������." << std::endl;
    return 0;
}