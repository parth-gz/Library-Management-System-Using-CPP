#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>

using namespace std;

// Structure to represent a book
struct Book {
    int number;
    string title;
    string author;
    int availableCopies;
};

// Structure to represent a student
struct Student {
    int membershipNumber;
    string name;
    int borrowedBookNumber; // Book number borrowed by the student
};

// Function to search for a book by book number
Book* searchBookByNumber(vector<Book>& books, int bookNumber);

// Function to read books from CSV file
vector<Book> readBooksFromCSV(const string& filename, set<int>& existingBookNumbers);

// Function to write books to CSV file
void writeBooksToCSV(const string& filename, const vector<Book>& books);

// Function to write students to CSV file
void writeStudentsToCSV(const string& filename, const vector<Student>& students);

// Function to borrow a book
void borrowBook(vector<Book>& books, vector<Student>& students, int bookNumber, int membershipNumber);

// Function to return a book
void returnBook(vector<Book>& books, vector<Student>& students, int bookNumber, int membershipNumber);

// Function to add a new book
void addNewBook(vector<Book>& books, set<int>& existingBookNumbers);

// Function to search for a student by membership number
Student* searchStudentByMembershipNumber(vector<Student>& students, int membershipNumber);

// Function to add a new student
void addNewStudent(vector<Student>& students, set<int>& existingMembershipNumbers, int& membershipCounter);

// Function to read students from CSV file
vector<Student> readStudentsFromCSV(const string& filename, set<int>& existingMembershipNumbers);

int main() {
    // Read books and students from CSV files
    set<int> existingBookNumbers;
    vector<Book> libraryBooks = readBooksFromCSV("record.csv", existingBookNumbers);

    set<int> existingMembershipNumbers;
    vector<Student> studentList = readStudentsFromCSV("student_list.csv", existingMembershipNumbers);
 
    // Display library menu
    cout<<endl<<endl;
    cout << "Library Management System\n";
    cout << "1. Search for a book by book number\n";
    cout << "2. Borrow a book\n";
    cout << "3. Return a book\n";
    cout << "4. Add a new book\n";
    cout << "5. Allot new membership number\n";
    cout << "6. Exit\n";

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    int membershipCounter = 0;  // Counter to allocate new membership IDs

    switch (choice) {
        case 1: {
            // Search for a book by book number
            int bookNumber;
            cout << "Enter book number: ";
            cin >> bookNumber;

            Book* foundBook = searchBookByNumber(libraryBooks, bookNumber);

            if (foundBook != nullptr) {
                // Display book details
                cout << "Book Details\n";
                cout << "Number: " << foundBook->number << endl;
                cout << "Title: " << foundBook->title << endl;
                cout << "Author: " << foundBook->author << endl;
                cout << "Available Copies: " << foundBook->availableCopies << endl;
            } else {
                cout << "Book not found\n";
            }
            break;
        }

        case 2: {
            // Borrow a book
            int borrowBookNumber, borrowMembershipNumber;
            cout << "Enter book number to borrow: ";
            cin >> borrowBookNumber;
            cout << "Enter your membership number: ";
            cin >> borrowMembershipNumber;
            borrowBook(libraryBooks, studentList, borrowBookNumber, borrowMembershipNumber);
            break;
        }

        case 3: {
            // Return a book
            int returnBookNumber, returnMembershipNumber;
            cout << "Enter book number to return: ";
            cin >> returnBookNumber;
            cout << "Enter your membership number: ";
            cin >> returnMembershipNumber;
            returnBook(libraryBooks, studentList, returnBookNumber, returnMembershipNumber);
            break;
        }

        case 4:
        // Add a new book
            addNewBook(libraryBooks, existingBookNumbers);
            break;

        case 5:
            // Add a new student
            addNewStudent(studentList, existingMembershipNumbers, membershipCounter);
            break;

        case 6:
            // Exit the program
            cout << "Exiting...\n";
            exit(1);
            break;

        default:
            cout << "Invalid choice\n";
    }
    

    // Write updated books and students back to CSV files
    writeBooksToCSV("record.csv", libraryBooks);
    writeStudentsToCSV("student_list.csv", studentList);

    return 0;
}

// Function definitions

Book* searchBookByNumber(vector<Book>& books, int bookNumber) {
    for (auto& book : books) {
        if (book.number == bookNumber) {
            return &book;
        }
    }
    // Return nullptr if not found
    return nullptr;
}

vector<Book> readBooksFromCSV(const string& filename, set<int>& existingBookNumbers) {
    vector<Book> books;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return books;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Book book;
        string token;

        // Read data from CSV
        getline(iss, token, ',');
        book.number = stoi(token);

        getline(iss, book.title, ',');
        getline(iss, book.author, ',');
        getline(iss, token, ',');
        book.availableCopies = stoi(token);

        books.push_back(book);

        // Add book number to set of existing book numbers
        existingBookNumbers.insert(book.number);
    }

    file.close();
    return books;
}

void writeBooksToCSV(const string& filename, const vector<Book>& books) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (const auto& book : books) {
        file << book.number << ',' << book.title << ',' << book.author << ',' << book.availableCopies << '\n';
    }

    file.close();
}

void writeStudentsToCSV(const string& filename, const vector<Student>& students) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (const auto& student : students) {
        file << student.membershipNumber << ',' << student.name << ',' << student.borrowedBookNumber << '\n';
    }

    file.close();
}

void borrowBook(vector<Book>& books, vector<Student>& students, int bookNumber, int membershipNumber) {
    Book* book = searchBookByNumber(books, bookNumber);
    Student* student = searchStudentByMembershipNumber(students, membershipNumber);

    if (book != nullptr && student != nullptr && book->availableCopies > 0 && student->borrowedBookNumber == 0) {
        // Book found, student found, available copies > 0, student has not borrowed any book
        cout << "Book borrowed successfully\n";
        book->availableCopies--;
        student->borrowedBookNumber = bookNumber;
    } else if (book != nullptr && book->availableCopies == 0) {
        // Book found but no available copies
        cout << "Sorry, the book is currently not available\n";
    } else if (student != nullptr && student->borrowedBookNumber != 0) {
        // Student has already borrowed a book
        cout << "You have already borrowed a book\n";
    } else {
        // Book or student not found or invalid conditions
        cout << "Invalid operation\n";
    }
}

void returnBook(vector<Book>& books, vector<Student>& students, int bookNumber, int membershipNumber) {
    Book* book = searchBookByNumber(books, bookNumber);
    Student* student = searchStudentByMembershipNumber(students, membershipNumber);

    if (book != nullptr && student != nullptr && student->borrowedBookNumber == bookNumber) {
        // Book found, student found, student has borrowed the book
        cout << "Book returned successfully\n";
        book->availableCopies++;
        student->borrowedBookNumber = 0;
    } else if (book != nullptr && student != nullptr && student->borrowedBookNumber == 0) {
        // Student has not borrowed any book
        cout << "You have not borrowed this book\n";
    } else {
        // Book or student not found or invalid conditions
        cout << "Invalid operation\n";
    }
}

void addNewBook(vector<Book>& books, set<int>& existingBookNumbers) {
    Book newBook = {0, "", "", 0}; // Initialize with default values

    // Prompt for a unique book number
    do {
        cout << "Enter new book number: ";
        cin >> newBook.number;

        if (existingBookNumbers.count(newBook.number) > 0) {
            cout << "Error: Book number already exists. Please choose a different number.\n";
        }
    } while (existingBookNumbers.count(newBook.number) > 0);

    // Add the new book to the set of existing book numbers
    existingBookNumbers.insert(newBook.number);

    cout << "Enter new book details:\n";
    cout << "Title: ";
    cin.ignore(); // Ignore newline character in the buffer
    getline(cin, newBook.title);
    cout << "Author: ";
    getline(cin, newBook.author);
    cout << "Available Copies: ";
    cin >> newBook.availableCopies;

    books.push_back(newBook);
    cout << "New book added successfully\n";
}

Student* searchStudentByMembershipNumber(vector<Student>& students, int membershipNumber) {
    for (auto& student : students) {
        if (student.membershipNumber == membershipNumber) {
            return &student;
        }
    }
    // Return nullptr if not found
    return nullptr;
}

void addNewStudent(vector<Student>& students, set<int>& existingMembershipNumbers, int& membershipCounter) {
    Student newStudent = {0, "", 0}; // Initialize with default values

    // Generate a unique membership number
    do {
        newStudent.membershipNumber = ++membershipCounter;
    } while (existingMembershipNumbers.count(newStudent.membershipNumber) > 0);

    // Add the new membership number to the set of existing membership numbers
    existingMembershipNumbers.insert(newStudent.membershipNumber);

    cout << "Enter new student details:\n";
    cout << "Name: ";
    cin.ignore(); // Ignore newline character in the buffer
    getline(cin, newStudent.name);

    students.push_back(newStudent);
    cout << "New student added successfully. Your membership number is: " << newStudent.membershipNumber << endl;
}

vector<Student> readStudentsFromCSV(const string& filename, set<int>& existingMembershipNumbers) {
    vector<Student> students;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return students;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Student student;
        string token;

        // Read data from CSV
        getline(iss, token, ',');
        student.membershipNumber = stoi(token);

        getline(iss, student.name, ',');
        getline(iss, token, ',');
        student.borrowedBookNumber = stoi(token);

        students.push_back(student);

        // Add membership number to set of existing membership numbers
        existingMembershipNumbers.insert(student.membershipNumber);
    }

    file.close();
    return students;
}