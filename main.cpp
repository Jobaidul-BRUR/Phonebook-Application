#include <iostream>
#include <limits>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
const int MAX = 1000;

void clear()
{
    #ifdef WIN32
    system("cls");
    #elif WIN64
    system("cls");
    #elif __linux__
    system("clear");
    #endif // WIN32
}
string removeSpaces(string s);


class Contact
{
    char name[20];
    char number[20];
public:
    Contact(string name="", string number="");
    string getName();
    string getNumber();
    void setName(string name);
    void setNumber(string number);
    bool operator <(Contact c);
    Contact operator =(Contact c);
};


class PhoneBook
{
    Contact contacts[MAX];
    int currSize;
public:
    PhoneBook();
    void addContact();
    void showAllContacts();
    bool match(string str, string key);
    void search();
    void deleteByNumber();
};

void saveToFile(PhoneBook &phoneBook);
void readFromFile(PhoneBook &phoneBook);

int main()
{
    PhoneBook phoneBook;
    readFromFile(phoneBook);

    bool invalid_flag = 0;
    while(1)
    {
        clear();
        cout << "\n\n     ** PhoneBook Application main menu **\n\n";
        cout << "     0. Close\n";
        cout << "     1. Add new Contact\n";
        cout << "     2. View all Contacts\n";
        cout << "     3. Search Contacts\n";
        cout << "     4. Delete Contact(By phone number)\n";

        if (invalid_flag == 1)
        {
            invalid_flag = 0;
            cout << "     Invalid choice!\n";
        }
        else cout << endl;


        cout << "     Enter your choice: ";
        int choice;
        if(not (cin >> choice)) // to handle invalid input
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            invalid_flag = 1;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice)
        {
        case 0:
            exit(0);
            break;
        case 1:
            phoneBook.addContact();
            saveToFile(phoneBook);
            break;
        case 2:
            phoneBook.showAllContacts();
            break;
        case 3:
            phoneBook.search();
            break;
        case 4:
            phoneBook.deleteByNumber();
            saveToFile(phoneBook);
            break;
        default:
            invalid_flag = 1;
            break;
        }
    }
    return 0;
}
string removeSpaces(string s)
{
    int i, j;
    for (i = 0, j = 0; j < (int)s.length();)
    {
        while (j < (int)s.length() and s[j] == ' ') j++;
        if (j < (int)s.length()) s[i++] = s[j++];
    }
    return s.substr(0, i);
}


//**************** Members of class Contact **************************
Contact::Contact(string name, string number)
{
    setName(name);
    setNumber(number);
}
string Contact::getName() { return string(name); }
string Contact::getNumber() { return string(number); }
void Contact::setName(string name) { strcpy(this->name, name.substr(0, 20).c_str()); }
void Contact::setNumber(string number) { strcpy(this->number, number.substr(0, 20).c_str()); }
bool Contact::operator <(Contact c)
{
    if (getName() == c.getName()) return getNumber() < c.getNumber();
    return getName() < c.getName();
}
Contact Contact::operator =(Contact c)
{
    setName(c.getName());
    setNumber(c.getNumber());
    return c;
}
//************************** Contact *********************************





//**************** Members of class PhoneBook ************************
PhoneBook::PhoneBook()
{
    currSize = 0;
}
void PhoneBook::addContact()
{
    clear();
    cout << "\n\n     ** Add new Contact **\n\n";
    if (currSize >= MAX)
    {
        cout << "PhoneBook is full!\n";
    }
    else
    {
        string name, number;
        cout << "     Enter Full Name: "; getline(cin, name);
        cout << "     Enter Number   : "; getline(cin, number);

        contacts[currSize++] = Contact(name, number);

        cout << "\n     Number added successfully.\n";
    }

    cout << "\n     Press enter to return to main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void PhoneBook::showAllContacts()
{
    clear();
    cout << "\n\n     ** Contact List **\n\n";
    if (currSize == 0)
    {
        cout << "     PhoneBook is empty!\n";
    }
    else
    {
        sort(contacts, contacts+currSize);
        for (int i = 0; i < currSize; i++)
        {
            cout << "\n     Name  : " << contacts[i].getName() << endl;
            cout << "     Number: " << contacts[i].getNumber() << endl;
        }
    }

    cout << "\n     Press enter to return to main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
bool PhoneBook::match(string str, string key)
{
    str = removeSpaces(str);
    key = removeSpaces(key);
    str = key+"#"+str;
    int n = str.length();
    for (int i = 0; i < n; i++) str[i] = tolower(str[i]);

    vector<int> z(n);
    int x = 0, y = 0;
    for (int i = 1; i < n; i++)
    {
        z[i] = max(0,min(z[i-x],y-i+1));
        while (i+z[i] < n && str[z[i]] == str[i+z[i]])
        {
            x = i; y = i+z[i]; z[i]++;
        }
        if (z[i] >= (int)key.length()) return true;
    }
    return false;
}
void PhoneBook::search()
{
    clear();
    cout << "\n\n     ** Search **\n\n";

    string key;
    cout << "     Enter name/number: "; getline(cin, key);

    sort(contacts, contacts+currSize);
    vector<int> res;
    for (int i = 0; i < currSize; i++)
    {
        if (match(contacts[i].getName(), key) or match(contacts[i].getNumber(), key))
        {
            res.push_back(i);
        }
    }

    if (res.size() == 0)
    {
        cout << "\n\n     No such contact found!\n";
    }
    else
    {
        for (int i = 0; i < (int)res.size(); i++)
        {
            cout << "\n     Name  : " << contacts[res[i]].getName() << endl;
            cout << "     Number: " << contacts[res[i]].getNumber() << endl;
        }
    }

    cout << "\n     Press enter to return to main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void PhoneBook::deleteByNumber()
{
    clear();
    cout << "\n\n     ** Delete Contacts by Number **\n\n";

    string number;
    cout << "     Enter Number: "; getline(cin, number);

    vector<int> del;
    for (int i = 0; i < currSize; i++)
    {
        if (removeSpaces(contacts[i].getNumber()) == removeSpaces(number)) del.push_back(i);
    }
    if (del.size() == 0)
    {
        cout << "\n     No such contact found!\n";
    }
    else
    {
        cout << "\n     Deleted contacts\n";
        for (int i = 0; i < (int)del.size(); i++)
        {
            cout << "\n     Name  : " << contacts[del[i]].getName() << endl;
            cout << "     Number: " << contacts[del[i]].getNumber() << endl;

            swap(contacts[del[i]], contacts[--currSize]);
        }
    }

    cout << "\n     Press enter to return to main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
//************************* PhoneBook ********************************


void readFromFile(PhoneBook &phoneBook)
{
    ifstream file("PhoneBook.dat", ios::in | ios::binary);
    if (not file)
    {
        cerr << "     Couldn't open the database!\n";
        cout << "     Creating new database.\n";
        return saveToFile(phoneBook);
    }
    file.read(reinterpret_cast<char *> (&phoneBook), sizeof(PhoneBook));
    if (not file)
    {
        cerr << "     Couldn't read data!\n";
        exit(1);
    }
}
void saveToFile(PhoneBook &phoneBook)
{
    ofstream file("PhoneBook.dat", ios::out | ios::binary);
    if (not file)
    {
        cerr << "     Couldn't open the database!\n";
        exit(1);
    }
    file.write(reinterpret_cast<const char *> (&phoneBook), sizeof(PhoneBook));
    if (not file)
    {
        cerr << "     Couldn't Write data!\n";
        exit(1);
    }
}
