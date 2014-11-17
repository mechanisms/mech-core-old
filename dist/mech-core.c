// mech-core.c
// version: 0.1.1
// author: Eric Hosick <erichosick@gmail.com>
// license: MIT
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// See Mechanism below
typedef struct Mechanism Mechanism;

// Function signature to free a mechanism
typedef void (*freeFuncPtr)(Mechanism*);

// Class: Aggregates functions for a given type of Mechanism.
// id - globally unique identifier for the class
// delete - function pointer to free memory allocated for this class
// lookup - lookup table of function pointers.
//   0 - goLong
//   1 - goDouble
//   2 - ????
typedef struct {
	freeFuncPtr delete;
	void* lookup[];
} Class;

// Mechanism: Instance of a mechanism
// class - the class for this instance
// parent - contains the parent mechanism instance
// data - the data for this instance
struct Mechanism {
	Class* class;
	Mechanism* parent;
	void* data;
};

// Function signatures
typedef long (*goLongFuncPtr)(Mechanism*);
typedef float (*goFloatFuncPtr)(Mechanism*);

// Run any mechanism as a long
long goLong(Mechanism* d) {
	goLongFuncPtr funct = d->class->lookup[0]; // TODO: Check if length of lookup is at least 1
	if (funct){ 
		return (funct)(d);
	} else {
		return 0;
	}
}

// Run any mechanism as a float
float goFloat(Mechanism* d) {
	goFloatFuncPtr funct = d->class->lookup[1]; // TODO: Check if length of lookup is at least 2
	if (funct) {
		return (funct)(d);
	} else {
		return 0;
	}
}

// Free any mechanism
void mechFree(Mechanism* d) {
	if (d) {
		assert(d->class);
		assert(d->class->delete);
	  freeFuncPtr funct = d->class->delete;
		(funct)(d);
	  free(d);
	}
	assert(d); // For development
}

// ----------------------------------------------------------------------------
// Numeric Mechanism
// ----------------------------------------------------------------------------
typedef struct {
	long val;
} NumData;

void numFree(Mechanism* mech) {
	if (mech) {
		free (mech->data);
	}
}

long numGoLong(Mechanism* mech) {
	if (mech) {
		NumData* data = (NumData*)mech->data;
		return data->val;
	} else {
		return 0; // TODO: Define NaN
	}
};

float numGoFloat(Mechanism* mech) {
	if (mech) {
		NumData* data = (NumData*)mech->data;
		return (float)data->val;
	} else {
		return 0; // TODO: Define NaN	
	}
};

Class numClass = { &numFree, { &numGoLong, &numGoFloat} };

Mechanism* num(long d) {
	Mechanism* mech = malloc(sizeof(Mechanism));
	NumData* data = malloc(sizeof(NumData));
	data->val = d;
	mech->class = &numClass;
	mech->data = data;
	return mech;
}

// ----------------------------------------------------------------------------
// SingleArg base Mechanism
// ----------------------------------------------------------------------------
typedef struct {
	Mechanism* left;
} SingleArgData;

void singleArgFree(Mechanism* mech) {
	if (mech) {
		SingleArgData* data = (SingleArgData*)mech->data;
		if (data) {
			mechFree(data->left);
		} else {
			// WARNING!!!
		}
		free (mech->data);
	}
}

// ----------------------------------------------------------------------------
// DualArg base Mechanism
// ----------------------------------------------------------------------------
typedef struct {
	Mechanism* left;
	Mechanism* right;
} DualArgData;

void dualArgFree(Mechanism* mech) {
	DualArgData* data = (DualArgData*)mech->data;
	if (data) {
		mechFree(data->left);
		mechFree(data->right);
	} else {
		// WARNING!!!
	}
	free (mech->data);
}

// ----------------------------------------------------------------------------
// Add Mechanism
// ----------------------------------------------------------------------------

long addGoLong(Mechanism* mech) {
	DualArgData* data = (DualArgData*)mech->data;
	if (data) {
		return goLong(data->left) + goLong(data->right);
	} else {
		return 0;
	}
};

float addGoFloat(Mechanism* mech) {
	DualArgData* data = (DualArgData*)mech->data;
	if (data) {
		return goFloat(data->left) + goFloat(data->right);
	} else {
		return 0;
	}
};

Class addClass = { &dualArgFree, { &addGoLong, &addGoFloat} };

Mechanism* add(Mechanism* left, Mechanism* right) {
	Mechanism* mech = malloc(sizeof(Mechanism));
	DualArgData* data = malloc(sizeof(DualArgData));
	data->left = left;
	left->parent = mech;
	data->right = right;  
	right->parent = mech;
	mech->class = &addClass;
	mech->data = data;
	return mech;
}

// ----------------------------------------------------------------------------
// writeLn Mechanism
// ----------------------------------------------------------------------------

long writeLnGoLong(Mechanism* mech) {
	SingleArgData* data = (SingleArgData*)mech->data;
	if (data) {
		long result = goLong(data->left);
		printf("%li\n", result);
		return result;
	} else {
		printf("0\n");
		return 0;
	}
};

float writeLnGoFloat(Mechanism* mech) {
	SingleArgData* data = (SingleArgData*)mech->data;
	if (data) {
		float result = goFloat(data->left);
		printf("%f\n", result);
		return result;
	} else {
		printf("0.00\n");
		return 0;
	}
};

Class writeLnClass = { &singleArgFree, { &writeLnGoLong, &writeLnGoFloat} };

Mechanism* writeLn(Mechanism* text) {
	Mechanism* mech = malloc(sizeof(Mechanism));
	SingleArgData* data = malloc(sizeof(SingleArgData));
	data->left = text;
	text->parent = mech;
	mech->class = &writeLnClass;
	mech->data = data;
	return mech;
}

int main() {
	Mechanism* num23 = num(23);
	printf("num23 as a long is %li and float is %f.\n", goLong(num23), goFloat(num23));
	mechFree(num23);

	Mechanism* addTwo = add(num(12), num(5));
	printf("addTwo as a long is %li and float is %f.\n", goLong(addTwo), goFloat(addTwo));
	mechFree(addTwo);

	Mechanism* writeAdd =
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