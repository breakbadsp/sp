#include <cassert>

#include "shared_ptr.hpp"
#include "TestSharedPtr.hpp"

struct Test {};
sp::shared_ptr<Test> global;


struct Temp
{
    Temp() = default;
    ~Temp() { delete a; } 

    Temp(int p_a) : a(new int(p_a)) {}

    Temp(const Temp& p_rhs)
    : a(new int(*p_rhs.a)) {
    }
    
    Temp& operator=(const Temp& p_rhs) {
        if(this == &p_rhs)
            return *this;

        a = new int(*p_rhs.a);
        return *this;
    }

    void Reset(int p_a) {
        *a = p_a;
    }

    int get_data() {
        return *a;
    }

    int* a = nullptr;
};


void TestSP() {
    sp::shared_ptr<Temp> op1(new Temp(1));
    assert(op1.use_count() == 1);
    assert(op1.get() != nullptr);
    std::cout << op1.get() << ' ' << op1->get_data() << ' ' << op1.use_count() << '\n';

    sp::shared_ptr<Temp> op2(op1);
    assert(op1.use_count() == 2);
    assert(op2.use_count() == 2);
    assert(op1.get() != nullptr);
    assert(op2.get() != nullptr);
    std::cout << op2.get() << ' ' << op2->get_data() << ' ' << op2.use_count() << '\n';

    sp::shared_ptr<Temp> op3;
    //assert(op3.use_count() == 0);
    assert(op3.get() == nullptr);
    
    op3 = op1;
    assert(op3.use_count() == 3);
    assert(op3.get() != nullptr);
    std::cout << op3.get() << ' ' << op3->get_data() << ' ' << op3.use_count() << '\n';

    sp::shared_ptr<Temp> opt(new Temp(1));
    sp::shared_ptr<Temp> op4(std::move(opt));
    assert(op4.use_count() == 1);
    assert(op4.get() != nullptr);
    //assert(opt.use_count() == 0);
    assert(opt.get() == nullptr);

    std::cout << op4.get() << ' ' << op4->get_data() << ' ' << op4.use_count() << '\n';

    
    sp::shared_ptr<Temp> opt1(new Temp(1));
    sp::shared_ptr<Temp> op5;
    op5 = std::move(opt1);
    assert(op5.use_count() == 1);
    assert(op5.get() != nullptr);
    //assert(opt.use_count() == 0);
    assert(opt1.get() == nullptr);
    std::cout << op5.get() << ' ' << op5->get_data() << ' ' << op5.use_count() << '\n';

    op5.reset(nullptr);
    assert(op5.get() == nullptr);
    op5.reset(new Temp(1));
    assert(op5.get() != nullptr);
    assert(op5.use_count() == 1);
}



void TestSharedPtr()
{
  sp::shared_ptr sp_test(new Test());
  sp::shared_ptr sp2 = sp_test;
  sp::shared_ptr sp3 = sp_test;
  sp::shared_ptr sp4 = sp3;
  //global = sp3;//this will leak so expected.
  TestSP();

  std::cout << "sp::shared_ptr test finished!\n";
}
