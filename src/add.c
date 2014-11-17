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
