// The Initial Design

// --------------------------------------------------
// Shared between all classes/objects/functions
// --------------------------------------------------

// object/function struct
typedef struct {
    int data_id; // maybe we need this so we know what is in data????
    void* class; // array of function ptrs: goInt, goFloat
    void* data; // data for any given mechanism
} Mech;

// All the function signatures: 1 for each primitive result we want to invoke as
int (*goIntPtr)(Mech*);
float (*goFloatPtr)(Mech*);

// invoke returning an int primitive
int goInt(Mech* mech) {
   return (*mech->class[0])(mech); // invoke function ptr at 0: always goInt
};

// invoke returning a float primitive
float goFloat(Mech* mech) {
   return mech->class[1](mech); // invoke function ptr at 1: always goFloat
};

// Integer Mechanism

typedef struct {
  int _val;
} IntData;

int intGoInt(Mech* mech) {
  IntData* intData = (IntData*)mech->data;
  return intData._val;
};

int intGoFloat(Mech* mech) {
  IntData* intData = (IntData*)mech->data;
  return (float)intData._val;
};

// setup the Integer Mechanism "class"
void* intClass() {
  void* class[] = { &int_goInt, &int_goFloat}; // pos 0 always goInt, pos1 always goFloat
  return class;
}

// constructor for Integer Mechanism
Mech* intCreate(int d) {
   Mech* mech =    
   IntMech* intData = (IntMech*)mech->data;
}

