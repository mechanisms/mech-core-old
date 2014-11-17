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
long goLong(Mechanism* mech) {
	if (mech) {
		assert(mech->class);
		assert(mech->class->lookup);
		assert(mech->class->lookup[0]);
		goLongFuncPtr funct = mech->class->lookup[0]; // TODO: Check if length of lookup is at least 1
		return (funct)(mech);
	} else {
		return 0; // TODO: NaN for C Lang
	}
}

// Run any mechanism as a float
float goFloat(Mechanism* mech) {
	if (mech) {
		assert(mech->class);
		assert(mech->class->lookup);
		assert(mech->class->lookup[0]);
		goFloatFuncPtr funct = mech->class->lookup[1]; // TODO: Check if length of lookup is at least 2
		return (funct)(mech);
	} else {
		return 0.0; // TODO: NaN for C Lang
	}
}

// Free any mechanism
void mechFree(Mechanism* mech) {
	if (mech) {
		assert(mech->class);
		assert(mech->class->delete);
	  freeFuncPtr funct = mech->class->delete;
		(funct)(mech);
	  free(mech);
	}
}

// ----------------------------------------------------------------------------
// Numeric Mechanism
// ----------------------------------------------------------------------------
typedef struct {
	long val;
} NumData;

void numFree(Mechanism* mech) {
	if (mech && mech->data) {
		free (mech->data);
	}
}

long numGoLong(Mechanism* mech) {
	if (mech && mech->data) {
		NumData* data = (NumData*)mech->data;
		return data->val;
	} else {
		return 0; // TODO: Define NaN
	}
};

float numGoFloat(Mechanism* mech) {
	if (mech && mech->data) {
		NumData* data = (NumData*)mech->data;
		return (float)data->val;
	} else {
		return 0; // TODO: Define NaN	
	}
};

Class numClass = { &numFree, { &numGoLong, &numGoFloat} };

Mechanism* num(long d) {
	Mechanism* mech = malloc(sizeof(Mechanism));
	if (mech) {
		NumData* data = malloc(sizeof(NumData));
		if (data) {
			data->val = d;
			mech->class = &numClass;
			mech->data = data;
		} else {
			free(mech); // TODO: Warning on out of memory
			mech = NULL;
		}
	}
	return mech;
}

// ----------------------------------------------------------------------------
// SingleArg base Mechanism
// ----------------------------------------------------------------------------
typedef struct {
	Mechanism* left;
} SingleArgData;

void singleArgFree(Mechanism* mech) {
	if (mech && mech->data) {
		SingleArgData* data = (SingleArgData*)mech->data;
		if (data->left) {
			mechFree(data->left);
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
	if (mech && mech->data) {
  	DualArgData* data = (DualArgData*)mech->data;
  	if (data->left) {
	  	mechFree(data->left);		
  	}
	  if (data->right) {
	  	mechFree(data->right);
  	}
	  free (mech->data);
	 }
}

// ----------------------------------------------------------------------------
// Add Mechanism
// ----------------------------------------------------------------------------

long addGoLong(Mechanism* mech) {
	if (mech && mech->data) {
  	DualArgData* data = (DualArgData*)mech->data;
		return goLong(data->left) + goLong(data->right);
	} else {
		return 0; // TODO: Define NaN	
	}
};

float addGoFloat(Mechanism* mech) {
	if (mech && mech->data) {
		DualArgData* data = (DualArgData*)mech->data;
		return goFloat(data->left) + goFloat(data->right);
	} else {
		return 0; // TODO: Define NaN	
	}
};

Class addClass = { &dualArgFree, { &addGoLong, &addGoFloat} };

Mechanism* add(Mechanism* left, Mechanism* right) {
	Mechanism* mech = malloc(sizeof(Mechanism));
	if (mech) {
		DualArgData* data = malloc(sizeof(DualArgData));
		if (data) {
			data->left = left;
			if (left) {
		  	left->parent = mech;
		  }
			data->right = right;  
			if (right) {
			  right->parent = mech;
			}
			mech->class = &addClass;
			mech->data = data;
		} else {
			free(mech); // TODO: Warning that memory allocation failed
			mech = NULL;
		}
	}
	return mech;
}

// ----------------------------------------------------------------------------
// writeLn Mechanism
// ----------------------------------------------------------------------------

long writeLnGoLong(Mechanism* mech) {
	if (mech && mech->data) {
		SingleArgData* data = (SingleArgData*)mech->data;
		long result = goLong(data->left);
		printf("%li\n", result);
		return result;
	} else {
		return 0; // TODO: Define NaN	
	}
};

float writeLnGoFloat(Mechanism* mech) {
	if (mech && mech->data) {	
		SingleArgData* data = (SingleArgData*)mech->data;
		float result = goFloat(data->left);
		printf("%f\n", result);
		return result;
	} else {
		return 0.0; // TODO: Define NaN	
	}
};

Class writeLnClass = { &singleArgFree, { &writeLnGoLong, &writeLnGoFloat} };

Mechanism* writeLn(Mechanism* text) {
	Mechanism* mech = malloc(sizeof(Mechanism));
	if (mech) {
		SingleArgData* data = malloc(sizeof(SingleArgData));
		if (data) {
			data->left = text;
			if (text) {
			  text->parent = mech;
			}
			mech->class = &writeLnClass;
			mech->data = data;
		} else {
			free(mech); // TODO: Warning that memory allocation failed
			mech = NULL;
		}
	}
	return mech;
}
