#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// -------------------- Person --------------------
class Person {
    string _name;
public:
    explicit Person(const string& name) : _name(name) {
        cout << "Person " << _name << " created" << endl;
    }

    ~Person() {
        cout << "Person " << _name << " destroyed" << endl;
    }

    const string& getName() const {
        return _name;
    }
};

// -------------------- Position --------------------
class Position {
    string _name;
    unsigned int _salary;
public:
    Position(const string& name, unsigned int salary)
        : _name(name), _salary(salary) {
        cout << "Position " << _name << " (" << _salary << ") created" << endl;
    }

    ~Position() {
        cout << "Position " << _name << " destroyed" << endl;
    }

    const string& getPositionName() const {
        return _name;
    }

    unsigned int getSalary() const {
        return _salary;
    }
};

// -------------------- Employee --------------------
class Employee {
    const Person& _who;
    const Position& _what;
public:
    Employee(const Person& who, const Position& what)
        : _who(who), _what(what) {
        cout << "Employee " << _who.getName()
            << " takes position " << _what.getPositionName() << endl;
    }

    ~Employee() {
        cout << "Employee " << _who.getName()
            << " fired from " << _what.getPositionName() << endl;
    }

    const Person& who() const {
        return _who;
    }

    const Position& what() const {
        return _what;
    }
};

// -------------------- Employer --------------------
class Employer {
private:
    struct Staff {
        Employee* _pemployee;
        bool _free;
    };

    size_t _volume;      // number of positions
    Staff* _office;      // array of positions
    const Person& _me;   // boss

public:
    Employer(const Person& boss, size_t volume)
        : _volume(volume),
        _office(new Staff[_volume]),
        _me(boss) {
        cout << "Employer " << _me.getName()
            << " with " << _volume << " positions was created" << endl;

        for (size_t i = 0; i < _volume; ++i) {
            _office[i]._free = true;
            _office[i]._pemployee = nullptr;
        }
    }

    ~Employer() {
        cout << "Employer " << _me.getName()
            << " with " << _volume << " positions was deleted" << endl;

        // delete all employees if they still exist
        for (size_t i = 0; i < _volume; ++i) {
            if (!_office[i]._free && _office[i]._pemployee != nullptr) {
                delete _office[i]._pemployee;
            }
        }

        delete[] _office;
    }

    size_t volume() const {
        return _volume;
    }

    // ---------- hire() with reaction "no vacant positions" ----------
    void hire(const Person& person, const Position& position) {
        cout << "Trying to hire " << person.getName()
            << " to position " << position.getPositionName() << endl;

        // Find free position
        size_t i = 0;
        while ((i < _volume) && (!_office[i]._free)) {
            ++i;
        }

        // If no free positions
        if (i == _volume) {
            cout << "No vacant positions for "
                << person.getName() << endl;
            return;
        }

        // Create employee only if there is a free place
        Employee* pemployee = new Employee(person, position);
        _office[i]._free = false;
        _office[i]._pemployee = pemployee;

        cout << "Hired " << person.getName()
            << " on position index " << i << endl;
    }

    // ---------- fire(i) ----------
    void fire(const size_t i) {
        cout << "Trying to fire position " << i << endl;

        // Check index
        if (i >= _volume) {
            cout << "Invalid position index: " << i << endl;
            return;
        }

        // Check if already free
        if (_office[i]._free) {
            cout << "Position " << i << " is already vacant." << endl;
            return;
        }

        // Real firing
        delete _office[i]._pemployee;
        _office[i]._pemployee = nullptr;
        _office[i]._free = true;

        cout << "Position " << i << " is now vacant." << endl;
    }

    // ---------- helper for screenshots ----------
    void printStaff() const {
        cout << "\n=== STAFF STATE ===" << endl;
        for (size_t i = 0; i < _volume; ++i) {
            cout << "Position index " << i << ": ";
            if (_office[i]._free || _office[i]._pemployee == nullptr) {
                cout << "VACANT" << endl;
            }
            else {
                cout << "OCCUPIED by "
                    << _office[i]._pemployee->who().getName()
                    << " (" << _office[i]._pemployee->what().getPositionName()
                    << ", salary "
                    << _office[i]._pemployee->what().getSalary()
                    << ")" << endl;
            }
        }
        cout << "===================\n" << endl;
    }
};

// -------------------- Calc + exe() --------------------
struct Calc {
    double _a; // accumulator
    double _x; // second operand
};

// Utility to execute operation
double exe(Calc& c, char op) {
    switch (op) {
    case '+':   // addition
        return c._a += c._x;

    case '-':   // subtraction
        return c._a -= c._x;

    case '*':   // multiplication
        return c._a *= c._x;

    case '/':   // division
        if (c._x == 0.0) {
            cerr << "Division by zero!" << endl;
            return c._a;
        }
        return c._a /= c._x;

    case '^':   // power
        return c._a = pow(c._a, c._x);

    case '%':   // modulo (for doubles)
        if (c._x == 0.0) {
            cerr << "Modulo by zero!" << endl;
            return c._a;
        }
        return c._a = fmod(c._a, c._x);

    case '=':   // a = x
        return c._a = c._x;

    case 'n':   // change x (for example, change sign)
        c._x = -c._x;
        return c._a;

    default:
        cerr << "Unknown operation: " << op << endl;
        return c._a;
    }
}

// -------------------- main() --------------------
int main() {
    // ===== PART 1: Employer::hire() & fire() =====
    cout << "=== DEMO: Employer::hire() and Employer::fire() ===\n\n";

    Person boss("Boss");
    Person ivan("Ivan Ivanov");
    Person petro("Petro Petrov");
    Person stepan("Stepan Stepanych");

    Position dev("Developer", 30000);
    Position qa("QA Engineer", 25000);

    Employer firm(boss, 2);
    firm.printStaff(); // screen #1

    cout << "\nHiring two employees...\n";
    firm.hire(ivan, dev);
    firm.hire(petro, qa);
    firm.printStaff(); // screen #2

    cout << "\nTrying to hire third employee (no vacancies)...\n";
    firm.hire(stepan, dev);
    firm.printStaff(); // screen #3

    cout << "\nFiring employee at position 0...\n";
    firm.fire(0);
    firm.printStaff(); // screen #4

    cout << "\nTrying to fire already vacant position 0...\n";
    firm.fire(0);      // screen #5 (console output)

    // ===== PART 2: Calc + exe() =====
    cout << "\n=== DEMO: Calc + exe() ===\n\n";

    Calc c{ 10.0, 3.0 };
    cout << "Initial: a = " << c._a << ", x = " << c._x << endl;

    exe(c, '+');
    cout << "After '+': a = " << c._a << ", x = " << c._x << endl;

    exe(c, '^');
    cout << "After '^': a = " << c._a << ", x = " << c._x << endl;

    exe(c, '%');
    cout << "After '%': a = " << c._a << ", x = " << c._x << endl;

    exe(c, '=');
    cout << "After '=': a = " << c._a << ", x = " << c._x << endl;

    exe(c, 'n');
    cout << "After 'n': a = " << c._a << ", x = " << c._x << endl;

    return 0;
}
