#include <iostream>
#include <vector>
#include <memory>

template<typename T>
class Iterator{
    public:
        virtual ~Iterator() = default;
        virtual bool hasNext() = 0;
        virtual T next() = 0;//返回迭代器当前元素并迭代到下一个
};

template<typename T>
class Aggregate{
    public:
        virtual ~Aggregate() = default;
        virtual std::unique_ptr<Iterator<T>> createIterator() = 0;
        virtual int size() const = 0;
        virtual T get(int index) const = 0;
};

template<typename T>
class CustomCollection : public Aggregate<T>{
    private:
        std::vector<T> items;

    protected:

    public:

        CustomCollection(){
            std::cout << "CustomCollection created" << std::endl;
        }

        ~CustomCollection(){
            std::cout << "CustomCollection destroyed" << std::endl;
        }

        void add(const T &item){
            items.push_back(item);
        }

        int size() const override{
            return items.size();
        }

        T get(int index) const override{
            if(index >= 0 && index < items.size()){
                return items[index];
            }
            throw std::out_of_range("Index out of range");
        }

        class ForwardIterator : public Iterator<T>{//内部类
            private:
                const CustomCollection<T> &collection;
                int currentIndex;
        
            protected:
        
            public:
                //列表初始化
                ForwardIterator(const CustomCollection<T> &coll) : collection(coll), currentIndex(0){}
        
                bool hasNext() override{
                    return currentIndex < collection.size();
                }
        
                T next() override{
                    if(!hasNext()){
                        throw std::out_of_range("No more elements");
                    }
                    return collection.get(currentIndex++);
                }
        };

        std::unique_ptr<Iterator<T>> createIterator() override{
            return std::make_unique<ForwardIterator>(*this);
        }
};

int main(){
    CustomCollection<int> collection;
    collection.add(1);
    collection.add(2);
    collection.add(3);
    collection.add(4);
    collection.add(5);

    std::cout << "Factory Pattern Example:" << std::endl;
    auto iterator = collection.createIterator();
    while(iterator->hasNext()){
        std::cout << iterator->next() << " ";
    }
    std::cout << std::endl;

    std::cout << "Outer class use inner class:" << std::endl;
    CustomCollection<int>::ForwardIterator forwardIterator(collection);
    while(forwardIterator.hasNext()){
        std::cout << forwardIterator.next() << " ";
    }
    std::cout << std::endl;

    return 0;
}
