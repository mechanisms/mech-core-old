

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
	int id;
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

Class numClass = { 1, &numFree, { &numGoLong, &numGoFloat} };

Mechanism* num(long d) {
	NumData* data = malloc(sizeof(NumData));
	data->val = d;
	Mechanism* mechInstance = malloc(sizeof(Mechanism));
	mechInstance->class = &numClass;
	mechInstance->data = data;
	return mechInstance;
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

Class addClass = { 2, &dualArgFree, { &addGoLong, &addGoFloat} };

Mechanism* add(Mechanism* left, Mechanism* right) {
	DualArgData* data = malloc(sizeof(DualArgData));
	data->left = left;
	data->right = right;  
	Mechanism* mechInstance = malloc(sizeof(Mechanism));
	mechInstance->class = &addClass;
	mechInstance->data = data;
	return mechInstance;
}


