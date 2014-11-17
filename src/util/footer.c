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