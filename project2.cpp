#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>
#include <Windows.h>
#include <iomanip>
using namespace std;
// Êëàññ äëÿ ðàáîòû ñ áàçîé äàííûõ òóðèñòè÷åñêîãî àãåíòñòâà
class TourismDatabase {
private:
    sqlite3* db;

    // Âñïîìîãàòåëüíàÿ ôóíêöèÿ äëÿ âûïîëíåíèÿ SQL-çàïðîñîâ
    void executeQuery(const string& sql) {
        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "Îøèáêà SQL: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }
public:

    void printTable(const string& sql) {
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Îøèáêà ïîäãîòîâêè çàïðîñà: " << sqlite3_errmsg(db) << endl;
            return;
        }

        // Ïîëó÷åíèå êîëè÷åñòâà ñòîëáöîâ
        int cols = sqlite3_column_count(stmt);

        // Âåêòîð äëÿ õðàíåíèÿ ìàêñèìàëüíîé øèðèíû êàæäîãî ñòîëáöà
        vector<int> col_widths(cols, 0);

        // Ïåðâûé ïðîõîä: îïðåäåëÿåì ìàêñèìàëüíóþ øèðèíó äëÿ êàæäîãî ñòîëáöà
        // Çàãîëîâêè ñòîëáöîâ
        for (int i = 0; i < cols; ++i) {
            const char* col_name = sqlite3_column_name(stmt, i);
            int width = string(col_name).length();
            if (width > col_widths[i]) {
                col_widths[i] = width;
            }
        }

        // Äàííûå
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

        // Ñáðîñ êóðñîðà äëÿ ïîâòîðíîãî ÷òåíèÿ äàííûõ
        sqlite3_reset(stmt);

        // Âòîðîé ïðîõîä: âûâîä äàííûõ ñ ïðàâèëüíûì âûðàâíèâàíèåì

        // Âûâîä çàãîëîâêîâ
        for (int i = 0; i < cols; ++i) {
            const char* col_name = sqlite3_column_name(stmt, i);
            cout << left << setw(col_widths[i] + 2) << col_name;
        }
        cout << endl;

        // Âûâîä ðàçäåëèòåëüíîé ëèíèè
        for (int i = 0; i < cols; ++i) {
            cout << string(col_widths[i] + 2, '-');
        }
        cout << endl;

        // Âûâîä äàííûõ
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < cols; ++i) {
                const char* text = (const char*)sqlite3_column_text(stmt, i);
                cout << left << setw(col_widths[i] + 2) << (text ? text : "NULL");
            }
            cout << endl;
        }

        sqlite3_finalize(stmt);
    }

    // Êîíñòðóêòîð - ñîçäàåò èëè îòêðûâàåò áàçó äàííûõ
    TourismDatabase(const string& dbName) {
        if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
            cerr << "Íå óäàëîñü îòêðûòü áàçó äàííûõ: " << sqlite3_errmsg(db) << endl;
            exit(1);
        }
        cout << "Áàçà äàííûõ óñïåøíî îòêðûòà/ñîçäàíà." << endl;
    }
    // Äåñòðóêòîð - çàêðûâàåò ñîåäèíåíèå ñ áàçîé äàííûõ
    ~TourismDatabase() {
        sqlite3_close(db);
        cout << "Ñîåäèíåíèå ñ áàçîé äàííûõ çàêðûòî." << endl;
    }
    // Èíèöèàëèçàöèÿ ñòðóêòóðû áàçû äàííûõ
    void initializeDatabase() {
        // Âêëþ÷åíèå ïîääåðæêè âíåøíèõ êëþ÷åé
        executeQuery("PRAGMA foreign_keys = ON;");
        // Ñîçäàíèå òàáëèöû ñòðàí (ñïðàâî÷íèê)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Countries ("
            "country_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL UNIQUE,"
            "visa_required BOOLEAN NOT NULL DEFAULT 0,"
            "description TEXT"
            ");"
        );
        // Ñîçäàíèå òàáëèöû òèïîâ òóðîâ (ñïðàâî÷íèê)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS TourTypes ("
            "type_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "type_name TEXT NOT NULL UNIQUE,"
            "description TEXT"
            ");"
        );
        // Ñîçäàíèå òàáëèöû êëèåíòîâ (ñïðàâî÷íèê)
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
        // Ñîçäàíèå òàáëèöû óñëóã (ñïðàâî÷íèê)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Services ("
            "service_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "service_name TEXT NOT NULL,"
            "description TEXT,"
            "base_price REAL NOT NULL"
            ");"
        );
        // Ñîçäàíèå òàáëèöû òóðîâ (ïåðåìåííàÿ èíôîðìàöèÿ)
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
        // Ñîçäàíèå òàáëèöû çàêàçîâ (ïåðåìåííàÿ èíôîðìàöèÿ)
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
        // Ñîçäàíèå òàáëèöû ñâÿçè òóðîâ è óñëóã (ìíîãèå-êî-ìíîãèì)
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
        cout << "Ñòðóêòóðà áàçû äàííûõ óñïåøíî èíèöèàëèçèðîâàíà." << endl;
    }
    // Ìåòîä äëÿ çàïîëíåíèÿ òåñòîâûìè äàííûìè
    void populateSampleData() {
        // Äîáàâëåíèå ñòðàí
        executeQuery("INSERT OR IGNORE INTO Countries (name, visa_required, description) VALUES "
            "('Èòàëèÿ', 1, 'Ðîìàíòè÷åñêèå êàíèêóëû â ñåðäöå Åâðîïû'),"
            "('Òóðöèÿ', 0, 'Îòäûõ íà ïëÿæàõ Ñðåäèçåìíîãî è Ýãåéñêîãî ìîðåé'),"
            "('ßïîíèÿ', 1, 'Óíèêàëüíîå ñî÷åòàíèå òðàäèöèé è ñîâðåìåííûõ òåõíîëîãèé'),"
            "('Ìàëüäèâû', 0, 'Ðàéñêèå îñòðîâà ñ áåëîñíåæíûìè ïëÿæàìè');");
        // Äîáàâëåíèå òèïîâ òóðîâ
        executeQuery("INSERT OR IGNORE INTO TourTypes (type_name, description) VALUES "
            "('Ïëÿæíûé îòäûõ', 'Îòäûõ íà ìîðå ñ îòåëåì ó ïëÿæà'),"
            "('Ýêñêóðñèîííûé', 'Ïîçíàâàòåëüíûå òóðû ñ îñìîòðîì äîñòîïðèìå÷àòåëüíîñòåé'),"
            "('Ãîðíîëûæíûé', 'Òóðû â ãîðíîëûæíûå êóðîðòû'),"
            "('Êðóèç', 'Ìîðñêèå ïóòåøåñòâèÿ íà êðóèçíûõ ëàéíåðàõ');");
        // Äîáàâëåíèå êëèåíòîâ
        executeQuery("INSERT OR IGNORE INTO Clients (first_name, last_name, passport_number, phone, email) VALUES "
            "('Èâàí', 'Ïåòðîâ', 'AB1234567', '+375291234567', 'ivan.petrov@example.com'),"
            "('Àííà', 'Ñèäîðîâà', 'CD7654321', '+375297654321', 'anna.sidorova@example.com'),"
            "('Ñåðãåé', 'Èâàíîâ', 'EF9876543', '+375299876543', 'sergey.ivanov@example.com');");
        // Äîáàâëåíèå óñëóã
        executeQuery("INSERT OR IGNORE INTO Services (service_name, description, base_price) VALUES "
            "('Ñòðàõîâêà', 'Ìåäèöèíñêàÿ ñòðàõîâêà íà âðåìÿ ïîåçäêè', 50.0),"
            "('Òðàíñôåð', 'Òðàíñôåð èç àýðîïîðòà â îòåëü è îáðàòíî', 80.0),"
            "('Ýêñêóðñèÿ', 'Îáçîðíàÿ ýêñêóðñèÿ ïî ãîðîäó', 120.0),"
            "('VIP-îáñëóæèâàíèå', 'Ïåðñîíàëüíûé ãèä è òðàíñïîðò', 350.0);");
        cout << "Òåñòîâûå äàííûå óñïåøíî äîáàâëåíû â áàçó äàííûõ." << endl;
    }
    void demonstrateDatabase() {
        cout << "\nÄåìîíñòðàöèÿ ðàáîòû áàçû äàííûõ:\n" << endl;

        // Âûâîä ñïèñêà ñòðàí (ñ ñîðòèðîâêîé ïî country_id)
        cout << "Äîñòóïíûå ñòðàíû:" << endl;
        printTable("SELECT country_id, name, visa_required FROM Countries ORDER BY country_id;");

        // Âûâîä òèïîâ òóðîâ (ñ ñîðòèðîâêîé ïî type_id)
        cout << "\nÒèïû òóðîâ:" << endl;
        printTable("SELECT type_id, type_name FROM TourTypes ORDER BY type_id;");

        // Âûâîä êëèåíòîâ (ñ ñîðòèðîâêîé ïî client_id)
        cout << "\nÇàðåãèñòðèðîâàííûå êëèåíòû:" << endl;
        printTable("SELECT client_id, first_name || ' ' || last_name AS full_name FROM Clients ORDER BY client_id;");

        // Ìîæíî òàêæå âûâåñòè ïðèìåð òóðîâ ñ ñîðòèðîâêîé:
        cout << "\nÏðèìåðû äîñòóïíûõ òóðîâ:" << endl;
        printTable("SELECT t.tour_id, c.name as country, tt.type_name, t.start_date, t.end_date "
            "FROM Tours t "
            "JOIN Countries c ON t.country_id = c.country_id "
            "JOIN TourTypes tt ON t.type_id = tt.type_id "
            "ORDER BY t.start_date;");
    }
};
int main() {
    // Óñòàíîâêà ëîêàëè äëÿ êîððåêòíîãî îòîáðàæåíèÿ êèðèëëèöû
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "RU");
    cout << "Ñîçäàíèå áàçû äàííûõ äëÿ òóðèñòè÷åñêîãî àãåíòñòâà" << endl;

    // Ñîçäàíèå è èíèöèàëèçàöèÿ áàçû äàííûõ
    TourismDatabase tourismDB("tourism_agency.db");
    tourismDB.initializeDatabase();
    tourismDB.populateSampleData();
    tourismDB.demonstrateDatabase();
    cout << "\nÁàçà äàííûõ ãîòîâà ê èñïîëüçîâàíèþ!" << endl;
    return 0;
}
