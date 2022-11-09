#include <cassert>
#include <iostream>

// TODO
// Ej gjort ordentligt, riktigt svår
void test()
{
    std::cout << "Function call!" << std::endl;
}

int add(int a, int b)
{
    return a + b;
}

struct Test
{
    void operator()()
    {
        std::cout << "Function object call!" << std::endl;
    }
};

struct Multiply
{
    int operator()(int a, int b)
    {
        return a * b;
    }
};

template <typename T>
class Function
{
};

template <typename Ret, typename... Args>
class Function<Ret(Args...)>
{
public:
    template <typename T>
    Function(T &&callback)
        : storage{new Callable<T>{std::forward<T>(callback)}}
    {
    }
    Ret operator()(Args... list)
    {
        return storage->call(list...);
    }
    template <typename T>
    Function &operator=(T &&callback)
    {
        delete storage;
        storage = new Callable<T>{std::forward<T>(callback)};
        return *this;
    }

private:
    struct Callable_Base
    {
        virtual Ret call(Args...) = 0;
    };

    template <typename T>
    class Callable : public Callable_Base
    {
    public:
        Callable(T &&callback) : callback{std::forward<T>(callback)} {}
        Ret call(Args... list) override
        {
            return callback(list...);
        };

        T callback;
    };

private:
    Callable_Base *storage{nullptr};
};

int main()
{
    /* Expected output:

     ==== Testcase 1: void() ====
     Function call!
     Function object call!
     x = 5
     ==== Testcase 2: int(int, int) ====
     7 * 4 = 28
     3 + 5 = 8
     11 - 5 = 6

     */

    std::cout << "==== Testcase 1: void() ====" << std::endl;
    {
        Function<void()> fun{test};
        fun();

        fun = Test{};
        fun();

        int x{5};
        fun = [&x]()
        { std::cout << "x = " << x << std::endl; };
        fun();
    }

    std::cout << "==== Testcase 2: int(int, int) ====" << std::endl;
    {
        Function<int(int, int)> fun{Multiply{}};

        std::cout << "7 * 4 = " << fun(7, 4) << std::endl;

        fun = add;
        std::cout << "3 + 5 = " << fun(3, 5) << std::endl;

        fun = [](int a, int b)
        { return a - b; };
        std::cout << "11 - 5 = " << fun(11, 5) << std::endl;
    }
}