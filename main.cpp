

#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/timer/timer.hpp>

typedef enum : int{
    STLQueue   = 1,
    ListQueue  = 2,
    ArrayQueue = 3,
} templateType;

typedef enum {
    TDDIntType,
    TDDDoubleType,
    TDDClassType,
} testType;

#pragma mark - CONSTANT

static const int kEDGE_LIMIT = 50;
static const unsigned long kSIZE = 1000300;

#pragma mark - Class for testing in container

class Object {
    
    std::string title;
    std::string description;
    unsigned long key;
    
public:
    
    Object() {
        
        title = " ";
        description = " ";
        key = 1;
    }
    
    Object(std::string title, std::string description, unsigned long key) {
        
        this -> title = title;
        this -> description = description;
        this -> key = key;
    }
    
    void setTitle(std::string title) {
        
        this -> title = title;
    }
    
    void setDescription(std::string description) {
        
        this -> description = description;
    }
    
    void setKey(int key) {
        
        this -> key = key;
    }
    
    std::string getTitle() {
        
        return title;
    }
    
    std::string getDescription() {
     
        return description;
    }
    
    unsigned long getKey() {
        
        return key;
    }
    
    
};

#pragma mark - Queue with List

template <class T>
class Queue_List {
    
    struct Element {
        
        T data;
        Element* next_elem;
        
        Element(T in_data) {
            
            data = in_data;
            next_elem = 0;
        }
    };
    
    Element* head;
    Element* tail;
    
public:
    
    Queue_List() {
        
        head = 0;
        tail = 0;
    }
    
    bool empty() {
        
        return head == 0;
    }
    
    T front() {
        
        if (empty()) {
            return NULL;
        }
        
        return head -> data;
        
    }
    
    void pop() {
        
        if (empty()) {
            return;
        }
        
        boost::shared_ptr<Element> tmp_element(new Element(head->data));
        
        tmp_element -> next_elem = head -> next_elem;
        head = tmp_element -> next_elem;
        
        if (head == NULL) {
            
            tail = NULL;
        }
        
    }
    
    void push(T data) {
        
        Element* tmp_tail = tail;
        tail = new Element(data);
        
        if (!head) {
            
            head = tail;
        }
        else {
            
            tmp_tail->next_elem = tail;
        }
    }
    
};

#pragma mark - Queue with Array

template <class T>
class Queue_Array {
    
    T ar_data[kSIZE];
    int first_ptr;
    int last_ptr;

    
public:
    
    Queue_Array() {
        
        first_ptr = last_ptr = 1;
        
    }
    
    bool empty() {
        
        return first_ptr == last_ptr;
    }
    
    void push(T data) {
        
        if (last_ptr % (kSIZE - 1) + 1 == first_ptr) {
            
            return;//Out of range
        }
        
        ar_data[last_ptr] = data;
        last_ptr = (last_ptr % (kSIZE - 1)) + 1;
    }
    
    void pop() {
        
        if (empty()) {
            
            return;
        }
        
        first_ptr = (first_ptr % (kSIZE - 1)) + 1;
    }
    
    T front() {
        
        return ar_data[first_ptr];
    }
};

#pragma mark - Testing

class TDD {
public:
    virtual double fillContainerWithRandomObjectsTest() = 0;
    virtual double deleteObjectsFromContainerTest()     = 0;
    virtual std::pair<double, double> driveAllTestsToContainer() = 0;
    
    
};

template <class TYPE>
class TDDInteger : public TDD {
    
    TYPE container;
    
public:
    
    double fillContainerWithRandomObjectsTest() {
        
        boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
        
        for (int i = 1; i < kSIZE; i++) {
            
            container.push(arc4random_uniform(51) + kEDGE_LIMIT);
        }
        
        boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
        return sec.count();
        
    }
    
    double deleteObjectsFromContainerTest() {
        
        boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
        
        while (!container.empty()) {
            container.pop();
        }
        
        boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
        return sec.count();
        
    }
    
    std::pair<double, double> driveAllTestsToContainer() {
        
        double containerFills_timer  = fillContainerWithRandomObjectsTest();
        double containerClears_timer = deleteObjectsFromContainerTest();
        
        return std::make_pair(containerFills_timer, containerClears_timer);
        
    }
    
};

template <class TYPE>
class TDDDouble : public TDD {
    
    TYPE container;
    
public:
    
    double fillContainerWithRandomObjectsTest() {
        
        boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
        
        for (int i = 1; i < kSIZE; i++) {
            
            container.push((double)arc4random_uniform(51) + kEDGE_LIMIT);
        }
        
        boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
        return sec.count();
        
    }
    
    double deleteObjectsFromContainerTest() {
        
        boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
        
        while (!container.empty()) {
            container.pop();
        }
        
        boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
        return sec.count();
        
    }
    
    std::pair<double, double> driveAllTestsToContainer() {
        
        double containerFills_timer  = fillContainerWithRandomObjectsTest();
        double containerClears_timer = deleteObjectsFromContainerTest();
        
        return std::make_pair(containerFills_timer, containerClears_timer);
        
    }
    
};

template <class TYPE>
class TDDClass : public TDD {
    
    TYPE container;
    
public:
    
    double fillContainerWithRandomObjectsTest() {
        
        std::vector<std::weak_ptr<Object>> objects; {
        
        for (int i = 1; i <= kSIZE; i++) {
          
            std::shared_ptr<Object> newSharedObject(new Object ("Object", "New Object", arc4random_uniform(128513)));
            std::weak_ptr<Object> newWeakObject(newSharedObject);
            objects.push_back(newWeakObject);
            
            
        }
        }
        
        std::random_shuffle(objects.begin(), objects.end());
        
        boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
        
        for (int i = 1; i < kSIZE; i++) {
            
            container.push(objects[i]);
            
        }
        
        boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
        return sec.count();
    }
    
    double deleteObjectsFromContainerTest() {
        
        boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

        while (!container.empty()) {
            
            container.pop();
        }
        
        boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
        return sec.count();
    }
    
    std::pair<double, double> driveAllTestsToContainer() {
        
        double fillClassObjects = fillContainerWithRandomObjectsTest();
        double deleteObjectsFromContainer = deleteObjectsFromContainerTest();
        
        return std::make_pair(fillClassObjects, deleteObjectsFromContainer);
    }
    
};

#pragma mark - Class Factory Pattern

class Factory {
    
public:
    virtual TDD* createTestWithTypes(templateType type) = 0;
};

class TDDIntegerFactory : public Factory {
public:
    TDD* createTestWithTypes(templateType type) {
        
        switch (type) {
            case STLQueue:   return new TDDInteger<std::queue<int>>;
            case ListQueue:  return new TDDInteger<Queue_List<int>>;
            case ArrayQueue: return new TDDInteger<Queue_Array<int>>;
            default: return NULL;
        }
    }
};

class TDDDoubleFactory : public Factory {
public:
    
    TDD* createTestWithTypes(templateType type) {
        
        switch (type) {
            case STLQueue:   return new TDDDouble<std::queue<double>>;
            case ListQueue:  return new TDDDouble<Queue_List<double>>;
            case ArrayQueue: return new TDDDouble<Queue_Array<double>>;
            default: return NULL;
        }
    }
    
};

class TDDClassFactory : public Factory {
public:
    
    TDD* createTestWithTypes(templateType type) {
        
        switch (type) {
            case STLQueue:   return new TDDClass<std::queue<std::weak_ptr<Object>>>;
            case ListQueue:  return new TDDClass<Queue_List<std::weak_ptr<Object>>>;
            case ArrayQueue: return new TDDClass<Queue_Array<std::weak_ptr<Object>>>;
            default: return NULL;
        }
    }
};

void launchTestWithType(testType type, std::ofstream& out) {
    
    std::vector<TDD*> testObj;
    Factory* factory;
    
    if (type == TDDClassType) {
        
        out << "--- Testing containers with class objects ---" << std::endl;
        
        factory = new TDDClassFactory;
        for (int i = 1; i <= 3; i++) {
            testObj.push_back(factory -> createTestWithTypes((templateType)i));
        }
    } else if (type == TDDIntType) {
        
        out << "--- Testing containers with int values ---" << std::endl;
        
        factory = new TDDIntegerFactory;
        for (int i = 1; i <= 3; i++) {
            testObj.push_back(factory -> createTestWithTypes((templateType)i));
        }
    } else if (type == TDDDoubleType) {
        
        out << "--- Testing containers with dauble values ---" << std::endl;
        
        factory = new TDDDoubleFactory;
        for (int i = 1; i <= 3; i++) {
            testObj.push_back(factory -> createTestWithTypes((templateType)i));
        }
    }
    
    if (!testObj.empty()) {
        
        for (int i = 0; i < testObj.size(); i++) {
            
            std::pair<double, double> curResult = testObj[i]->driveAllTestsToContainer();
            switch ((templateType)i + 1) {
                case STLQueue: out << "Время заполнения STL очереди:\t " << curResult.first << " Время удаления из STL очереди:\t " << curResult.second << std::endl;
                    break;
                case ArrayQueue: out << "Время заполнения ArrayQueue очереди:\t " << curResult.first << " Время удаления из ArrayQueue очереди:\t " << curResult.second << std::endl;
                    break;
                case ListQueue: out << "Время заполнения ListQueue очереди:\t " << curResult.first << " Время удаления из ListQueue очереди:\t " << curResult.second << std::endl;
                    break;
                default:
                    break;
            }
        }
    }
}


#pragma mark - Main

int main() {
    
    std::ifstream in ("input.txt");
    std::ofstream out ("/Users/PSKuznetsov/Documents/Projects/Xcode/Queue_15/Queue_15/output.txt");
    
    
    launchTestWithType(TDDDoubleType, out);
    launchTestWithType(TDDClassType, out);
    launchTestWithType(TDDIntType, out);
    
    return 0;
}
