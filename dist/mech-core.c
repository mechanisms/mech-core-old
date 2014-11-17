// mech-core.c
// version: 0.1.1
// author: Eric Hosick <erichosick@gmail.com>
// license: MIT
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// NOTE: About assertions
//   assert(this->class); // ALL mechanisms must have a class
//   assert(this->class->delete);; // ALL classes must have delete
//   assert(this->data); // mechAlloc checks for this->data and this->data should NEVER be null
//   assert(class->dataSize > 0); // ALL classes must have a non zero data size.

// See Mechanism below
typedef struct MechanismStruct MechanismStruct;

typedef MechanismStruct* MechanismPtr;

// Function signatures
typedef void (*freeFuncPtr)(MechanismPtr); // Free a mechanism
typedef long (*goLongFuncPtr)(MechanismPtr);
typedef float (*goFloatFuncPtr)(MechanismPtr);
// ... Others to come

// Class: Aggregates functions to define a Mechanism.
// dataSize: size of data (not including references)
// Function Pointers:
// delete
// goLong
// goFloat
typedef struct {
	int dataSize;
	freeFuncPtr delete;
	goLongFuncPtr goLong;
  goFloatFuncPtr goFloat;
	// void* lookup[]; // TODO: Added lookup for extended behavior.
} Class;

// MechanismStruct: Structure that contains an instance of a mechanism
// class - the class for this instance
// parent - contains the parent mechanism instance
// data - the data for this instance
struct MechanismStruct {
	Class* class;
	MechanismPtr parent;
	void* data;
};

// Free the memory of any mechanism
void mechFree(MechanismPtr this)
{
	if(this) {
		assert(this->class);
		assert(this->class->delete);
		(this->class->delete)(this);
		free(this);
	}
}

// Allocate memory for any mechanism
MechanismPtr mechAlloc(Class* class) {
	MechanismPtr this = NULL;
	if(class) {
		this = malloc(sizeof(MechanismStruct));
		if(this) {
			assert(class->dataSize > 0);
			void* data = malloc(class->dataSize);
			if(data) {
				this->class = class;
				this->data = data;
			} else {
				free(this); // TODO: Warning that memory allocation failed
				this = NULL; // TODO: Create a null mechanism
			}
		}
	} // else TODO: Warning class not provided
	return this;
}

// Invoke a mechanism as a long primitive
long goLong(MechanismPtr this)
{
	long result = 0; // TODO: NaN for C Lang
	if(this) {
		assert(this->class);
		if(this->class->goLong)	{
			result = (this->class->goLong)(this);
		}
	}
	return result; 
}

// Invoke a mechanism as a float primitive
float goFloat(MechanismPtr this)
{
	float result = 0.0; // TODO: NaN for C Lang
	if(this) {
		assert(this->class);
		if(this->class->goFloat) {
			result = (this->class->goFloat)(this);
		}
	}
	return result;
}

// singleArg base Mechanism
typedef struct {
	MechanismPtr left;
} SingleArgData;

void singleArgFree(MechanismPtr this)
{
	if(this && this->data) {
		SingleArgData* data = (SingleArgData*)this->data;
		if(data->left) {
			mechFree(data->left);
		}
		free(this->data);
	} // no else - don't care if try to free empty data
}

// dualArg base Mechanism
typedef struct {
	MechanismPtr left;
	MechanismPtr right;
} DualArgData;

void dualArgFree(MechanismPtr this)
{
	if(this && this->data) {
		DualArgData* data = (DualArgData*)this->data;
		if(data->left) {
			mechFree(data->left);		
		}
		if(data->right) {
			mechFree(data->right);
		}
		free(this->data);
	} // no else - don't care if try to free empty data
}

// num Mechanism
typedef struct {
	long val;
} NumData;

void numFree(MechanismPtr this)
{
	if(this) {
		free(this->data);
	}
}

long numGoLong(MechanismPtr this)
{
	if(this) {
		assert(this->data);
		return ((NumData*)this->data)->val;
	} // else warn on this
	return 0; // TODO: Define NaN
};

float numGoFloat(MechanismPtr this)
{
	if(this) {
		assert(this->data);		
		return (float)((NumData*)this->data)->val;
	} // else warn on this
	return 0; // TODO: Define NaN
};

Class numClass = {sizeof(NumData), &numFree, &numGoLong, &numGoFloat};

MechanismPtr num(long d)
{
	MechanismPtr this = mechAlloc(&numClass);
	if(this) {
		assert(this->data);
		((NumData*)this->data)->val = d;
	} // No else because warning from mechAlloc
	return this;
}

// Add Mechanism
long addGoLong(MechanismPtr this)
{
	if(this) {
		assert(this->data);
		DualArgData* data = (DualArgData*)this->data;
		return goLong(data->left) + goLong(data->right);
	} else {
		return 0; // TODO: Define NaN	
	}
};

float addGoFloat(MechanismPtr this)
{
	if(this && this->data) {
		DualArgData* data = (DualArgData*)this->data;
		return goFloat(data->left) + goFloat(data->right);
	} else {
		return 0; // TODO: Define NaN	
	}
};

Class addClass = {sizeof(DualArgData), &dualArgFree, &addGoLong, &addGoFloat};

MechanismPtr add(MechanismPtr left, MechanismPtr right)
{
	MechanismPtr this = mechAlloc(&addClass);
	if(this) {
		assert(this->data);
		DualArgData* data = (DualArgData*)this->data;
		data->left = left;
		if(left) {
			left->parent = this;
		}
		data->right = right;  
		if(right) {
			right->parent = this;
		}
	} // No else because warning from mechAlloc
	return this;
}

// writeLn Mechanism
long writeLnGoLong(MechanismPtr this)
{
	if(this) {
		assert(this->data);
		SingleArgData* data = (SingleArgData*)this->data;
		long result = goLong(data->left);
		printf("%li\n", result);
		return result;
	} else {
		return 0; // TODO: Define NaN	
	}
};

float writeLnGoFloat(MechanismPtr this)
{
	if(this) {
		assert(this->data);
		SingleArgData* data = (SingleArgData*)this->data;
		float result = goFloat(data->left);
		printf("%f\n", result);
		return result;
	} else {
		return 0.0; // TODO: Define NaN	
	}
};

Class writeLnClass = {sizeof(SingleArgData), &singleArgFree, &writeLnGoLong, &writeLnGoFloat};

MechanismPtr writeLn(MechanismPtr text)
{
	MechanismPtr this = mechAlloc(&writeLnClass);
	if(this) {
		assert(this->data);
		SingleArgData* data = (SingleArgData*)this->data;
		data->left = text;
		if(text) {
			text->parent = this;
		}
	}
	return this;
}

int main() {
	MechanismPtr num23 = num(23);
	printf("num23 as a long is %li and float is %f.\n", goLong(num23), goFloat(num23));
	mechFree(num23);

	MechanismPtr addTwo = add(num(12), num(5));
	printf("addTwo as a long is %li and float is %f.\n", goLong(addTwo), goFloat(addTwo));
	mechFree(addTwo);

	MechanismPtr writeAdd =
	writeLn(
		add(
			add(num(2), num(3)),
			add(num(4), num(5))
		)
	);
	goLong(writeAdd);
	goFloat(writeAdd);


	mechFree(writeAdd);

	return 0;
}