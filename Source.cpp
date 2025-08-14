#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>
#include <Windows.h>
#include <iomanip>
using namespace std;
// ����� ��� ������ � ����� ������ �������������� ���������
class TourismDatabase {
private:
    sqlite3* db;

    // ��������������� ������� ��� ���������� SQL-��������
    void executeQuery(const string& sql) {
        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "������ SQL: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }
public:

    void printTable(const string& sql) {
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "������ ���������� �������: " << sqlite3_errmsg(db) << endl;
            return;
        }

        // ��������� ���������� ��������
        int cols = sqlite3_column_count(stmt);

        // ������ ��� �������� ������������ ������ ������� �������
        vector<int> col_widths(cols, 0);

        // ������ ������: ���������� ������������ ������ ��� ������� �������
        // ��������� ��������
        for (int i = 0; i < cols; ++i) {
            const char* col_name = sqlite3_column_name(stmt, i);
            int width = string(col_name).length();
            if (width > col_widths[i]) {
                col_widths[i] = width;
            }
        }

        // ������
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

        // ����� ������� ��� ���������� ������ ������
        sqlite3_reset(stmt);

        // ������ ������: ����� ������ � ���������� �������������

        // ����� ����������
        for (int i = 0; i < cols; ++i) {
            const char* col_name = sqlite3_column_name(stmt, i);
            cout << left << setw(col_widths[i] + 2) << col_name;
        }
        cout << endl;

        // ����� �������������� �����
        for (int i = 0; i < cols; ++i) {
            cout << string(col_widths[i] + 2, '-');
        }
        cout << endl;

        // ����� ������
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < cols; ++i) {
                const char* text = (const char*)sqlite3_column_text(stmt, i);
                cout << left << setw(col_widths[i] + 2) << (text ? text : "NULL");
            }
            cout << endl;
        }

        sqlite3_finalize(stmt);
    }

    // ����������� - ������� ��� ��������� ���� ������
    TourismDatabase(const string& dbName) {
        if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
            cerr << "�� ������� ������� ���� ������: " << sqlite3_errmsg(db) << endl;
            exit(1);
        }
        cout << "���� ������ ������� �������/�������." << endl;
    }
    // ���������� - ��������� ���������� � ����� ������
    ~TourismDatabase() {
        sqlite3_close(db);
        cout << "���������� � ����� ������ �������." << endl;
    }
    // ������������� ��������� ���� ������
    void initializeDatabase() {
        // ��������� ��������� ������� ������
        executeQuery("PRAGMA foreign_keys = ON;");
        // �������� ������� ����� (����������)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Countries ("
            "country_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL UNIQUE,"
            "visa_required BOOLEAN NOT NULL DEFAULT 0,"
            "description TEXT"
            ");"
        );
        // �������� ������� ����� ����� (����������)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS TourTypes ("
            "type_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "type_name TEXT NOT NULL UNIQUE,"
            "description TEXT"
            ");"
        );
        // �������� ������� �������� (����������)
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
        // �������� ������� ����� (����������)
        executeQuery(
            "CREATE TABLE IF NOT EXISTS Services ("
            "service_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "service_name TEXT NOT NULL,"
            "description TEXT,"
            "base_price REAL NOT NULL"
            ");"
        );
        // �������� ������� ����� (���������� ����������)
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
        // �������� ������� ������� (���������� ����������)
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
        // �������� ������� ����� ����� � ����� (������-��-������)
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
        cout << "��������� ���� ������ ������� ����������������." << endl;
    }
    // ����� ��� ���������� ��������� �������
    void populateSampleData() {
        // ���������� �����
        executeQuery("INSERT OR IGNORE INTO Countries (name, visa_required, description) VALUES "
            "('������', 1, '������������� �������� � ������ ������'),"
            "('������', 0, '����� �� ������ ������������ � ��������� �����'),"
            "('������', 1, '���������� ��������� �������� � ����������� ����������'),"
            "('��������', 0, '������� ������� � ������������ �������');");
        // ���������� ����� �����
        executeQuery("INSERT OR IGNORE INTO TourTypes (type_name, description) VALUES "
            "('������� �����', '����� �� ���� � ������ � �����'),"
            "('�������������', '�������������� ���� � �������� ����������������������'),"
            "('�����������', '���� � ����������� �������'),"
            "('�����', '������� ����������� �� �������� ��������');");
        // ���������� ��������
        executeQuery("INSERT OR IGNORE INTO Clients (first_name, last_name, passport_number, phone, email) VALUES "
            "('����', '������', 'AB1234567', '+375291234567', 'ivan.petrov@example.com'),"
            "('����', '��������', 'CD7654321', '+375297654321', 'anna.sidorova@example.com'),"
            "('������', '������', 'EF9876543', '+375299876543', 'sergey.ivanov@example.com');");
        // ���������� �����
        executeQuery("INSERT OR IGNORE INTO Services (service_name, description, base_price) VALUES "
            "('���������', '����������� ��������� �� ����� �������', 50.0),"
            "('��������', '�������� �� ��������� � ����� � �������', 80.0),"
            "('���������', '�������� ��������� �� ������', 120.0),"
            "('VIP-������������', '������������ ��� � ���������', 350.0);");
        cout << "�������� ������ ������� ��������� � ���� ������." << endl;
    }
    void demonstrateDatabase() {
        cout << "\n������������ ������ ���� ������:\n" << endl;

        // ����� ������ ����� (� ����������� �� country_id)
        cout << "��������� ������:" << endl;
        printTable("SELECT country_id, name, visa_required FROM Countries ORDER BY country_id;");

        // ����� ����� ����� (� ����������� �� type_id)
        cout << "\n���� �����:" << endl;
        printTable("SELECT type_id, type_name FROM TourTypes ORDER BY type_id;");

        // ����� �������� (� ����������� �� client_id)
        cout << "\n������������������ �������:" << endl;
        printTable("SELECT client_id, first_name || ' ' || last_name AS full_name FROM Clients ORDER BY client_id;");

        // ����� ����� ������� ������ ����� � �����������:
        cout << "\n������� ��������� �����:" << endl;
        printTable("SELECT t.tour_id, c.name as country, tt.type_name, t.start_date, t.end_date "
            "FROM Tours t "
            "JOIN Countries c ON t.country_id = c.country_id "
            "JOIN TourTypes tt ON t.type_id = tt.type_id "
            "ORDER BY t.start_date;");
    }
};
int main() {
    // ��������� ������ ��� ����������� ����������� ���������
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "RU");
    cout << "�������� ���� ������ ��� �������������� ���������" << endl;

    // �������� � ������������� ���� ������
    TourismDatabase tourismDB("tourism_agency.db");
    tourismDB.initializeDatabase();
    tourismDB.populateSampleData();
    tourismDB.demonstrateDatabase();
    cout << "\n���� ������ ������ � �������������!" << endl;
    return 0;
}