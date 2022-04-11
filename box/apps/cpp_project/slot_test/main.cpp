#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;
class A{
    public:
        void Clicked(int id)
        {
            cout << "A::Clicked" << endl;	
        }
};

class B{
    public:
        void Clicked(int id)
        {
            cout << "B::Clicked" << endl;	
        }
};

class SlotBase{
    public:
        virtual void OnSignal(int id) = 0;
};

template<class T>
class Slot : public SlotBase{
    public:
        typedef void (T::*MemberFuncType)(int); 
        Slot(T* obj, MemberFuncType func)
            :obj_(obj), func_(func)
        {
        }

        virtual void OnSignal(int id)
        {
            (obj_->*func_)(id);
        }
    private:
        T* obj_;
        MemberFuncType func_;
};

template<class T>
SlotBase* MakeSlot(T * p, void (T::*func)(int))
{
    return new Slot<T>( p, func ) ; 
}

class Signal
{
    public:
        virtual ~Signal()
        {
            std::for_each(slots_.begin(), slots_.end(), SignalDelete());
        }
        
        struct SignalDelete: public unary_function<SlotBase*, void>
        {
            public:
                void operator()(SlotBase*& slot) const
                {
                    delete slot;
                }
        };
        
        struct SignalAction: public binary_function<SlotBase*, int, void>
        {
            public:
                void operator()(SlotBase*& slot, const int& i) const
                {
                    slot->OnSignal(i);
                }
        };
        
        void signal(int id)
        { 
            std::for_each(slots_.begin(), slots_.end(), bind2nd(SignalAction(), id));
        }
        
        void connect( SlotBase* s) 
        {
            slots_.push_back(s); 
        }
        
        protected:
            vector< SlotBase* >   slots_;
};

int main(int argc, char* argv[])
{
    A a;
    B b;
    Signal signal;
    signal.connect(MakeSlot(&a, &A::Clicked));
    signal.connect(MakeSlot(&b, &B::Clicked));
    signal.signal(2);
    return 0;
}

