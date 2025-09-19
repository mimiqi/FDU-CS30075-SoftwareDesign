#include <iostream>
#include <string>
#include <vector>
#include <memory>

// 前向声明
class Lion;
class Tiger;
// 抽象访问者基类
class AnimalVisitor {
public:
    virtual ~AnimalVisitor() = default;
    virtual void visit(const Lion& lion) = 0;
    virtual void visit(const Tiger& tiger) = 0;
};

// 抽象元素基类
class Animal {
public:
    virtual ~Animal() = default;
    virtual void accept(AnimalVisitor& visitor) const = 0;
};

// 具体元素类
class Lion : public Animal {
private:
    std::string name;

public:
    Lion(const std::string& name) : name(name) {
        std::cout << "Lion " << name << " created" << std::endl;
    }

    std::string getName() const {
        return name;
    }

    void accept(AnimalVisitor& visitor) const override {
        visitor.visit(*this);
    }

    ~Lion() {
        std::cout << "Lion " << name << " destroyed" << std::endl;
    }
};

class Tiger : public Animal{
    private:
        std::string name;
    
    protected:

    public:
        Tiger(const std::string& name){
            this->name = name;
            std::cout << "Tiger " << name << " created" << std::endl;
        }

        ~Tiger(){
            std::cout << "Tiger " << name << " destroyed" << std::endl;
        }

        std::string getName() const {
            return name;
        }

        void accept(AnimalVisitor& visitor) const override{
            visitor.visit(*this);
        }
};

// 具体访问者类
class FeedingVisitor : public AnimalVisitor {
public:
    FeedingVisitor() {
        std::cout << "FeedingVisitor created" << std::endl;
    }

    ~FeedingVisitor() {
        std::cout << "FeedingVisitor destroyed" << std::endl;
    }

    void visit(const Lion& lion) override {
        std::cout << "Feeding to Lion: " << lion.getName() << std::endl;
    }

    void visit(const Tiger& tiger) override {
        std::cout << "Feeding to Tiger: " << tiger.getName() << std::endl;
    }
};

// 对象结构类
class Zoo {
private:
    std::vector<std::unique_ptr<Animal>> animals;

public:
    Zoo() {
        std::cout << "Zoo created" << std::endl;
    }

    ~Zoo() {
        std::cout << "Zoo destroyed" << std::endl;
    }

    void addAnimal(std::unique_ptr<Animal> animal) {
        animals.push_back(std::move(animal));
    }

    void accept(AnimalVisitor& visitor) {
        std::cout << "---START---" << std::endl;
        for (const auto& animal : animals) {
            animal->accept(visitor);
        }
        std::cout << "----END----" << std::endl;
    }
};

int main() {
    // 使用智能指针管理内存
    Zoo zoo;
    zoo.addAnimal(std::make_unique<Lion>("Simba"));
    zoo.addAnimal(std::make_unique<Lion>("Mufasa"));
    zoo.addAnimal(std::make_unique<Tiger>("Shere Khan"));
    zoo.addAnimal(std::make_unique<Tiger>("Sher Khan"));
    
    FeedingVisitor feedingVisitor;
    zoo.accept(feedingVisitor);
    
    return 0;
}