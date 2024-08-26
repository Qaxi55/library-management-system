#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

using namespace std;

// Base Class: Item
class Item {
protected:
    string itemID;
    string title;
    bool available;

public:
    Item(string id, string t) : itemID(id), title(t), available(true) {}

    virtual void getItemDetails() = 0;
    virtual bool checkAvailability() {
        return available;
    }
    virtual void checkOut() {
        if (available) {
            available = false;
            cout << title << " has been checked out." << endl;
        }
        else {
            cout << title << " is not available." << endl;
        }
    }
    virtual void checkIn() {
        available = true;
        cout << title << " has been checked in." << endl;
    }
};

// Derived Class: Book
class Book : public Item {
    string author;
    string ISBN;

public:
    Book(string id, string t, string a, string isbn) : Item(id, t), author(a), ISBN(isbn) {}

    void getItemDetails() override {
        cout << "Book ID: " << itemID << "\nTitle: " << title << "\nAuthor: " << author << "\nISBN: " << ISBN << "\nAvailability: " << (available ? "Available" : "Checked out") << endl;
    }
};

// Derived Class: Journal
class Journal : public Item {
    string publisher;
    int issueNumber;

public:
    Journal(string id, string t, string p, int issue) : Item(id, t), publisher(p), issueNumber(issue) {}

    void getItemDetails() override {
        cout << "Journal ID: " << itemID << "\nTitle: " << title << "\nPublisher: " << publisher << "\nIssue Number: " << issueNumber << "\nAvailability: " << (available ? "Available" : "Checked out") << endl;
    }
};

// Class: Member
class Member {
    string memberID;
    string name;
    string contactInfo;
    int loanLimit;
    vector<Item*> loanHistory;

public:
    Member(string id, string n, string contact, int limit) : memberID(id), name(n), contactInfo(contact), loanLimit(limit) {}

    void borrowItem(Item* item) {
        if (loanHistory.size() < loanLimit && item->checkAvailability()) {
            item->checkOut();
            loanHistory.push_back(item);
        }
        else {
            cout << "Cannot borrow item. Loan limit reached or item not available." << endl;
        }
    }

    void returnItem(Item* item) {
        auto it = find(loanHistory.begin(), loanHistory.end(), item);
        if (it != loanHistory.end()) {
            item->checkIn();
            loanHistory.erase(it);
        }
        else {
            cout << "Item not found in loan history." << endl;
        }
    }

    void getLoanHistory() {
        cout << "Loan History for " << name << ":" << endl;
        for (Item* item : loanHistory) {
            item->getItemDetails();
        }
    }
};

// Class: Loan
class Loan {
    string loanID;
    Item* item;
    Member* member;
    time_t dueDate;
    time_t returnDate;

public:
    Loan(string id, Item* i, Member* m, time_t due) : loanID(id), item(i), member(m), dueDate(due), returnDate(0) {}

    void returnItem(time_t retDate) {
        returnDate = retDate;
        item->checkIn();
        member->returnItem(item);
        calculateFine();
    }

    void calculateFine() {
        if (returnDate > dueDate) {
            double fine = difftime(returnDate, dueDate) / (60 * 60 * 24) * 10;  // $10 per day
            cout << "Fine for late return: $" << fine << endl;
        }
        else {
            cout << "No fine for this return." << endl;
        }
    }
};

// Simplified example usage
int main() {
    // Create items
    Book book1("B001", "The Great Gatsby", "F. Scott Fitzgerald", "123456789");
    Journal journal1("J001", "Nature Journal", "Nature Publishing", 15);

    // Create a member
    Member member1("M001", "John Doe", "john.doe@example.com", 3);

    // Member borrows a book and a journal
    member1.borrowItem(&book1);
    member1.borrowItem(&journal1);

    // Display loan history
    member1.getLoanHistory();

    // Return the book with no fine
    time_t now = time(0);
    Loan loan1("L001", &book1, &member1, now + 10 * 24 * 60 * 60);  // Due in 10 days
    loan1.returnItem(now + 5 * 24 * 60 * 60);  // Returned in 5 days

    // Return the journal with a fine
    Loan loan2("L002", &journal1, &member1, now + 10 * 24 * 60 * 60);  // Due in 10 days
    loan2.returnItem(now + 15 * 24 * 60 * 60);  // Returned in 15 days

    return 0;
}