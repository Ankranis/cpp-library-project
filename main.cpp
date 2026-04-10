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

    void addBook(string i, string t, string a) {
        books.push_back(Book("", i, t, a, "Available"));
        saveToFile();
        output = "Book Added";
    }

    void issueBook(string r, string i) {
        for (auto &b : books) {
            if (b.getId() == i) {
                if (b.getStatus() == "Available") {
                    b.setStatus("Issued");
                    b = Book(r, b.getId(), b.getTitle(), b.getAuthor(), "Issued");
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
            if (b.getId() == i && b.getRegNo() == r) {
                b.setStatus("Available");
                b = Book("", b.getId(), b.getTitle(), b.getAuthor(), "Available");
                saveToFile();
                output = "Book Returned";
                return;
            }
        }
        output = "Book Not Found";
    }

    string showAll() {
        string res;
        for (auto &b : books) {
            res += b.getId() + " | " + b.getTitle() + " | " + b.getAuthor() + " | " + b.getStatus() + "\n";
        }
        return res.empty() ? "No Books" : res;
    }

    string showCount() {
        int issued = 0, available = 0;

        for (auto &b : books) {
            if (b.getStatus() == "Issued") issued++;
            else available++;
        }

        return "Issued: " + to_string(issued) + "\nAvailable: " + to_string(available);
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
    }
};

Library lib;

/* ================= WASM ================= */

extern "C" {

void addBook(const char* i, const char* t, const char* a) {
    lib.addBook(i,t,a);
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

/* ================= MENU DRIVEN ================= */

int main() {
    int choice;
    string r,i,t,a;

    do {
        cout << "\n1.Add 2.Issue 3.Return 4.Show 5.Count 0.Exit\n";
        cin >> choice;

        switch(choice) {
            case 1:
                cout << "ID Title Author:";
                cin >> i >> t >> a;
                lib.addBook(i,t,a);
                break;

            case 2:
                cout << "Reg ID:";
                cin >> r >> i;
                lib.issueBook(r,i);
                break;

            case 3:
                cout << "Reg ID:";
                cin >> r >> i;
                lib.returnBook(r,i);
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