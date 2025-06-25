#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class BankAccount {
    int accNo;
    char name[50];
    char type; // 'S' for Savings, 'C' for Current
    float balance;

public:
    void createAccount();
    void showAccount() const;
    void modify();
    void deposit(float);
    void withdraw(float);
    void report() const;
    int getAccountNo() const { return accNo; }
    float getBalance() const { return balance; }
};

void BankAccount::createAccount() {
    cout << "\nEnter Account No.: ";
    cin >> accNo;
    cin.ignore();
    cout << "Enter Account Holder Name: ";
    cin.getline(name, 50);
    cout << "Enter Account Type (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "Enter Initial Deposit Amount (>=500 for Saving and >=1000 for Current): ";
    cin >> balance;
    cout << "\nAccount Created Successfully!";
}

void BankAccount::showAccount() const {
    cout << "\nAccount No.: " << accNo;
    cout << "\nAccount Holder Name: " << name;
    cout << "\nType of Account: " << type;
    cout << "\nBalance: ?" << balance;
}

void BankAccount::modify() {
    cout << "\nModify Account Holder Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Modify Type of Account (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "Modify Balance Amount: ";
    cin >> balance;
}

void BankAccount::deposit(float amt) {
    balance += amt;
}

void BankAccount::withdraw(float amt) {
    if (balance - amt < 0)
        cout << "\nInsufficient Balance!";
    else
        balance -= amt;
}

void BankAccount::report() const {
    cout << setw(10) << accNo << setw(20) << name << setw(10) << type << setw(10) << balance << endl;
}

// Function Prototypes
void writeAccount();
void displayAccount(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAll();
void depositWithdraw(int, int);

// Main Function
int main() {
    char choice;
    int num;

    do {
        system("cls");
        cout << "\n\n\tBANK MANAGEMENT SYSTEM";
        cout << "\n\n\t1. NEW ACCOUNT";
        cout << "\n\t2. DEPOSIT AMOUNT";
        cout << "\n\t3. WITHDRAW AMOUNT";
        cout << "\n\t4. BALANCE ENQUIRY";
        cout << "\n\t5. ALL ACCOUNT HOLDER LIST";
        cout << "\n\t6. CLOSE AN ACCOUNT";
        cout << "\n\t7. MODIFY AN ACCOUNT";
        cout << "\n\t8. EXIT";
        cout << "\n\n\tSelect Your Option (1-8): ";
        cin >> choice;

        switch (choice) {
        case '1':
            writeAccount();
            break;
        case '2':
            cout << "\nEnter The account No. : "; cin >> num;
            depositWithdraw(num, 1);
            break;
        case '3':
            cout << "\nEnter The account No. : "; cin >> num;
            depositWithdraw(num, 2);
            break;
        case '4':
            cout << "\nEnter The account No. : "; cin >> num;
            displayAccount(num);
            break;
        case '5':
            displayAll();
            break;
        case '6':
            cout << "\nEnter The account No. : "; cin >> num;
            deleteAccount(num);
            break;
        case '7':
            cout << "\nEnter The account No. : "; cin >> num;
            modifyAccount(num);
            break;
        case '8':
            cout << "\nThanks for using Bank Management System!";
            break;
        default:
            cout << "\a\nInvalid Choice!";
        }
        cin.ignore();
        cin.get();
    } while (choice != '8');

    return 0;
}

// Function Definitions

void writeAccount() {
    BankAccount acc;
    ofstream outFile;
    outFile.open("accounts.dat", ios::binary | ios::app);
    acc.createAccount();
    outFile.write(reinterpret_cast<char *>(&acc), sizeof(BankAccount));
    outFile.close();
}

void displayAccount(int n) {
    BankAccount acc;
    ifstream inFile;
    inFile.open("accounts.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNo() == n) {
            acc.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "\n\nAccount number does not exist!";
}

void modifyAccount(int n) {
    BankAccount acc;
    fstream File;
    File.open("accounts.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (!File.eof() && !found) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount));
        if (acc.getAccountNo() == n) {
            acc.showAccount();
            cout << "\n\nEnter New Details of Account" << endl;
            acc.modify();
            File.seekp(pos);
            File.write(reinterpret_cast<char *>(&acc), sizeof(BankAccount));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\n Record Not Found ";
}

void deleteAccount(int n) {
    BankAccount acc;
    ifstream inFile;
    ofstream outFile;

    inFile.open("accounts.dat", ios::binary);
    outFile.open("Temp.dat", ios::binary);

    inFile.seekg(0, ios::beg);
    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNo() != n) {
            outFile.write(reinterpret_cast<char *>(&acc), sizeof(BankAccount));
        }
    }
    inFile.close();
    outFile.close();
    remove("accounts.dat");
    rename("Temp.dat", "accounts.dat");
    cout << "\n\n\tRecord Deleted...";
}

void displayAll() {
    BankAccount acc;
    ifstream inFile;
    inFile.open("accounts.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open!";
        return;
    }
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.       NAME           Type    Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount))) {
        acc.report();
    }
    inFile.close();
}

void depositWithdraw(int n, int option) {
    float amt;
    bool found = false;
    BankAccount acc;
    fstream File;

    File.open("accounts.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open!";
        return;
    }

    while (!File.eof() && !found) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char *>(&acc), sizeof(BankAccount));
        if (acc.getAccountNo() == n) {
            acc.showAccount();
            if (option == 1) {
                cout << "\n\nTo Deposit Amount: ";
                cin >> amt;
                acc.deposit(amt);
            } else {
                cout << "\n\nTo Withdraw Amount: ";
                cin >> amt;
                acc.withdraw(amt);
            }
            File.seekp(pos);
            File.write(reinterpret_cast<char *>(&acc), sizeof(BankAccount));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\n Record Not Found ";
}
