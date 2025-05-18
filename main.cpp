#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include<bits/stdc++.h>
#include <filesystem>
using namespace std;
const unsigned int MAX_LENGTH = 256;



bool isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;

    int zi = std::stoi(date.substr(0, 2));
    int luna = std::stoi(date.substr(3, 2));
    int an = std::stoi(date.substr(6, 4));

    if (zi < 1 || zi > 31) return false;
    if (luna < 1 || luna > 12) return false;
    if (an < 2024) return false;

    if (luna == 2 && zi > 29) return false; // februarie maxim 29 zile
    if ((luna == 4 || luna == 6 || luna == 9 || luna == 11) && zi > 30) return false;

    return true;
}


class User {
private:
    const int userId;
    char* name;
    char* password;
    bool isAdmin;
    int loyaltypoints;

    static int generatorUId;
    static int szListOfUsers;
    static User* listOfUsers;
public:
    User() : userId(generatorUId++) {
        name = new char[MAX_LENGTH];
        password = new char[MAX_LENGTH];
        loyaltypoints = 0;
        isAdmin = false;
    }
    User(char* name, char* password, bool isAdmin, int loyaltypoints) : userId(generatorUId++) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
        this->isAdmin = isAdmin;
        this->loyaltypoints = loyaltypoints;
    }
    User(char* name) : userId(generatorUId++) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->password = nullptr;
        isAdmin = false;
        this->loyaltypoints = 0;

    }
    User(char* name, char* password) : userId(generatorUId++) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
        this->isAdmin = false;
        this->loyaltypoints = 0;

    }
    User(const User& user) : userId(generatorUId++) {
        this->isAdmin = user.isAdmin;
        this->loyaltypoints = user.loyaltypoints;/*
        if (this->name != nullptr) {
            delete[] this->name;
        }*/
        this->name = new char[strlen(user.name) + 1];
        strcpy(this->name, user.name);

        /*if (this->password != nullptr) {
            delete[] this->password;
        }*/
        this->password = new char[strlen(user.password) + 1];
        strcpy(this->password, user.password);
    }
    ~User() {
        if (this->name != nullptr) {
            delete[] this->name;
        }
        if (this->password != nullptr) {
            delete[] this->password;
        }
    }
    friend istream& operator>>(istream& in, User& user) {
        cout << "Introdu numele: ";
        in.get(user.name, MAX_LENGTH);
        in.get();

        cout << "Introdu parola: ";
        in.get(user.password, MAX_LENGTH);
        in.get();
        if (strcmp(user.name, "admin") == 0)
            user.isAdmin = true;
        else
            user.isAdmin = false;
        return in;
    }
    friend ostream& operator<<(ostream& out, const User& user) {
        out << "ID user: " << user.userId << "\n";
        out << "Nume user: " << user.name << '\n';
        out << "Parola user: " << user.password << '\n';
        if (!user.isAdmin)
            out << "Puncte de fidelitate: " << user.loyaltypoints << '\n';
        return out;
    }
    User& operator=(const User& user) {
        if (this == &user)
            return *this;

        this->isAdmin = user.isAdmin;
        this->loyaltypoints = user.loyaltypoints;
        if (this->name != nullptr) {
            delete[] this->name;
        }
        this->name = new char[strlen(user.name) + 1];
        strcpy(this->name, user.name);

        if (this->password != nullptr) {
            delete[] this->password;
        }
        this->password = new char[strlen(user.password) + 1];
        strcpy(this->password, user.password);

        return *this;
    }
    char operator[](int index) {
        if (index >= 0 && index < strlen(name)) {
            return name[index];
        }
        return 'X';
    }

    bool operator==(const User& user) const {
        return (strcmp(this->name, user.name) == 0) && (strcmp(this->password, user.password) == 0);
    }
    User& operator++() {
        this->loyaltypoints++;
        return *this;
    }
    User& operator+(int x) {
        this->loyaltypoints += x;
        return *this;
    }

    friend User& operator+(int x, User& user) {
        return user + x;
    }
    User& operator-(int x) {
        this->loyaltypoints -= x;
        return *this;
    }
    friend User& operator-(int x, User& user) {
        user.loyaltypoints = x - user.loyaltypoints;
        return user;
    }
    bool operator<(const User& user) const {
        return this->loyaltypoints < user.loyaltypoints;
    }

    static void addUserToList(const User& user);
    static bool verifyUser(const User& user);
    static User* getUserByName(char* name);
    static User* getUserById(int Id);
    static User* getUsers();
    static int getSize();

    int getUserId();
    char* getName();
    char* getPassword();
    bool getAdmin();
    int getLoyaltyPoints();
    void setName(char* name);
    void setPassword(char* password);
    void setAdmin(bool isAdmin);
    void setLoyaltyPoints(int loyaltypoints);
    void addLoyaltyPoints(int points);
    static void showAllUsers();
    static void deleteUserList() {
        if (listOfUsers != nullptr) {
            delete[] listOfUsers;
            listOfUsers = nullptr;
            szListOfUsers = 0;
        }
    }


};

User* User::getUserByName(char* name) {
    for (int i = 0;i < szListOfUsers;i++) {
        if (strcmp(name, listOfUsers[i].name) == 0) {
            return &listOfUsers[i];
        }
    }
    return nullptr;
}
User* User::getUserById(int id) {
    for (int i = 0;i < szListOfUsers;i++) {
        if (listOfUsers[i].userId == id) {
            return &listOfUsers[i];
        }
    }
    return nullptr;
}
void User::addUserToList(const User& user) {
    listOfUsers[szListOfUsers++] = user;
}
bool User::verifyUser(const User& user) {
    for (int i = 0;i < szListOfUsers;i++)
        if (strcmp(listOfUsers[i].name, user.name) == 0)
            if (strcmp(listOfUsers[i].password, user.password) == 0)
                return true;
    return false;
}
void User::addLoyaltyPoints(int points) {
    this->loyaltypoints += points;
}
int User::getUserId() {
    return this->userId;
}
char* User::getName() {
    return this->name;
}
char* User::getPassword() {
    return this->password;
}
bool User::getAdmin() {
    return this->isAdmin;
}
int User::getLoyaltyPoints() {
    return this->loyaltypoints;
}
void User::setName(char* name) {
    if (this->name != nullptr) {
        delete[] this->name;
    }
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}
void User::setPassword(char* password) {
    if (this->password != nullptr) {
        delete[] this->password;
    }
    this->password = new char[strlen(password) + 1];
    strcpy(this->password, password);
}
void User::setAdmin(bool isAdmin) {
    this->isAdmin = isAdmin;
}
void User::setLoyaltyPoints(int loyaltypoints) {
    this->loyaltypoints = loyaltypoints;
}
int User::generatorUId = 1;
int User::szListOfUsers = 0;
User* User::listOfUsers = new User[MAX_LENGTH];

void User::showAllUsers() {
    for (int i = 0; i < szListOfUsers; i++) {
        cout << listOfUsers[i] << "\n";
    }
}
User* User::getUsers() {
    return listOfUsers;
}

int User::getSize() {
    return szListOfUsers;
}

class Product {
protected:
    const int productId;
    string name;
    float price;
    string description;

    static int generatorProductId;
public:
    Product() :productId(generatorProductId++) {
        name = "ProductX";
        price = 0.0;
        description = "";
    }
    Product(string name, float price, string description) :productId(generatorProductId++) {
        this->name = name;
        this->price = price;
        this->description = description;
    }
    Product(string name) :productId(generatorProductId++) {
        this->name = name;
        price = 0.0;
        description = "";
    }
    Product(string name, float price) :productId(generatorProductId++) {
        this->name = name;
        this->price = price;
        this->description = "";
    }
    Product(const Product& p) :productId(p.productId) {
        this->name = p.name;
        this->price = p.price;
        this->description = p.description;
    }
    Product& operator=(const Product& p) {
        if (this != &p) {
            name = p.name;
            price = p.price;
            description = p.description;
        }
        return *this;
    }
    virtual ~Product() {}

    friend istream& operator>>(istream& in, Product& p) {
        cout << "Denumire: ";
        getline(in, p.name);
        cout << "Pret: ";
        in >> p.price;
        in.get();
        cout << "Descriere: ";
        getline(in, p.description);
        return in;
    }

    friend ostream& operator<<(ostream& out, const Product& p) {
        out << "Id produs: " << p.productId << "\n";
        out << "Denumire: " << p.name << '\n';
        out << "Pretul: " << p.price << '\n';
        out << "Descriere: " << p.description << '\n';
        return out;
    }

    virtual void showDetails() const = 0;
    virtual string getProductType() const = 0;

    int getProductId() {
        return productId;
    }
    string getName() {
        return name;
    }
    float getPrice() {
        return price;
    }
    string getDescription() {
        return description;
    }
    void setName(string name) {
        this->name = name;
    }
    void setPrice(float price) {
        this->price = price;
    }
    void setDescription(string description) {
        this->description = description;
    }

};

int Product::generatorProductId = 10;

class Food : public virtual Product {
protected:
    int kcal;
    bool vegan;
public:
    Food() :Product() {
        kcal = 0;
        vegan = false;
    }
    Food(string name, float price, string description, int kcal, bool vegan)
        : Product(name, price, description) {
        this->kcal = kcal;
        this->vegan = vegan;
    }
    Food(string name, float price) :Product(name, price) {
        kcal = 0;
        vegan = false;
    }
    Food(string name) : Product(name) {
        kcal = 0;
        vegan = 0;
    }
    Food(string name, float price, int kcal) :Product(name, price) {
        this->kcal = kcal;
        this->vegan = false;
    }
    Food(const Food& f) : Product(f) {
        this->kcal = f.kcal;
        this->vegan = f.vegan;
    }
    Food& operator=(const Food& f) {
        if (this != &f) {
            Product::operator=(f);
            kcal = f.kcal;
            vegan = f.vegan;
        }
        return *this;
    }
    friend ostream& operator<<(ostream& out, const Food& f) {
        out << (Product&)f;
        cout << "KiloCalorii: ";
        out << f.kcal << "\n";
        cout << "Vegan: ";
        out << (f.vegan ? "Da\n" : "Nu\n");
        return out;
    }

    friend istream& operator>>(istream& in, Food& f) {
        in >> (Product&)f;
        cout << "Calorii: ";
        in >> f.kcal;
        in.get();
        cout << "Este vegana? (1/0): ";
        in >> f.vegan;
        in.get();
        return in;
    }
    void showDetails() const;
    string getProductType() const;

    int getKcal() {
        return kcal;
    }
    bool getVegan() {
        return vegan;
    }
    void setKcal(int kcal) {
        this->kcal = kcal;
    }
    void setVegan(bool vegan) {
        this->vegan = vegan;
    }
}; 

void Food::showDetails() const {
    cout << "[MANCARE]";
    cout << ": #" << productId << '\n';
    cout << "Denumire: " << name << '\n';
    cout << "Pretul: " << price << '\n';
    cout << "Descriere: " << description << '\n';
    cout << "KiloCalorii: " << kcal << "\n";
    cout << "Vegan: " << (vegan ? "Da" : "Nu") << "\n";
}
string Food::getProductType() const {
    return "Mancare";
}

class Drink : public virtual Product {
protected:
    float amount;
    bool caffeine;
public:
    Drink() :Product() {
        amount = 0;
        caffeine = false;
    }
    Drink(string name, float price, string description, float amount, bool caffeine) :
        Product(name, price, description) {
        this->amount = amount;
        this->caffeine = caffeine;
    }
    Drink(string name, float price) : Product(name, price) {
        amount = 0;
        caffeine = false;
    }
    Drink(string name, float price, int amount) : Product(name, price) {
        this->amount = amount;
        caffeine = false;
    }
    Drink(string name) :Product(name) {
        amount = 0;
        caffeine = false;
    }
    Drink& operator=(const Drink& d) {
        if (this != &d) {
            Product::operator=(d);
            amount = d.amount;
            caffeine = d.caffeine;
        }
        return *this;
    }
    friend ostream& operator<<(ostream& out, const Drink& d) {
        out << (Product&)d;
        cout << "Cantitate: ";
        out << d.amount << "\n";
        cout << "Cofeina: ";
        out << (d.caffeine ? "Da\n" : "Nu\n");
        return out;
    }

    friend istream& operator>>(istream& in, Drink& d) {
        in >> (Product&)d;
        cout << "Cantitate: ";
        in >> d.amount;
        in.get();
        cout << "Contine cofeina? (1/0): ";
        in >> d.caffeine;
        in.get();
        return in;
    }
    void showDetails() const;
    string getProductType() const;

    float getAmount() {
        return amount;
    }
    bool getCaffeine() {
        return caffeine;
    }
    void setAmount(float amount) {
        this->amount = amount;
    }
    void setCaffeine(bool caffeine) {
        this->caffeine = caffeine;
    }
};


void Drink::showDetails() const {
    cout << "[BAUTURA]";
    cout << ": #" << productId << '\n';
    cout << "Denumire: " << name << '\n';
    cout << "Pretul: " << price << '\n';
    cout << "Descriere: " << description << '\n';
    cout << "Cantitate: " << amount << "\n";
    cout << "Cofeina: " << (caffeine ? "Da" : "Nu") << "\n";
}

string Drink::getProductType() const {
    return "Bautura";
}

class Offer : public Drink, public Food {

public:
    Offer() : Drink(), Food() {}
    Offer(string name, float price, string description, float amount, bool caffeine, int calories, bool vegan)
        : Product(name, price, description) {
        this->amount = amount;
        this->caffeine = caffeine;
        this->kcal = calories;
        this->vegan = vegan;
    }
    Offer(string name, float price) : Drink(name, price), Food(name, price) {}
    Offer(string name) : Food(name), Drink(name) {}
    Offer(const Offer& other)
        : Product(other), Drink(other), Food(other) {
    }

    Offer& operator=(const Offer& other) {
        if (this != &other) {
            Drink::operator=(other);
            Food::operator=(other);
        }
        return *this;
    }

    friend ostream& operator<<(ostream& out, const Offer& o) {
        out << (Product&)o;
        cout << "--- Detalii bautura ---\n";
        cout << "Cantitate: ";
        out << o.amount << "\n";
        cout << "Cofeina: ";
        out << (o.caffeine ? "Da\n" : "Nu\n");
        cout << "--- Detalii mancare ---\n";
        cout << "KiloCalorii: ";
        out << o.kcal << "\n";
        cout << "Vegan: ";
        out << (o.vegan ? "Da\n" : "Nu\n");
        return out;
    }

    friend istream& operator>>(istream& in, Offer& o) {
        in >> (Product&)o;
        cout << "--- Introdu detalii bautura ---\n";
        cout << "Cantitate: ";
        in >> o.amount;
        in.get();
        cout << "Contine cofeina? (1/0): ";
        in >> o.caffeine;
        in.get();
        cout << "--- Introdu detalii mancare ---\n";
        cout << "Calorii: ";
        in >> o.kcal;
        in.get();
        cout << "Este vegana? (1/0): ";
        in >> o.vegan;
        in.get();
        return in;
    }

    void showDetails() const;
    string getProductType() const;

};

void Offer::showDetails() const {
    cout << "[OFERTA]";
    cout << ": #" << productId << '\n';
    cout << "Nume: " << name << "\n";
    cout << "Pretul: " << price << "\n";
    cout << "Descriere: " << description << "\n";
    cout << "--- Detalii bautura ---\n";
    cout << "Cantitate: " << amount << "\n";
    cout << "Cofeina: " << (caffeine ? "Da\n" : "Nu\n");
    cout << "--- Detalii mancare ---\n";
    cout << "KiloCalorii: " << kcal << "\n";
    cout << "Vegan: " << (vegan ? "Da\n" : "Nu\n");
}
string Offer::getProductType() const {
    return "Oferta";
}
vector<Product*>listOfProducts;

bool sort_criteria(Product* A, Product* B) {
    return A->getProductType() < B->getProductType();
}

class Order {
private:
    const long long orderId;
    User* customer;
    vector<Product*>productsOrdered;
    vector<int> quantities;
    float totalValue;
    bool usedLoyaltyPoints;

    static long long generatorIdOrd;
    static int szListOfOrders;
    static Order* listOfOrders;

public:
    Order() : orderId(generatorIdOrd++) {
        customer = nullptr;
        totalValue = 0;
        usedLoyaltyPoints = false;
    }

    Order(User* customer, vector<Product*>& products, vector<int>& qty, bool usedLoyaltyPoints) : orderId(generatorIdOrd++) {
        this->customer = customer;
        productsOrdered = products;
        quantities = qty;
        totalValue = 0;
        this->usedLoyaltyPoints = usedLoyaltyPoints;
        for (int i = 0; i < products.size(); ++i) {
            totalValue += products[i]->getPrice() * quantities[i];
        }
    }

    Order(User* customer) : orderId(generatorIdOrd++) {
        totalValue = 0;
        this->customer = customer;
        usedLoyaltyPoints = false;
    }
    Order(const Order& o) : orderId(generatorIdOrd++) {
        this->customer = o.customer;
        this->quantities = o.quantities;
        this->totalValue = o.totalValue;
        this->usedLoyaltyPoints = o.usedLoyaltyPoints;
        this->productsOrdered = o.productsOrdered;
    }
    ~Order() {
        productsOrdered.clear();
        quantities.clear();
    }

    Order& operator=(const Order& o) {
        if (this != &o) {
            customer = o.customer;
            productsOrdered = o.productsOrdered;
            quantities = o.quantities;
            totalValue = o.totalValue;
            usedLoyaltyPoints = o.usedLoyaltyPoints;
        }
        return *this;
    }

    friend istream& operator>>(istream& in, Order& order) {
        int numProducts;
        cout << "Cate produse diferite vrei sa comanzi? ";
        in >> numProducts;
        order.productsOrdered.clear();
        order.quantities.clear();
        order.totalValue = 0;

        for (int i = 0; i < numProducts; ++i) {
            int id;
            cout << "ID produs: ";
            in >> id;
            in.get();

            Product* found = nullptr;
            for (Product* p : listOfProducts) {
                if (p->getProductId() == id) {
                    found = p;
                    break;
                }
            }

            if (!found) {
                cout << "Produs inexistent.\n";
                --i;
                continue;
            }

            int qty;
            cout << "Cantitate: ";
            in >> qty;
            in.get();

            order.productsOrdered.push_back(found);
            order.quantities.push_back(qty);
            order.totalValue += found->getPrice() * qty;
        }
        char use;
        cout << "Folosesti puncte de fidelitate? (y/n): ";
        in >> use;
        in.get();
        order.usedLoyaltyPoints = (use == 'y' || use == 'Y');

        return in;
    }

    friend ostream& operator<<(ostream& out, const Order& order) {
        out << "Comanda dumneavoastra #" << order.orderId << "\n";
        for (int i = 0; i < order.productsOrdered.size(); i++) {
            out << "Produs: " << order.productsOrdered[i]->getName() << "\n";
            out << "Nr de produse: " << order.quantities[i] << "\n";
            cout << "Pret per bucata: " << order.productsOrdered[i]->getPrice() << "\n";
        }
        out << "Total: " << order.totalValue << "\n";
        return out;
    }

    Order& operator++() {
        for (int i = 0; i < productsOrdered.size(); i++) {
            quantities[i]++;
            totalValue += productsOrdered[i]->getPrice();
        }
        return *this;
    }

    Order& operator+(float extra) {
        this->totalValue += extra;
        return *this;
    }
    friend Order& operator+(float extra, Order& o) {
        return o + extra;
    }
    Order& operator-(float extra) {
        this->totalValue -= extra;
        return *this;
    }
    friend Order& operator-(int x, Order& o) {
        o.totalValue = x - o.totalValue;
        return o;
    }

    bool operator==(const Order& o) const {
        return customer == o.customer;
    }

    bool operator<(const Order& o) const {
        return this->totalValue < o.totalValue;
    }
    float getTotalValue() const {
        return totalValue;
    }
    void completeOrder() {
        int aux = 0;
        if (usedLoyaltyPoints) {
            if (totalValue >= customer->getLoyaltyPoints()) {
                aux = customer->getLoyaltyPoints();
                totalValue -= aux;
                customer->setLoyaltyPoints(0);
            }
            else {
                aux = int(totalValue);
                *customer = *customer - int(totalValue);
                totalValue = 0;
            }
        }
        cout << "Comanda a fost finalizata!\n";
        cout << "Au fost folosite " << aux << " puncte de fidelitate.\n";
        cout << "Total: " << totalValue << "\n";
        *customer = static_cast<int>(totalValue / 20.0f) + *customer;
    }

    static void addOrderToList(const Order& order);
    static Order* getOrderById(long long orderId);
    static void deleteOrderList() {
        if (listOfOrders != nullptr) {
            delete[] listOfOrders;
            listOfOrders = nullptr;
            szListOfOrders = 0;
        }
    }
    static int getSzOrders() {
        return szListOfOrders;
    }

};

long long Order::generatorIdOrd = 100;
int Order::szListOfOrders = 0;
Order* Order::listOfOrders = new Order[1500];

void Order::addOrderToList(const Order& order) {
    listOfOrders[szListOfOrders++] = order;
}

Order* Order::getOrderById(long long orderId) {
    for (int i = 0; i < szListOfOrders; i++) {
        if (listOfOrders[i].orderId == orderId) {
            return &listOfOrders[i];
        }
    }
    return nullptr;
}


class Reservation {
private:
    const int reservationId;
    int customerId;
    int numberOfSeats;
    char* reservationTime;
    
    static int generatorIdRes;
    static int szListOfReservations;
    static Reservation* listOfReservations;

public:
    Reservation() : reservationId(generatorIdRes++) {
        customerId = -1;
        numberOfSeats = 0;
        numberOfSeats = 0;
        reservationTime = new char[MAX_LENGTH];
    }
    Reservation(int customerId, int numberOfSeats, char* reservationTime) : reservationId(generatorIdRes++) {
        this->customerId = customerId;
        this->numberOfSeats = numberOfSeats;
        this->reservationTime = new char[strlen(reservationTime) + 1];
        strcpy(this->reservationTime, reservationTime);
    }

    Reservation(int customerId) : reservationId(generatorIdRes++) {
        this->customerId = customerId;
        this->numberOfSeats = 0;
        this->reservationTime = new char[MAX_LENGTH];
        reservationTime[0] = '\0';
    }

    Reservation(int customerId, char* reservationTime) : reservationId(generatorIdRes++) {
        this->customerId = customerId;
        this->numberOfSeats = 0;
        this->reservationTime = new char[strlen(reservationTime) + 1];
        strcpy(this->reservationTime, reservationTime);
    }

    Reservation(const Reservation& r) : reservationId(generatorIdRes++) {
        this->customerId = r.customerId;
        this->numberOfSeats = r.numberOfSeats;
        this->reservationTime = new char[strlen(r.reservationTime) + 1];
        strcpy(this->reservationTime, r.reservationTime);
    }

    ~Reservation() {
        if (reservationTime != nullptr)
            delete[] reservationTime;
    }
    Reservation& operator=(const Reservation& reserv) {
        if (this == &reserv) return *this;

        this->customerId = reserv.customerId;
        this->numberOfSeats = reserv.numberOfSeats;
        if (this->reservationTime != nullptr) {
            delete[] this->reservationTime;
        }
        this->reservationTime = new char[strlen(reserv.reservationTime) + 1];
        strcpy(this->reservationTime, reserv.reservationTime);

        return *this;
    }
    char operator[](int index) const {
        if (index >= 0 && index < strlen(reservationTime)) {
            return reservationTime[index];
        }
        return 'X';
    }
    Reservation& operator++() {
        ++numberOfSeats;
        return *this;
    }

    Reservation& operator--() {
        --numberOfSeats;
        return *this;
    }

    Reservation& operator+(int val) {
        this->numberOfSeats += val;
        return *this;
    }

    Reservation& operator-(int val) {
        this->numberOfSeats -= val;
        return *this;
    }
    friend Reservation& operator+(int val, Reservation& reserv) {
        return reserv + val;
    }

    friend Reservation& operator-(int x, Reservation& r) {
        r.numberOfSeats = x - r.numberOfSeats;
        return r;
    }
    bool operator==(const Reservation& r) const {
        return this->customerId == r.customerId;
    }

    bool operator<(const Reservation& r) const {
        return strcmp(this->reservationTime, r.reservationTime) < 0;
    }
    friend istream& operator>>(istream& in, Reservation& res) {
        char* reservationTime = new char[MAX_LENGTH];
        cout << "Introdu data si ora rezervarii(YY/MM/DD HH:MM): ";
        in.get(reservationTime, MAX_LENGTH);
        in.get();
        int nseats;
        cout << "Pentru cate persoane? ";
        in >> nseats;
        in.get();
        res = Reservation(res.customerId, nseats, reservationTime);
        delete[] reservationTime;
        return in;
    }
    friend ostream& operator<<(ostream& out, const Reservation& res) {
        out << "Id-ul rezervarii: " << res.reservationId << '\n';
        User* user = User::getUserById(res.customerId);
        out << "Nume: " << (*user).getName() << '\n';
        out << "Numar de persoane: " << res.numberOfSeats << '\n';
        out << "Data si ora: " << res.reservationTime;
        out << "\n";

        return out;
    }
    int getReservationId();
    User* getCustomer();
    int getNumberOfSeats();
    char* getReservationTime();

    void setReservationTime(char* reservationTime);
    void setCustomer(User* customer);
    void setNumberOfSeats(int numberOfSeats);

    static void addReservationToList(const Reservation& reservation);
    static void deleteReservationById(int reservationId);
    static void showReservationsForUser(User& user);
    static void showAllReservation();
    static int getSize();
    static Reservation* getReservations();
    static void deleteReservationList() {
        if (listOfReservations != nullptr) {
            delete[] listOfReservations;
            listOfReservations = nullptr;
            szListOfReservations = 0;
        }
    }


};
int Reservation::generatorIdRes = 1000;
int Reservation::szListOfReservations = 0;
Reservation* Reservation::listOfReservations = new Reservation[MAX_LENGTH];

int Reservation::getReservationId() {
    return reservationId;
}
User* Reservation::getCustomer() {
    return User::getUserById(this->customerId);
}
int Reservation::getNumberOfSeats() {
    return numberOfSeats;
}
char* Reservation::getReservationTime() {
    return reservationTime;
}
void Reservation::setReservationTime(char* reservationTime) {
    if (this->reservationTime != nullptr)
        delete[] this->reservationTime;
    this->reservationTime = new char[MAX_LENGTH];
    strcpy(this->reservationTime, reservationTime);
}
void Reservation::setCustomer(User* customer) {
    this->customerId = customer->getUserId();
}
void Reservation::setNumberOfSeats(int numberOfSeats) {
    this->numberOfSeats = numberOfSeats;
}

void Reservation::addReservationToList(const Reservation& reservation) {
    if (szListOfReservations < MAX_LENGTH) {
        listOfReservations[szListOfReservations++] = reservation;
    }
    else {
        cout << "Eroare: S-a atins numarul maxim de rezervari!\n";
    }
}


void Reservation::deleteReservationById(int reservationId) {
    bool startDelete = false;
    if (reservationId == listOfReservations[szListOfReservations - 1].reservationId) {
        szListOfReservations--;
        return;
    }

    for (int i = 1; i < szListOfReservations; i++) {
        if (reservationId == listOfReservations[i - 1].reservationId) {
            startDelete = true;
        }
        if (startDelete) {
            listOfReservations[i - 1] = listOfReservations[i];
        }
    }

    if (startDelete) {
        szListOfReservations--;
    }
    else {
        cout << "Rezervarea cu ID-ul " << reservationId << " nu a fost gasita.\n";
    }
}

void Reservation::showReservationsForUser(User& user) {
    bool found = false;
    for (int i = 0; i < szListOfReservations; i++) {
        if (listOfReservations[i].customerId == user.getUserId()) {
            cout << listOfReservations[i];
            found = true;
        }
    }
    if (!found) {
        cout << "Nu ai nicio rezervare.\n";
    }
}
void Reservation::showAllReservation() {
    sort(listOfReservations, listOfReservations + szListOfReservations);
    for (int i = 0; i < szListOfReservations; i++)
        cout << listOfReservations[i];
}
int Reservation::getSize() {
    return szListOfReservations;
}
Reservation* Reservation::getReservations() {
    return listOfReservations;
}

int getMaxZi(int luna, int an) {
    if (luna == 1) {
        // An bisect
        if ((an % 4 == 0 && an % 100 != 0) || (an % 400 == 0))
            return 29;
        else
            return 28;
    }
    if (luna == 3 || luna == 5 || luna == 8 || luna == 10)
        return 30;
    return 31;
}


enum class AppState {
    MainMenu,
    Login,
    Register,
    AdminMenu,
    CustomerMenu,
    AddProduct,
    ViewMenu,
    DeleteProduct,
    UpdateProduct,
    ShowReservations,
    ShowCustomers,
    PlaceOrder,
    VerifyLPoints,
    Reservation, 
    ViewMyReservs, 
    CancelReservation,
    Export,
    Exit
};


template <class T>
class Validator {
public:
    Validator() = default;
    virtual ~Validator() = default;

    static void throwIfNotPositive(T value, const string& label = "Number") {
        if (value < 0)
            throw invalid_argument(label + " trebuie sa fie >=0!");
    }

    static void throwIfNotStrictPositive(T value, const string& label = "Number") {
        if (value <= 0)
            throw runtime_error(label + " trebuie sa fie >0!");
    }

    static void throwIfInRange(T value, T low, T high, const string& label = "Number") {
        if (value < low || value > high)
            throw out_of_range(label + " trebuie sa fie intre " + to_string(low) + " si intre " + to_string(high) + "!");
    }

    static void throwIfGreaterThan(T value, T max, const string& label = "Number") {
        if (value > max)
            throw runtime_error(label + " treebuie sa fie mai mic/a decat " + to_string(max) + "!");
    }

    static void throwIfNull(T ptr, const string& label = "Pointer") {
        if (ptr == nullptr)
            throw logic_error(label + " nu trebuie sa fie 0!");
    }
};

extern bool isValidDate(const string&);  

template <>
class Validator<string> {
public:
    static void throwIfEmptyOrNull(const string& value, const string& label = "String") {
        if (value.empty())
            throw runtime_error(label + " nu trebuie sa fie gol!");
    }

    static void throwIfTimeIntervalIncorrect(const string& time, const string& label = "Interval") {
        // HH:MM logic (dacă vrei să o păstrezi pentru ora)
        if (time.length() != 5 || time[2] != ':' ||
            !isdigit(time[0]) || !isdigit(time[1]) ||
            !isdigit(time[3]) || !isdigit(time[4]))
            throw runtime_error(label + " trebuie sa fie in formatul HH:MM!");

        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
            throw runtime_error(label + " trebuie sa fie valid (00:00 - 23:59)!");
    }

    static void throwIfInvalidDate(const string& date, const string& label = "Data") {
        if (!isValidDate(date))
            throw runtime_error(label + " este invalida! Format corect: dd/mm/yyyy");
    }
    static void throwIfDateInPast(const string& dateStr, const string& label = "Data") {
        if (dateStr.length() != 10 || dateStr[2] != '/' || dateStr[5] != '/') {
            throw runtime_error(label + " are un format invalid! (dd/mm/yyyy)");
        }

        int zi = stoi(dateStr.substr(0, 2));
        int luna = stoi(dateStr.substr(3, 2));
        int an = stoi(dateStr.substr(6, 4));

        // data curenta
        time_t t = time(nullptr);
        tm* now = localtime(&t);

        int ziCurenta = now->tm_mday;
        int lunaCurenta = now->tm_mon + 1;
        int anCurent = now->tm_year + 1900;

        if (an < anCurent ||
            (an == anCurent && luna < lunaCurenta) ||
            (an == anCurent && luna == lunaCurenta && zi < ziCurenta)) {
            throw runtime_error(label + " este in trecut!");
        }
    }
};

class Statistici {
private:
    static Statistici* instance;

    Statistici() = default;

    Statistici(const Statistici&) = delete;
    Statistici& operator=(const Statistici&) = delete;

public:
    static Statistici& getInstance() {
        if (instance == nullptr) {
            instance = new Statistici();
        }
        return *instance;
    }

    static void deleteInstance() {
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
        }
    }
    void importaProduse(const string& filename) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Nu s-a putut deschide fisierul pentru import: " << filename << "\n";
            return;
        }

        string type;
        while (getline(fin, type)) {
            if (type == "Bautura") {
                string name, description;
                float price, amount;
                bool caffeine;

                getline(fin, name);
                fin >> price;
                fin.get();
                getline(fin, description);
                fin >> amount >> caffeine;
                fin.get();

                Product* d = new Drink(name, price, description, amount, caffeine);
                listOfProducts.push_back(d);
            }
            else if (type == "Mancare") {
                string name, description;
                float price;
                int kcal;
                bool vegan;

                getline(fin, name);
                fin >> price;
                fin.get();
                getline(fin, description);
                fin >> kcal >> vegan;
                fin.get();

                Product* f = new Food(name, price, description, kcal, vegan);
                listOfProducts.push_back(f);
            }
            else if (type == "Oferta") {
                string name, description;
                float price, amount;
                bool caffeine;
                int kcal;
                bool vegan;

                getline(fin, name);
                fin >> price;
                fin.get();
                getline(fin, description);
                fin >> amount >> caffeine >> kcal >> vegan;
                fin.get();

                Product* o = new Offer(name, price, description, amount, caffeine, kcal, vegan);
                listOfProducts.push_back(o);
            }
        }

        fin.close();
    }

    void exportaStatistici(const string& fisier) {
        ofstream out(fisier);
        out << "Nr. utilizatori: " << User::getSize() << "\n";
        out << "Nr. produse: " << listOfProducts.size() << "\n";
        out << "Nr. rezervari: " << Reservation::getSize() << "\n";
        out << "Nr. comenzi: " << Order::getSzOrders() << "\n";
        out.close();
    }

    template <typename T>
    void exportaDate(const vector<T>& obiecte, const string& fisier) {
        ofstream out(fisier);
        for (const auto& obj : obiecte) {
            out << obj << "\n";
        }
        out.close();
    }

    template <typename T>
    void exportaDate(const vector<T*>& obiecte, const string& fisier) {
        ofstream out(fisier);
        for (const auto& obj : obiecte) {
            if (obj) out << *obj << "\n";
        }
        out.close();
    }
};

Statistici* Statistici::instance = nullptr;



void SetCafeTheme()
{
    ImGuiStyle& s = ImGui::GetStyle();

    s.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.17f, 0.14f, 1.00f); 
    s.Colors[ImGuiCol_PopupBg] = ImVec4(0.23f, 0.19f, 0.16f, 1.00f);
    s.Colors[ImGuiCol_TitleBg] = ImVec4(0.28f, 0.22f, 0.18f, 1.00f);
    s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.35f, 0.27f, 0.20f, 1.00f);

    s.Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.38f, 0.27f, 1.00f);
    s.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.43f, 0.31f, 1.00f);
    s.Colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.46f, 0.33f, 1.00f);

    s.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.59f, 0.46f, 0.29f, 1.00f);
    s.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.51f, 0.33f, 1.00f);
    s.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.55f, 0.36f, 1.00f);

    s.Colors[ImGuiCol_Header] = ImVec4(0.67f, 0.53f, 0.35f, 0.55f);
    s.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.72f, 0.58f, 0.39f, 0.80f);
    s.Colors[ImGuiCol_HeaderActive] = ImVec4(0.72f, 0.58f, 0.39f, 1.00f);
    s.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.80f, 0.60f, 1.00f);

    s.Colors[ImGuiCol_Text] = ImVec4(0.93f, 0.89f, 0.84f, 1.00f); 
    s.Colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.47f, 0.40f, 1.00f);

    s.FrameRounding = 6.0f;
    s.WindowRounding = 6.0f;
    s.ScrollbarSize = 16.0f;
}


int main() {
    bool loggedin = 0;
    User admin;
    User user;
    int option;
    char s[256];
    strcpy(s, "@admin");
    admin.setName(s);
    strcpy(s, "admin_password");
    admin.setPassword(s);
    admin.setAdmin(1);
    User::addUserToList(admin);
    User user1;
    strcpy(s, "Diana");
    user1.setName(s);
    strcpy(s, "Delia");
    user1.setPassword(s);
    user1.setAdmin(0);
    user1.setLoyaltyPoints(100);
    User::addUserToList(user1);
    User user2;
    strcpy(s, "Maria");
    user2.setName(s);
    strcpy(s, "Ioana");
    user2.setPassword(s);
    user2.setAdmin(0);
    user2.setLoyaltyPoints(10);
    User::addUserToList(user2);
    //readProductsFromFile("meniu.txt");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Cafenea GUI");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    ImGuiStyle& sty = ImGui::GetStyle();
    sty.FrameRounding = 8.f;     
    sty.WindowBorderSize = 0.f;     
    sty.ItemSpacing = { 8.f, 6.f };
    sty.GrabRounding = 12.f;    
    sty.Colors[ImGuiCol_FrameBg].w = 0.75f; 

    SetCafeTheme();

    ImGuiIO& io = ImGui::GetIO();

    ImFont* uiFont = io.Fonts->AddFontFromFileTTF(
        "Pacifico-Regular.ttf", 28.0f,
        nullptr, io.Fonts->GetGlyphRangesDefault());


    io.FontDefault = uiFont;         
    ImGui::SFML::UpdateFontTexture(); 


    AppState currentState = AppState::MainMenu;
    std::string username, password;
    bool isAdmin = false;
    bool loginFailed = false;
    bool pct = false;
    sf::Clock deltaClock;


    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("1.jpg");
    sf::Sprite backgroundSprite(backgroundTexture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();
        window.draw(backgroundSprite);
        if (currentState == AppState::MainMenu) {
            ImGui::Begin("Meniu Principal");

            if (ImGui::Button("Intra in cont")) {
                currentState = AppState::Login;
            }
            if (ImGui::Button("Creeaza cont")) {
                currentState = AppState::Register;
            }

            ImGui::End();
        }
        else if (currentState == AppState::Login) {
            ImGui::Begin("Login");

            static char userStr[128] = "";
            static char passStr[128] = "";
            static bool firstFrame = true;

            if (firstFrame) {
                loginFailed = false;
                firstFrame = false;
            }
            ImGui::InputText("Username", userStr, 128);
            ImGui::InputText("Parola", passStr, 128, ImGuiInputTextFlags_Password);

            if (ImGui::Button("Login")) {
                user.setName(userStr);
                user.setPassword(passStr);
                memset(userStr, 0, sizeof(userStr));
                memset(passStr, 0, sizeof(passStr));
                if (!User::verifyUser(user)) {
                    cout << "Utilizator negasit!\n";
                    loginFailed = true;
                }
                else {
                    if (user[0] == '@')
                        currentState = AppState::AdminMenu;
                    else
                        currentState = AppState::CustomerMenu;
                    firstFrame = true;
                }
            }
            if (loginFailed) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Utilizator negasit!");
            }
            if (ImGui::Button("Inapoi")) {
                currentState = AppState::MainMenu;
                firstFrame = true;
            }

            ImGui::End();
        }
        else if (currentState == AppState::Register) {
            ImGui::Begin("Inregistrare");

            static char User[128] = "";
            static char Pass[128] = "";
            static bool firstFrame = true;

            if (firstFrame) {
                pct = false;
                firstFrame = false;
            }
            ImGui::InputText("Username", User, 128);
            ImGui::InputText("Parola", Pass, 128, ImGuiInputTextFlags_Password);
            if (ImGui::Button("Inregistreaza")) {
                user.setName(User);
                user.setPassword(Pass);
                user.setLoyaltyPoints(5);
                User::addUserToList(user);
                pct = true;
                memset(User, 0, sizeof(User));
                memset(Pass, 0, sizeof(Pass));
            }
            if(pct)
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Ai primit 5 puncte din partea noastra pentru inregistrare!  ");

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::MainMenu;
                firstFrame = true;
            }

            ImGui::End();
        }
        else if (currentState == AppState::AdminMenu) {
            ImGui::Begin("Meniu Admin");
            ImGui::Text("Bine ai venit, admin!");
            if (ImGui::Button("Adauga produs")) {
                currentState = AppState::AddProduct;
            }
            if (ImGui::Button("Afiseaza meniul complet")) {
                currentState = AppState::ViewMenu;
            }
            if (ImGui::Button("Actualizeaza un produs existent")) {
                currentState = AppState::UpdateProduct;
            }
            if (ImGui::Button("Sterge un produs")) {
                currentState = AppState::DeleteProduct;
            }
            if (ImGui::Button("Afiseaza toate rezervarile")) {
                currentState = AppState::ShowReservations;
            }
            if (ImGui::Button("Afiseaza toti clientii")) {
                currentState = AppState::ShowCustomers;
            }

            if (ImGui::Button("Importa produse")) {
                Statistici::getInstance().importaProduse("meniu.txt");
                ImGui::OpenPopup("ProduseImportate");
            }

            if (ImGui::BeginPopupModal("ProduseImportate", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Produsele au fost incarcate din fisierul 'meniu.txt'");
                if (ImGui::Button("OK")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            if (ImGui::Button("Exporta date/statistici")) {
                currentState = AppState::Export;
            }

            if (ImGui::Button("Logout")) {
                currentState = AppState::MainMenu;
            }
            ImGui::End();
        }
        else if (currentState == AppState::AddProduct) {
            ImGui::Begin("Adauga Produs");

            static int productType = 0; // 0 = Bautura, 1 = Mancare, 2 = Oferta
            static char name[128] = "";
            static float price = 0.0f;
            static char description[200]="";
            static float quantity = 0.0f;
            static bool hasCaffeine = false;

            static int calories = 0;
            static bool isVegan = false;

            static bool inputError = false; // pentru eroare la input

            ImGui::Text("Selecteaza tipul produsului:");
            ImGui::RadioButton("Bautura", &productType, 0); ImGui::SameLine();
            ImGui::RadioButton("Mancare", &productType, 1); ImGui::SameLine();
            ImGui::RadioButton("Oferta", &productType, 2);

            ImGui::InputText("Nume produs", name, 128);
            ImGui::InputFloat("Pret", &price);
            ImGui::InputText("Descriere", description, 200);
            if (productType == 0 || productType == 2) { 
                ImGui::InputFloat("Cantitate (ml)", &quantity);
                ImGui::Checkbox("Contine cofeina?", &hasCaffeine);
            }
            if (productType == 1 || productType == 2) { 
                ImGui::InputInt("Calorii", &calories);
                ImGui::Checkbox("Este vegana?", &isVegan);
            }

            ImGui::Separator();
            ImGui::Text("Preview produs:");

            if (strlen(name) > 0 && price > 0.0f) {
                const char* typeLabel = "";

                if (productType == 0) {
                    typeLabel = "Bautura";
                }
                else if (productType == 1) {
                    typeLabel = "Mancare";
                }
                else if (productType == 2) {
                    typeLabel = "Oferta";
                }

                ImGui::Text("%s: %s - %.2f lei", typeLabel, name, price);
            }
            else {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Completeaza toate campurile pentru a vedea produsul!");
            }

            ImGui::Separator();

            if (ImGui::Button("Salveaza")) {
                inputError = false;

                // Validare
                if (strlen(name) == 0 || price <= 0.0f) {
                    inputError = true;
                }
                if ((productType == 0 || productType == 2) && quantity <= 0.0f) {
                    inputError = true;
                }
                if ((productType == 1 || productType == 2) && calories <= 0) {
                    inputError = true;
                }

                if (!inputError) {
                    Product* p = nullptr;
                    if (productType == 0) { // Bautura
                        Drink* d = new Drink();
                        d->setName(name);
                        d->setPrice(price);
                        d->setDescription(description);
                        d->setAmount(quantity);
                        d->setCaffeine(hasCaffeine);
                        p = d;
                    }
                    else if (productType == 1) { // Mancare
                        Food* f = new Food();
                        f->setName(name);
                        f->setPrice(price);
                        f->setDescription(description);
                        f->setKcal(calories);
                        f->setVegan(isVegan);
                        p = f;
                    }
                    else if (productType == 2) { // Oferta
                        Offer* o = new Offer();
                        o->setName(name);
                        o->setPrice(price);
                        o->setDescription(description);
                        o->setAmount(quantity);
                        o->setCaffeine(hasCaffeine);
                        o->setKcal(calories);
                        o->setVegan(isVegan);
                        p = o;
                    }

                    if (p) {
                        listOfProducts.push_back(p);
                        ImGui::OpenPopup("Succes");

                        // Resetam toate campurile
                        name[0] = '\0';
                        price = 0.0f;
                        description[0] = '\0';
                        quantity = 0.0f;
                        hasCaffeine = false;
                        calories = 0;
                        isVegan = false;
                        productType = 0;
                    }
                }
            }

            if (inputError) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Eroare: Completeaza toate campurile corect!");
            }

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
                // Resetam campurile cand plecam
                name[0] = '\0';
                price = 0.0f;
                description[0] = '\0';
                quantity = 0.0f;
                hasCaffeine = false;
                calories = 0;
                isVegan = false;
                productType = 0;
                inputError = false;
            }

            if (ImGui::BeginPopupModal("Succes", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Produs adaugat cu succes!");
                if (ImGui::Button("OK")) {
                    ImGui::CloseCurrentPopup();
                    currentState = AppState::AdminMenu;
                }
                ImGui::EndPopup();
            }

            ImGui::End();
        }
        else if (currentState == AppState::ViewMenu) {
            ImGui::Begin("Meniul Complet");

            ImGui::Text("Lista produselor disponibile:");
            ImGui::Separator();

            bool hasDrinks = false, hasFoods = false, hasOffers = false;
            sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);

            // Verificam daca avem produse pentru fiecare categorie
            for (auto p : listOfProducts) {
                if (dynamic_cast<Offer*>(p)) hasOffers = true;
                if (dynamic_cast<Drink*>(p)) hasDrinks = true;
                if (dynamic_cast<Food*>(p)) hasFoods = true;
            }

            if (listOfProducts.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Momentan nu exista produse in meniu!");
            }
            else {
                if (hasDrinks) {
                    ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "Bauturi:");
                    ImGui::Separator();
                    for (auto p : listOfProducts) {
                        if (Drink* d = dynamic_cast<Drink*>(p)) {
                            if (dynamic_cast<Offer*>(p)) continue;
                            ImGui::BulletText("%s - %.2f lei - %s \n- Cantitate: %.0f ml \n- Cofeina: %s",
                                d->getName().c_str(), d->getPrice(), d->getDescription().c_str(), d->getAmount(), d->getCaffeine() ? "Da" : "Nu");
                        }
                    }
                    ImGui::Spacing();
                }

                if (hasFoods) {
                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Mancare:");
                    ImGui::Separator();
                    for (auto p : listOfProducts) {
                        if (Food* f = dynamic_cast<Food*>(p)) {
                            if (dynamic_cast<Offer*>(p)) continue;
                            ImGui::BulletText("%s - %.2f lei - %s \n- Calorii: %d \n- Vegan: %s",
                                f->getName().c_str(), f->getPrice(), f->getDescription().c_str(), f->getKcal(), f->getVegan() ? "Da" : "Nu");
                        }
                    }
                    ImGui::Spacing();
                }

                if (hasOffers) {
                    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Oferte:");
                    ImGui::Separator();
                    for (auto p : listOfProducts) {
                        if (Offer* o = dynamic_cast<Offer*>(p)) {
                            ImGui::BulletText("%s - %.2f lei - %s\n- Cantitate: %.0f ml \n- Cofeina: %s \n- Calorii: %d \n- Vegan: %s",
                                o->getName().c_str(), o->getPrice(), o->getDescription().c_str(), o->getAmount(), o->getCaffeine() ? "Da" : "Nu", o->getKcal(), o->getVegan() ? "Da" : "Nu");
                        }
                    }
                    ImGui::Spacing();
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::UpdateProduct) {
            ImGui::Begin("Meniul Complet");

            static bool openEditPopup = false;
            static Product* productToEdit = nullptr;

            static char editName[128] = "";
            static float editPrice = 0.0f;
            static float editQuantity = 0.0f;
            static bool editCaffeine = false;
            static int editCalories = 0;
            static bool editVegan = false;

            ImGui::Text("Lista produselor disponibile:");
            ImGui::Separator();

            bool hasDrinks = false, hasFoods = false, hasOffers = false;
            sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);

            for (auto p : listOfProducts) {
                if (dynamic_cast<Offer*>(p)) hasOffers = true;
                if (dynamic_cast<Drink*>(p)) hasDrinks = true;
                if (dynamic_cast<Food*>(p)) hasFoods = true;
            }

            if (listOfProducts.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Momentan nu exista produse in meniu!");
            }
            else {
                for (size_t i = 0; i < listOfProducts.size(); ++i) {
                    Product* p = listOfProducts[i];

                    if (p->getProductType()=="Oferta") {
                        Offer* o = dynamic_cast<Offer*>(p);
                        if (i == 0 || listOfProducts[i-1]->getProductType()!="Oferta") {
                            ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Oferte:");
                            ImGui::Separator();
                        }
                        ImGui::BulletText("%s - %.2f lei - %s\n- Cantitate: %.0f ml \n- Cofeina: %s \n- Calorii: %d \n- Vegan: %s",
                            o->getName().c_str(), o->getPrice(), o->getDescription().c_str(), o->getAmount(), o->getCaffeine() ? "Da" : "Nu", o->getKcal(), o->getVegan() ? "Da" : "Nu");
                    }
                    if (p->getProductType() == "Bautura") {
                        Drink* d = dynamic_cast<Drink*>(p);
                        if (i == 0 || listOfProducts[i - 1]->getProductType() != "Bautura") {
                            ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "Bauturi:");
                            ImGui::Separator();
                        }
                        ImGui::BulletText("%s - %.2f lei - %s \n- Cantitate: %.0f ml \n- Cofeina: %s",
                            d->getName().c_str(), d->getPrice(), d->getDescription().c_str(), d->getAmount(), d->getCaffeine() ? "Da" : "Nu");
                    }
                    if (p->getProductType() == "Mancare") {
                        Food* f = dynamic_cast<Food*>(p);
                        if (i == 0 || listOfProducts[i - 1]->getProductType() != "Mancare") {
                            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Mancare:");
                            ImGui::Separator();
                        }
                        ImGui::BulletText("%s - %.2f lei - %s \n- Calorii: %d \n- Vegan: %s",
                            f->getName().c_str(), f->getPrice(), f->getDescription().c_str(), f->getKcal(), f->getVegan() ? "Da" : "Nu");
                    }
                    

                    ImGui::SameLine();
                    std::string updateId = "Actualizeaza##" + std::to_string(i);
                    if (ImGui::Button(updateId.c_str())) {
                        openEditPopup = true;
                        productToEdit = listOfProducts[i];

                        if (Offer* o = dynamic_cast<Offer*>(productToEdit)) {
                            strcpy(editName, o->getName().c_str());
                            editPrice = o->getPrice();
                            editQuantity = o->getAmount();
                            editCaffeine = o->getCaffeine();
                            editCalories = o->getKcal();
                            editVegan = o->getVegan();
                        }
                        else if (Drink* d = dynamic_cast<Drink*>(productToEdit) ) {
                            strcpy(editName, d->getName().c_str());
                            editPrice = d->getPrice();
                            editQuantity = d->getAmount();
                            editCaffeine = d->getCaffeine();
                        }
                        else if (Food* f = dynamic_cast<Food*>(productToEdit)) {
                            strcpy(editName, f->getName().c_str());
                            editPrice = f->getPrice();
                            editCalories = f->getKcal();
                            editVegan = f->getVegan();
                        }
                        
                    }
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            // Popup de actualizare produs
            if (openEditPopup && productToEdit != nullptr) {
                ImGui::OpenPopup("Actualizeaza Produs");
            }

            if (ImGui::BeginPopupModal("Actualizeaza Produs", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::InputText("Nume produs", editName, 128);
                ImGui::InputFloat("Pret", &editPrice);

                if (dynamic_cast<Drink*>(productToEdit)) {
                    ImGui::InputFloat("Cantitate (ml)", &editQuantity);
                    ImGui::Checkbox("Contine cofeina?", &editCaffeine);
                }
                if (dynamic_cast<Food*>(productToEdit)) {
                    ImGui::InputInt("Calorii", &editCalories);
                    ImGui::Checkbox("Este vegana?", &editVegan);
                }

                if (ImGui::Button("Salveaza modificari")) {
                    productToEdit->setName(editName);
                    productToEdit->setPrice(editPrice);

                    if (Offer* o = dynamic_cast<Offer*>(productToEdit)) {
                        o->setAmount(editQuantity);
                        o->setCaffeine(editCaffeine);
                        o->setKcal(editCalories);
                        o->setVegan(editVegan);
                    }
                    else if (Drink* d = dynamic_cast<Drink*>(productToEdit)) {
                        d->setAmount(editQuantity);
                        d->setCaffeine(editCaffeine);
                    }
                    else if (Food* f = dynamic_cast<Food*>(productToEdit)) {
                        f->setKcal(editCalories);
                        f->setVegan(editVegan);
                    }
                    

                    openEditPopup = false;
                    productToEdit = nullptr;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();

                if (ImGui::Button("Anuleaza")) {
                    openEditPopup = false;
                    productToEdit = nullptr;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::End();
        }

        else if (currentState == AppState::DeleteProduct) {
            ImGui::Begin("Sterge un produs");

            ImGui::Text("Lista produselor disponibile:");
            ImGui::Separator();

            bool hasDrinks = false, hasFoods = false, hasOffers = false;
            sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);

            // Verificam existenta produselor pe categorii
            for (auto p : listOfProducts) {
                if (dynamic_cast<Offer*>(p)) hasOffers = true;
                if (dynamic_cast<Drink*>(p)) hasDrinks = true;
                if (dynamic_cast<Food*>(p)) hasFoods = true;
            }

            if (listOfProducts.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Momentan nu exista produse in meniu!");
            }
            else {
                int index = 0;
                // Folosim un index pentru a ști ce ștergem
                if (hasDrinks) {
                    ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "Bauturi:");
                    ImGui::Separator();
                    for (size_t i = 0; i < listOfProducts.size(); ++i) {
                        if (Drink* d = dynamic_cast<Drink*>(listOfProducts[i])) {
                            if (dynamic_cast<Offer*>(listOfProducts[i])) continue;
                            ImGui::BulletText("%s - %.2f lei - %s \n- Cantitate: %.0f ml \n- Cofeina: %s",
                                d->getName().c_str(), d->getPrice(), d->getDescription().c_str(), d->getAmount(), d->getCaffeine() ? "Da" : "Nu");

                            ImGui::SameLine();
                            std::string buttonId = "Sterge##" + std::to_string(i);
                            if (ImGui::Button(buttonId.c_str())) {
                                delete listOfProducts[i];
                                listOfProducts.erase(listOfProducts.begin() + i);
                                break; // IMPORTANT: break ca să nu stricăm iteratorul
                            }
                        }
                    }
                    ImGui::Spacing();
                }

                if (hasFoods) {
                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Mancare :");
                    ImGui::Separator();
                    for (size_t i = 0; i < listOfProducts.size(); ++i) {
                        if (Food* f = dynamic_cast<Food*>(listOfProducts[i])) {
                            if (dynamic_cast<Offer*>(listOfProducts[i])) continue;
                            ImGui::BulletText("%s - %.2f lei - %s \n- Calorii: %d \n- Vegan: %s",
                                f->getName().c_str(), f->getPrice(), f->getDescription().c_str(), f->getKcal(), f->getVegan() ? "Da" : "Nu");

                            ImGui::SameLine();
                            std::string buttonId = "Sterge##" + std::to_string(i);
                            if (ImGui::Button(buttonId.c_str())) {
                                delete listOfProducts[i];
                                listOfProducts.erase(listOfProducts.begin() + i);
                                break;
                            }
                        }
                    }
                    ImGui::Spacing();
                }

                if (hasOffers) {
                    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Oferte:");
                    ImGui::Separator();
                    for (size_t i = 0; i < listOfProducts.size(); ++i) {
                        if (Offer* o = dynamic_cast<Offer*>(listOfProducts[i])) {
                            ImGui::BulletText("%s - %.2f lei - %s\n- Cantitate: %.0f ml \n- Cofeina: %s \n- Calorii: %d \n- Vegan: %s",
                                o->getName().c_str(), o->getPrice(), o->getDescription().c_str(), o->getAmount(), o->getCaffeine() ? "Da" : "Nu", o->getKcal(), o->getVegan() ? "Da" : "Nu");

                            ImGui::SameLine();
                            std::string buttonId = "Sterge##" + std::to_string(i);
                            if (ImGui::Button(buttonId.c_str())) {
                                delete listOfProducts[i];
                                listOfProducts.erase(listOfProducts.begin() + i);
                                break;
                            }
                        }
                    }
                    ImGui::Spacing();
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::ShowReservations) {
            ImGui::Begin("Toate Rezervarile");

            if (Reservation::getSize() == 0) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nu exista rezervari momentan.");
            }
            else {
                std::sort(Reservation::getReservations(), Reservation::getReservations() + Reservation::getSize());

                ImGui::Text("Lista rezervarilor:");
                ImGui::Separator();

                ImGui::Columns(3, "Rezervari"); // coloane: UserID, Numar persoane, Ora
                ImGui::Text("ID Client"); ImGui::NextColumn();
                ImGui::Text("Numar Persoane"); ImGui::NextColumn();
                ImGui::Text("Data si Ora"); ImGui::NextColumn();
                ImGui::Separator();

                for (int i = 0; i < Reservation::getSize(); ++i) {
                    Reservation& r = Reservation::getReservations()[i];

                    ImGui::Text("%d", r.getCustomer()->getUserId()); ImGui::NextColumn();
                    ImGui::Text("%d", r.getNumberOfSeats()); ImGui::NextColumn();
                    ImGui::Text("%s", r.getReservationTime()); ImGui::NextColumn();
                }

                ImGui::Columns(1);
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::ShowCustomers) {
            ImGui::Begin("Lista Clientilor");

            if (User::getSize() == 0) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nu exista clienti momentan.");
            }
            else {
                ImGui::Text("Clientii inregistrati:");
                ImGui::Separator();

                ImGui::Columns(3, "Clienti"); // 3 coloane: Nume, ID, Puncte
                ImGui::Text("Nume Client"); ImGui::NextColumn();
                ImGui::Text("ID"); ImGui::NextColumn();
                ImGui::Text("Puncte Fidelitate"); ImGui::NextColumn();
                ImGui::Separator();

                for (int i = 0; i < User::getSize(); ++i) {
                    User& u = User::getUsers()[i];

                    ImGui::Text("%s", u.getName()); ImGui::NextColumn();
                    ImGui::Text("%d", u.getUserId()); ImGui::NextColumn();
                    ImGui::Text("%d", u.getLoyaltyPoints()); ImGui::NextColumn();
                }

                ImGui::Columns(1);
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            ImGui::End();
        }


        else if (currentState == AppState::CustomerMenu) {
            ImGui::Begin("Meniu Client");
            ImGui::Text("Bine ai venit!");

            if (ImGui::Button("Plaseaza o comanda")) {
                currentState = AppState::PlaceOrder;
            }
            if (ImGui::Button("Verifica puncte de fidelitate")) {
                currentState = AppState::VerifyLPoints;
            }
            if (ImGui::Button("Rezerva un loc")) {
                currentState = AppState::Reservation;
            }
            if (ImGui::Button("Vezi rezervarile tale")) {
                currentState = AppState::ViewMyReservs;
            }
            if (ImGui::Button("Anuleaza o rezervare")) {
                currentState = AppState::CancelReservation;
            }
            if (ImGui::Button("Logout")) {
                currentState = AppState::MainMenu;
            }
            ImGui::End();
        }
        else if (currentState == AppState::PlaceOrder) {
            ImGui::Begin("Plasare Comanda");
            char* nameu = user.getName();
            User* temp;
            temp = User::getUserByName(nameu);
            static int selectedProductIndex = -1;
            static int currentQuantity = 1;
            static std::vector<Product*> selectedProducts;
            static std::vector<int> quantities;
            static bool useLoyalty = false;
            static bool showPopup = false;
            static float totalBefore = 0.0f;
            static float totalAfter = 0.0f;
            static int puncteFolosite = 0;

            ImGui::Text("Meniu complet - Selecteaza un produs:");
            ImGui::Separator();
            sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);
            int index_cat = 1;
            for (int i = 0; i < listOfProducts.size(); ++i) {
                Product* p = listOfProducts[i];

                if (index_cat == 3 && dynamic_cast<Offer*>(p)) {
                    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Oferte:");
                    ImGui::Separator();
                    index_cat++;
                }
                else if (index_cat == 1 && dynamic_cast<Drink*>(p)) {
                    ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "Bauturi:");
                    ImGui::Separator();
                    index_cat++;
                }
                else if (index_cat == 2 && dynamic_cast<Food*>(p)) {
                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Mancare:");
                    ImGui::Separator();
                    index_cat++;
                }

                std::string label = p->getName() + " - " + std::to_string(p->getPrice()) + " lei";

                bool isSelected = (selectedProductIndex == i);
                if (ImGui::RadioButton(label.c_str(), isSelected)) {
                    selectedProductIndex = isSelected ? -1 : i;  // toggle
                }

                ImGui::Indent();
                ImGui::Text("Descriere: %s", p->getDescription().c_str());
                
                if (Offer* o = dynamic_cast<Offer*>(p)) {
                    ImGui::Text("Tip: Oferta | Cantitate: %.0f ml | Cofeina: %s | Calorii: %d | Vegan: %s",
                        o->getAmount(), o->getCaffeine() ? "Da" : "Nu", o->getKcal(), o->getVegan() ? "Da" : "Nu");
                }
                else if (Drink* d = dynamic_cast<Drink*>(p)) {

                    ImGui::Text("Tip: Bautura | Cantitate: %.0f ml | Cofeina: %s",
                        d->getAmount(), d->getCaffeine() ? "Da" : "Nu");
                }
                else if (Food* f = dynamic_cast<Food*>(p)) {
                    ImGui::Text("Tip: Mancare | Calorii: %d | Vegan: %s",
                        f->getKcal(), f->getVegan() ? "Da" : "Nu");
                }

                if (selectedProductIndex == i) {
                    ImGui::InputInt("Cantitate", &currentQuantity);
                    currentQuantity = std::max(currentQuantity, 1);

                    if (ImGui::Button("Adauga in comanda")) {
                        selectedProducts.push_back(p);
                        quantities.push_back(currentQuantity);
                        selectedProductIndex = -1;
                        currentQuantity = 1;
                    }
                }

                ImGui::Unindent();
                ImGui::Separator();
            }

            ImGui::Text("Produse adaugate in comanda:");
            ImGui::Separator();

            totalBefore = 0;
            for (int i = 0; i < selectedProducts.size(); ++i) {
                float partial = selectedProducts[i]->getPrice() * quantities[i];
                totalBefore += partial;

                ImGui::BulletText("%s x%d = %.2f lei",
                    selectedProducts[i]->getName().c_str(), quantities[i], partial);

                ImGui::SameLine();
                std::string btn = "Sterge##" + std::to_string(i);
                if (ImGui::Button(btn.c_str())) {
                    selectedProducts.erase(selectedProducts.begin() + i);
                    quantities.erase(quantities.begin() + i);
                    break;
                }
            }

            ImGui::Separator();
            ImGui::Checkbox("Foloseste puncte de fidelitate?", &useLoyalty);
            ImGui::Text("Total: %.2f lei", totalBefore);

            if (ImGui::Button("Plaseaza comanda") && !selectedProducts.empty()) {
                Order comanda(temp, selectedProducts, quantities, useLoyalty);
                float valoareComanda = comanda.getTotalValue();
                int puncteUser = (*temp).getLoyaltyPoints();
                comanda.completeOrder();
                Order::addOrderToList(comanda);

                if (useLoyalty) {
                    if (puncteUser <= valoareComanda)
                        puncteFolosite = puncteUser;
                    else
                        puncteFolosite = (int)valoareComanda;
                }
                else
                    puncteFolosite = 0;
                totalAfter = comanda.getTotalValue();/*
                cout << totalBefore << " " << totalAfter << " " << puncteFolosite << "\n";
                cout << temp->getLoyaltyPoints();*/
               
                showPopup = true;
            }

            if (ImGui::Button("Inapoi")) {
                selectedProducts.clear();
                quantities.clear();
                useLoyalty = false;
                currentState = AppState::CustomerMenu;
            }

            if (showPopup) ImGui::OpenPopup("Comanda plasata!");
            if (ImGui::BeginPopupModal("Comanda plasata!", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Comanda ta a fost plasata cu succes!");

                ImGui::Text("Total inainte: %.2f lei", totalBefore);
                ImGui::Text("Puncte folosite: %d", puncteFolosite);
                ImGui::Text("Total final: %.2f lei", totalAfter);
                if (ImGui::Button("OK")) {
                    showPopup = false;
                    ImGui::CloseCurrentPopup();
                    currentState = AppState::CustomerMenu;
                    selectedProducts.clear();
                    quantities.clear();
                    useLoyalty = false;
                }
                ImGui::EndPopup();
            }

            ImGui::End();
         }
        else if (currentState == AppState::VerifyLPoints) {
            ImGui::Begin("Puncte dvs de loialitate");
            char* nameu = user.getName();
            User* temp;
            temp = User::getUserByName(nameu);

            if (temp != nullptr) {
                ImGui::Text("Nume: %s", temp->getName());
                ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.0f, 1.0f), "Puncte de fidelitate acumulate: %d", temp->getLoyaltyPoints());

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Text("Un punct de fidelitate valoreaza 1 leu.");
                ImGui::Text("Cum poti acumula puncte?");
                ImGui::BulletText("Pentru fiecare 20 lei cheltuiti, primesti 1 punct de fidelitate.");
                ImGui::BulletText("La inregistrare primesti automat 5 puncte.");
                ImGui::BulletText("Foloseste punctele atunci cand plasezi o comanda.");
                ImGui::SameLine();
                ImGui::TextDisabled("(?)");
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Bifeaza optiunea 'Foloseste puncte de fidelitate' la finalizarea comenzii.");

                ImGui::Spacing();
            }
            else 
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Eroare: utilizatorul nu a fost gasit.");
                

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::CustomerMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::Reservation) {
            ImGui::Begin("Rezerva un loc");

            static int zi = 0, luna = 0, an = 2025;
            static int ora = 0, minut = 0;
            static int nrPersoane = 0;
            static bool showSuccess = false;
            static bool validareActivata = true;
            char what[256];

            ImGui::Text("Completeaza datele rezervarii:");

            ImGui::InputInt("Zi", &zi);
            ImGui::InputInt("Luna", &luna);
            ImGui::InputInt("An", &an);

            ImGui::InputInt("Ora", &ora);
            ImGui::InputInt("Minut", &minut);

            ImGui::InputInt("Numar persoane", &nrPersoane);

            // formatare data
            std::string formatted =
                (zi < 10 ? "0" : "") + to_string(zi) + "/" +
                (luna < 10 ? "0" : "") + to_string(luna) + "/" +
                to_string(an) + " " +
                (ora < 10 ? "0" : "") + to_string(ora) + ":" +
                (minut < 10 ? "0" : "") + to_string(minut);

            if (ImGui::Button("Rezerva")) {
                try {
                    Validator<int>::throwIfInRange(zi, 1, 31, "Ziua");
                    Validator<int>::throwIfInRange(luna, 1, 12, "Luna");
                    Validator<int>::throwIfInRange(ora, 0, 23, "Ora");
                    Validator<int>::throwIfInRange(minut, 0, 59, "Minutul");
                    Validator<int>::throwIfNotStrictPositive(nrPersoane, "Numarul de persoane");

                    Validator<string>::throwIfInvalidDate(formatted.substr(0, 10), "Data");
                    Validator<string>::throwIfDateInPast(formatted.substr(0, 10), "Data");
                    Validator<string>::throwIfTimeIntervalIncorrect(formatted.substr(11), "Ora");
                    Validator<User*>::throwIfNull(User::getUserByName(user.getName()), "Client");
                    

                    User* u = User::getUserByName(user.getName());
                    Reservation r(u->getUserId(), nrPersoane, &formatted[0]);
                    Reservation::addReservationToList(r);
                    showSuccess = true;
                }
                catch (const std::exception& ex) {
                    strcpy(what, ex.what());
                    ImGui::OpenPopup("EroareValidare");
                }
            }

            if (showSuccess) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Rezervarea a fost efectuata!");
                ImGui::Text("Data si ora: %s", formatted.c_str());
            }

            if (ImGui::BeginPopupModal("EroareValidare", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Date invalide! Verifica campurile introduse.");
                ImGui::TextColored(ImVec4(1, 0, 0, 1), what);
                if (ImGui::Button("OK")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::CustomerMenu;
                showSuccess = false;
            }
            ImGui::End();
        }

        else if (currentState == AppState::ViewMyReservs) {
            ImGui::Begin("Rezervarile Mele");
            User* currentUser = User::getUserByName(user.getName());
            bool found = false;

            if (currentUser != nullptr) {
                for (int i = 0; i < Reservation::getSize(); ++i) {
                    Reservation& r = Reservation::getReservations()[i];
                    /*cout << *r.getCustomer() << "\n";
                    cout << *currentUser << "\n";*/
                    if (r.getCustomer()->getUserId() == currentUser->getUserId()) {
                        found = true;

                        ImGui::Separator();
                        ImGui::Text("Rezervare #%d", r.getReservationId());
                        ImGui::BulletText("Data si ora: %s", r.getReservationTime());
                        ImGui::BulletText("Numar persoane: %d", r.getNumberOfSeats());
                    }
                }
                if (!found) {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Nu ai nicio rezervare activa.");
                }
            }
            else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Eroare: utilizator negasit.");
            }

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::CustomerMenu;
            }

            ImGui::End();
        }

        else if (currentState == AppState::CancelReservation) {
            ImGui::Begin("Anuleaza o rezervare");

            User* currentUser = User::getUserByName(user.getName());
            bool found = false;

            if (currentUser) {
                for (int i = 0; i < Reservation::getSize(); ++i) {
                    Reservation& r = Reservation::getReservations()[i];

                    if (r.getCustomer() && r.getCustomer()->getUserId() == currentUser->getUserId()) {
                        found = true;

                        ImGui::Separator();
                        ImGui::Text("Rezervare #%d", r.getReservationId());
                        ImGui::BulletText("Data si ora: %s", r.getReservationTime());
                        ImGui::BulletText("Numar persoane: %d", r.getNumberOfSeats());

                        std::string btnId = "Anuleaza##" + std::to_string(r.getReservationId());
                        if (ImGui::Button(btnId.c_str())) {
                            Reservation::deleteReservationById(r.getReservationId());
                            ImGui::OpenPopup("Succes");
                            break;  
                        }
                    }
                }

                if (!found) {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Nu ai rezervari active.");
                }
            }
            else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Eroare: utilizator negasit.");
            }

            if (ImGui::BeginPopupModal("Succes", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Rezervarea a fost anulata cu succes!");
                if (ImGui::Button("OK")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::CustomerMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::Export) {
            ImGui::Begin("Exporta Date / Statistici");

            static bool exportaProduse = false;
            static bool exportaUtilizatori = false;
            static bool exportaRezervari = false;
            static bool exportaStatistici = false;

            ImGui::Text("Selecteaza ce vrei sa exporti:");
            ImGui::Checkbox("Produse (produse.txt)", &exportaProduse);
            ImGui::Checkbox("Utilizatori (useri.txt)", &exportaUtilizatori);
            ImGui::Checkbox("Rezervari (rezervari.txt)", &exportaRezervari);
            ImGui::Checkbox("Statistici generale (statistici.txt)", &exportaStatistici);

            if (ImGui::Button("Exporta")) {
                Statistici& stats = Statistici::getInstance();
                 
                if (exportaProduse)
                    stats.exportaDate(listOfProducts, "produse.txt");

                if (exportaUtilizatori) {
                    vector<User> useri(User::getUsers(), User::getUsers() + User::getSize());
                    stats.exportaDate(useri, "useri.txt");
                }

                if (exportaRezervari) {
                    vector<Reservation> rezervari(Reservation::getReservations(), Reservation::getReservations() + Reservation::getSize());
                    stats.exportaDate(rezervari, "rezervari.txt");
                }

                if (exportaStatistici)
                    stats.exportaStatistici("statistici.txt");

                ImGui::OpenPopup("SuccesExport");
            }

            ImGui::SameLine();
            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            if (ImGui::BeginPopupModal("SuccesExport", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Exportul a fost realizat cu succes!");
                if (ImGui::Button("OK")) {
                    ImGui::CloseCurrentPopup();
                    currentState = AppState::AdminMenu;
                }
                ImGui::EndPopup();
            }

            ImGui::End();
            }



        //window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    User::deleteUserList();
    Order::deleteOrderList();
    Reservation::deleteReservationList();
    for (auto p : listOfProducts) {
        delete p;
    }
    listOfProducts.clear();


    return 0;
=======
﻿#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include<bits/stdc++.h>
#include <filesystem>
using namespace std;
const unsigned int MAX_LENGTH = 256;

int frequency[MAX_LENGTH];


bool isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;

    int zi = std::stoi(date.substr(0, 2));
    int luna = std::stoi(date.substr(3, 2));
    int an = std::stoi(date.substr(6, 4));

    if (zi < 1 || zi > 31) return false;
    if (luna < 1 || luna > 12) return false;
    if (an < 2024) return false;

    if (luna == 2 && zi > 29) return false; // februarie maxim 29 zile
    if ((luna == 4 || luna == 6 || luna == 9 || luna == 11) && zi > 30) return false;

    return true;
}


class User {
private:
    const int userId;
    char* name;
    char* password;
    bool isAdmin;
    int loyaltypoints;

    static int generatorUId;
    static int szListOfUsers;
    static User* listOfUsers;
public:
    User() : userId(generatorUId++) {
        name = new char[MAX_LENGTH];
        password = new char[MAX_LENGTH];
        loyaltypoints = 0;
        isAdmin = false;
    }
    User(char* name, char* password, bool isAdmin, int loyaltypoints) : userId(generatorUId++) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
        this->isAdmin = isAdmin;
        this->loyaltypoints = loyaltypoints;
    }
    User(char* name) : userId(generatorUId++) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->password = nullptr;
        isAdmin = false;
        this->loyaltypoints = 0;

    }
    User(char* name, char* password) : userId(generatorUId++) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
        this->isAdmin = false;
        this->loyaltypoints = 0;

    }
    User(const User& user) : userId(generatorUId++) {
        this->isAdmin = user.isAdmin;
        this->loyaltypoints = user.loyaltypoints;
        if (this->name != nullptr) {
            delete[] this->name;
        }
        this->name = new char[strlen(user.name) + 1];
        strcpy(this->name, user.name);

        if (this->password != nullptr) {
            delete[] this->password;
        }
        this->password = new char[strlen(user.password) + 1];
        strcpy(this->password, user.password);
    }
    ~User() {
        if (this->name != nullptr) {
            delete[] this->name;
        }
        if (this->password != nullptr) {
            delete[] this->password;
        }
    }
    friend istream& operator>>(istream& in, User& user) {
        cout << "Introdu numele: ";
        in.get(user.name, MAX_LENGTH);
        in.get();

        cout << "Introdu parola: ";
        in.get(user.password, MAX_LENGTH);
        in.get();
        if (strcmp(user.name, "admin") == 0)
            user.isAdmin = true;
        else
            user.isAdmin = false;
        return in;
    }
    void readFromFile(ifstream& inFile) {
        inFile.get(name, 256);
        inFile.get();
        inFile.get(password, 256);
        inFile.get();
        inFile >> isAdmin;
        inFile.get();
        inFile >> loyaltypoints;
        inFile.get();
    }
    friend ostream& operator<<(ostream& out, const User& user) {
        out << "ID user: " << user.userId << "\n";
        out << "Nume user: " << user.name << '\n';
        out << "Parola user: " << user.password << '\n';
        if (!user.isAdmin)
            out << "Puncte de fidelitate: " << user.loyaltypoints << '\n';
        return out;
    }
    User& operator=(const User& user) {
        if (this == &user)
            return *this;

        this->isAdmin = user.isAdmin;
        this->loyaltypoints = user.loyaltypoints;
        if (this->name != nullptr) {
            delete[] this->name;
        }
        this->name = new char[strlen(user.name) + 1];
        strcpy(this->name, user.name);

        if (this->password != nullptr) {
            delete[] this->password;
        }
        this->password = new char[strlen(user.password) + 1];
        strcpy(this->password, user.password);

        return *this;
    }
    char operator[](int index) {
        if (index >= 0 && index < strlen(name)) {
            return name[index];
        }
        return 'X';
    }

    bool operator==(const User& user) const {
        return (strcmp(this->name, user.name) == 0) && (strcmp(this->password, user.password) == 0);
    }
    User& operator++() {
        this->loyaltypoints++;
        return *this;
    }
    User& operator+(int x) {
        this->loyaltypoints += x;
        return *this;
    }

    friend User& operator+(int x, User& user) {
        return user + x;
    }
    User& operator-(int x) {
        this->loyaltypoints -= x;
        return *this;
    }
    friend User& operator-(int x, User& user) {
        user.loyaltypoints = x - user.loyaltypoints;
        return user;
    }
    bool operator<(const User& user) const {
        return this->loyaltypoints < user.loyaltypoints;
    }

    static void addUserToList(const User& user);
    static bool verifyUser(const User& user);
    static User* getUserByName(char* name);
    static User* getUserById(int Id);
    static User* getUsers();
    static int getSize();

    int getUserId();
    char* getName();
    char* getPassword();
    bool getAdmin();
    int getLoyaltyPoints();
    void setName(char* name);
    void setPassword(char* password);
    void setAdmin(bool isAdmin);
    void setLoyaltyPoints(int loyaltypoints);
    void addLoyaltyPoints(int points);
    static void showAllUsers();
    static void deleteUserList() {
        if (listOfUsers != nullptr) {
            delete[] listOfUsers;
            listOfUsers = nullptr;
            szListOfUsers = 0;
        }
    }


};

User* User::getUserByName(char* name) {
    for (int i = 0;i < szListOfUsers;i++) {
        if (strcmp(name, listOfUsers[i].name) == 0) {
            return &listOfUsers[i];
        }
    }
    return nullptr;
}
User* User::getUserById(int id) {
    for (int i = 0;i < szListOfUsers;i++) {
        if (listOfUsers[i].userId == id) {
            return &listOfUsers[i];
        }
    }
    return nullptr;
}
void User::addUserToList(const User& user) {
    listOfUsers[szListOfUsers++] = user;
}
bool User::verifyUser(const User& user) {
    for (int i = 0;i < szListOfUsers;i++)
        if (strcmp(listOfUsers[i].name, user.name) == 0)
            if (strcmp(listOfUsers[i].password, user.password) == 0)
                return true;
    return false;
}
void User::addLoyaltyPoints(int points) {
    this->loyaltypoints += points;
}
int User::getUserId() {
    return this->userId;
}
char* User::getName() {
    return this->name;
}
char* User::getPassword() {
    return this->password;
}
bool User::getAdmin() {
    return this->isAdmin;
}
int User::getLoyaltyPoints() {
    return this->loyaltypoints;
}
void User::setName(char* name) {
    if (this->name != nullptr) {
        delete[] this->name;
    }
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}
void User::setPassword(char* password) {
    if (this->password != nullptr) {
        delete[] this->password;
    }
    this->password = new char[strlen(password) + 1];
    strcpy(this->password, password);
}
void User::setAdmin(bool isAdmin) {
    this->isAdmin = isAdmin;
}
void User::setLoyaltyPoints(int loyaltypoints) {
    this->loyaltypoints = loyaltypoints;
}
int User::generatorUId = 1;
int User::szListOfUsers = 0;
User* User::listOfUsers = new User[MAX_LENGTH];

void User::showAllUsers() {
    for (int i = 0; i < szListOfUsers; i++) {
        cout << listOfUsers[i] << "\n";
    }
}
User* User::getUsers() {
    return listOfUsers;
}

int User::getSize() {
    return szListOfUsers;
}

class Product {
protected:
    const int productId;
    string name;
    float price;
    string description;

    static int generatorProductId;
public:
    Product() :productId(generatorProductId++) {
        name = "ProductX";
        price = 0.0;
        description = "";
    }
    Product(string name, float price, string description) :productId(generatorProductId++) {
        this->name = name;
        this->price = price;
        this->description = description;
    }
    Product(string name) :productId(generatorProductId++) {
        this->name = name;
        price = 0.0;
        description = "";
    }
    Product(string name, float price) :productId(generatorProductId++) {
        this->name = name;
        this->price = price;
        this->description = "";
    }
    Product(const Product& p) :productId(p.productId) {
        this->name = p.name;
        this->price = p.price;
        this->description = p.description;
    }
    Product& operator=(const Product& p) {
        if (this != &p) {
            name = p.name;
            price = p.price;
            description = p.description;
        }
        return *this;
    }
    virtual ~Product() {}

    friend istream& operator>>(istream& in, Product& p) {
        cout << "Denumire: ";
        getline(in, p.name);
        cout << "Pret: ";
        in >> p.price;
        in.get();
        cout << "Descriere: ";
        getline(in, p.description);
        return in;
    }

    friend ostream& operator<<(ostream& out, const Product& p) {
        out << "Id produs: " << p.productId << "\n";
        out << "Denumire: " << p.name << '\n';
        out << "Pretul: " << p.price << '\n';
        out << "Descriere: " << p.description << '\n';
        return out;
    }

    virtual void showDetails() const = 0;
    virtual string getProductType() const = 0;

    int getProductId() {
        return productId;
    }
    string getName() {
        return name;
    }
    float getPrice() {
        return price;
    }
    string getDescription() {
        return description;
    }
    void setName(string name) {
        this->name = name;
    }
    void setPrice(float price) {
        this->price = price;
    }
    void setDescription(string description) {
        this->description = description;
    }

};

int Product::generatorProductId = 10;

class Food : public virtual Product {
protected:
    int kcal;
    bool vegan;
public:
    Food() :Product() {
        kcal = 0;
        vegan = false;
    }
    Food(string name, float price, string description, int kcal, bool vegan)
        : Product(name, price, description) {
        this->kcal = kcal;
        this->vegan = vegan;
    }
    Food(string name, float price) :Product(name, price) {
        kcal = 0;
        vegan = false;
    }
    Food(string name) : Product(name) {
        kcal = 0;
        vegan = 0;
    }
    Food(string name, float price, int kcal) :Product(name, price) {
        this->kcal = kcal;
        this->vegan = false;
    }
    Food(const Food& f) : Product(f) {
        this->kcal = f.kcal;
        this->vegan = f.vegan;
    }
    Food& operator=(const Food& f) {
        if (this != &f) {
            Product::operator=(f);
            kcal = f.kcal;
            vegan = f.vegan;
        }
        return *this;
    }
    friend ostream& operator<<(ostream& out, const Food& f) {
        out << (Product&)f;
        cout << "KiloCalorii: ";
        out << f.kcal << "\n";
        cout << "Vegan: ";
        out << (f.vegan ? "Da\n" : "Nu\n");
        return out;
    }

    friend istream& operator>>(istream& in, Food& f) {
        in >> (Product&)f;
        cout << "Calorii: ";
        in >> f.kcal;
        in.get();
        cout << "Este vegana? (1/0): ";
        in >> f.vegan;
        in.get();
        return in;
    }
    void showDetails() const;
    string getProductType() const;

    int getKcal() {
        return kcal;
    }
    bool getVegan() {
        return vegan;
    }
    void setKcal(int kcal) {
        this->kcal = kcal;
    }
    void setVegan(bool vegan) {
        this->vegan = vegan;
    }
}; 

void Food::showDetails() const {
    cout << "[MANCARE]";
    cout << ": #" << productId << '\n';
    cout << "Denumire: " << name << '\n';
    cout << "Pretul: " << price << '\n';
    cout << "Descriere: " << description << '\n';
    cout << "KiloCalorii: " << kcal << "\n";
    cout << "Vegan: " << (vegan ? "Da" : "Nu") << "\n";
}
string Food::getProductType() const {
    return "Mancare";
}

class Drink : public virtual Product {
protected:
    float amount;
    bool caffeine;
public:
    Drink() :Product() {
        amount = 0;
        caffeine = false;
    }
    Drink(string name, float price, string description, float amount, bool caffeine) :
        Product(name, price, description) {
        this->amount = amount;
        this->caffeine = caffeine;
    }
    Drink(string name, float price) : Product(name, price) {
        amount = 0;
        caffeine = false;
    }
    Drink(string name, float price, int amount) : Product(name, price) {
        this->amount = amount;
        caffeine = false;
    }
    Drink(string name) :Product(name) {
        amount = 0;
        caffeine = false;
    }
    Drink& operator=(const Drink& d) {
        if (this != &d) {
            Product::operator=(d);
            amount = d.amount;
            caffeine = d.caffeine;
        }
        return *this;
    }
    friend ostream& operator<<(ostream& out, const Drink& d) {
        out << (Product&)d;
        cout << "Cantitate: ";
        out << d.amount << "\n";
        cout << "Cofeina: ";
        out << (d.caffeine ? "Da\n" : "Nu\n");
        return out;
    }

    friend istream& operator>>(istream& in, Drink& d) {
        in >> (Product&)d;
        cout << "Cantitate: ";
        in >> d.amount;
        in.get();
        cout << "Contine cofeina? (1/0): ";
        in >> d.caffeine;
        in.get();
        return in;
    }
    void showDetails() const;
    string getProductType() const;

    float getAmount() {
        return amount;
    }
    bool getCaffeine() {
        return caffeine;
    }
    void setAmount(float amount) {
        this->amount = amount;
    }
    void setCaffeine(bool caffeine) {
        this->caffeine = caffeine;
    }
};


void Drink::showDetails() const {
    cout << "[BAUTURA]";
    cout << ": #" << productId << '\n';
    cout << "Denumire: " << name << '\n';
    cout << "Pretul: " << price << '\n';
    cout << "Descriere: " << description << '\n';
    cout << "Cantitate: " << amount << "\n";
    cout << "Cofeina: " << (caffeine ? "Da" : "Nu") << "\n";
}

string Drink::getProductType() const {
    return "Bautura";
}

class Offer : public Drink, public Food {

public:
    Offer() : Drink(), Food() {}
    Offer(string name, float price, string description, float amount, bool caffeine, int calories, bool vegan)
        : Product(name, price, description) {
        this->amount = amount;
        this->caffeine = caffeine;
        this->kcal = calories;
        this->vegan = vegan;
    }
    Offer(string name, float price) : Drink(name, price), Food(name, price) {}
    Offer(string name) : Food(name), Drink(name) {}
    Offer(const Offer& other)
        : Product(other), Drink(other), Food(other) {
    }

    Offer& operator=(const Offer& other) {
        if (this != &other) {
            Drink::operator=(other);
            Food::operator=(other);
        }
        return *this;
    }

    friend ostream& operator<<(ostream& out, const Offer& o) {
        out << (Product&)o;
        cout << "--- Detalii bautura ---\n";
        cout << "Cantitate: ";
        out << o.amount << "\n";
        cout << "Cofeina: ";
        out << (o.caffeine ? "Da\n" : "Nu\n");
        cout << "--- Detalii mancare ---\n";
        cout << "KiloCalorii: ";
        out << o.kcal << "\n";
        cout << "Vegan: ";
        out << (o.vegan ? "Da\n" : "Nu\n");
        return out;
    }

    friend istream& operator>>(istream& in, Offer& o) {
        in >> (Product&)o;
        cout << "--- Introdu detalii bautura ---\n";
        cout << "Cantitate: ";
        in >> o.amount;
        in.get();
        cout << "Contine cofeina? (1/0): ";
        in >> o.caffeine;
        in.get();
        cout << "--- Introdu detalii mancare ---\n";
        cout << "Calorii: ";
        in >> o.kcal;
        in.get();
        cout << "Este vegana? (1/0): ";
        in >> o.vegan;
        in.get();
        return in;
    }

    void showDetails() const;
    string getProductType() const;

};

void Offer::showDetails() const {
    cout << "[OFERTA]";
    cout << ": #" << productId << '\n';
    cout << "Nume: " << name << "\n";
    cout << "Pretul: " << price << "\n";
    cout << "Descriere: " << description << "\n";
    cout << "--- Detalii bautura ---\n";
    cout << "Cantitate: " << amount << "\n";
    cout << "Cofeina: " << (caffeine ? "Da\n" : "Nu\n");
    cout << "--- Detalii mancare ---\n";
    cout << "KiloCalorii: " << kcal << "\n";
    cout << "Vegan: " << (vegan ? "Da\n" : "Nu\n");
}
string Offer::getProductType() const {
    return "Oferta";
}
vector<Product*>listOfProducts;

bool sort_criteria(Product* A, Product* B) {
    return A->getProductType() < B->getProductType();
}

void showAllProducts() {
    cout << " ------------------------------------\n";
    cout << "|              MENIU                 |\n";
    cout << " ------------------------------------\n" << flush;
    sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);
    for (auto p : listOfProducts) {
        cout << "-------------------------------------\n";
        p->showDetails();
        cout << "-------------------------------------\n"<<flush;
    }
}

class Order {
private:
    const long long orderId;
    User* customer;
    vector<Product*>productsOrdered;
    vector<int> quantities;
    float totalValue;
    bool usedLoyaltyPoints;

    static long long generatorIdOrd;
    static int szListOfOrders;
    static Order* listOfOrders;

public:
    Order() : orderId(generatorIdOrd++) {
        customer = nullptr;
        totalValue = 0;
        usedLoyaltyPoints = false;
    }

    Order(User* customer, vector<Product*>& products, vector<int>& qty, bool usedLoyaltyPoints) : orderId(generatorIdOrd++) {
        this->customer = customer;
        productsOrdered = products;
        quantities = qty;
        totalValue = 0;
        this->usedLoyaltyPoints = usedLoyaltyPoints;
        for (int i = 0; i < products.size(); ++i) {
            totalValue += products[i]->getPrice() * quantities[i];
        }
    }

    Order(User* customer) : orderId(generatorIdOrd++) {
        totalValue = 0;
        this->customer = customer;
        usedLoyaltyPoints = false;
    }
    Order(const Order& o) : orderId(generatorIdOrd++) {
        this->customer = o.customer;
        this->quantities = o.quantities;
        this->totalValue = o.totalValue;
        this->usedLoyaltyPoints = o.usedLoyaltyPoints;
        this->productsOrdered = o.productsOrdered;
    }
    ~Order() {
        productsOrdered.clear();
        quantities.clear();
    }

    Order& operator=(const Order& o) {
        if (this != &o) {
            customer = o.customer;
            productsOrdered = o.productsOrdered;
            quantities = o.quantities;
            totalValue = o.totalValue;
            usedLoyaltyPoints = o.usedLoyaltyPoints;
        }
        return *this;
    }

    friend istream& operator>>(istream& in, Order& order) {
        int numProducts;
        cout << "Cate produse diferite vrei sa comanzi? ";
        in >> numProducts;
        order.productsOrdered.clear();
        order.quantities.clear();
        order.totalValue = 0;

        for (int i = 0; i < numProducts; ++i) {
            int id;
            cout << "ID produs: ";
            in >> id;
            in.get();

            Product* found = nullptr;
            for (Product* p : listOfProducts) {
                if (p->getProductId() == id) {
                    found = p;
                    break;
                }
            }

            if (!found) {
                cout << "Produs inexistent.\n";
                --i;
                continue;
            }

            int qty;
            cout << "Cantitate: ";
            in >> qty;
            in.get();

            order.productsOrdered.push_back(found);
            order.quantities.push_back(qty);
            order.totalValue += found->getPrice() * qty;
        }
        char use;
        cout << "Folosesti puncte de fidelitate? (y/n): ";
        in >> use;
        in.get();
        order.usedLoyaltyPoints = (use == 'y' || use == 'Y');

        return in;
    }

    friend ostream& operator<<(ostream& out, const Order& order) {
        out << "Comanda dumneavoastra #" << order.orderId << "\n";
        for (int i = 0; i < order.productsOrdered.size(); i++) {
            out << "Produs: " << order.productsOrdered[i]->getName() << "\n";
            out << "Nr de produse: " << order.quantities[i] << "\n";
            cout << "Pret per bucata: " << order.productsOrdered[i]->getPrice() << "\n";
        }
        out << "Total: " << order.totalValue << "\n";
        return out;
    }

    Order& operator++() {
        for (int i = 0; i < productsOrdered.size(); i++) {
            quantities[i]++;
            totalValue += productsOrdered[i]->getPrice();
        }
        return *this;
    }

    Order& operator+(float extra) {
        this->totalValue += extra;
        return *this;
    }
    friend Order& operator+(float extra, Order& o) {
        return o + extra;
    }
    Order& operator-(float extra) {
        this->totalValue -= extra;
        return *this;
    }
    friend Order& operator-(int x, Order& o) {
        o.totalValue = x - o.totalValue;
        return o;
    }

    bool operator==(const Order& o) const {
        return customer == o.customer;
    }

    bool operator<(const Order& o) const {
        return this->totalValue < o.totalValue;
    }
    float getTotalValue() const {
        return totalValue;
    }
    void completeOrder() {
        int aux = 0;
        if (usedLoyaltyPoints) {
            if (totalValue >= customer->getLoyaltyPoints()) {
                aux = customer->getLoyaltyPoints();
                totalValue -= aux;
                customer->setLoyaltyPoints(0);
            }
            else {
                aux = int(totalValue);
                *customer = *customer - int(totalValue);
                totalValue = 0;
            }
        }
        cout << "Comanda a fost finalizata!\n";
        cout << "Au fost folosite " << aux << " puncte de fidelitate.\n";
        cout << "Total: " << totalValue << "\n";
        *customer = static_cast<int>(totalValue / 20.0f) + *customer;
    }

    static void addOrderToList(const Order& order);
    static Order* getOrderById(long long orderId);
    static void deleteOrderList() {
        if (listOfOrders != nullptr) {
            delete[] listOfOrders;
            listOfOrders = nullptr;
            szListOfOrders = 0;
        }
    }

};

long long Order::generatorIdOrd = 100;
int Order::szListOfOrders = 0;
Order* Order::listOfOrders = new Order[1500];

void Order::addOrderToList(const Order& order) {
    listOfOrders[szListOfOrders++] = order;
}

Order* Order::getOrderById(long long orderId) {
    for (int i = 0; i < szListOfOrders; i++) {
        if (listOfOrders[i].orderId == orderId) {
            return &listOfOrders[i];
        }
    }
    return nullptr;
}


class Reservation {
private:
    const int reservationId;
    int customerId;
    int numberOfSeats;
    char* reservationTime;
    
    static int generatorIdRes;
    static int szListOfReservations;
    static Reservation* listOfReservations;

public:
    Reservation() : reservationId(generatorIdRes++) {
        customerId = -1;
        numberOfSeats = 0;
        numberOfSeats = 0;
        reservationTime = new char[MAX_LENGTH];
    }
    Reservation(int customerId, int numberOfSeats, char* reservationTime) : reservationId(generatorIdRes++) {
        this->customerId = customerId;
        this->numberOfSeats = numberOfSeats;
        this->reservationTime = new char[strlen(reservationTime) + 1];
        strcpy(this->reservationTime, reservationTime);
    }

    Reservation(int customerId) : reservationId(generatorIdRes++) {
        this->customerId = customerId;
        this->numberOfSeats = 0;
        this->reservationTime = new char[MAX_LENGTH];
        reservationTime[0] = '\0';
    }

    Reservation(int customerId, char* reservationTime) : reservationId(generatorIdRes++) {
        this->customerId = customerId;
        this->numberOfSeats = 0;
        this->reservationTime = new char[strlen(reservationTime) + 1];
        strcpy(this->reservationTime, reservationTime);
    }

    Reservation(const Reservation& r) : reservationId(generatorIdRes++) {
        this->customerId = r.customerId;
        this->numberOfSeats = r.numberOfSeats;
        this->reservationTime = new char[strlen(r.reservationTime) + 1];
        strcpy(this->reservationTime, r.reservationTime);
    }

    ~Reservation() {
        if (reservationTime != nullptr)
            delete[] reservationTime;
    }
    Reservation& operator=(const Reservation& reserv) {
        if (this == &reserv) return *this;

        this->customerId = reserv.customerId;
        this->numberOfSeats = reserv.numberOfSeats;
        if (this->reservationTime != nullptr) {
            delete[] this->reservationTime;
        }
        this->reservationTime = new char[strlen(reserv.reservationTime) + 1];
        strcpy(this->reservationTime, reserv.reservationTime);

        return *this;
    }
    char operator[](int index) const {
        if (index >= 0 && index < strlen(reservationTime)) {
            return reservationTime[index];
        }
        return 'X';
    }
    Reservation& operator++() {
        ++numberOfSeats;
        return *this;
    }

    Reservation& operator--() {
        --numberOfSeats;
        return *this;
    }

    Reservation& operator+(int val) {
        this->numberOfSeats += val;
        return *this;
    }

    Reservation& operator-(int val) {
        this->numberOfSeats -= val;
        return *this;
    }
    friend Reservation& operator+(int val, Reservation& reserv) {
        return reserv + val;
    }

    friend Reservation& operator-(int x, Reservation& r) {
        r.numberOfSeats = x - r.numberOfSeats;
        return r;
    }
    bool operator==(const Reservation& r) const {
        return this->customerId == r.customerId;
    }

    bool operator<(const Reservation& r) const {
        return strcmp(this->reservationTime, r.reservationTime) < 0;
    }
    friend istream& operator>>(istream& in, Reservation& res) {
        char* reservationTime = new char[MAX_LENGTH];
        cout << "Introdu data si ora rezervarii(YY/MM/DD HH:MM): ";
        in.get(reservationTime, MAX_LENGTH);
        in.get();
        int nseats;
        cout << "Pentru cate persoane? ";
        in >> nseats;
        in.get();
        res = Reservation(res.customerId, nseats, reservationTime);
        delete[] reservationTime;
        return in;
    }
    friend ostream& operator<<(ostream& out, const Reservation& res) {
        out << "Id-ul rezervarii: " << res.reservationId << '\n';
        User* user = User::getUserById(res.customerId);
        out << "Nume: " << (*user).getName() << '\n';
        out << "Numar de persoane: " << res.numberOfSeats << '\n';
        out << "Data si ora: " << res.reservationTime;
        out << "\n";

        return out;
    }
    int getReservationId();
    User* getCustomer();
    int getNumberOfSeats();
    char* getReservationTime();

    void setReservationTime(char* reservationTime);
    void setCustomer(User* customer);
    void setNumberOfSeats(int numberOfSeats);

    static void addReservationToList(const Reservation& reservation);
    static void deleteReservationById(int reservationId);
    static void showReservationsForUser(User& user);
    static void showAllReservation();
    static int getSize();
    static Reservation* getReservations();
    static void deleteReservationList() {
        if (listOfReservations != nullptr) {
            delete[] listOfReservations;
            listOfReservations = nullptr;
            szListOfReservations = 0;
        }
    }


};
int Reservation::generatorIdRes = 1000;
int Reservation::szListOfReservations = 0;
Reservation* Reservation::listOfReservations = new Reservation[MAX_LENGTH];

int Reservation::getReservationId() {
    return reservationId;
}
User* Reservation::getCustomer() {
    return User::getUserById(this->customerId);
}
int Reservation::getNumberOfSeats() {
    return numberOfSeats;
}
char* Reservation::getReservationTime() {
    return reservationTime;
}
void Reservation::setReservationTime(char* reservationTime) {
    if (this->reservationTime != nullptr)
        delete[] this->reservationTime;
    this->reservationTime = new char[MAX_LENGTH];
    strcpy(this->reservationTime, reservationTime);
}
void Reservation::setCustomer(User* customer) {
    this->customerId = customer->getUserId();
}
void Reservation::setNumberOfSeats(int numberOfSeats) {
    this->numberOfSeats = numberOfSeats;
}

void Reservation::addReservationToList(const Reservation& reservation) {
    if (szListOfReservations < MAX_LENGTH) {
        listOfReservations[szListOfReservations++] = reservation;
    }
    else {
        cout << "Eroare: S-a atins numarul maxim de rezervari!\n";
    }
}


void Reservation::deleteReservationById(int reservationId) {
    bool startDelete = false;
    if (reservationId == listOfReservations[szListOfReservations - 1].reservationId) {
        szListOfReservations--;
        return;
    }

    for (int i = 1; i < szListOfReservations; i++) {
        if (reservationId == listOfReservations[i - 1].reservationId) {
            startDelete = true;
        }
        if (startDelete) {
            listOfReservations[i - 1] = listOfReservations[i];
        }
    }

    if (startDelete) {
        szListOfReservations--;
    }
    else {
        cout << "Rezervarea cu ID-ul " << reservationId << " nu a fost gasita.\n";
    }
}

void Reservation::showReservationsForUser(User& user) {
    bool found = false;
    for (int i = 0; i < szListOfReservations; i++) {
        if (listOfReservations[i].customerId == user.getUserId()) {
            cout << listOfReservations[i];
            found = true;
        }
    }
    if (!found) {
        cout << "Nu ai nicio rezervare.\n";
    }
}
void Reservation::showAllReservation() {
    sort(listOfReservations, listOfReservations + szListOfReservations);
    for (int i = 0; i < szListOfReservations; i++)
        cout << listOfReservations[i];
}
int Reservation::getSize() {
    return szListOfReservations;
}
Reservation* Reservation::getReservations() {
    return listOfReservations;
}
void readProductsFromFile(const string& filename) {
    ifstream fin(filename);

    string type;
    while (getline(fin, type)) {
        if (type == "Bautura") {
            string name, description;
            float price, amount;
            bool caffeine;

            getline(fin, name);
            fin >> price;
            fin.get();
            getline(fin, description);
            fin >> amount >> caffeine;
            fin.get();

            Product* d = new Drink(name, price, description, amount, caffeine);
            listOfProducts.push_back(d);
        }
        else if (type == "Mancare") {
            string name, description;
            float price;
            int kcal;
            bool vegan;

            getline(fin, name);
            fin >> price;
            fin.get();
            getline(fin, description);
            fin >> kcal >> vegan;
            fin.get();

            Product* f = new Food(name, price, description, kcal, vegan);
            listOfProducts.push_back(f);
        }
        else if (type == "Oferta") {
            string name, description;
            float price, amount;
            bool caffeine;
            int kcal;
            bool vegan;

            getline(fin, name);
            fin >> price;
            fin.get();
            getline(fin, description);
            fin >> amount >> caffeine >> kcal >> vegan;
            fin.get();

            Product* o = new Offer(name, price, description, amount, caffeine, kcal, vegan);
            listOfProducts.push_back(o);
        }
    }

    fin.close();
}


int getMaxZi(int luna, int an) {
    if (luna == 1) {
        // An bisect
        if ((an % 4 == 0 && an % 100 != 0) || (an % 400 == 0))
            return 29;
        else
            return 28;
    }
    if (luna == 3 || luna == 5 || luna == 8 || luna == 10)
        return 30;
    return 31;
}


enum class AppState {
    MainMenu,
    Login,
    Register,
    AdminMenu,
    CustomerMenu,
    AddProduct,
    ViewMenu,
    DeleteProduct,
    UpdateProduct,
    ShowReservations,
    ShowCustomers,
    PlaceOrder,
    VerifyLPoints,
    Reservation, 
    ViewMyReservs, 
    CancelReservation,
    Exit
};


template <class T>
class Validator {
public:
    Validator() = default;
    virtual ~Validator() = default;

    static void throwIfNotPositive(T value, const string& label = "Number") {
        if (value < 0)
            throw runtime_error(label + " trebuie sa fie >=0!");
    }

    static void throwIfNotStrictPositive(T value, const string& label = "Number") {
        if (value <= 0)
            throw runtime_error(label + " trebuie sa fie >0!");
    }

    static void throwIfInRange(T value, T low, T high, const string& label = "Number") {
        if (value < low || value > high)
            throw runtime_error(label + " trebuie sa fie intre " + to_string(low) + " si intre " + to_string(high) + "!");
    }

    static void throwIfGreaterThan(T value, T max, const string& label = "Number") {
        if (value > max)
            throw runtime_error(label + " treebuie sa fie mai mic/a decat " + to_string(max) + "!");
    }

    static void throwIfNull(T ptr, const string& label = "Pointer") {
        if (ptr == nullptr)
            throw runtime_error(label + " nu trebuie sa fie 0!");
    }
};

extern bool isValidDate(const string&);  

template <>
class Validator<string> {
public:
    static void throwIfEmptyOrNull(const string& value, const string& label = "String") {
        if (value.empty())
            throw runtime_error(label + " must not be empty!");
    }

    static void throwIfTimeIntervalIncorrect(const string& time, const string& label = "Interval") {
        // HH:MM logic (dacă vrei să o păstrezi pentru ora)
        if (time.length() != 5 || time[2] != ':' ||
            !isdigit(time[0]) || !isdigit(time[1]) ||
            !isdigit(time[3]) || !isdigit(time[4]))
            throw runtime_error(label + " must be in format HH:MM!");

        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
            throw runtime_error(label + " must be a valid time (00:00 - 23:59)!");
    }

    static void throwIfInvalidDate(const string& date, const string& label = "Data") {
        if (!isValidDate(date))
            throw runtime_error(label + " este invalida! Format corect: dd/mm/yyyy");
    }
    static void throwIfDateInPast(const string& dateStr, const string& label = "Data") {
        if (dateStr.length() != 10 || dateStr[2] != '/' || dateStr[5] != '/') {
            throw runtime_error(label + " are un format invalid! (dd/mm/yyyy)");
        }

        int zi = stoi(dateStr.substr(0, 2));
        int luna = stoi(dateStr.substr(3, 2));
        int an = stoi(dateStr.substr(6, 4));

        // data curenta
        time_t t = time(nullptr);
        tm* now = localtime(&t);

        int ziCurenta = now->tm_mday;
        int lunaCurenta = now->tm_mon + 1;
        int anCurent = now->tm_year + 1900;

        if (an < anCurent ||
            (an == anCurent && luna < lunaCurenta) ||
            (an == anCurent && luna == lunaCurenta && zi < ziCurenta)) {
            throw runtime_error(label + " este in trecut!");
        }
    }
};
void SetCafeTheme()
{
    ImGuiStyle& s = ImGui::GetStyle();

    s.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.17f, 0.14f, 1.00f); 
    s.Colors[ImGuiCol_PopupBg] = ImVec4(0.23f, 0.19f, 0.16f, 1.00f);
    s.Colors[ImGuiCol_TitleBg] = ImVec4(0.28f, 0.22f, 0.18f, 1.00f);
    s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.35f, 0.27f, 0.20f, 1.00f);

    s.Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.38f, 0.27f, 1.00f);
    s.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.43f, 0.31f, 1.00f);
    s.Colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.46f, 0.33f, 1.00f);

    s.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.59f, 0.46f, 0.29f, 1.00f);
    s.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.51f, 0.33f, 1.00f);
    s.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.55f, 0.36f, 1.00f);

    s.Colors[ImGuiCol_Header] = ImVec4(0.67f, 0.53f, 0.35f, 0.55f);
    s.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.72f, 0.58f, 0.39f, 0.80f);
    s.Colors[ImGuiCol_HeaderActive] = ImVec4(0.72f, 0.58f, 0.39f, 1.00f);
    s.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.80f, 0.60f, 1.00f);

    s.Colors[ImGuiCol_Text] = ImVec4(0.93f, 0.89f, 0.84f, 1.00f); 
    s.Colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.47f, 0.40f, 1.00f);

    s.FrameRounding = 6.0f;
    s.WindowRounding = 6.0f;
    s.ScrollbarSize = 16.0f;
}


int main() {
    frequency[0] = 10000;
    bool loggedin = 0;
    User admin;
    User user;
    int option;
    char s[256];
    strcpy(s, "@admin");
    admin.setName(s);
    strcpy(s, "admin_password");
    admin.setPassword(s);
    admin.setAdmin(1);
    User::addUserToList(admin);
    User user1;
    strcpy(s, "Diana");
    user1.setName(s);
    strcpy(s, "Delia");
    user1.setPassword(s);
    user1.setAdmin(0);
    user1.setLoyaltyPoints(100);
    User::addUserToList(user1);
    User user2;
    strcpy(s, "Maria");
    user2.setName(s);
    strcpy(s, "Ioana");
    user2.setPassword(s);
    user2.setAdmin(0);
    user2.setLoyaltyPoints(10);
    User::addUserToList(user2);
    readProductsFromFile("meniu.txt");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Cafenea GUI");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    ImGuiStyle& sty = ImGui::GetStyle();
    sty.FrameRounding = 8.f;     
    sty.WindowBorderSize = 0.f;     
    sty.ItemSpacing = { 8.f, 6.f };
    sty.GrabRounding = 12.f;    
    sty.Colors[ImGuiCol_FrameBg].w = 0.75f; 

    SetCafeTheme();

    ImGuiIO& io = ImGui::GetIO();

    ImFont* uiFont = io.Fonts->AddFontFromFileTTF(
        "Pacifico-Regular.ttf", 28.0f,
        nullptr, io.Fonts->GetGlyphRangesDefault());


    io.FontDefault = uiFont;         
    ImGui::SFML::UpdateFontTexture(); 


    AppState currentState = AppState::MainMenu;
    std::string username, password;
    bool isAdmin = false;
    bool loginFailed = false;
    bool pct = false;
    sf::Clock deltaClock;


    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("1.jpg");
    sf::Sprite backgroundSprite(backgroundTexture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();
        window.draw(backgroundSprite);
        if (currentState == AppState::MainMenu) {
            ImGui::Begin("Meniu Principal");

            if (ImGui::Button("Intra in cont")) {
                currentState = AppState::Login;
            }
            if (ImGui::Button("Creeaza cont")) {
                currentState = AppState::Register;
            }

            ImGui::End();
        }
        else if (currentState == AppState::Login) {
            ImGui::Begin("Login");

            static char userStr[128] = "";
            static char passStr[128] = "";
            static bool firstFrame = true;

            if (firstFrame) {
                loginFailed = false;
                firstFrame = false;
            }
            ImGui::InputText("Username", userStr, 128);
            ImGui::InputText("Parola", passStr, 128, ImGuiInputTextFlags_Password);

            if (ImGui::Button("Login")) {
                user.setName(userStr);
                user.setPassword(passStr);
                memset(userStr, 0, sizeof(userStr));
                memset(passStr, 0, sizeof(passStr));
                if (!User::verifyUser(user)) {
                    cout << "Utilizator negasit!\n";
                    loginFailed = true;
                }
                else {
                    if (user[0] == '@')
                        currentState = AppState::AdminMenu;
                    else
                        currentState = AppState::CustomerMenu;
                    firstFrame = true;
                }
            }
            if (loginFailed) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Utilizator negasit!");
            }
            if (ImGui::Button("Inapoi")) {
                currentState = AppState::MainMenu;
                firstFrame = true;
            }

            ImGui::End();
        }
        else if (currentState == AppState::Register) {
            ImGui::Begin("Inregistrare");

            static char User[128] = "";
            static char Pass[128] = "";
            static bool firstFrame = true;

            if (firstFrame) {
                pct = false;
                firstFrame = false;
            }
            ImGui::InputText("Username", User, 128);
            ImGui::InputText("Parola", Pass, 128, ImGuiInputTextFlags_Password);
            if (ImGui::Button("Inregistreaza")) {
                user.setName(User);
                user.setPassword(Pass);
                user.setLoyaltyPoints(5);
                User::addUserToList(user);
                pct = true;
                memset(User, 0, sizeof(User));
                memset(Pass, 0, sizeof(Pass));
            }
            if(pct)
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Ai primit 5 puncte din partea noastra pentru inregistrare!  ");

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::MainMenu;
                firstFrame = true;
            }

            ImGui::End();
        }
        else if (currentState == AppState::AdminMenu) {
            ImGui::Begin("Meniu Admin");
            ImGui::Text("Bine ai venit, admin!");
            if (ImGui::Button("Adauga produs")) {
                currentState = AppState::AddProduct;
            }
            if (ImGui::Button("Afiseaza meniul complet")) {
                currentState = AppState::ViewMenu;
            }
            if (ImGui::Button("Actualizeaza un produs existent")) {
                currentState = AppState::UpdateProduct;
            }
            if (ImGui::Button("Sterge un produs")) {
                currentState = AppState::DeleteProduct;
            }
            if (ImGui::Button("Afiseaza toate rezervarile")) {
                currentState = AppState::ShowReservations;
            }
            if (ImGui::Button("Afiseaza toti clientii")) {
                currentState = AppState::ShowCustomers;
            }

            if (ImGui::Button("Logout")) {
                currentState = AppState::MainMenu;
            }
            ImGui::End();
        }
        else if (currentState == AppState::AddProduct) {
            ImGui::Begin("Adauga Produs");

            static int productType = 0; // 0 = Bautura, 1 = Mancare, 2 = Oferta
            static char name[128] = "";
            static float price = 0.0f;
            static char description[200]="";
            static float quantity = 0.0f;
            static bool hasCaffeine = false;

            static int calories = 0;
            static bool isVegan = false;

            static bool inputError = false; // pentru eroare la input

            ImGui::Text("Selecteaza tipul produsului:");
            ImGui::RadioButton("Bautura", &productType, 0); ImGui::SameLine();
            ImGui::RadioButton("Mancare", &productType, 1); ImGui::SameLine();
            ImGui::RadioButton("Oferta", &productType, 2);

            ImGui::InputText("Nume produs", name, 128);
            ImGui::InputFloat("Pret", &price);
            ImGui::InputText("Descriere", description, 200);
            if (productType == 0 || productType == 2) { 
                ImGui::InputFloat("Cantitate (ml)", &quantity);
                ImGui::Checkbox("Contine cofeina?", &hasCaffeine);
            }
            if (productType == 1 || productType == 2) { 
                ImGui::InputInt("Calorii", &calories);
                ImGui::Checkbox("Este vegana?", &isVegan);
            }

            ImGui::Separator();
            ImGui::Text("Preview produs:");

            if (strlen(name) > 0 && price > 0.0f) {
                const char* typeLabel = "";

                if (productType == 0) {
                    typeLabel = "Bautura";
                }
                else if (productType == 1) {
                    typeLabel = "Mancare";
                }
                else if (productType == 2) {
                    typeLabel = "Oferta";
                }

                ImGui::Text("%s: %s - %.2f lei", typeLabel, name, price);
            }
            else {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Completeaza toate campurile pentru a vedea produsul!");
            }

            ImGui::Separator();

            if (ImGui::Button("Salveaza")) {
                inputError = false;

                // Validare
                if (strlen(name) == 0 || price <= 0.0f) {
                    inputError = true;
                }
                if ((productType == 0 || productType == 2) && quantity <= 0.0f) {
                    inputError = true;
                }
                if ((productType == 1 || productType == 2) && calories <= 0) {
                    inputError = true;
                }

                if (!inputError) {
                    Product* p = nullptr;
                    if (productType == 0) { // Bautura
                        Drink* d = new Drink();
                        d->setName(name);
                        d->setPrice(price);
                        d->setDescription(description);
                        d->setAmount(quantity);
                        d->setCaffeine(hasCaffeine);
                        p = d;
                    }
                    else if (productType == 1) { // Mancare
                        Food* f = new Food();
                        f->setName(name);
                        f->setPrice(price);
                        f->setDescription(description);
                        f->setKcal(calories);
                        f->setVegan(isVegan);
                        p = f;
                    }
                    else if (productType == 2) { // Oferta
                        Offer* o = new Offer();
                        o->setName(name);
                        o->setPrice(price);
                        o->setDescription(description);
                        o->setAmount(quantity);
                        o->setCaffeine(hasCaffeine);
                        o->setKcal(calories);
                        o->setVegan(isVegan);
                        p = o;
                    }

                    if (p) {
                        listOfProducts.push_back(p);
                        ImGui::OpenPopup("Succes");

                        // Resetam toate campurile
                        name[0] = '\0';
                        price = 0.0f;
                        description[0] = '\0';
                        quantity = 0.0f;
                        hasCaffeine = false;
                        calories = 0;
                        isVegan = false;
                        productType = 0;
                    }
                }
            }

            if (inputError) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Eroare: Completeaza toate campurile corect!");
            }

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
                // Resetam campurile cand plecam
                name[0] = '\0';
                price = 0.0f;
                description[0] = '\0';
                quantity = 0.0f;
                hasCaffeine = false;
                calories = 0;
                isVegan = false;
                productType = 0;
                inputError = false;
            }

            if (ImGui::BeginPopupModal("Succes", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Produs adaugat cu succes!");
                if (ImGui::Button("OK")) {
                    ImGui::CloseCurrentPopup();
                    currentState = AppState::AdminMenu;
                }
                ImGui::EndPopup();
            }

            ImGui::End();
        }
        else if (currentState == AppState::ViewMenu) {
            ImGui::Begin("Meniul Complet");

            ImGui::Text("Lista produselor disponibile:");
            ImGui::Separator();

            bool hasDrinks = false, hasFoods = false, hasOffers = false;
            sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);

            // Verificam daca avem produse pentru fiecare categorie
            for (auto p : listOfProducts) {
                if (dynamic_cast<Offer*>(p)) hasOffers = true;
                if (dynamic_cast<Drink*>(p)) hasDrinks = true;
                if (dynamic_cast<Food*>(p)) hasFoods = true;
            }

            if (listOfProducts.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Momentan nu exista produse in meniu!");
            }
            else {
                if (hasDrinks) {
                    ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "Bauturi:");
                    ImGui::Separator();
                    for (auto p : listOfProducts) {
                        if (Drink* d = dynamic_cast<Drink*>(p)) {
                            if (dynamic_cast<Offer*>(p)) continue;
                            ImGui::BulletText("%s - %.2f lei - %s \n- Cantitate: %.0f ml \n- Cofeina: %s",
                                d->getName().c_str(), d->getPrice(), d->getDescription().c_str(), d->getAmount(), d->getCaffeine() ? "Da" : "Nu");
                        }
                    }
                    ImGui::Spacing();
                }

                if (hasFoods) {
                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Mancare:");
                    ImGui::Separator();
                    for (auto p : listOfProducts) {
                        if (Food* f = dynamic_cast<Food*>(p)) {
                            if (dynamic_cast<Offer*>(p)) continue;
                            ImGui::BulletText("%s - %.2f lei - %s \n- Calorii: %d \n- Vegan: %s",
                                f->getName().c_str(), f->getPrice(), f->getDescription().c_str(), f->getKcal(), f->getVegan() ? "Da" : "Nu");
                        }
                    }
                    ImGui::Spacing();
                }

                if (hasOffers) {
                    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Oferte:");
                    ImGui::Separator();
                    for (auto p : listOfProducts) {
                        if (Offer* o = dynamic_cast<Offer*>(p)) {
                            ImGui::BulletText("%s - %.2f lei - %s\n- Cantitate: %.0f ml \n- Cofeina: %s \n- Calorii: %d \n- Vegan: %s",
                                o->getName().c_str(), o->getPrice(), o->getDescription().c_str(), o->getAmount(), o->getCaffeine() ? "Da" : "Nu", o->getKcal(), o->getVegan() ? "Da" : "Nu");
                        }
                    }
                    ImGui::Spacing();
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::UpdateProduct) {
            ImGui::Begin("Meniul Complet");

            static bool openEditPopup = false;
            static Product* productToEdit = nullptr;

            static char editName[128] = "";
            static float editPrice = 0.0f;
            static float editQuantity = 0.0f;
            static bool editCaffeine = false;
            static int editCalories = 0;
            static bool editVegan = false;

            ImGui::Text("Lista produselor disponibile:");
            ImGui::Separator();

            bool hasDrinks = false, hasFoods = false, hasOffers = false;
            sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);

            for (auto p : listOfProducts) {
                if (dynamic_cast<Offer*>(p)) hasOffers = true;
                if (dynamic_cast<Drink*>(p)) hasDrinks = true;
                if (dynamic_cast<Food*>(p)) hasFoods = true;
            }

            if (listOfProducts.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Momentan nu exista produse in meniu!");
            }
            else {
                for (size_t i = 0; i < listOfProducts.size(); ++i) {
                    Product* p = listOfProducts[i];

                    if (p->getProductType()=="Oferta") {
                        Offer* o = dynamic_cast<Offer*>(p);
                        if (i == 0 || listOfProducts[i-1]->getProductType()!="Oferta") {
                            ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Oferte:");
                            ImGui::Separator();
                        }
                        ImGui::BulletText("%s - %.2f lei - %s\n- Cantitate: %.0f ml \n- Cofeina: %s \n- Calorii: %d \n- Vegan: %s",
                            o->getName().c_str(), o->getPrice(), o->getDescription().c_str(), o->getAmount(), o->getCaffeine() ? "Da" : "Nu", o->getKcal(), o->getVegan() ? "Da" : "Nu");
                    }
                    if (p->getProductType() == "Bautura") {
                        Drink* d = dynamic_cast<Drink*>(p);
                        if (i == 0 || listOfProducts[i - 1]->getProductType() != "Bautura") {
                            ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "Bauturi:");
                            ImGui::Separator();
                        }
                        ImGui::BulletText("%s - %.2f lei - %s \n- Cantitate: %.0f ml \n- Cofeina: %s",
                            d->getName().c_str(), d->getPrice(), d->getDescription().c_str(), d->getAmount(), d->getCaffeine() ? "Da" : "Nu");
                    }
                    if (p->getProductType() == "Mancare") {
                        Food* f = dynamic_cast<Food*>(p);
                        if (i == 0 || listOfProducts[i - 1]->getProductType() != "Mancare") {
                            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Mancare:");
                            ImGui::Separator();
                        }
                        ImGui::BulletText("%s - %.2f lei - %s \n- Calorii: %d \n- Vegan: %s",
                            f->getName().c_str(), f->getPrice(), f->getDescription().c_str(), f->getKcal(), f->getVegan() ? "Da" : "Nu");
                    }
                    

                    ImGui::SameLine();
                    std::string updateId = "Actualizeaza##" + std::to_string(i);
                    if (ImGui::Button(updateId.c_str())) {
                        openEditPopup = true;
                        productToEdit = listOfProducts[i];

                        if (Offer* o = dynamic_cast<Offer*>(productToEdit)) {
                            strcpy(editName, o->getName().c_str());
                            editPrice = o->getPrice();
                            editQuantity = o->getAmount();
                            editCaffeine = o->getCaffeine();
                            editCalories = o->getKcal();
                            editVegan = o->getVegan();
                        }
                        else if (Drink* d = dynamic_cast<Drink*>(productToEdit) ) {
                            strcpy(editName, d->getName().c_str());
                            editPrice = d->getPrice();
                            editQuantity = d->getAmount();
                            editCaffeine = d->getCaffeine();
                        }
                        else if (Food* f = dynamic_cast<Food*>(productToEdit)) {
                            strcpy(editName, f->getName().c_str());
                            editPrice = f->getPrice();
                            editCalories = f->getKcal();
                            editVegan = f->getVegan();
                        }
                        
                    }
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            // Popup de actualizare produs
            if (openEditPopup && productToEdit != nullptr) {
                ImGui::OpenPopup("Actualizeaza Produs");
            }

            if (ImGui::BeginPopupModal("Actualizeaza Produs", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::InputText("Nume produs", editName, 128);
                ImGui::InputFloat("Pret", &editPrice);

                if (dynamic_cast<Drink*>(productToEdit)) {
                    ImGui::InputFloat("Cantitate (ml)", &editQuantity);
                    ImGui::Checkbox("Contine cofeina?", &editCaffeine);
                }
                if (dynamic_cast<Food*>(productToEdit)) {
                    ImGui::InputInt("Calorii", &editCalories);
                    ImGui::Checkbox("Este vegana?", &editVegan);
                }

                if (ImGui::Button("Salveaza modificari")) {
                    productToEdit->setName(editName);
                    productToEdit->setPrice(editPrice);

                    if (Offer* o = dynamic_cast<Offer*>(productToEdit)) {
                        o->setAmount(editQuantity);
                        o->setCaffeine(editCaffeine);
                        o->setKcal(editCalories);
                        o->setVegan(editVegan);
                    }
                    else if (Drink* d = dynamic_cast<Drink*>(productToEdit)) {
                        d->setAmount(editQuantity);
                        d->setCaffeine(editCaffeine);
                    }
                    else if (Food* f = dynamic_cast<Food*>(productToEdit)) {
                        f->setKcal(editCalories);
                        f->setVegan(editVegan);
                    }
                    

                    openEditPopup = false;
                    productToEdit = nullptr;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();

                if (ImGui::Button("Anuleaza")) {
                    openEditPopup = false;
                    productToEdit = nullptr;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::End();
        }

        else if (currentState == AppState::DeleteProduct) {
            ImGui::Begin("Sterge un produs");

            ImGui::Text("Lista produselor disponibile:");
            ImGui::Separator();

            bool hasDrinks = false, hasFoods = false, hasOffers = false;
            sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);

            // Verificam existenta produselor pe categorii
            for (auto p : listOfProducts) {
                if (dynamic_cast<Offer*>(p)) hasOffers = true;
                if (dynamic_cast<Drink*>(p)) hasDrinks = true;
                if (dynamic_cast<Food*>(p)) hasFoods = true;
            }

            if (listOfProducts.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Momentan nu exista produse in meniu!");
            }
            else {
                int index = 0;
                // Folosim un index pentru a ști ce ștergem
                if (hasDrinks) {
                    ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "Bauturi:");
                    ImGui::Separator();
                    for (size_t i = 0; i < listOfProducts.size(); ++i) {
                        if (Drink* d = dynamic_cast<Drink*>(listOfProducts[i])) {
                            if (dynamic_cast<Offer*>(listOfProducts[i])) continue;
                            ImGui::BulletText("%s - %.2f lei - %s \n- Cantitate: %.0f ml \n- Cofeina: %s",
                                d->getName().c_str(), d->getPrice(), d->getDescription().c_str(), d->getAmount(), d->getCaffeine() ? "Da" : "Nu");

                            ImGui::SameLine();
                            std::string buttonId = "Sterge##" + std::to_string(i);
                            if (ImGui::Button(buttonId.c_str())) {
                                delete listOfProducts[i];
                                listOfProducts.erase(listOfProducts.begin() + i);
                                break; // IMPORTANT: break ca să nu stricăm iteratorul
                            }
                        }
                    }
                    ImGui::Spacing();
                }

                if (hasFoods) {
                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Mancare :");
                    ImGui::Separator();
                    for (size_t i = 0; i < listOfProducts.size(); ++i) {
                        if (Food* f = dynamic_cast<Food*>(listOfProducts[i])) {
                            if (dynamic_cast<Offer*>(listOfProducts[i])) continue;
                            ImGui::BulletText("%s - %.2f lei - %s \n- Calorii: %d \n- Vegan: %s",
                                f->getName().c_str(), f->getPrice(), f->getDescription().c_str(), f->getKcal(), f->getVegan() ? "Da" : "Nu");

                            ImGui::SameLine();
                            std::string buttonId = "Sterge##" + std::to_string(i);
                            if (ImGui::Button(buttonId.c_str())) {
                                delete listOfProducts[i];
                                listOfProducts.erase(listOfProducts.begin() + i);
                                break;
                            }
                        }
                    }
                    ImGui::Spacing();
                }

                if (hasOffers) {
                    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Oferte:");
                    ImGui::Separator();
                    for (size_t i = 0; i < listOfProducts.size(); ++i) {
                        if (Offer* o = dynamic_cast<Offer*>(listOfProducts[i])) {
                            ImGui::BulletText("%s - %.2f lei - %s\n- Cantitate: %.0f ml \n- Cofeina: %s \n- Calorii: %d \n- Vegan: %s",
                                o->getName().c_str(), o->getPrice(), o->getDescription().c_str(), o->getAmount(), o->getCaffeine() ? "Da" : "Nu", o->getKcal(), o->getVegan() ? "Da" : "Nu");

                            ImGui::SameLine();
                            std::string buttonId = "Sterge##" + std::to_string(i);
                            if (ImGui::Button(buttonId.c_str())) {
                                delete listOfProducts[i];
                                listOfProducts.erase(listOfProducts.begin() + i);
                                break;
                            }
                        }
                    }
                    ImGui::Spacing();
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::ShowReservations) {
            ImGui::Begin("Toate Rezervarile");

            if (Reservation::getSize() == 0) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nu exista rezervari momentan.");
            }
            else {
                std::sort(Reservation::getReservations(), Reservation::getReservations() + Reservation::getSize());

                ImGui::Text("Lista rezervarilor:");
                ImGui::Separator();

                ImGui::Columns(3, "Rezervari"); // coloane: UserID, Numar persoane, Ora
                ImGui::Text("ID Client"); ImGui::NextColumn();
                ImGui::Text("Numar Persoane"); ImGui::NextColumn();
                ImGui::Text("Data si Ora"); ImGui::NextColumn();
                ImGui::Separator();

                for (int i = 0; i < Reservation::getSize(); ++i) {
                    Reservation& r = Reservation::getReservations()[i];

                    ImGui::Text("%d", r.getCustomer()->getUserId()); ImGui::NextColumn();
                    ImGui::Text("%d", r.getNumberOfSeats()); ImGui::NextColumn();
                    ImGui::Text("%s", r.getReservationTime()); ImGui::NextColumn();
                }

                ImGui::Columns(1);
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::ShowCustomers) {
            ImGui::Begin("Lista Clientilor");

            if (User::getSize() == 0) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nu exista clienti momentan.");
            }
            else {
                ImGui::Text("Clientii inregistrati:");
                ImGui::Separator();

                ImGui::Columns(3, "Clienti"); // 3 coloane: Nume, ID, Puncte
                ImGui::Text("Nume Client"); ImGui::NextColumn();
                ImGui::Text("ID"); ImGui::NextColumn();
                ImGui::Text("Puncte Fidelitate"); ImGui::NextColumn();
                ImGui::Separator();

                for (int i = 0; i < User::getSize(); ++i) {
                    User& u = User::getUsers()[i];

                    ImGui::Text("%s", u.getName()); ImGui::NextColumn();
                    ImGui::Text("%d", u.getUserId()); ImGui::NextColumn();
                    ImGui::Text("%d", u.getLoyaltyPoints()); ImGui::NextColumn();
                }

                ImGui::Columns(1);
            }

            ImGui::Separator();

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::AdminMenu;
            }

            ImGui::End();
        }


        else if (currentState == AppState::CustomerMenu) {
            ImGui::Begin("Meniu Client");
            ImGui::Text("Bine ai venit!");

            if (ImGui::Button("Plaseaza o comanda")) {
                currentState = AppState::PlaceOrder;
            }
            if (ImGui::Button("Verifica puncte de fidelitate")) {
                currentState = AppState::VerifyLPoints;
            }
            if (ImGui::Button("Rezerva un loc")) {
                currentState = AppState::Reservation;
            }
            if (ImGui::Button("Vezi rezervarile tale")) {
                currentState = AppState::ViewMyReservs;
            }
            if (ImGui::Button("Anuleaza o rezervare")) {
                currentState = AppState::CancelReservation;
            }
            if (ImGui::Button("Logout")) {
                currentState = AppState::MainMenu;
            }
            ImGui::End();
        }
        else if (currentState == AppState::PlaceOrder) {
            ImGui::Begin("Plasare Comanda");
            char* nameu = user.getName();
            User* temp;
            temp = User::getUserByName(nameu);
            static int selectedProductIndex = -1;
            static int currentQuantity = 1;
            static std::vector<Product*> selectedProducts;
            static std::vector<int> quantities;
            static bool useLoyalty = false;
            static bool showPopup = false;
            static float totalBefore = 0.0f;
            static float totalAfter = 0.0f;
            static int puncteFolosite = 0;

            ImGui::Text("Meniu complet - Selecteaza un produs:");
            ImGui::Separator();
            sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);
            int index_cat = 1;
            for (int i = 0; i < listOfProducts.size(); ++i) {
                Product* p = listOfProducts[i];

                if (index_cat == 3 && dynamic_cast<Offer*>(p)) {
                    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Oferte:");
                    ImGui::Separator();
                    index_cat++;
                }
                else if (index_cat == 1 && dynamic_cast<Drink*>(p)) {
                    ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "Bauturi:");
                    ImGui::Separator();
                    index_cat++;
                }
                else if (index_cat == 2 && dynamic_cast<Food*>(p)) {
                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Mancare:");
                    ImGui::Separator();
                    index_cat++;
                }

                std::string label = p->getName() + " - " + std::to_string(p->getPrice()) + " lei";

                bool isSelected = (selectedProductIndex == i);
                if (ImGui::RadioButton(label.c_str(), isSelected)) {
                    selectedProductIndex = isSelected ? -1 : i;  // toggle
                }

                ImGui::Indent();
                ImGui::Text("Descriere: %s", p->getDescription().c_str());
                
                if (Offer* o = dynamic_cast<Offer*>(p)) {
                    ImGui::Text("Tip: Oferta | Cantitate: %.0f ml | Cofeina: %s | Calorii: %d | Vegan: %s",
                        o->getAmount(), o->getCaffeine() ? "Da" : "Nu", o->getKcal(), o->getVegan() ? "Da" : "Nu");
                }
                else if (Drink* d = dynamic_cast<Drink*>(p)) {

                    ImGui::Text("Tip: Bautura | Cantitate: %.0f ml | Cofeina: %s",
                        d->getAmount(), d->getCaffeine() ? "Da" : "Nu");
                }
                else if (Food* f = dynamic_cast<Food*>(p)) {
                    ImGui::Text("Tip: Mancare | Calorii: %d | Vegan: %s",
                        f->getKcal(), f->getVegan() ? "Da" : "Nu");
                }

                if (selectedProductIndex == i) {
                    ImGui::InputInt("Cantitate", &currentQuantity);
                    currentQuantity = std::max(currentQuantity, 1);

                    if (ImGui::Button("Adauga in comanda")) {
                        selectedProducts.push_back(p);
                        quantities.push_back(currentQuantity);
                        selectedProductIndex = -1;
                        currentQuantity = 1;
                    }
                }

                ImGui::Unindent();
                ImGui::Separator();
            }

            ImGui::Text("Produse adaugate in comanda:");
            ImGui::Separator();

            totalBefore = 0;
            for (int i = 0; i < selectedProducts.size(); ++i) {
                float partial = selectedProducts[i]->getPrice() * quantities[i];
                totalBefore += partial;

                ImGui::BulletText("%s x%d = %.2f lei",
                    selectedProducts[i]->getName().c_str(), quantities[i], partial);

                ImGui::SameLine();
                std::string btn = "Sterge##" + std::to_string(i);
                if (ImGui::Button(btn.c_str())) {
                    selectedProducts.erase(selectedProducts.begin() + i);
                    quantities.erase(quantities.begin() + i);
                    break;
                }
            }

            ImGui::Separator();
            ImGui::Checkbox("Foloseste puncte de fidelitate?", &useLoyalty);
            ImGui::Text("Total: %.2f lei", totalBefore);

            if (ImGui::Button("Plaseaza comanda") && !selectedProducts.empty()) {
                Order comanda(temp, selectedProducts, quantities, useLoyalty);
                float valoareComanda = comanda.getTotalValue();
                int puncteUser = (*temp).getLoyaltyPoints();
                comanda.completeOrder();
                Order::addOrderToList(comanda);

                if (useLoyalty) {
                    if (puncteUser <= valoareComanda)
                        puncteFolosite = puncteUser;
                    else
                        puncteFolosite = (int)valoareComanda;
                }
                else
                    puncteFolosite = 0;
                totalAfter = comanda.getTotalValue();
                cout << totalBefore << " " << totalAfter << " " << puncteFolosite << "\n";
                cout << temp->getLoyaltyPoints();
               
                showPopup = true;
            }

            if (ImGui::Button("Inapoi")) {
                selectedProducts.clear();
                quantities.clear();
                useLoyalty = false;
                currentState = AppState::CustomerMenu;
            }

            if (showPopup) ImGui::OpenPopup("Comanda plasata!");
            if (ImGui::BeginPopupModal("Comanda plasata!", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Comanda ta a fost plasata cu succes!");

                ImGui::Text("Total inainte: %.2f lei", totalBefore);
                ImGui::Text("Puncte folosite: %d", puncteFolosite);
                ImGui::Text("Total final: %.2f lei", totalAfter);
                if (ImGui::Button("OK")) {
                    showPopup = false;
                    ImGui::CloseCurrentPopup();
                    currentState = AppState::CustomerMenu;
                    selectedProducts.clear();
                    quantities.clear();
                    useLoyalty = false;
                }
                ImGui::EndPopup();
            }

            ImGui::End();
         }
        else if (currentState == AppState::VerifyLPoints) {
            ImGui::Begin("Puncte dvs de loialitate");
            char* nameu = user.getName();
            User* temp;
            temp = User::getUserByName(nameu);

            if (temp != nullptr) {
                ImGui::Text("Nume: %s", temp->getName());
                ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.0f, 1.0f), "Puncte de fidelitate acumulate: %d", temp->getLoyaltyPoints());

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Text("Un punct de fidelitate valoreaza 1 leu.");
                ImGui::Text("Cum poti acumula puncte?");
                ImGui::BulletText("Pentru fiecare 20 lei cheltuiti, primesti 1 punct de fidelitate.");
                ImGui::BulletText("La inregistrare primesti automat 5 puncte.");
                ImGui::BulletText("Foloseste punctele atunci cand plasezi o comanda.");
                ImGui::SameLine();
                ImGui::TextDisabled("(?)");
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Bifeaza optiunea 'Foloseste puncte de fidelitate' la finalizarea comenzii.");

                ImGui::Spacing();
            }
            else 
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Eroare: utilizatorul nu a fost gasit.");
                

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::CustomerMenu;
            }

            ImGui::End();
        }
        else if (currentState == AppState::Reservation) {
            ImGui::Begin("Rezerva un loc");

            static int zi = 0, luna = 0, an = 2025;
            static int ora = 0, minut = 0;
            static int nrPersoane = 0;
            static bool showSuccess = false;
            static bool validareActivata = true;
            char what[256];

            ImGui::Text("Completeaza datele rezervarii:");

            ImGui::InputInt("Zi", &zi);
            ImGui::InputInt("Luna", &luna);
            ImGui::InputInt("An", &an);

            ImGui::InputInt("Ora", &ora);
            ImGui::InputInt("Minut", &minut);

            ImGui::InputInt("Numar persoane", &nrPersoane);

            // formatare data
            std::string formatted =
                (zi < 10 ? "0" : "") + to_string(zi) + "/" +
                (luna < 10 ? "0" : "") + to_string(luna) + "/" +
                to_string(an) + " " +
                (ora < 10 ? "0" : "") + to_string(ora) + ":" +
                (minut < 10 ? "0" : "") + to_string(minut);

            if (ImGui::Button("Rezerva")) {
                try {
                    Validator<int>::throwIfInRange(zi, 1, 31, "Ziua");
                    Validator<int>::throwIfInRange(luna, 1, 12, "Luna");
                    Validator<int>::throwIfInRange(ora, 0, 23, "Ora");
                    Validator<int>::throwIfInRange(minut, 0, 59, "Minutul");
                    Validator<int>::throwIfNotStrictPositive(nrPersoane, "Numarul de persoane");

                    Validator<string>::throwIfInvalidDate(formatted.substr(0, 10), "Data");
                    Validator<string>::throwIfDateInPast(formatted.substr(0, 10), "Data");
                    Validator<string>::throwIfTimeIntervalIncorrect(formatted.substr(11), "Ora");
                    Validator<User*>::throwIfNull(User::getUserByName(user.getName()), "Client");
                    

                    User* u = User::getUserByName(user.getName());
                    Reservation r(u->getUserId(), nrPersoane, &formatted[0]);
                    Reservation::addReservationToList(r);
                    showSuccess = true;
                }
                catch (const std::exception& ex) {
                    strcpy(what, ex.what());
                    ImGui::OpenPopup("EroareValidare");
                }
            }

            if (showSuccess) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Rezervarea a fost efectuata!");
                ImGui::Text("Data si ora: %s", formatted.c_str());
            }

            if (ImGui::BeginPopupModal("EroareValidare", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Date invalide! Verifica campurile introduse.");
                ImGui::TextColored(ImVec4(1, 0, 0, 1), what);
                if (ImGui::Button("OK")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::CustomerMenu;
                showSuccess = false;
            }
            ImGui::End();
        }

        else if (currentState == AppState::ViewMyReservs) {
            ImGui::Begin("Rezervarile Mele");
            User* currentUser = User::getUserByName(user.getName());
            bool found = false;

            if (currentUser != nullptr) {
                for (int i = 0; i < Reservation::getSize(); ++i) {
                    Reservation& r = Reservation::getReservations()[i];
                    /*cout << *r.getCustomer() << "\n";
                    cout << *currentUser << "\n";*/
                    if (r.getCustomer()->getUserId() == currentUser->getUserId()) {
                        found = true;

                        ImGui::Separator();
                        ImGui::Text("Rezervare #%d", r.getReservationId());
                        ImGui::BulletText("Data si ora: %s", r.getReservationTime());
                        ImGui::BulletText("Numar persoane: %d", r.getNumberOfSeats());
                    }
                }
                if (!found) {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Nu ai nicio rezervare activa.");
                }
            }
            else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Eroare: utilizator negasit.");
            }

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::CustomerMenu;
            }

            ImGui::End();
        }

        else if (currentState == AppState::CancelReservation) {
            ImGui::Begin("Anuleaza o rezervare");

            User* currentUser = User::getUserByName(user.getName());
            bool found = false;

            if (currentUser) {
                for (int i = 0; i < Reservation::getSize(); ++i) {
                    Reservation& r = Reservation::getReservations()[i];

                    if (r.getCustomer() && r.getCustomer()->getUserId() == currentUser->getUserId()) {
                        found = true;

                        ImGui::Separator();
                        ImGui::Text("Rezervare #%d", r.getReservationId());
                        ImGui::BulletText("Data si ora: %s", r.getReservationTime());
                        ImGui::BulletText("Numar persoane: %d", r.getNumberOfSeats());

                        std::string btnId = "Anuleaza##" + std::to_string(r.getReservationId());
                        if (ImGui::Button(btnId.c_str())) {
                            Reservation::deleteReservationById(r.getReservationId());
                            ImGui::OpenPopup("Succes");
                            break;  
                        }
                    }
                }

                if (!found) {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Nu ai rezervari active.");
                }
            }
            else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Eroare: utilizator negasit.");
            }

            if (ImGui::BeginPopupModal("Succes", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Rezervarea a fost anulata cu succes!");
                if (ImGui::Button("OK")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (ImGui::Button("Inapoi")) {
                currentState = AppState::CustomerMenu;
            }

            ImGui::End();
            }


        //window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    User::deleteUserList();
    Order::deleteOrderList();
    Reservation::deleteReservationList();
    for (auto p : listOfProducts) {
        delete p;
    }
    listOfProducts.clear();


    return 0;
>>>>>>> eac809886df793f7a3a68e5fe4b8eabbc57eb546
}