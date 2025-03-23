#include<bits/stdc++.h>
using namespace std;
const unsigned int MAX_LENGTH = 256;


int frequency[MAX_LENGTH];

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


class Coffee {
private:
    const int coffeeId;
    char* name;
    float price;
    char* description;
    double amount;

    static int generatorIdCoff;
    static int szListOfCoffees;
    static Coffee* listOfCoffees;
public:
    Coffee(): coffeeId(generatorIdCoff++) {
        name=new char[MAX_LENGTH];
        price=0.0;
        description=new char[MAX_LENGTH];
        amount=0.0;
    }
    Coffee(char* name, float price, char* description, double amount): coffeeId(generatorIdCoff++) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->description=new char[strlen(description)+1];
        strcpy(this->description, description);
        this->price=price;
        this->amount=amount;
    }
    Coffee(char* name): coffeeId(generatorIdCoff++) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->price=0.0;
        this->description=nullptr;
        this->amount=0.0;
    }
    Coffee(char* name, float price) : coffeeId(generatorIdCoff++) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->price=0.0;
        this->description=nullptr;
        this->amount=0.0;
    }
    Coffee(const Coffee& coffee): coffeeId(generatorIdCoff++) {
        this->price=coffee.price;

        if (this->name != nullptr) {
            delete [] this->name;
        }
        this->name=new char[strlen(coffee.name)+1];
        strcpy(this->name, coffee.name);

        if (this->description != nullptr) {
            delete [] this->description;
        }
        this->description=new char[strlen(coffee.description)+1];
        strcpy(this->description, coffee.description);
        this->amount=0.0;

    }
    ~Coffee() {
        if (this->name != nullptr) {
            delete [] this->name;
        }
        if (this->description != nullptr) {
            delete [] this->description;
        }
    }
    friend istream& operator>>(istream& in, Coffee& coffee) {
        cout << "Introdu denumirea cafelei: ";
        in.get(coffee.name, MAX_LENGTH);
        in.get();
        cout << "Introdu pretul: ";
        in >> coffee.price;
        in.get();
        cout << "Introdu descrierea: ";
        in.get(coffee.description, MAX_LENGTH);
        in.get();
        cout <<"Introdu cantitatea: ";
        in >> coffee.amount;
        in.get();
        return in;
    }

    friend ostream& operator<<(ostream& out, const Coffee& coffee) {
        out << "Id cafea: " << coffee.coffeeId << "\n";
        out << "Denumire cafea: " << coffee.name << '\n';
        out << "Pretul: " << coffee.price << '\n';
        out << "Descriere: " << coffee.description << '\n';
        out << "Cantitate: " << coffee.amount << '\n';
        return out;
    }
    Coffee& operator=(const Coffee& coffee) {
        this->price = coffee.price;
        this->amount=coffee.amount;

        if (this->name != nullptr) {
            delete[] this->name;
        }
        this->name = new char[strlen(coffee.name) + 1];
        strcpy(this->name, coffee.name);

        if (this->description != nullptr) {
            delete [] this->description;
        }
        this->description=new char[strlen(coffee.description)+1];
        strcpy(this->description, coffee.description);

        return *this;
    }
    char operator[](int index) const {
        if (index >= 0 && index < strlen(name)) {
            return name[index];
        }
        return 'X';
    }
    Coffee& operator++() {
        this->price++;
        return *this;
    }
    Coffee& operator--() {
        this->price--;
        return *this;
    }
    Coffee& operator+(float val)  {
        this->price+=val;
        return *this;
    }

    Coffee& operator-(float val)  {
        this->price-=val;
        return *this;
    }

    friend Coffee& operator+(float val, Coffee& coffee) {
        return coffee + val;
    }

    friend Coffee& operator-(float val,  Coffee& coffee) {
        coffee.price = val - coffee.price;
        return coffee;
    }
    bool operator==(const Coffee& c) const {
        return strcmp(this->name, c.name) == 0 && abs(this->price - c.price) < 0.01;
    }

    bool operator<(const Coffee& c) const {
        return this->price < c.price;
    }
    char* getname();
    float getprice();
    char* getdescription();
    double getamount();
    void setname(char* name);
    void setprice(float price);
    void setdescription(char* description);
    void setamount(double amount);
    static void addCoffeeToList(const Coffee& coffee);
    static void showAllCoffees();
    static Coffee* getCoffeeById(int coffeeId);
    static void deleteCoffeeById(int coffeeId);
    static void deleteCoffeeList();
};
char* Coffee::getname() {
    return this->name;
}

float Coffee::getprice() {
    return this->price;
}
char* Coffee::getdescription() {
    return this->description;
}
double Coffee::getamount() {
    return this->amount;
}

void Coffee::setname(char *name) {
    if (this->name != nullptr) {
        delete[] this->name;
    }
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

void Coffee::setprice(float price) {
    this->price = price;
}

void Coffee::setdescription(char *description) {
    if (this->description != nullptr) {
        delete[] this->description;
    }
    this->description = new char[strlen(description) + 1];
    strcpy(this->description, description);
}

void Coffee::setamount(double amount) {
    this->amount = amount;
}

void Coffee::addCoffeeToList(const Coffee& coffee) {
    listOfCoffees[szListOfCoffees++] = coffee;
}

void Coffee::showAllCoffees() {
    for (int i = 0; i < szListOfCoffees; i++) {
        cout << listOfCoffees[i] << endl;
    }
}

Coffee* Coffee::getCoffeeById(int coffeeId) {
    for (int i=0; i< szListOfCoffees; i++) {
        if (listOfCoffees[i].coffeeId == coffeeId) {
            return &listOfCoffees[i];
        }
    }
    return nullptr;
}

void Coffee::deleteCoffeeById(int coffeeId) {
    bool startDelete = false;
    if (coffeeId == listOfCoffees[szListOfCoffees - 1].coffeeId) {
        szListOfCoffees -= 1;
        return;
    }

    for (int i = 1; i < szListOfCoffees; i++) {
        if (coffeeId == listOfCoffees[i - 1].coffeeId) {
            startDelete = true;
        }
        if (startDelete) {
            listOfCoffees[i - 1] = listOfCoffees[i];
        }
    }

    if (startDelete) {
        szListOfCoffees -= 1;
    }
    else {
        cout << "Nu am gasit produsul cu ID-ul mentionat: " << coffeeId << "\n";
    }
}

void Coffee::deleteCoffeeList() {
    szListOfCoffees = 0;
    if (listOfCoffees != nullptr) {
        delete[] listOfCoffees;
    }
}

int Coffee::generatorIdCoff=10;
int Coffee::szListOfCoffees = 0;
Coffee* Coffee::listOfCoffees = new Coffee[MAX_LENGTH];

class Order {
private:
    const long long orderId;
    User* customer;
    Coffee** coffeesOrdered;
    int* quantities;
    int numCoffees;
    float totalValue;
    bool usedLoyaltyPoints;

    static long long generatorIdOrd;
    static int szListOfOrders;
    static Order* listOfOrders;

public:
    Order() : orderId(generatorIdOrd++) {
        customer = nullptr;
        coffeesOrdered = new Coffee*[MAX_LENGTH];
        quantities = new int[MAX_LENGTH];
        numCoffees = 0;
        totalValue = 0;
        usedLoyaltyPoints = false;
    }

    Order(User* customer, Coffee** coffees, int* qty, int numProducts, bool usedLoyaltyPoints) : orderId(generatorIdOrd++) {
        this->customer = customer;
        coffeesOrdered = new Coffee*[numProducts];
        quantities = new int[numProducts];
        numCoffees = numProducts;
        totalValue = 0;
        this->usedLoyaltyPoints = usedLoyaltyPoints;
        for (int i = 0; i < numProducts; i++) {
            coffeesOrdered[i] = coffees[i];
            quantities[i] = qty[i];
            totalValue += coffeesOrdered[i]->getprice() * quantities[i];
        }
    }

    Order(User* customer) : orderId(generatorIdOrd++) {
        coffeesOrdered = new Coffee*[MAX_LENGTH];
        quantities = new int[MAX_LENGTH];
        numCoffees = 0;
        totalValue = 0;
        this->customer = customer;
    }

    ~Order() {
        delete[] coffeesOrdered;
        delete[] quantities;
    }

    Order& operator=(const Order& o) {
        delete[] coffeesOrdered;
        delete[] quantities;

        coffeesOrdered = new Coffee*[o.numCoffees];
        quantities = new int[o.numCoffees];

        numCoffees = o.numCoffees;
        totalValue = o.totalValue;
        usedLoyaltyPoints = o.usedLoyaltyPoints;
        customer = o.customer;

        for (int i = 0; i < numCoffees; i++) {
            coffeesOrdered[i] = o.coffeesOrdered[i];
            quantities[i] = o.quantities[i];
        }

        return *this;
    }

    friend istream& operator>>(istream& in, Order& order) {
        Coffee::showAllCoffees();
        int numProducts;
        cout << "Cate produse diferite vrei sa comanzi? ";
        in >> numProducts;
        Coffee** coffees = new Coffee*[numProducts];
        int* quantities = new int[numProducts];

        for (int i = 0; i < numProducts; i++) {
            int coffeeID;
            cout << "Introdu Id-ul cafelei: ";
            in >> coffeeID;
            frequency[coffeeID-10]++;
            coffees[i] = Coffee::getCoffeeById(coffeeID);
            in.get();
            cout << "Introdu numarul de bauturi dorite: ";
            in >> quantities[i];
            in.get();
        }

        char usePoints;
        cout << "Doresti sa folosesti punctele de fidelitate pentru reducere? (y/n): ";
        in >> usePoints;
        in.get();

        cout<<"aici";
        if (usePoints == 'y' || usePoints == 'Y')
            order = Order(order.customer, coffees, quantities, numProducts, true);
        else
            order = Order(order.customer, coffees, quantities, numProducts, false);
        cout<<"aici2";

        delete[] coffees;
        delete[] quantities;

        return in;
    }

    friend ostream& operator<<(ostream& out, const Order& order) {
        out << "Comanda dumneavoastra:\n";
        for (int i = 0; i < order.numCoffees; i++) {
            out << "Cafea: " << order.coffeesOrdered[i]->getname() << "\n";
            out<<"Nr de bauturi: " << order.quantities[i] << "\n";
            cout<<"Pret per bucata: " << order.coffeesOrdered[i]->getprice() << "\n";
        }
        out<<"Total: "<<order.totalValue<<"\n";
        return out;
    }
    Coffee* operator[](int index) const {
        if (index >= 0 && index < numCoffees)
            return coffeesOrdered[index];
        return nullptr;
    }

    Order& operator++() {
        for (int i = 0; i < numCoffees; i++) {
            quantities[i]++;
            totalValue += coffeesOrdered[i]->getprice();
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

    // Operator de adunare
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
        cout << "Rezervarea cu ID-ul " << reservationId << " nu a fost gasită.\n";
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
int main() {
    frequency[0] = 10000;
    bool loggedin=0;
    User admin;
    User user;
    int option;
    admin.setName("admin");
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
    // User::showAllUsers();
    // User* user5=User::getUserById(2);
    // cout<<*user5;
    Coffee::addCoffeeToList(Coffee("Espresso", 10.00, "0:1:0 (lapte: cafea: spuma)", 30));
    Coffee::addCoffeeToList(Coffee("Cappucciono", 13.00, "1:1:1 (lapte: cafea: spuma)", 150));
    Coffee::addCoffeeToList(Coffee("Latte", 17.00, "3:1:0 (lapte: cafea: spuma)", 300));
    Coffee::addCoffeeToList(Coffee("Macchiato", 12.00, "1:2:0 (lapte: cafea: spuma)", 60));
    Coffee::addCoffeeToList(Coffee("Flat White", 15.00, "2:1:0 (lapte: cafea: spuma)", 150));
    Coffee::addCoffeeToList(Coffee("Mocha", 20.00, "2:1:1 (lapte: cafea: ciocolata)", 200));
    Coffee::addCoffeeToList(Coffee("Affogato", 22.00, "0:1:1 (lapte: cafea: i1nghetata)", 250));
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
            cout<<"[Coffee Shop] -- MENIU INTERACTIV --\n";
            if (user==admin) {
                cout << "1. Adauga cafea\n";
                cout << "2. Afiseaza meniu\n";
                cout << "3. Actualizeaza cafea\n";
                cout << "4. Sterge cafea\n";
                cout << "5. Afiseaza rezervarile\n";
                cout << "6. Afiseaza clientii\n";
                cout << "0. Iesire din cont\n";
                cout << "Alegeti o optiune: ";
                cin>>option;
                cin.get();
                switch (option) {
                    case 1: {
                        Coffee coffee;
                        cin>>coffee;
                        Coffee::addCoffeeToList(coffee);
                        break;
                    }
                    case 2: {
                        Coffee::showAllCoffees();
                        break;
                    }
                    case 3: {
                        int coffeeIdToSearch;
                        cout<<"Introdu id-ul produsului pe care vrei sa il actualizezi: ";
                        cin>>coffeeIdToSearch;
                        cin.get();

                        Coffee *coffee = Coffee::getCoffeeById(coffeeIdToSearch);
                        if (coffee != nullptr) {
                            Coffee Coffaux;
                            cin>>Coffaux;
                            *coffee = Coffaux;
                        }
                        else {
                            cout<<"Produsul nu se regaseste in meniu cu id-ul: "<<coffeeIdToSearch<<"\n";
                        }
                        break;
                    }
                    case 4: {
                        int coffeeIdToDelete;
                        cout<<"Introdu id-ul produsului pe care vrei sa il stergi: ";
                        cin>>coffeeIdToDelete;
                        cin.get();
                        Coffee *coffee = Coffee::getCoffeeById(coffeeIdToDelete);
                        if (coffee != nullptr) {
                            Coffee::deleteCoffeeById(coffeeIdToDelete);
                        }
                        else {
                            cout<<"Produsul nu se regaseste in meniu cu id-ul: "<<coffeeIdToDelete<<"\n";
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
                cout << "1. Plaseaza o comanda\n";
                cout << "2. Verifica puncte fidelitate\n";
                cout << "3. Rezerva un loc\n";
                cout << "4. Vezi rezervarile tale\n";
                cout << "5. Anuleaza o rezervare\n";
                cout << "0. Iesire din cont\n";
                cout << "Alegeti optiunea: ";
                cin >> option;
                cin.get();
                switch (option) {
                    case 0: {
                        cout<<"Logged out!\n";
                        loggedin=0;
                        break;
                    }
                    case 1: {
                        char *nameu=user.getName();
                        User* temp;
                        temp=User::getUserByName(nameu);
                        cout<<*temp;
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

                        for (int i=0;i<MAX_LENGTH;i++)
                            if (frequency[i]>10000) {
                                Coffee *cof;
                                cof=Coffee::getCoffeeById(i+10);
                                // cout<<*cof;
                                ++(*cof);
                                frequency[i]-=10000;
                            }
                        break;
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

    return 0;
}
