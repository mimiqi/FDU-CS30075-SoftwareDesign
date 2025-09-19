# 迭代器模式 (Iterator Pattern)

## 1. 模式定义

迭代器模式是一种行为型设计模式，它提供一种方法顺序访问一个聚合对象中各个元素，而又不暴露该对象的内部表示。迭代器模式将遍历逻辑从聚合对象中分离出来，使得聚合对象和遍历算法可以独立变化。

## 2. 模式结构

### 2.1 核心组件

```cpp
// 抽象迭代器接口
template<typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual bool hasNext() = 0;      // 是否有下一个元素
    virtual T next() = 0;            // 返回当前元素并移动到下一个
};

// 抽象聚合接口
template<typename T>
class Aggregate {
public:
    virtual ~Aggregate() = default;
    virtual std::unique_ptr<Iterator<T>> createIterator() = 0;  // 创建迭代器
    virtual int size() const = 0;    // 获取大小（const方法）
    virtual T get(int index) const = 0;  // 根据索引获取元素（const方法）
};
```

### 2.2 具体实现

```cpp
// 具体聚合类
template<typename T>
class CustomCollection : public Aggregate<T> {
private:
    std::vector<T> items;            // 存储数据的容器
    
public:
    // 添加元素
    void add(const T &item) {
        items.push_back(item);
    }
    
    // 获取大小
    int size() const override {
        return items.size();
    }
    
    // 根据索引获取元素
    T get(int index) const override {
        if(index >= 0 && index < items.size()) {
            return items[index];
        }
        throw std::out_of_range("Index out of range");
    }
};
```

## 3. 内部类详解

### 3.1 内部类定义

```cpp
template<typename T>
class CustomCollection : public Aggregate<T> {
public:
    // 内部类：ForwardIterator
    class ForwardIterator : public Iterator<T> {
    private:
        const CustomCollection<T> &collection;  // 持有外部类const引用
        int currentIndex;                       // 当前索引
        
    public:
        // 构造函数：列表初始化
        ForwardIterator(const CustomCollection<T> &coll) 
            : collection(coll), currentIndex(0) {}
        
        // 是否有下一个元素
        bool hasNext() override {
            return currentIndex < collection.size();
        }
        
        // 获取下一个元素
        T next() override {
            if(!hasNext()) {
                throw std::out_of_range("No more elements");
            }
            return collection.get(currentIndex++);
        }
    };
};
```

### 3.2 内部类特点

#### 3.2.1 访问权限
- **特殊访问权**：内部类可以访问外部类的私有成员
- **封装性**：迭代器与集合紧密相关，封装在集合内部
- **独立性**：内部类可以独立创建和使用

#### 3.2.2 使用方式
```cpp
// 方式1：通过工厂方法（推荐）
auto iterator = collection.createIterator();

// 方式2：直接创建内部类对象
CustomCollection<int>::ForwardIterator forwardIterator(collection);
```

### 3.3 内部类优势

1. **逻辑关联**：迭代器与集合逻辑紧密相关
2. **访问权限**：可以直接访问外部类的私有成员
3. **封装性**：迭代器实现细节对外部隐藏
4. **命名空间**：避免全局命名空间污染

## 4. make_unique 详解

### 4.1 基本概念

```cpp
std::unique_ptr<Iterator<T>> createIterator() override {
    return std::make_unique<ForwardIterator>(*this);
    //     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //     C++14引入的智能指针工厂函数
}
```

### 4.2 make_unique 工作原理

#### 4.2.1 语法结构
```cpp
std::make_unique<ForwardIterator>(*this)
//  ^^^^^^^^^^^^  ^^^^^^^^^^^^^^^  ^^^^^^
//  工厂函数      类型参数          构造函数参数
```

#### 4.2.2 等价实现
```cpp
// make_unique方式（推荐）
return std::make_unique<ForwardIterator>(*this);

// 等价于传统方式
return std::unique_ptr<Iterator<T>>(new ForwardIterator(*this));
```

### 4.3 make_unique 优势

#### 4.3.1 异常安全
```cpp
// 不安全的传统方式
void unsafeFunction() {
    process(std::unique_ptr<Iterator<T>>(new ForwardIterator(*this)),
            std::unique_ptr<Iterator<T>>(new ReverseIterator(*this)));
    // 如果第二个new抛出异常，第一个对象会内存泄漏
}

// 安全的make_unique方式
void safeFunction() {
    process(std::make_unique<ForwardIterator>(*this),
            std::make_unique<ReverseIterator>(*this));
    // 异常安全，不会内存泄漏
}
```

#### 4.3.2 代码简洁
```cpp
// 传统方式
std::unique_ptr<Iterator<T>> createIterator() override {
    return std::unique_ptr<Iterator<T>>(new ForwardIterator(*this));
}

// make_unique方式
std::unique_ptr<Iterator<T>> createIterator() override {
    return std::make_unique<ForwardIterator>(*this);
}
```

### 4.4 C++版本要求

- **std::unique_ptr**：C++11
- **std::make_unique**：C++14
- **override**：C++11
- **auto**：C++11

## 5. 工厂模式应用

### 5.1 工厂方法实现

```cpp
std::unique_ptr<Iterator<T>> createIterator() override {
    return std::make_unique<ForwardIterator>(*this);
}
```

### 5.2 工厂模式优势

1. **封装创建逻辑**：客户端不需要知道具体创建什么类型的迭代器
2. **多态支持**：返回基类指针，支持多态
3. **扩展性**：可以轻松添加新的迭代器类型
4. **内存管理**：使用智能指针自动管理内存

## 6. 使用示例

### 6.1 基本使用

```cpp
int main() {
    // 创建集合
    CustomCollection<int> collection;
    collection.add(1);
    collection.add(2);
    collection.add(3);
    collection.add(4);
    collection.add(5);

    // 使用工厂方法创建迭代器
    std::cout << "Factory Pattern Example:" << std::endl;
    auto iterator = collection.createIterator();
    while(iterator->hasNext()) {
        std::cout << iterator->next() << " ";
    }
    std::cout << std::endl;

    // 直接创建内部类对象
    std::cout << "Outer class use inner class:" << std::endl;
    CustomCollection<int>::ForwardIterator forwardIterator(collection);
    while(forwardIterator.hasNext()) {
        std::cout << forwardIterator.next() << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### 6.2 输出结果

```
CustomCollection created
Factory Pattern Example:
1 2 3 4 5 
Outer class use inner class:
1 2 3 4 5 
CustomCollection destroyed
```

## 7. 设计模式优势

### 7.1 解耦
- 遍历逻辑与集合实现分离
- 支持多种遍历方式
- 易于维护和扩展

### 7.2 统一接口
- 不同集合使用相同的遍历方式
- 支持多态调用
- 代码复用性高

### 7.3 内存安全
- 使用智能指针管理内存
- 异常安全保证
- 自动资源清理

## 8. 注意事项

### 8.1 内部类模板声明
```cpp
// 错误：重复模板声明
template<typename T>
class CustomCollection {
    template<typename T>  // ❌ 不需要重复声明
    class ForwardIterator : public Iterator<T> {
        // ...
    };
};

// 正确：直接使用外部类模板参数
template<typename T>
class CustomCollection {
    class ForwardIterator : public Iterator<T> {  // ✅ 直接使用T
        // ...
    };
};
```

### 8.2 const方法的重要性
```cpp
// 内部类使用const引用，需要const方法支持
class ForwardIterator {
    const CustomCollection<T> &collection;  // const引用
    
    bool hasNext() override {
        return currentIndex < collection.size();  // 需要const方法
    }
    
    T next() override {
        return collection.get(currentIndex++);  // 需要const方法
    }
};
```

### 8.3 异常处理
```cpp
T next() override {
    if(!hasNext()) {
        throw std::out_of_range("No more elements");
    }
    return collection.get(currentIndex++);
}
```

## 9. 总结

这个迭代器模式实现展示了：

1. **内部类**：提供特殊访问权限的独立类，实现迭代器与集合的紧密关联
2. **make_unique**：C++14的智能指针工厂函数，提供异常安全的内存管理
3. **工厂模式**：封装对象创建逻辑，支持多态和扩展
4. **模板编程**：支持泛型，提高代码复用性
5. **现代C++特性**：智能指针、auto关键字、override标识符
6. **const正确性**：使用const引用和const方法，确保类型安全和语义清晰

### 9.1 关键修复点

1. **移除模板参数**：`ForwardIterator`是内部类，不是模板类
2. **添加const方法**：`size()`和`get()`方法应该是const方法
3. **使用const引用**：迭代器使用const引用，确保不修改集合
4. **参数类型优化**：`add()`方法使用`const T &item`参数

这是一个完整的、现代C++风格的迭代器模式实现，体现了良好的软件设计原则！
