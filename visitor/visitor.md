# 访问者模式 (Visitor Pattern)

## 1. 模式定义

访问者模式是一种行为型设计模式，它允许你在不改变对象结构的前提下，定义作用于这些对象的新操作。访问者模式将数据结构与数据操作分离，使得操作可以独立变化。

## 2. 模式结构

### 核心组件

- **Visitor（访问者）**：定义访问具体元素的操作接口
- **ConcreteVisitor（具体访问者）**：实现访问者接口，定义对具体元素的操作
- **Element（元素）**：定义一个accept方法，接受访问者
- **ConcreteElement（具体元素）**：实现accept方法，调用访问者的visit方法
- **ObjectStructure（对象结构）**：能枚举它的元素，提供高层接口让访问者访问元素

## 3. 模式特点

### 优点
- ✅ **易于添加新操作**：新增访问者无需修改现有代码
- ✅ **集中相关操作**：相关操作集中在访问者中
- ✅ **易于访问复杂对象结构**：可以访问对象结构中的各个元素

### 缺点
- ❌ **增加新元素困难**：每增加一个元素类，都要修改访问者接口
- ❌ **破坏封装**：访问者需要访问元素的内部状态
- ❌ **依赖具体类**：访问者依赖具体元素类

## 4. 适用场景

- 对象结构稳定，但经常需要在此结构上定义新操作
- 需要对一个对象结构中的对象进行很多不同且不相关的操作
- 需要避免"污染"这些类的代码

## 5. 代码分析

### 5.1 抽象访问者 (AnimalVisitor)

```cpp
class AnimalVisitor {
public:
    virtual ~AnimalVisitor() = default;
    virtual void visit(const Lion& lion) = 0;    // 访问狮子
    virtual void visit(const Tiger& tiger) = 0;  // 访问老虎
};
```

**作用**：
- 定义访问不同类型动物的接口
- 使用纯虚函数确保子类必须实现所有访问方法
- 虚析构函数确保多态析构正确

### 5.2 抽象元素 (Animal)

```cpp
class Animal {
public:
    virtual ~Animal() = default;
    virtual void accept(AnimalVisitor& visitor) const = 0;
};
```

**作用**：
- 定义接受访问者的接口
- `const`方法表示不修改对象状态
- 为所有动物提供统一的访问接口

### 5.3 具体元素 (Lion, Tiger)

```cpp
class Lion : public Animal {
private:
    std::string name;
public:
    void accept(AnimalVisitor& visitor) const override {
        visitor.visit(*this);  // 调用访问者的visit方法
    }
};
```

**关键点**：
- `accept`方法调用访问者的`visit`方法
- `*this`传递当前对象给访问者
- 使用`const`确保不修改对象状态

### 5.4 具体访问者 (FeedingVisitor)

```cpp
class FeedingVisitor : public AnimalVisitor {
public:
    void visit(const Lion& lion) override {
        std::cout << "Feeding to Lion: " << lion.getName() << std::endl;
    }
    
    void visit(const Tiger& tiger) override {
        std::cout << "Feeding to Tiger: " << tiger.getName() << std::endl;
    }
};
```

**作用**：
- 实现具体的访问操作（喂食）
- 针对不同动物类型有不同的处理逻辑
- 集中管理相关操作

### 5.5 对象结构 (Zoo)

```cpp
class Zoo {
private:
    std::vector<std::unique_ptr<Animal>> animals;
public:
    void accept(AnimalVisitor& visitor) {
        for (const auto& animal : animals) {
            animal->accept(visitor);  // 让每个动物接受访问
        }
    }
};
```

**作用**：
- 管理动物集合
- 提供统一的访问接口
- 使用智能指针管理内存

## 6. 执行流程

```
1. 创建访问者对象 (FeedingVisitor)
2. 调用Zoo的accept方法
3. Zoo遍历所有动物
4. 每个动物调用自己的accept方法
5. 动物的accept方法调用访问者的visit方法
6. 访问者执行具体操作
```

## 7. 模式优势体现

### 7.1 易于扩展新操作

```cpp
// 新增医疗访问者，无需修改现有代码
class MedicalVisitor : public AnimalVisitor {
public:
    void visit(const Lion& lion) override {
        std::cout << "Medical check for Lion: " << lion.getName() << std::endl;
    }
    
    void visit(const Tiger& tiger) override {
        std::cout << "Medical check for Tiger: " << tiger.getName() << std::endl;
    }
};
```

### 7.2 操作集中管理

所有喂食相关的逻辑都集中在`FeedingVisitor`中，便于维护和修改。

## 8. 注意事项

### 8.1 const正确性

#### 8.1.1 基本概念

在C++中，**在const成员函数内部，`*this`的类型是`const T&`**

```cpp
class MyClass {
public:
    void normalMethod() {
        // this 的类型是 MyClass*
        // *this 的类型是 MyClass&
    }
    
    void constMethod() const {
        // this 的类型是 const MyClass*
        // *this 的类型是 const MyClass&
    }
};
```

#### 8.1.2 在访问者模式中的应用

```cpp
class Lion : public Animal {
public:
    void accept(AnimalVisitor& visitor) const override {
        // 因为accept是const方法
        // *this 的类型是 const Lion&
        visitor.visit(*this);  // 传递const Lion&给visit方法
    }
};
```

#### 8.1.3 类型匹配验证

```cpp
class AnimalVisitor {
public:
    virtual void visit(const Lion& lion) = 0;  // 参数类型：const Lion&
};

// 在Lion::accept中调用
visitor.visit(*this);  // 传递：const Lion&
// 类型完全匹配！✅
```

#### 8.1.4 类型不匹配的错误示例

```cpp
// 错误的访问者接口
class WrongVisitor {
public:
    virtual void visit(Lion& lion) = 0;  // 参数类型：Lion&（非const）
};

// 在const方法中调用
void accept(WrongVisitor& visitor) const override {
    visitor.visit(*this);  // 错误！const Lion& 不能转换为 Lion&
    // 编译错误：binding reference of type 'Lion&' to 'const Lion' discards qualifiers
}
```

#### 8.1.5 完整的类型推导过程

```cpp
class Lion : public Animal {
private:
    std::string name;
public:
    // 1. accept是const成员函数
    void accept(AnimalVisitor& visitor) const override {
        // 2. 在const成员函数中，this的类型是const Lion*
        // 3. 因此*this的类型是const Lion&
        // 4. visitor.visit(const Lion&) 调用成功
        visitor.visit(*this);
    }
};
```

#### 8.1.6 关键要点总结

- **const成员函数中**：`*this` 的类型是 `const T&`
- **非const成员函数中**：`*this` 的类型是 `T&`
- **类型匹配**：访问者方法的参数类型必须与 `*this` 的类型匹配
- **编译错误**：类型不匹配会导致"discards qualifiers"错误

### 8.2 内存管理

```cpp
// 使用智能指针避免内存泄漏
std::vector<std::unique_ptr<Animal>> animals;
zoo.addAnimal(std::make_unique<Lion>("Simba"));
```

### 8.3 前向声明

```cpp
// 解决循环依赖问题
class Lion;
class Tiger;
```

## 9. 实际应用场景

- **编译器**：AST节点访问（语法分析、代码生成）
- **文件系统**：文件/目录访问（统计、搜索、备份）
- **图形系统**：图形元素访问（渲染、导出、打印）
- **数据库**：查询计划访问（优化、执行）

## 10. 与其他模式的关系

- **组合模式**：访问者模式常用于访问组合模式构建的对象结构
- **迭代器模式**：访问者模式可以替代迭代器模式
- **命令模式**：访问者模式可以封装操作，类似命令模式

## 11. 总结

访问者模式通过将操作从对象结构中分离出来，实现了操作与数据结构的解耦。虽然增加了新元素时比较困难，但在需要频繁添加新操作的场景下，访问者模式提供了很好的解决方案。

**核心思想**：让操作"访问"对象，而不是让对象"执行"操作。
