#include <iostream>
#include <algorithm>
using namespace std;

class Item
{
public:
    Item(string name, int price) : name_(name), price_(price) {}

    virtual void PrintInfo() const
    {
        cout << "[이름: " << name_ << ", 가격: " << price_ << "G]" << endl;
    }

    virtual ~Item() {}
    virtual Item* Clone() const = 0;

    string GetName() const
    {
        return name_;
    }
    int GetPrice() const
    {
        return price_;
    }

protected:
    string name_;
    int price_;
};

class Weapon : public Item
{
public:
    Weapon(string name, int price) : Item(name, price) {}
    Item* Clone() const override
    {
        return new Weapon(*this);
    }
};

class Potion : public Item
{
public:
    Potion(string name, int price) : Item(name, price) {}
    Item* Clone() const override
    {
        return new Potion(*this);
    }
};

template <typename T>
class Inventory
{
public:
    Inventory(int capacity = 10) : capacity_(capacity), size_(0)
    {
        if (capacity_ <= 0)
            capacity_ = 1;

        pItems_ = new T[capacity_];
        for (int i = 0; i < capacity_; i++)
            pItems_[i] = nullptr;
    }

    Inventory(const Inventory<T>& other)
    {
        capacity_ = other.capacity_;
        size_ = other.size_;
        pItems_ = new T[capacity_];
        for (int i = 0; i < size_; ++i)
        {
            pItems_[i] = other.pItems_[i]->Clone();
        }
        for (int i = size_; i < capacity_; i++)
            pItems_[i] = nullptr;
    }

    ~Inventory()
    {
        Clear();
        delete[] pItems_;
        pItems_ = nullptr;
    }

    Inventory<T>& operator=(const Inventory<T>& other)
    {
        if (this == &other)
            return *this;

        Clear();
        delete[] pItems_;

        capacity_ = other.capacity_;
        size_ = other.size_;
        pItems_ = new T[capacity_];
        for (int i = 0; i < size_; i++)
            pItems_[i] = other.pItems_[i]->Clone();
        for (int i = size_; i < capacity_; i++)
            pItems_[i] = nullptr;

        return *this;
    }

    void AddItem(const T& item)
    {
        if (size_ == capacity_)
        {
            cout << "인벤토리가 꽉 차서 인벤토리의 용량을 2배 늘립니다." << endl;
            Resize(capacity_ * 2);
        }

        pItems_[size_] = item->Clone();
        size_++;
    }

    void RemoveLastItem()
    {
        if (size_ > 0)
        {
            int lastIndex = size_ - 1;
            if (pItems_[lastIndex] != nullptr)
            {
                delete pItems_[lastIndex];
                pItems_[lastIndex] = nullptr;
            }
            --size_;
        }
        else
        {
            cout << "인벤토리가 비어있습니다." << endl;
        }
    }

    int GetSize() const { return size_; }
    int GetCapacity() const { return capacity_; }

    void PrintAllItems() const
    {
        for (int i = 0; i < size_; i++)
        {
            if (pItems_[i])
                pItems_[i]->PrintInfo();
        }
    }

    static bool CompareItems(Item* a, Item* b)
    {
        if (a->GetPrice() == b->GetPrice())
        {
            return a->GetName() < b->GetName();
        }

        return a->GetPrice() < b->GetPrice();
    }

    void SortItems()
    {
        std::sort(pItems_, pItems_ + size_, CompareItems);
    }

private:
    void Clear()
    {
        for (int i = 0; i < size_; i++)
        {
            delete pItems_[i];
            pItems_[i] = nullptr;
        }
        size_ = 0;
    }

    void Resize(int newCapacity)
    {
        T* tempItems = new T[newCapacity];
        for (int i = 0; i < newCapacity; i++)
            tempItems[i] = nullptr;

        for (int i = 0; i < size_; i++)
            tempItems[i] = pItems_[i];

        delete[] pItems_;
        pItems_ = tempItems;
        capacity_ = newCapacity;
    }

    T* pItems_;
    int capacity_;
    int size_;
};

int main()
{
    Inventory<Item*> inv;
    inv.AddItem(new Weapon("Sword", 1000));
    inv.AddItem(new Weapon("Bow", 2000));
    inv.AddItem(new Weapon("Sword", 1000));
    inv.AddItem(new Weapon("Bow", 2000));
    inv.AddItem(new Weapon("Sword", 1000));
    inv.AddItem(new Weapon("Bow", 2000));
    inv.AddItem(new Potion("HP Potion", 1500));
    inv.AddItem(new Potion("MP Potion", 1500));
    inv.AddItem(new Potion("HP Potion", 1500));
    inv.AddItem(new Potion("MP Potion", 1500));
    inv.AddItem(new Potion("HP Potion", 1500));
    inv.AddItem(new Potion("MP Potion", 1500));

    inv.PrintAllItems();
    cout << "size: " << inv.GetSize() << ", capacity: " << inv.GetCapacity() << endl;

    cout << "\n마지막 아이템 삭제" << endl;
    inv.RemoveLastItem();

    inv.PrintAllItems();
    cout << "size: " << inv.GetSize() << ", capacity: " << inv.GetCapacity() << endl;

    inv.SortItems();
    inv.PrintAllItems();
}
