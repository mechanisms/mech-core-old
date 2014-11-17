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
