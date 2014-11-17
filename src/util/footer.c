int main() {
	Mechanism* num23 = num(23);
	printf("num23 as a long is %li and float is %f.\n", goLong(num23), goFloat(num23));
	mechFree(num23);

	Mechanism* addTwo = add(num(12), num(5));
	printf("addTwo as a long is %li and float is %f.\n", goLong(addTwo), goFloat(addTwo));
	mechFree(addTwo);

	Mechanism* addAlot =
		add(
			add(num(2), num(3)),
			add(num(4), num(5))
		);
	printf("addAlot as a long is %li and float is %f.\n", goLong(addAlot), goFloat(addAlot));
	mechFree(addAlot);

	return 0;
}