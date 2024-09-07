
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Book {
    int id;
    string title;
    string author;
    bool isIssued;
    string issuedTo;
    Book* next;
};

class Library {
private:
    Book* head;
    queue<Book*> issuedBooks;

public:
    Library() : head(nullptr) {
        loadBooksFromFile();
    }

    ~Library() {
        saveBooksToFile();
        clearList();
    }

    void addBook(int id, string title, string author) {
        Book* newBook = new Book{id, title, author, false, "", nullptr};
        if (!head) {
            head = newBook;
        } else {
            Book* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newBook;
        }
        cout << "Book added successfully." << endl;
    }

    void searchBookById(int id) const {
        Book* book = findBookById(id);
        if (book) {
            displayBook(book);
        } else {
            cout << "Book not found." << endl;
        }
    }

    void searchBookByTitle(const string& title) const {
        Book* book = findBookByTitle(title);
        if (book) {
            displayBook(book);
        } else {
            cout << "Book not found." << endl;
        }
    }

    void issueBook(int id, const string& studentName) {
        Book* book = findBookById(id);
        if (book) {
            if (!book->isIssued) {
                book->isIssued = true;
                book->issuedTo = studentName;
                issuedBooks.push(book);
                cout << "Book issued successfully to " << studentName << "." << endl;
            } else {
                cout << "Book is already issued to " << book->issuedTo << "." << endl;
            }
        } else {
            cout << "Book not found." << endl;
        }
    }

    void returnBook(int id) {
        Book* book = findBookById(id);
        if (book) {
            if (book->isIssued) {
                book->isIssued = false;
                book->issuedTo = "";
                cout << "Book returned successfully." << endl;
            } else {
                cout << "Book is not issued." << endl;
            }
        } else {
            cout << "Book not found." << endl;
        }
    }

    void listAllBooks() const {
        vector<Book*> bookList = getAllBooks();

        sort(bookList.begin(), bookList.end(), [](Book* a, Book* b) {
            return a->id < b->id;
        });

        for (const auto& book : bookList) {
            displayBook(book);
        }
    }

    void removeBook(int id) {
        if (!head) {
            cout << "Book not found." << endl;
            return;
        }

        if (head->id == id) {
            Book* toDelete = head;
            head = head->next;
            delete toDelete;
            cout << "Book removed successfully." << endl;
            return;
        }

        Book* temp = head;
        while (temp->next && temp->next->id != id) {
            temp = temp->next;
        }

        if (!temp->next) {
            cout << "Book not found." << endl;
        } else {
            Book* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
            cout << "Book removed successfully." << endl;
        }
    }

private:
    Book* findBookById(int id) const {
        Book* temp = head;
        while (temp) {
            if (temp->id == id) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    Book* findBookByTitle(const string& title) const {
        Book* temp = head;
        while (temp) {
            if (temp->title == title) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    vector<Book*> getAllBooks() const {
        vector<Book*> bookList;
        Book* temp = head;
        while (temp) {
            bookList.push_back(temp);
            temp = temp->next;
        }
        return bookList;
    }

    void displayBook(Book* book) const {
        cout << "ID: " << book->id << ", Title: " << book->title << ", Author: " << book->author
            << ", Status: " << (book->isIssued ? "Issued" : "Available") << endl;
        if (book->isIssued) {
            cout << "Issued to: " << book->issuedTo << endl;
        }
    }

    void loadBooksFromFile() {
        ifstream inFile("librarybooks.txt");
        if (inFile.is_open()) {
            int id;
            string title;
            string author;
            bool isIssued;
            string issuedTo;

            while (inFile >> id) {
                inFile.ignore(); // Ignore newline
                getline(inFile, title);
                getline(inFile, author);
                inFile >> isIssued;
                inFile.ignore(); // Ignore newline
                getline(inFile, issuedTo);

                Book* newBook = new Book{id, title, author, isIssued, issuedTo, nullptr};
                if (!head) {
                    head = newBook;
                } else {
                    Book* temp = head;
                    while (temp->next) {
                        temp = temp->next;
                    }
                    temp->next = newBook;
                }

                if (isIssued) {
                    issuedBooks.push(newBook);
                }
            }
            inFile.close();
        }
    }

    void saveBooksToFile() const {
        ofstream outFile("librarybooks.txt");
        if (outFile.is_open()) {
            Book* temp = head;
            while (temp) {
                outFile << temp->id << '\n'
                        << temp->title << '\n'
                        << temp->author << '\n'
                        << temp->isIssued << '\n'
                        << temp->issuedTo << '\n';
                temp = temp->next;
            }
            outFile.close();
        }
    }

    void clearList() {
        while (head) {
            Book* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

void displayMenu() {
    cout << "\nWelcome to Library Management System\n";
    cout << "1. Add Book\n";
    cout << "2. Search Book by ID\n";
    cout << "3. Search Book by Title\n";
    cout << "4. Issue Book\n";
    cout << "5. Return Book\n";
    cout << "6. List All Books\n";
    cout << "7. Remove Book\n";
    cout << "8. Exit LMS\n";
    cout << "Enter your choice: ";
}

int main() {
    Library lib;
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;
        switch (choice) {
case 1: {
                int id;
                string title, author;
                cout << "Enter book ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter book author: ";
                getline(cin, author);
                lib.addBook(id, title, author);
                break;
            }
            case 2: {
                int id;
                cout << "Enter book ID: ";
                cin >> id;
                lib.searchBookById(id);
                break;
            }
            case 3: {
                string title;
                cout << "Enter book title: ";
                cin.ignore();
                getline(cin, title);
                lib.searchBookByTitle(title);
                break;
            }
            case 4: {
                int id;
                string studentName;
                cout << "Enter book ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter student name: ";
                getline(cin, studentName);
                lib.issueBook(id, studentName);
                break;
            }
            case 5: {
                int id;
                cout << "Enter book ID: ";
                cin >> id;
                lib.returnBook(id);
                break;
            }
            case 6: {
                lib.listAllBooks();
                break;
            }
            
            case 7: {
                int id;
                cout << "Enter book ID: ";
                cin >> id;
                lib.removeBook(id);
                break;
            }
            case 8: {
                cout << "Exiting the system." << endl;
                return 0;
            }
            default:
                cout << "Invalid choice. Please try again!!" << endl;
        }
    }
}
