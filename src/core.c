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
	if (NULL != funct ){ 
		return (funct)(d);
	} else {
		return 0;
	}
}

// Run any mechanism as a float
float goFloat(Mechanism* d) {
	goFloatFuncPtr funct = d->class->lookup[1]; // TODO: Check if length of lookup is at least 2
	if (NULL != funct) {
		return (funct)(d);
	} else {
		return 0;
	}
}

// Free any mechanism
void mechFree(Mechanism* d) {
	freeFuncPtr funct = d->class->delete;
	if (NULL != funct) {
		(funct)(d);
	} else {
		// WARNING: Class does not contain a delete
	}
	free(d);
}

// ----------------------------------------------------------------------------
// Numeric Mechanism
// ----------------------------------------------------------------------------
typedef struct {
	long val;
} NumData;

void numFree(Mechanism* mech) {
	free (mech->data);
}

long numGoLong(Mechanism* mech) {
	NumData* data = (NumData*)mech->data;
	return data->val;
};

float numGoFloat(Mechanism* mech) {
	NumData* data = (NumData*)mech->data;
	return (float)data->val;
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
	SingleArgData* data = (SingleArgData*)mech->data;
	if (NULL != data) {
		mechFree(data->left);
	} else {
		// WARNING!!!
	}
	free (mech->data);
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
	if (NULL != data) {
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
	if (NULL != data) {
		return goLong(data->left) + goLong(data->right);
	} else {
		return 0;
	}
};

float addGoFloat(Mechanism* mech) {
	DualArgData* data = (DualArgData*)mech->data;
	if (NULL != data) {
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
	if (NULL != data) {
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
	if (NULL != data) {
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
