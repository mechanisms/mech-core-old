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
