#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;
//coffeshop constants
const int FIRSTCUSTOMERS = 5, MAXTIME = 20, OPENINGTIME = 1;
const int HELPCUSTOMERPROB = 40, NEWCUSTOMERPROB = 60, LASTCUSTOMERLEAVEPROB = 20, ANYCUSTOMERLEAVEPROB=10, VIPPROB = 10;

class DoublyLinkedList {
private:
    struct Node {
        string data;
        Node* prev;
        Node* next;
        Node(string val, Node* p = nullptr, Node* n = nullptr) {
            data = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void insert_after(string value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            temp->next->prev = newNode;
        }
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_val(string value) {
        if (!head) return;

        Node* temp = head;
        
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) return; 

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next; 

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev; 

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            pop_front();
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            pop_back();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void leave_line(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }    
        Node* temp = head;
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
        }
        cout<<"\t"<<temp->data<<" leaves the line.\n";
        delete_pos(pos);            
    }

    void push_back(string v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void join_line(string v){
        cout<<"\t"<<v<<" joins the line.\n";
        push_back(v);
    }
    
    void push_front(string v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void VIP_joins(string v) {
        cout<<"\t"<<v<<" (VIP) joins the front of the line.\n";
        push_front(v);
    }
    
    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void serve_front() {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
        cout<<"\t"<<head->data<<" is served.\n";
        pop_front();
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void leave_end_of_line(){
        cout<<"\t"<<tail->data<<" exits the rear of the line.\n";
        pop_back();
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        cout<<"\tResulting Line:\n";
        while (current) {
            cout <<"\t\t"<<current->data << endl;
            current = current->next;
        }
    }

    int getListSize(){
        int size=0;
        Node* current = head;
        while(current){
            current=current->next;
            size++;
        }
        return size;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) { 
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->prev;
        }
        cout << endl;
    }
};

//function that returns a random name from the names vector.
string randomname(vector <string>);

int main() {
    srand(time(NULL));
    DoublyLinkedList CoffeeShop;
    //populate list of random names into vector
    vector <string> names;
    fstream file;
    file.open("names.txt");
    if(file.good()){
        string line;
        while(getline(file,line))
            names.push_back(line);
    }
    for(int minute=0;minute<MAXTIME;minute++){
        if(minute<OPENINGTIME){ //the opening time has a number of FIRSTCUSTOMERS arrive.
            cout<<"Store Opens:\n";
            for(int i=0;i<FIRSTCUSTOMERS;i++){
                CoffeeShop.join_line(randomname(names));
            }
            CoffeeShop.print();
            continue;
        }
        cout<<"Time Step #"<<minute+1<<":\n";
        //after the first five minutes:
        //help customer
        if((rand()%100)+1 < HELPCUSTOMERPROB) {
            CoffeeShop.serve_front();
        }

        //new customer
        if((rand()%100+1) < NEWCUSTOMERPROB){
            CoffeeShop.join_line(randomname(names));
        }
        //Customer at end of line leaves
        if((rand()%100+1) < LASTCUSTOMERLEAVEPROB){
            CoffeeShop.leave_end_of_line();
        }
        //Customer at any point of line leaves
        if((rand()%100+1) < ANYCUSTOMERLEAVEPROB){
            CoffeeShop.leave_line(rand()%CoffeeShop.getListSize());
        }
        if((rand()%100+1) < VIPPROB){
            CoffeeShop.VIP_joins(randomname(names));
        }

    CoffeeShop.print();
    }
    return 0;
}

string randomname(vector <string> input){
    return input[rand()%input.size()];
}
