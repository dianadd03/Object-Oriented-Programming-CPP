#include<bits/stdc++.h>
#include <windows.h>
using namespace std;
const unsigned int MAX_LENGTH = 256;

int frequency[MAX_LENGTH];

void SetColor(int ForgC){
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void setupConsole() {
    SetConsoleOutputCP(CP_UTF8);
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
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
        name=new char[MAX_LENGTH];
        password=new char[MAX_LENGTH];
        loyaltypoints=0;
        isAdmin=false;
    }
    User(char* name, char* password, bool isAdmin, int loyaltypoints) : userId(generatorUId++) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->password=new char[strlen(password)+1];
        strcpy(this->password, password);
        this->isAdmin=isAdmin;
        this->loyaltypoints=loyaltypoints;
    }
    User(char* name) : userId(generatorUId++) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->password=nullptr;
        isAdmin=false;
        this->loyaltypoints=0;

    }
    User(char* name, char* password) : userId(generatorUId++) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->password=new char[strlen(password)+1];
        strcpy(this->password, password);
        this->isAdmin=false;
        this->loyaltypoints=0;

    }
    User(const User& user) : userId(generatorUId++) {
        this->isAdmin=user.isAdmin;
        this->loyaltypoints=user.loyaltypoints;
        if (this->name != nullptr) {
            delete [] this->name;
        }
        this->name=new char[strlen(user.name)+1];
        strcpy(this->name, user.name);

        if (this->password != nullptr) {
            delete [] this->password;
        }
        this->password=new char[strlen(user.password)+1];
        strcpy(this->password, user.password);
    }
    ~User() {
        if (this->name != nullptr) {
            delete [] this->name;
        }
        if (this->password != nullptr) {
            delete [] this->password;
        }
    }
    friend istream& operator>>(istream& in, User& user) {
        cout << "Introdu numele: ";
        in.get(user.name, MAX_LENGTH);
        in.get();

        cout << "Introdu parola: ";
        in.get(user.password, MAX_LENGTH);
        in.get();
        if (strcmp(user.name, "admin")==0)
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
        inFile>>isAdmin;
        inFile.get();
        inFile>>loyaltypoints;
        inFile.get();
    }
    friend ostream& operator<<(ostream& out, const User& user) {
        out<<"ID user: "<<user.userId<<"\n";
        out << "Nume user: " << user.name << '\n';
        out << "Parola user: " << user.password << '\n';
        if (!user.isAdmin)
            out<<"Puncte de fidelitate: "<<user.loyaltypoints<<'\n';
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
            delete [] this->password;
        }
        this->password=new char[strlen(user.password)+1];
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
    User& operator+(int x)  {
        this->loyaltypoints += x;
        return *this;
    }

    friend User& operator+(int x, User& user) {
        return user+x;
    }
    User& operator-(int x) {
        this->loyaltypoints-=x;
        return *this;
    }
    friend User& operator-(int x, User& user) {
        user.loyaltypoints=x-user.loyaltypoints;
        return user;
    }
    bool operator<(const User& user) const {
        return this->loyaltypoints < user.loyaltypoints;
    }

    void getUserById() const;
    static void addUserToList(const User& user);
    static bool verifyUser(const User& user);
    static User* getUserByName(char* name);
    static User* getUserById(int Id);
    int getUserId();
    char* getName();
    char* getPassword();
    bool getAdmin();
    int getLoyaltyPoints();
    void setName(char *name);
    void setPassword(char *password);
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
    for (int i=0;i<szListOfUsers;i++) {
        if (strcmp(name, listOfUsers[i].name) == 0) {
            return &listOfUsers[i];
        }
    }
    return nullptr;
}
User* User::getUserById(int id) {
    for (int i=0;i<szListOfUsers;i++) {
        if (listOfUsers[i].userId==id) {
            return &listOfUsers[i];
        }
    }
    return nullptr;
}
void User::addUserToList(const User& user){
    listOfUsers[szListOfUsers++] = user;
}
bool User::verifyUser(const User& user) {
    for (int i=0;i<szListOfUsers;i++)
        if (strcmp(listOfUsers[i].name, user.name)==0)
            if (strcmp(listOfUsers[i].password, user.password)==0)
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
char *User::getPassword() {
    return this->password;
}
bool User::getAdmin() {
    return this->isAdmin;
}
int User::getLoyaltyPoints() {
    return this->loyaltypoints;
}
void User::setName(char *name) {
    if (this->name != nullptr) {
        delete[] this->name;
    }
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}
void User::setPassword(char *password) {
    if (this->password != nullptr) {
        delete [] this->password;
    }
    this->password=new char[strlen(password)+1];
    strcpy(this->password, password);
}
void User::setAdmin(bool isAdmin) {
    this->isAdmin=isAdmin;
}
void User::setLoyaltyPoints(int loyaltypoints) {
    this->loyaltypoints = loyaltypoints;
}
int User::generatorUId = 1;
int User::szListOfUsers = 0;
User* User::listOfUsers = new User[MAX_LENGTH];

void User::showAllUsers(){
    for (int i = 0; i < szListOfUsers; i++) {
        cout << listOfUsers[i] << "\n";
    }
}
class Product {
protected:
    const int productId;
    string name;
    float price;
    string description;

    static int generatorProductId;
public:
    Product():productId(generatorProductId++) {
        name="ProductX";
        price=0.0;
        description="";
    }
    Product(string name, float price, string description):productId(generatorProductId++) {
        this->name=name;
        this->price=price;
        this->description=description;
    }
    Product(string name):productId(generatorProductId++) {
        this->name=name;
        price=0.0;
        description="";
    }
    Product(string name, float price):productId(generatorProductId++) {
        this->name=name;
        this->price=price;
        this->description="";
    }
    Product(const Product& p):productId(p.productId) {
        this->name=p.name;
        this->price=p.price;
        this->description=p.description;
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
    virtual bool matchesKeyword(const string& keyword) const = 0;

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
    Food():Product() {
        kcal=0;
        vegan=false;
    }
    Food(string name, float price, string description, int kcal, bool vegan)
        : Product(name, price, description) {
            this->kcal=kcal;
            this->vegan=vegan;
    }
    Food(string name, float price):Product(name, price) {
        kcal=0;
        vegan=false;
    }
    Food(string name) : Product(name) {
        kcal=0;
        vegan=0;
    }
    Food(string name, float price, int kcal):Product(name, price) {
        this->kcal=kcal;
        this->vegan=false;
    }
    Food(const Food& f) : Product(f) {
        this->kcal=f.kcal;
        this->vegan=f.vegan;
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
        out<< f.kcal << "\n";
        cout<<"Vegan: ";
        out<< (f.vegan ? "Da\n" : "Nu\n");
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
    void showDetails() const ;
    string getProductType() const;
    bool matchesKeyword(const string& keyword) const ;

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
bool Food::matchesKeyword(const string& keyword) const {
    string loweredName = toLower(name);
    string loweredDesc = toLower(description);
    string loweredKeyword = toLower(keyword);

    return loweredName.find(loweredKeyword) != string::npos ||
           loweredDesc.find(loweredKeyword) != string::npos;
}

void Food::showDetails() const {
    SetColor(1);
    cout <<"[MANCARE]";
    SetColor(15);
    cout<<": #"<<productId<<'\n';
    cout << "Denumire: " << name << '\n';
    cout << "Pretul: " << price << '\n';
    cout << "Descriere: " << description << '\n';
    cout  << "KiloCalorii: " << kcal << "\n";
    cout  << "Vegan: " << (vegan ? "Da" : "Nu") << "\n";
}
string Food::getProductType() const {
    return "Mancare";
}

class Drink : public virtual Product {
protected:
    float amount;
    bool caffeine;
public:
    Drink():Product() {
        amount=0;
        caffeine=false;
    }
    Drink(string name, float price, string description, float amount, bool caffeine):
        Product(name, price, description) {
            this->amount=amount;
            this->caffeine=caffeine;
    }
    Drink(string name, float price) : Product(name, price) {
        amount=0;
        caffeine=false;
    }
    Drink(string name, float price, int amount) : Product(name, price) {
        this->amount=amount;
        caffeine=false;
    }
    Drink(string name):Product(name) {
        amount=0;
        caffeine=false;
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
        out<< d.amount << "\n";
        cout<<"Cofeina: ";
        out<< (d.caffeine ? "Da\n" : "Nu\n");
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
    void showDetails() const ;
    string getProductType() const;
    bool matchesKeyword(const string& keyword) const;

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
        this->caffeine=caffeine;
    }
};

bool Drink::matchesKeyword(const string& keyword) const {
    string loweredName = toLower(name);
    string loweredDesc = toLower(description);
    string loweredKeyword = toLower(keyword);

    return loweredName.find(loweredKeyword) != string::npos ||
           loweredDesc.find(loweredKeyword) != string::npos;
}

void Drink::showDetails() const {
    SetColor(5);
    cout <<"[BAUTURA]";
    SetColor(15);
    cout<<": #"<<productId<<'\n';
    cout << "Denumire: " << name << '\n';
    cout << "Pretul: " << price << '\n';
    cout << "Descriere: " << description << '\n';
    cout  << "Cantitate: " << amount << "\n";
    cout  << "Cofeina: " << (caffeine ? "Da" : "Nu") << "\n";
}

string Drink::getProductType() const {
    return "Bautura";
}

class Offer: public Drink, public Food {

public:
    Offer() : Drink(), Food() {}
    Offer(string name, float price, string description, float amount, bool caffeine, int calories, bool vegan)
        : Product(name, price, description) {
            this->amount=amount;
            this->caffeine=caffeine;
            this->kcal=calories;
            this->vegan=vegan;
    }
    Offer(string name, float price) : Drink(name, price), Food(name, price) {}
    Offer(string name) : Food(name), Drink(name) {}
    Offer(const Offer& other)
        : Product(other), Drink(other), Food(other) {}

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
        out<< o.amount << "\n";
        cout<<"Cofeina: ";
        out<< (o.caffeine ? "Da\n" : "Nu\n");
        cout << "--- Detalii mancare ---\n";
        cout << "KiloCalorii: ";
        out<< o.kcal << "\n";
        cout<<"Vegan: ";
        out<< (o.vegan ? "Da\n" : "Nu\n");
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

    void showDetails() const ;
    string getProductType() const;
    bool matchesKeyword(const string& keyword) const;

};

bool Offer::matchesKeyword(const string& keyword) const {
    string loweredName = toLower(name);
    string loweredDesc = toLower(description);
    string loweredKeyword = toLower(keyword);

    return loweredName.find(loweredKeyword) != string::npos ||
           loweredDesc.find(loweredKeyword) != string::npos;
}

void Offer::showDetails() const  {
    SetColor(4);
    cout <<"[OFERTA]";
    SetColor(15);
    cout<<": #"<<productId<<'\n';
    cout << "Nume: " << name <<"\n";
    cout << "Pretul: " << price << "\n";
    cout << "Descriere: " << description << "\n";
    cout << "--- Detalii bautura ---\n";
    cout << "Cantitate: "<< amount << "\n";
    cout<<"Cofeina: "<< (caffeine ? "Da\n" : "Nu\n");
    cout << "--- Detalii mancare ---\n";
    cout << "KiloCalorii: "<< kcal << "\n";
    cout<<"Vegan: "<< (vegan ? "Da\n" : "Nu\n");
}
string Offer::getProductType() const {
    return "Oferta";
}
vector<Product*>listOfProducts;

bool sort_criteria( Product *A, Product *B) {
    return A->getProductType() < B->getProductType();
}

void showAllProducts() {
    sort(listOfProducts.begin(), listOfProducts.end(), sort_criteria);
    for (auto p : listOfProducts) {
        p->showDetails();
        cout << "-------------------------\n";
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

    Order(User* customer, vector<Product*>& products, vector<int>& qty,  bool usedLoyaltyPoints) : orderId(generatorIdOrd++) {
        this->customer = customer;
        productsOrdered=products;
        quantities=qty;
        totalValue = 0;
        this->usedLoyaltyPoints = usedLoyaltyPoints;
        for (int i = 0; i < products.size(); ++i) {
            totalValue += products[i]->getPrice() * quantities[i];
        }
    }

    Order(User* customer) : orderId(generatorIdOrd++) {
        totalValue = 0;
        this->customer = customer;
    }
    Order(const Order& o) : orderId(generatorIdOrd++){
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
        out << "Comanda dumneavoastra #"<<order.orderId << "\n";
        for (int i = 0; i < order.productsOrdered.size(); i++) {
            out << "Produs: " << order.productsOrdered[i]->getName() << "\n";
            out<<"Nr de produse: " << order.quantities[i] << "\n";
            cout<<"Pret per bucata: " << order.productsOrdered[i]->getPrice() << "\n";
        }
        out<<"Total: "<<order.totalValue<<"\n";
        return out;
    }

    Order& operator++() {
        for (int i = 0; i < productsOrdered.size(); i++) {
            quantities[i]++;
            totalValue += productsOrdered[i]->getPrice();
        }
        return *this;
    }

    Order& operator+(float extra)  {
        this->totalValue += extra;
        return *this;
    }
    friend Order& operator+(float extra, Order& o) {
        return o + extra;
    }
    Order& operator-(float extra)  {
        this->totalValue -= extra;
        return *this;
    }
    friend Order& operator-(int x, Order& o) {
        o.totalValue=x-o.totalValue;
        return o;
    }

    bool operator==(const Order& o) const {
        return customer==o.customer;
    }

    bool operator<(const Order& o) const {
        return this->totalValue < o.totalValue;
    }

    void completeOrder() {
        int aux=0;
        if (usedLoyaltyPoints) {
            if (totalValue >= customer->getLoyaltyPoints()) {
                aux = customer->getLoyaltyPoints();
                totalValue -= aux;
                customer->setLoyaltyPoints(0);
            } else {
                aux = int(totalValue);
                *customer=*customer - int(totalValue);
                totalValue = 0;
            }
        }
        cout << "Comanda a fost finalizata!\n";
        cout << "Au fost folosite " << aux << " puncte de fidelitate.\n";
        cout << "Total: " << totalValue << "\n";
        *customer=(int(totalValue) / 10)+*customer;
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
    Reservation(): reservationId(generatorIdRes++) {
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
            delete [] reservationTime;
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
        r.numberOfSeats=x-r.numberOfSeats;
        return r;
    }
    bool operator==(const Reservation& r) const {
        return this->customerId==r.customerId;
    }

    bool operator<(const Reservation& r) const {
        return strcmp(this->reservationTime, r.reservationTime)<0;
    }
    friend istream& operator>>(istream& in, Reservation& res) {
        char* reservationTime= new char[MAX_LENGTH];
        cout<<"Introdu data si ora rezervarii(YY/MM/DD HH:MM): ";
        in.get(reservationTime, MAX_LENGTH);
        in.get();
        int nseats;
        cout<<"Pentru cate persoane? ";
        in>>nseats;
        in.get();
        res = Reservation(res.customerId, nseats, reservationTime);
        delete[] reservationTime;
        return in;
    }
    friend ostream& operator<<(ostream& out, const Reservation& res) {
        out<<"Id-ul rezervarii: "<<res.reservationId<<'\n';
        User* user = User::getUserById(res.customerId);
        out<<"Nume: "<<(*user).getName()<<'\n';
        out<<"Numar de persoane: "<<res.numberOfSeats<<'\n';
        out<<"Data si ora: "<<res.reservationTime;
        out<<"\n";

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
    return User::getUserById(reservationId);
}
int Reservation::getNumberOfSeats() {
    return numberOfSeats;
}
char* Reservation::getReservationTime() {
    return reservationTime;
}
void Reservation::setReservationTime(char* reservationTime) {
    this->reservationTime = new char[MAX_LENGTH];
    strcpy(this->reservationTime, reservationTime);
}
void Reservation::setCustomer(User* customer) {
    this->customerId = customerId;
}
void Reservation::setNumberOfSeats(int numberOfSeats) {
    this->numberOfSeats = numberOfSeats;
}

void Reservation::addReservationToList(const Reservation& reservation) {
    listOfReservations[szListOfReservations++] = reservation;
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
    } else {
        cout << "Rezervarea cu ID-ul " << reservationId << " nu a fost gasita.\n";
    }
}

void Reservation::showReservationsForUser( User& user) {
    bool found = false;
    for (int i = 0; i < szListOfReservations; i++) {
        if (listOfReservations[i].customerId == user.getUserId()) {
            cout<<listOfReservations[i];
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
        cout<<listOfReservations[i];
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

void searchProductByKeyword(const string& keyword) {
    bool found = false;
    for (auto p : listOfProducts) {
        if (p->matchesKeyword(keyword)) {
            p->showDetails();
            found = true;
        }
    }
    if (!found)
        cout << "Niciun produs nu se potriveste cu: " << keyword << "\n";
}

void showClientMenu() {
    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║           WELCOME TO OUR COFFEE SHOP         ║\n";
    cout << "╠══════════════════════════════════════════════╣\n";
    cout << "║  1. Plaseaza o comanda                       ║\n";
    cout << "║  2. Verifica puncte de fidelitate            ║\n";
    cout << "║  3. Rezerva un loc                           ║\n";
    cout << "║  4. Vezi rezervarile tale                    ║\n";
    cout << "║  5. Anuleaza o rezervare                     ║\n";
    cout << "║  0. Iesire din cont                          ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    cout << "Selecteaza optiunea dorita: "<<flush;
}

void showAdminMenu() {
    cout << "╔═════════════════════════════════════════════════╗\n";
    cout << "║            ADMIN COFFEE CONTROL PANEL           ║\n";
    cout << "╠═════════════════════════════════════════════════╣\n";
    cout << "║  1. Adauga produs                               ║\n";
    cout << "║  2. Afiseaza meniul complet                     ║\n";
    cout << "║  3. Actualizeaza un produs existent             ║\n";
    cout << "║  4. Sterge un produs                            ║\n";
    cout << "║  5. Afiseaza toate rezervarile                  ║\n";
    cout << "║  6. Afiseaza lista clientilor                   ║\n";
    cout << "║  0. Deconectare                                 ║\n";
    cout << "╚═════════════════════════════════════════════════╝\n";
    cout << "Selecteaza o optiune din meniul de mai sus: "<<flush;
}


int main() {
    setupConsole();
    frequency[0] = 10000;
    bool loggedin=0;
    User admin;
    User user;
    int option;
    admin.setName("@admin");
    admin.setPassword("admin_password");
    admin.setAdmin(1);
    User::addUserToList(admin);
    User user1;
    user1.setName("Diana");
    user1.setPassword("Delia");
    user1.setAdmin(0);
    user1.setLoyaltyPoints(100);
    User::addUserToList(user1);
    User user2;
    user2.setName("Maria");
    user2.setPassword("Ioana");
    user2.setAdmin(0);
    user2.setLoyaltyPoints(10);
    User::addUserToList(user2);
    readProductsFromFile("meniu.txt");
    do {
        if (!loggedin) {

            cout<<"[Coffee Shop]--AUTENTIFICARE--\n";
            cout<<"1.Intra in cont.\n";
            cout<<"2.Creeaza cont.\n";
            cout<<"0.Iesire\n";
            cout<<"Introdu optiunea ta:";
            cin>>option;
            cin.get();
            if (option==1) {
                cin>>user;
                loggedin=1;
                if (!User::verifyUser(user)) {
                    cout<<"Utilizator negasit!\n";
                    loggedin=0;
                }
            }
            else if (option==2) {
                cin>>user;
                loggedin=1;
                user.setLoyaltyPoints(5);
                User::addUserToList(user);
                cout<<"Ai primit 5 puncte din partea noastra pentru inregistrare!\n";
            }
            else if (option==0) {
                cout<<"Ai inchis aplicatia!";
                break;
            }
            else {
                cout<<"Optiune invalida!\n";
            }
        }
        if (loggedin) {
            if (user[0]=='@') {
                showAdminMenu();
                cin>>option;
                cin.get();
                switch (option) {
                    case 1: {
                        cout << "a. Bautura\nb. Mancare\nc. Oferta\n";
                        char type;
                        cin >> type;
                        cin.get();

                        Product* p = nullptr;

                        if (type == 'a') {
                            Drink* d = new Drink();
                            cin >> *d;
                            p = d;
                        } else if (type == 'b') {
                            Food* f = new Food();
                            cin >> *f;
                            p = f;
                        } else if (type == 'c') {
                            Offer* o = new Offer();
                            cin >> *o;
                            p = o;
                        }

                        if (p) {
                            listOfProducts.push_back(p);
                            cout << "Produs adaugat cu succes!\n";
                        }
                        break;
                    }
                    case 2: {
                        showAllProducts();
                        break;
                    }
                    case 3: {
                        int idToUpdate;
                        cout << "Introdu ID-ul produsului de actualizat: ";
                        cin >> idToUpdate;
                        cin.get();

                        bool found = false;

                        for (Product* p : listOfProducts) {
                            if (p && p->getProductType() == "Bautura" && dynamic_cast<Drink*>(p)->getProductId() == idToUpdate) {
                                cout << "Produs gasit! Introdu noile date:\n";
                                cin >> *dynamic_cast<Drink*>(p);
                                found = true;
                                break;
                            }
                            if (p && p->getProductType() == "Mancare" && dynamic_cast<Food*>(p)->getProductId() == idToUpdate) {
                                cin >> *dynamic_cast<Food*>(p);
                                found = true;
                                break;
                            }
                            if (p && p->getProductType() == "Oferta" && dynamic_cast<Offer*>(p)->getProductId() == idToUpdate) {
                                cin >> *dynamic_cast<Offer*>(p);
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                            cout << "Produsul nu a fost gasit.\n";

                        break;
                    }
                    case 4: {
                        int prodIdToDelete;
                        cout<<"Introdu id-ul produsului pe care vrei sa il stergi: ";
                        cin>>prodIdToDelete;
                        cin.get();
                        bool found = false;

                        for (size_t i = 0; i < listOfProducts.size(); ++i) {
                            if (listOfProducts[i]->getProductId() == prodIdToDelete) {
                                delete listOfProducts[i];
                                listOfProducts.erase(listOfProducts.begin() + i);
                                found = true;
                                cout << "Produsul a fost sters cu succes!\n";
                                break;
                            }
                        }

                        if (!found) {
                            cout << "Nu am gasit produsul cu ID-ul mentionat.\n";
                        }
                        break;
                    }
                    case 5: {
                        Reservation::showAllReservation();
                        break;
                    }
                    case 6: {
                        User::showAllUsers();
                        break;
                    }
                    case 0: {
                        loggedin=0;
                        break;
                    }
                    default:
                        cout<<"Ai introdus o optiune invalida!\n";

                }
            }
            else {
               showClientMenu();
                cin >> option;
                cin.get();
                switch (option) {
                    case 0: {
                        cout<<"Logged out!\n";
                        loggedin=0;
                        break;
                    }
                    case 1: {
                        cout<<"\n";
                        cout<<"Vrei sa cauti un produs anume?[1/0]: ";
                        int option;
                        cin>>option;
                        if (option) {
                            cout<<"Introdu un cuvant sugestiv pentru cautarea ta: ";
                            string keyword;
                            cin>>keyword;
                            searchProductByKeyword(keyword);
                        }
                        else
                            showAllProducts();

                        char *nameu=user.getName();
                        User* temp;
                        temp=User::getUserByName(nameu);
                        // cout<<*temp;
                        Order order(temp);
                        cin>>order;
                        cout<<"Previzualizare comanda:\n\n";
                        cout<<order;
                        cout<<"Finalizezi comanda?[y/n]: ";
                        char yn;
                        cin>>yn;
                        if (tolower(yn)=='y')
                            order.completeOrder();
                        else if (tolower(yn)!='n')
                            cout<<"Optiune invalida!\n";

                    }
                    case 2: {
                        char *nameu=user.getName();
                        User* temp;
                        temp=User::getUserByName(nameu);
                        cout<<"Punctele dvs de fidelitate: "<<(*temp).getLoyaltyPoints();
                        cout<<'\n';
                        break;
                    }
                    case 3: {

                        char *nameu=user.getName();
                        User* temp;
                        temp=User::getUserByName(nameu);
                        int idu=(*temp).getUserId();
                        Reservation reserv(idu);
                        cin>>reserv;
                        cout<<reserv;
                        cout<<"Confirmi rezervarea?[Y/N]:";
                        char r;
                        cin>>r;
                        if (toupper(r)=='Y') {
                            Reservation::addReservationToList(reserv);
                            cout<<"Rezervare confirmata!\n";
                        }
                        else if (tolower(r)!='n')
                            cout<<"Optiune invalida!\n";
                        break;
                    }
                    case 4: {
                        char *nameu=user.getName();
                        User* temp;
                        temp=User::getUserByName(nameu);
                        Reservation::showReservationsForUser(*temp);
                        break;
                    }
                    case 5: {
                        int id;
                        cout<<"Introdu Id-ul rezervarii pe care vrei sa o stergi: ";
                        cin>>id;
                        Reservation::deleteReservationById(id);
                        break;
                    }
                    default:
                        cout<<"Optiune invalida!\n";

                }
            }
        }
    }while (true);



    User::deleteUserList();
    Order::deleteOrderList();
    Reservation::deleteReservationList();
    for (auto p : listOfProducts) {
        delete p;
    }
    listOfProducts.clear();


    return 0;
}
