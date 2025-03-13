#include <iostream>
using namespace std;

struct node{
    int val;
    node *next;
};

class Stiva {
private:
    int size;
    node* top;

public:
    Stiva() {
        top = NULL;
        size=0;
    }
    Stiva(int val) {
        size=1;
        top = new node;
        top->val=val;
        top->next=NULL;
    }
    ~Stiva(){
        while(top!=NULL)
            pop();
    }
    void push(int val);
    int pop();
    int topOfStack();
    void display();
    int getSize();
};

void Stiva::push(int val){
    size++;
    if(top==NULL){
        top = new node;
        top->val=val;
        top->next=NULL;
    }
    else{
        node *p;
        p = new node;
        p->val=val;
        p->next=top;
        top=p;
    }
}

int Stiva::pop(){
    if(top==NULL)
        return -1;
    size--;
    node *p = top;
    int val=p->val;
    top=top->next;
    delete p;
    return val;
}
int Stiva::topOfStack(){
    if(top==NULL)
        return -1;
    return top->val;
}
void Stiva::display(){
    node *q=top;
    while(q!=NULL){
        cout<<q->val<<" ";
        q=q->next;
    }
    cout<<"\n";
}
int Stiva::getSize() {
    return size;
}

class Vector {
private:
    int* elements;
    int size;
    const int max_size;
public:
    Vector(): max_size(0) {
        elements = nullptr;
        size=0;
    }
    Vector(int max_size) : max_size(max_size) {
        size=0;
        if (max_size > 0)
            elements=new int[max_size];
        else
            elements=nullptr;
    }
    Vector(const Vector& Vector) : max_size(Vector.max_size){
        this->size=Vector.size;
        if (this->elements != nullptr) {
            delete[] this->elements;
        }
        this->elements = new int[Vector.max_size];
        for (int i=0 ; i<size ; i++ )
            elements[i] = Vector.elements[i];
    }
    ~Vector() {
        if (this->elements != nullptr)
            delete [] elements;
    }
    void push(int number);
    void pop();
    int getElementByIndex(int index);
    int getSize();
    void showElements();

};

void Vector::push(int number) {
    if (size==max_size){
        cout<<"No more memory for the new element\n";
        return;
    }
    elements[size++]=number;
}
void Vector::pop() {
    if (size==0) {
        cout<<"Empty vector\n";
        return;
    }
    size--;
}
int Vector::getElementByIndex(int index) {
    if (index<0 || index>=size) {
        cout<<"Index out of range\n";
        return -1;
    }
    return elements[index];
}

int Vector::getSize() {
    return size;
}

void Vector::showElements() {
    for (int i=0; i<size; i++)
        cout<<elements[i]<<" ";
    cout<<"\n";
    return;
}
int main() {
    cout<<"Vector: \n";
    Vector v(100);
    v.push(10);
    v.push(20);
    v.push(30);
    v.push(40);
    v.push(50);
    v.showElements();
    v.pop();
    v.showElements();
    cout<<v.getElementByIndex(0)<<"\n";
    cout<<v.getElementByIndex(3)<<"\n";
    cout<<v.getSize()<<"\n";
    cout<<"Stack:\n";
    Stiva s(2);
    s.push(10);
    s.push(20);
    s.push(30);
    s.push(40);
    cout<<s.topOfStack()<<'\n';
    s.display();
    cout<<s.getSize()<<"\n";
    cout<<"Deleted element: "<<s.pop()<<'\n';
    s.display();
    return 0;
}
