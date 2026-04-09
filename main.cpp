#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Book {
private:
    string regNo;
    string id;
    string title;
    string author;
    string status;

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
    string output = "";

public:
    Library() {
        loadFromFile();
    }

    void addBook(string reg, string id, string title, string author) {
        books.push_back(Book(reg, id, title, author, "Available"));
        saveToFile();
        output = "Book Added Successfully";
    }

    void issueBook(string reg, string id) {
        for (auto &b : books) {
            if (b.getRegNo() == reg && b.getId() == id) {
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

    void returnBook(string reg, string id) {
        for (auto &b : books) {
            if (b.getRegNo() == reg && b.getId() == id) {
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
        string res = "";
        for (auto &b : books) {
            if (b.getStatus() == "Available") {
                res += b.getRegNo() + " | " + b.getId() + " | " + b.getTitle() + " | " + b.getAuthor() + "\n";
            }
        }
        return res;
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
            file << b.getRegNo() << "," << b.getId() << "," << b.getTitle() << "," << b.getAuthor() << "," << b.getStatus() << endl;
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file) return;

        string reg, id, title, author, status;

        while (getline(file, reg, ',')) {
            getline(file, id, ',');
            getline(file, title, ',');
            getline(file, author, ',');
            getline(file, status);

            books.push_back(Book(reg, id, title, author, status));
        }

        file.close();
    }
};

Library lib;

extern "C" {

void addBook(const char* reg, const char* id, const char* title, const char* author) {
    lib.addBook(reg, id, title, author);
}

void issueBook(const char* reg, const char* id) {
    lib.issueBook(reg, id);
}

void returnBook(const char* reg, const char* id) {
    lib.returnBook(reg, id);
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

int main() {
    Library temp;
    return 0;
}