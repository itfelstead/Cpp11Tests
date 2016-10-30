class MyClass
{
    public:
        // Rule of 3: If class needs to define any of the following,
        // then it should define them all;
        //  - non-default DTOR
        //  - Copy CTOR
        //  - Copy Assignment Operator
        MyClass();
        MyClass( char *somePtr_ );
        ~MyClass();                              // 1. DTOR

        MyClass( const MyClass & );               // 2. Copy ctor
        MyClass& operator=( const MyClass & );     // 3. Copy Assignment 

        // Rule of 5 (C+=11)... to support Moves in C++ you'll also need;
        //  - Move Constructor 
        //  - Move Assignment Operator
        MyClass( MyClass&& ) noexcept;            // 4. Move Constructor
        MyClass& operator=( MyClass&& ) noexcept; // 5. Move assignment operator

        void Dump( void ) const;

    private:
        char *m_SomePtr = nullptr;
};