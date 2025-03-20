#include<bits/stdc++.h>
using namespace std;
const unsigned int MAX_LENGTH = 256;
/*Meniu interactiv pentru un utilizator pentru a inregistra o comanda*/
/*
 * 4 clase: utilizator, meniu(cafele), bilant, comanda
 *
 * obs: poate poti face un sistem de rezervari
 */

class User {
private:
    char* name;
    char* password;
    bool isAdmin;
    int loyaltypoints;

    static int szListOfUsers;
    static User* listOfUsers;
public:
    User() {
        name=new char[MAX_LENGTH];
        password=new char[MAX_LENGTH];
        loyaltypoints=0;
        isAdmin=false;
    }
    User(char* name, char* password, bool isAdmin, int loyaltypoints) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->password=new char[strlen(password)+1];
        strcpy(this->password, password);
        this->isAdmin=isAdmin;
        this->loyaltypoints=0;
    }
    User(char* name) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->password=nullptr;
        isAdmin=false;
        this->loyaltypoints=0;

    }
    User(char* name, char* password) {
        this->name=new char[strlen(name)+1];
        strcpy(this->name, name);
        this->password=new char[strlen(password)+1];
        strcpy(this->password, password);
        this->isAdmin=false;
        this->loyaltypoints=0;

    }
    User(const User& user) {
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
        user.loyaltypoints=0;
        return in;
    }

    friend ostream& operator<<(ostream& out, const User& user) {
        out << "Nume user: " << user.name << '\n';
        out << "Parola user: " << user.password << '\n';
        if (!user.isAdmin)
            out<<"Puncte de fidelitate: "<<user.loyaltypoints<<'\n';
        return out;
    }
    User& operator=(const User& user) {
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
    bool operator==(const User& user) const {
        return (strcmp(this->name, user.name) == 0) && (strcmp(this->password, user.password) == 0);
    }

    static void addUserToList(const User& user);
    static bool verifyUser(const User& user);
    static User* getUserByName(char* name);

    char* getName();
    char* getPassword();
    bool getAdmin();
    int getLoyaltyPoints();
    void setName(char *name);
    void setPassword(char *password);
    void setAdmin(bool isAdmin);
    void setLoyaltyPoints(int loyaltypoints);
    void addLoyaltyPoints(int points);
};
User* User::getUserByName(char* name) {
    for (int i=0;i<szListOfUsers;i++) {
        if (strcmp(name, listOfUsers[i].name) == 0) {
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

int User::szListOfUsers = 0;
User* User::listOfUsers = new User[MAX_LENGTH];


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
        cout << "Cate produse vrei sa comanzi? ";
        in >> numProducts;
        Coffee** coffees = new Coffee*[numProducts];
        int* quantities = new int[numProducts];

        for (int i = 0; i < numProducts; i++) {
            int coffeeID;
            cout << "Introdu Id-ul cafelei: ";
            in >> coffeeID;
            coffees[i] = Coffee::getCoffeeById(coffeeID);
            in.get();
            cout << "Introdu numarul de bauturi dorite: ";
            in >> quantities[i];
            in.get();
        }

        char* name = new char[MAX_LENGTH];
        int ok=0;
        User* user;
        while (!ok) {
            cout << "Numele clientului pe care se face comanda: ";
            in.get(name, MAX_LENGTH);
            in.get();
            user = User::getUserByName(name);
            if (user == nullptr) {
                cout<<"User invalid!";
            }
            else
                ok=1;
        }
        char usePoints;
        cout << "Doresti sa folosesti punctele de fidelitate pentru reducere? (y/n): ";
        in >> usePoints;
        in.get();
        if (usePoints == 'y' || usePoints == 'Y')
            order = Order(user, coffees, quantities, numProducts, true);
        else
            order = Order(user, coffees, quantities, numProducts, false);

        delete[] coffees;
        delete[] quantities;

        return in;
    }

    friend ostream& operator<<(ostream& out, const Order& order) {
        cout << "Comanda dumneavoastra:\n";
        for (int i = 0; i < order.numCoffees; i++) {
            cout << "Cafea: " << order.coffeesOrdered[i]->getname() << ", Cantitate: " << order.quantities[i] << ", Pret: " << order.coffeesOrdered[i]->getprice() << "\n";
        }
        return out;
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
                customer->setLoyaltyPoints(customer->getLoyaltyPoints() - int(totalValue));
                totalValue = 0;
            }
        }
        cout << "Comanda a fost finalizata!\n";
        cout << "Au fost folosite " << aux << " puncte de fidelitate.\n";
        cout << "Total: " << totalValue << "\n";
        customer->addLoyaltyPoints(int(totalValue) / 10);
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


int main() {
    bool loggedin=0;
    User user;
    int option;
    user.setName("admin");
    user.setPassword("admin_password");
    user.setAdmin(1);
    User::addUserToList(user);
    Coffee::addCoffeeToList(Coffee("Espresso", 10.00, "1 shot de espresso", 30));
    Coffee::addCoffeeToList(Coffee("Cappucciono", 13.00, "1 shot de espresso, lapte", 150));
    Coffee::addCoffeeToList(Coffee("Latte", 10.00, "1 shot de espresso, lapte, lapte spuma", 300));
    do {
        if (!loggedin) {
            cout<<"[Coffee Shop]--AUTENTIFICARE--\n";
            cout<<"1.Intra in cont.\n";
            cout<<"2.Creaza cont.\n";
            cout<<"3.Iesire\n";
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
                User::addUserToList(user);
            }
            else if (option==3) {
                cout<<"Ai inchis aplicatia!";
                break;
            }
            else {
                cout<<"Optiune invalida!\n";
            }
        }
        if (loggedin) {
            cout<<"[Coffee Shop] -- MENIU INTERACTIV --\n";
            if (user.getAdmin()) {
                cout<<"1. Adauga cafea\n2. Afiseaza meniu\n3. Actualizeaza cafea\n4. Sterge cafea\n5. Afiseaza comenzi\n0. Iesire din cont\nAlegeti o optiune: ";
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
                    case 4:
                        int coffeeIdToDelete;
                        cout<<"Introdu id-ul produsului pe care vrei sa il stergi: ";
                        cin>>coffeeIdToDelete;
                        cin.get();
                        Coffee::deleteCoffeeById(coffeeIdToDelete);
                        break;
                    // case 5:

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
                cout << "0. Iesire din cont\n";
                cout << "Alegeti optiunea: ";
                cin >> option;
                cin.get();
                switch (option) {
                    case 0: {
                        cout<<"PA\n";
                        loggedin=0;
                        break;
                    }
                    case 1: {
                        Order order;
                        cin>>order;
                        cout<<"Previzualizare comanda:\n";
                        cout<<order;
                        cout<<"Finalizezi comanda?[y/n]:";
                        char yn;
                        cin>>yn;
                        if (yn=='y')
                            order.completeOrder();
                        break;
                    }
                    case 2: {
                        cout<<"Punctele dvs de fidelitate: ";
                        user.getLoyaltyPoints();
                        cout<<'\n';
                    }
                    default:
                        break;
                }
            }
        }
    }while (true);





    return 0;
}