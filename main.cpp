#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <emscripten/emscripten.h>
using namespace std;

stringstream buffer;

void printToBuffer(string text) {
    buffer << text << endl;
}

class Book {
private:
    string bookID;
    string title;
    string author;
    string status;
    string issuedTo;

public:
    Book() {}

    Book(string id, string t, string a) {
        bookID = id;
        title = t;
        author = a;
        status = "Available";
        issuedTo = "";
    }

    string getID() { return bookID; }
    string getTitle() { return title; }
    string getAuthor() { return author; }
    string getStatus() { return status; }
    string getIssuedTo() { return issuedTo; }

    void issue(string regNo) {
        status = "Issued";
        issuedTo = regNo;
    }

    void returnBook() {
        status = "Available";
        issuedTo = "";
    }

    void display() {
        printToBuffer(bookID + " | " + title + " | " + author + " | " + status + (status == "Issued" ? (" | " + issuedTo) : ""));
    }
};

class Library {
private:
    vector<Book> books;
    string filename = "library.txt";

public:
    Library() {
        loadFromFile();
    }

    void saveToFile() {
        ofstream file(filename);
        for (auto &b : books) {
            file << b.getID() << "," << b.getTitle() << "," << b.getAuthor() << ","
                 << b.getStatus() << "," << b.getIssuedTo() << endl;
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file) return;

        string id, title, author, status, issuedTo;
        while (getline(file, id, ',')) {
            getline(file, title, ',');
            getline(file, author, ',');
            getline(file, status, ',');
            getline(file, issuedTo);

            Book b(id, title, author);
            if (status == "Issued") b.issue(issuedTo);
            books.push_back(b);
        }
        file.close();
    }

    bool exists(string id) {
        for (auto &b : books) {
            if (b.getID() == id) return true;
        }
        return false;
    }

    void addBook(string id, string title, string author) {
        if (exists(id)) {
            printToBuffer("Duplicate Book ID not allowed");
            return;
        }
        books.push_back(Book(id, title, author));
        saveToFile();
        printToBuffer("Book Added");
    }

    void issueBook(string id, string regNo) {
        for (auto &b : books) {
            if (b.getID() == id) {
                if (b.getStatus() == "Available") {
                    b.issue(regNo);
                    saveToFile();
                    printToBuffer("Book Issued");
                } else {
                    printToBuffer("Book already issued");
                }
                return;
            }
        }
        printToBuffer("Book not found");
    }

    void returnBook(string id) {
        for (auto &b : books) {
            if (b.getID() == id) {
                if (b.getStatus() == "Issued") {
                    b.returnBook();
                    saveToFile();
                    printToBuffer("Book Returned");
                } else {
                    printToBuffer("Book already available");
                }
                return;
            }
        }
        printToBuffer("Book not found");
    }

    void search(string keyword) {
        for (auto &b : books) {
            if (b.getID() == keyword || b.getTitle().find(keyword) != string::npos) {
                b.display();
            }
        }
    }

    void showIssued() {
        for (auto &b : books) {
            if (b.getStatus() == "Issued") {
                b.display();
            }
        }
    }

    void showCount() {
        int issued = 0, available = 0;
        for (auto &b : books) {
            if (b.getStatus() == "Issued") issued++;
            else available++;
        }
        printToBuffer("Issued: " + to_string(issued));
        printToBuffer("Available: " + to_string(available));
    }
};

Library lib;

extern "C" {

void addBook(const char* id, const char* title, const char* author) {
    lib.addBook(id, title, author);
}

void issueBook(const char* id, const char* regNo) {
    lib.issueBook(id, regNo);
}

void returnBook(const char* id) {
    lib.returnBook(id);
}

void searchBook(const char* keyword) {
    lib.search(keyword);
}

void showIssued() {
    lib.showIssued();
}

void showCount() {
    lib.showCount();
}

void getOutput(char* output) {
    string temp = buffer.str();
    strcpy(output, temp.c_str());
    buffer.str("");
}

}