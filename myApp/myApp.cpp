#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <time.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;

const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "741852963n";
int CountLenOfFile(string FileName) {
    ifstream file;
    file.open(FileName);
    ifstream inFile(FileName);
    int c = count(istreambuf_iterator<char>(inFile),
        istreambuf_iterator<char>(), '\n');
    c++;
    file.close();
    return c;
}
string *ReadNames(string FileName, int c) {
    ifstream file;
    file.open(FileName);
    string word;
    string *name = new string[c];
    int k = 0;
    while (file >> word)
    {
        *(name + k) = word;
        k++;
        
    }
    file.close();
    return name;
}
int Random(int s,int e) {
    const int range_from = s;
    const int range_to = e;
    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<int>  distr(range_from, range_to);

    return distr(generator);
}

int main()
{
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::PreparedStatement* prep_stmt;
    sql::ResultSet* res;
    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
    con->setSchema("mybase");
    int input;
    cin >> input;
    switch (input)
    {
    case(1): {
        stmt = con->createStatement();
        stmt->execute("USE mybase");
        stmt->execute("DROP TABLE IF EXISTS users");
        stmt->execute("CREATE TABLE users (id MEDIUMINT NOT NULL AUTO_INCREMENT, name VARCHAR(50), birthdate date, sex VARCHAR(1),PRIMARY KEY (id));");
        //stmt->execute("CREATE TABLE users (name VARCHAR(50), birthdate date, sex VARCHAR(1));");
        cout << "table 'users' created\n";
        break;
    }
    case(2): {
        prep_stmt = con->prepareStatement("INSERT INTO users(name, birthdate,sex) VALUES (?, ?, ?)");
        string name, birthdate, sex;
        cin >> name >> birthdate >> sex;
        prep_stmt->setString(1, name);
        prep_stmt->setString(2, birthdate);
        prep_stmt->setString(3, sex);
        prep_stmt->execute();
        break;
    }
    case(3): {
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT name,birthdate, sex,TIMESTAMPDIFF(YEAR, birthdate, CURDATE()) AS years_old FROM users GROUP BY name,birthdate, sex ORDER BY name;");
        while (res->next()) {
            cout << res->getString("name") << " " << res->getString("birthdate") << " " << res->getString("sex") << " " << res->getString("years_old") << " " << endl;
        }
        break;
    case(4): {
        int c[3] = { CountLenOfFile("LastNames.txt"), CountLenOfFile("MaleNames.txt"),CountLenOfFile("FemNames.txt") };
        string* LastNames = new string[c[0]];
        LastNames = ReadNames("LastNames.txt", c[0]);
        string* MaleNames = new string[c[1]];
        MaleNames = ReadNames("MaleNames.txt", c[1]);
        string* FemNames = new string[c[2]];
        FemNames = ReadNames("FemNames.txt", c[2]);
        int s;
        for (int i = 0; i < 1000000; i++) {
            //stmt = con->createStatement();
            s = Random(0, 1);
            prep_stmt = con->prepareStatement("INSERT INTO users(name, birthdate,sex) VALUES (?, (SELECT CURRENT_DATE - INTERVAL FLOOR(RAND() * 100 *360) DAY AS BIRTHDATE), ?)");
            if (s == 0) {
                prep_stmt->setString(1, LastNames[Random(0, c[0] - 1)] + " " + MaleNames[Random(0, c[1] - 1)]);
                prep_stmt->setString(2, "m");
                prep_stmt->execute();
            }
            else {
                prep_stmt->setString(1, LastNames[Random(0, c[0] - 1)] + " " + FemNames[Random(0, c[2] - 1)]);
                prep_stmt->setString(2, "f");
                prep_stmt->execute();
            }
            delete prep_stmt;
        }
        string* LastNames_MalesF = new string[c[0]]; //выбираем все фамилии на F
        int r = 0;
        for (int i = 0; i < c[0]; i++) {
            string Last = LastNames[Random(0, (c[0] - 1))];
            char k = Last[0];
            if (k == 'F') {
                LastNames_MalesF[r] = Last;
                r++;
            }
        }
        for (int i = 0; i < 100; i++) { // 100 мужчин с фамилией на F
            prep_stmt = con->prepareStatement("INSERT INTO users(name, birthdate,sex) VALUES (?, (SELECT CURRENT_DATE - INTERVAL FLOOR(RAND() * 100 *360) DAY AS BIRTHDATE), ?)");
            prep_stmt->setString(1, LastNames_MalesF[Random(0, r-1)] + " " + MaleNames[Random(0, c[1]-1)]);
            prep_stmt->setString(2, "m");
            prep_stmt->execute();
        }

        break;
    }
    case(5): {

        stmt = con->createStatement();
        clock_t tStart = clock();
        res = stmt->executeQuery("SELECT * FROM users WHERE name LIKE 'F%' AND sex LIKE 'm';");
        while (res->next()) {
            cout << res->getString("name") << " " << res->getString("birthdate") << " " << res->getString("sex") << endl;
        }
        cout << endl;
        cout<< "Time taken: "<<(double)(clock() - tStart) / CLOCKS_PER_SEC<<endl;
        delete res;
        break;
    }

    }
    default:
        break;
    }
    system("PAUSE");
}