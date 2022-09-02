int main() {
	int i;
	int sum = 0;

	#pragma omp parallel for
	for(i = 0; i < 10; ++i){
		sum = sum + i;
	}
}