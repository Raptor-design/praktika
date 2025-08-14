#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>
#include <Windows.h>
#include <iomanip>
using namespace std;
// Класс для работы с базой данных туристического агентства
class TourismDatabase {
private:
    sqlite3* db;

    // Вспомогательная функция для выполнения SQL-запросов
    void executeQuery(const string& sql) {
        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "Ошибка SQL: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }
public:

    void printTable(const string& sql) {
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << endl;
            return;
        }

        // Получение количества столбцов
        int cols = sqlite3_column_count(stmt);

        // Вектор для хранения максимальной ширины каждого столбца
        vector<int> col_widths(cols, 0);

        // Первый проход: определяем максимальную ширину для каждого столбца
        // Заголовки столбцов
        for (int i = 0; i < cols; ++i) {
            const char* col_name = sqlite3_column_name(stmt, i);
            int width = string(col_name).length();
            if (width > col_widths[i]) {
                col_widths[i] = width;
            }
        }

        // Данные
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < cols; ++i) {
                const char* text = (const char*)sqlite3_column_text(stmt, i);
                if (text) {
                    int width = string(text).length();
                    if (width > col_widths[i]) {
                        col_widths[i] = width;
                    }
                }
            }
        }

        // Сброс курсора для повторного чтения данных
        sqlite3_reset(stmt);

        // Второй проход: вывод данных с правильным выравниванием

        // Вывод заголовков
        for (int i = 0; i < cols; ++i) {
            const char* col_name = sqlite3_column_name(stmt, i);
            cout << left << setw(col_widths[i] + 2) << col_name;
        }
        cout << endl;

        // Вывод разделительной линии
        for (int i = 0; i < cols; ++i) {
            cout << string(col_widths[i] + 2, '-');
        }
        cout << endl;

        // Вывод данных
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < cols; ++i) {
                const char* text = (const char*)sqlite3_column_text(stmt, i);
                cout << left << setw(col_widths[i] + 2) << (text ? text : "NULL");
            }
            cout << endl;
        }

        sqlite3_finalize(stmt);
    }

    // Конструктор - создает или открывает базу данных
    TourismDatabase(const string& dbName) {
        if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
            cerr << "Не удалось открыть базу данных: " << sqlite3_errmsg(db) << endl;
            exit(1);
        }
        cout << "База данных успешно открыта/создана." << endl;
    }
    // Деструктор - закрывает соединение с базой данных
    ~TourismDatabase() {
        sqlite3_close(db);
        cout << "Соединение с базой данных закрыто." << endl;
    }
    // Инициализация структуры базы данных
    void initializeDatabase() {
        // Включение поддержки внешних ключей
        executeQuery("PRAGMA foreign_keys = ON;");
        // Создание таблицы стран (справочник)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Countries ("
            "country_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL UNIQUE,"
            "visa_required BOOLEAN NOT NULL DEFAULT 0,"
            "description TEXT"
            ");"
        );
        // Создание таблицы типов туров (справочник)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS TourTypes ("
            "type_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "type_name TEXT NOT NULL UNIQUE,"
            "description TEXT"
            ");"
        );
        // Создание таблицы клиентов (справочник)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Clients ("
            "client_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "first_name TEXT NOT NULL,"
            "last_name TEXT NOT NULL,"
            "passport_number TEXT UNIQUE NOT NULL,"
            "phone TEXT,"
            "email TEXT"
            ");"
        );
        // Создание таблицы услуг (справочник)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Services ("
            "service_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "service_name TEXT NOT NULL,"
            "description TEXT,"
            "base_price REAL NOT NULL"
            ");"
        );
        // Создание таблицы туров (переменная информация)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Tours ("
            "tour_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "tour_name TEXT NOT NULL,"
            "country_id INTEGER NOT NULL,"
            "type_id INTEGER NOT NULL,"
            "start_date DATE NOT NULL,"
            "end_date DATE NOT NULL,"
            "price REAL NOT NULL,"
            "max_capacity INTEGER NOT NULL,"
            "FOREIGN KEY (country_id) REFERENCES Countries(country_id) ON DELETE RESTRICT,"
            "FOREIGN KEY (type_id) REFERENCES TourTypes(type_id) ON DELETE RESTRICT,"
            "CHECK (end_date > start_date),"
            "CHECK (price > 0),"
            "CHECK (max_capacity > 0)"
            ");"
        );
        // Создание таблицы заказов (переменная информация)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Orders ("
            "order_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "client_id INTEGER NOT NULL,"
            "tour_id INTEGER NOT NULL,"
            "order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
            "persons INTEGER NOT NULL DEFAULT 1,"
            "total_price REAL NOT NULL,"
            "status TEXT DEFAULT 'new',"
            "FOREIGN KEY (client_id) REFERENCES Clients(client_id) ON DELETE CASCADE,"
            "FOREIGN KEY (tour_id) REFERENCES Tours(tour_id) ON DELETE CASCADE,"


            "CHECK (persons > 0),"
            "CHECK (total_price > 0)"
            ");"
        );
        // Создание таблицы связи туров и услуг (многие-ко-многим)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS TourServices ("
            "tour_id INTEGER NOT NULL,"
            "service_id INTEGER NOT NULL,"
            "actual_price REAL NOT NULL,"
            "PRIMARY KEY (tour_id, service_id),"
            "FOREIGN KEY (tour_id) REFERENCES Tours(tour_id) ON DELETE CASCADE,"
            "FOREIGN KEY (service_id) REFERENCES Services(service_id) ON DELETE CASCADE"
            ");"
        );
        cout << "Структура базы данных успешно инициализирована." << endl;
    }
    // Метод для заполнения тестовыми данными
    void populateSampleData() {
        // Добавление стран
        executeQuery("INSERT OR IGNORE INTO Countries (name, visa_required, description) VALUES "
            "('Италия', 1, 'Романтические каникулы в сердце Европы'),"
            "('Турция', 0, 'Отдых на пляжах Средиземного и Эгейского морей'),"
            "('Япония', 1, 'Уникальное сочетание традиций и современных технологий'),"
            "('Мальдивы', 0, 'Райские острова с белоснежными пляжами');");
        // Добавление типов туров
        executeQuery("INSERT OR IGNORE INTO TourTypes (type_name, description) VALUES "
            "('Пляжный отдых', 'Отдых на море с отелем у пляжа'),"
            "('Экскурсионный', 'Познавательные туры с осмотром достопримечательностей'),"
            "('Горнолыжный', 'Туры в горнолыжные курорты'),"
            "('Круиз', 'Морские путешествия на круизных лайнерах');");
        // Добавление клиентов
        executeQuery("INSERT OR IGNORE INTO Clients (first_name, last_name, passport_number, phone, email) VALUES "
            "('Иван', 'Петров', 'AB1234567', '+375291234567', 'ivan.petrov@example.com'),"
            "('Анна', 'Сидорова', 'CD7654321', '+375297654321', 'anna.sidorova@example.com'),"
            "('Сергей', 'Иванов', 'EF9876543', '+375299876543', 'sergey.ivanov@example.com');");
        // Добавление услуг
        executeQuery("INSERT OR IGNORE INTO Services (service_name, description, base_price) VALUES "
            "('Страховка', 'Медицинская страховка на время поездки', 50.0),"
            "('Трансфер', 'Трансфер из аэропорта в отель и обратно', 80.0),"
            "('Экскурсия', 'Обзорная экскурсия по городу', 120.0),"
            "('VIP-обслуживание', 'Персональный гид и транспорт', 350.0);");
        cout << "Тестовые данные успешно добавлены в базу данных." << endl;
    }
    void demonstrateDatabase() {
        cout << "\nДемонстрация работы базы данных:\n" << endl;

        // Вывод списка стран (с сортировкой по country_id)
        cout << "Доступные страны:" << endl;
        printTable("SELECT country_id, name, visa_required FROM Countries ORDER BY country_id;");

        // Вывод типов туров (с сортировкой по type_id)
        cout << "\nТипы туров:" << endl;
        printTable("SELECT type_id, type_name FROM TourTypes ORDER BY type_id;");

        // Вывод клиентов (с сортировкой по client_id)
        cout << "\nЗарегистрированные клиенты:" << endl;
        printTable("SELECT client_id, first_name || ' ' || last_name AS full_name FROM Clients ORDER BY client_id;");

        // Можно также вывести пример туров с сортировкой:
        cout << "\nПримеры доступных туров:" << endl;
        printTable("SELECT t.tour_id, c.name as country, tt.type_name, t.start_date, t.end_date "
            "FROM Tours t "
            "JOIN Countries c ON t.country_id = c.country_id "
            "JOIN TourTypes tt ON t.type_id = tt.type_id "
            "ORDER BY t.start_date;");
    }
};
int main() {
    // Установка локали для корректного отображения кириллицы
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "RU");
    cout << "Создание базы данных для туристического агентства" << endl;

    // Создание и инициализация базы данных
    TourismDatabase tourismDB("tourism_agency.db");
    tourismDB.initializeDatabase();
    tourismDB.populateSampleData();
    tourismDB.demonstrateDatabase();
    cout << "\nБаза данных готова к использованию!" << endl;
    return 0;
}
