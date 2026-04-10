#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Book {
private:
    string regNo, id, title, author, status;

public:
    Book(string r, string i, string t, string a, string s) {
        regNo = r;
        id = i;
        title = t;
        author = a;
        status = s;
    }

    string getRegNo() { return regNo; }
    string getId() { return id; }
    string getTitle() { return title; }
    string getAuthor() { return author; }
    string getStatus() { return status; }

    void setStatus(string s) { status = s; }
};

class Library {
private:
    vector<Book> books;
    string filename = "library.txt";
    string output;

public:
    Library() {
        loadFromFile();
    }

    void addBook(string r, string i, string t, string a) {
        books.push_back(Book(r, i, t, a, "Available"));
        saveToFile();
        output = "Book Added";
    }

    void issueBook(string r, string i) {
        for (auto &b : books) {
            if (b.getRegNo() == r && b.getId() == i) {
                if (b.getStatus() == "Available") {
                    b.setStatus("Issued");
                    saveToFile();
                    output = "Book Issued";
                } else {
                    output = "Already Issued";
                }
                return;
            }
        }
        output = "Book Not Found";
    }

    void returnBook(string r, string i) {
        for (auto &b : books) {
            if (b.getRegNo() == r && b.getId() == i) {
                if (b.getStatus() == "Issued") {
                    b.setStatus("Available");
                    saveToFile();
                    output = "Book Returned";
                } else {
                    output = "Already Available";
                }
                return;
            }
        }
        output = "Book Not Found";
    }

    string showAll() {
        string res;

        res += "=== AVAILABLE BOOKS ===\n";
        for (auto &b : books) {
            if (b.getStatus() == "Available") {
                res += b.getId() + " | " + b.getTitle() + " | " + b.getAuthor() + "\n";
            }
        }

        res += "\n=== ISSUED BOOKS ===\n";
        for (auto &b : books) {
            if (b.getStatus() == "Issued") {
                res += b.getId() + " | " + b.getTitle() + " | " + b.getRegNo() + "\n";
            }
        }

        if (res.empty()) return "No Books Found";
        return res;
    }

    string showCount() {
        int issued = 0, available = 0;

        for (auto &b : books) {
            if (b.getStatus() == "Issued") issued++;
            else available++;
        }

        return "Issued: " + to_string(issued) +
               "\nAvailable: " + to_string(available);
    }

    string getOutput() {
        return output;
    }

    void saveToFile() {
        ofstream file(filename);

        for (auto &b : books) {
            file << b.getRegNo() << ","
                 << b.getId() << ","
                 << b.getTitle() << ","
                 << b.getAuthor() << ","
                 << b.getStatus() << "\n";
        }

        file.close();
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file) return;

        string r,i,t,a,s;

        while (getline(file, r, ',')) {
            getline(file, i, ',');
            getline(file, t, ',');
            getline(file, a, ',');
            getline(file, s);

            books.push_back(Book(r,i,t,a,s));
        }

        file.close();
    }
};

Library lib;

/* ---------------- WASM FUNCTIONS ---------------- */

extern "C" {

void addBook(const char* r, const char* i, const char* t, const char* a) {
    lib.addBook(r,i,t,a);
}

void issueBook(const char* r, const char* i) {
    lib.issueBook(r,i);
}

void returnBook(const char* r, const char* i) {
    lib.returnBook(r,i);
}

const char* showAll() {
    static string res;
    res = lib.showAll();
    return res.c_str();
}

const char* showCount() {
    static string res;
    res = lib.showCount();
    return res.c_str();
}

const char* getOutput() {
    static string res;
    res = lib.getOutput();
    return res.c_str();
}

}

/* ---------------- MENU DRIVEN PART (VIVA REQUIREMENT) ---------------- */

int main() {
    int choice;
    string r,i,t,a;

    do {
        cout << "\n===== LIBRARY MENU =====\n";
        cout << "1. Add Book\n";
        cout << "2. Issue Book\n";
        cout << "3. Return Book\n";
        cout << "4. Show All\n";
        cout << "5. Show Count\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {

            case 1:
                cout << "Reg No: "; cin >> r;
                cout << "Book ID: "; cin >> i;
                cout << "Title: "; cin >> t;
                cout << "Author: "; cin >> a;
                lib.addBook(r,i,t,a);
                cout << "Added\n";
                break;

            case 2:
                cout << "Reg No: "; cin >> r;
                cout << "Book ID: "; cin >> i;
                lib.issueBook(r,i);
                cout << "Issued\n";
                break;

            case 3:
                cout << "Reg No: "; cin >> r;
                cout << "Book ID: "; cin >> i;
                lib.returnBook(r,i);
                cout << "Returned\n";
                break;

            case 4:
                cout << lib.showAll();
                break;

            case 5:
                cout << lib.showCount();
                break;
        }

    } while(choice != 0);

    return 0;
}