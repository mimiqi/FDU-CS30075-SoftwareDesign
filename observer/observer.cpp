#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

class Observer;


class Subject{
    private:

    protected:

    public:
        virtual ~Subject() = default;
        virtual void attach(Observer* observer) = 0;
        virtual void detach(Observer* observer) = 0;
        virtual void notify() = 0;
        virtual int getState() const = 0;
};

class Observer{
    private:

    protected:

    public:
        virtual ~Observer() = default;
        virtual void update(Subject* subject) = 0;
};

class ConcreteSubject : public Subject{
    private:
        int state;
        std::vector<Observer*> observers;

    protected:

    public:
        ConcreteSubject(){
            std::cout << "ConcreteSubject created" << std::endl;
        }

        ~ConcreteSubject(){
            std::cout << "ConcreteSubject destroyed" << std::endl;
        }

        void attach(Observer* observer) override{
            observers.push_back(observer);
        }

        void detach(Observer* observer) override{
            observers.erase(
                std::remove_if(observers.begin(), observers.end(),
                    [observer](Observer* ptr) {
                        return ptr == observer;
                    }),
                observers.end()
            );
        }

        void notify() override{
            for(auto& observer : observers){
                observer->update(this);
            }
        }

        void setState(int state){
            this->state = state;
        }

        int getState() const override {
            return state;
        }
};

class ConcreteObserver : public Observer{
    private:
        int state;

    protected:

    public:
        ConcreteObserver(){
            std::cout << "ConcreteObserver created" << std::endl;
        }

        ~ConcreteObserver(){
            std::cout << "ConcreteObserver destroyed" << std::endl;
        }

        void update(Subject* subject) override{
            if (auto* concreteSubject = dynamic_cast<ConcreteSubject*>(subject)) {
                this->state = concreteSubject->getState();
                std::cout << "ConcreteObserver updated: " << this->state << std::endl;
            }
        }


};

int main(){

    ConcreteSubject subject;
    ConcreteObserver observer;
    subject.attach(&observer);
    subject.setState(1);
    subject.notify();
    subject.setState(2);
    subject.notify();
    subject.detach(&observer);

    return 0;
}