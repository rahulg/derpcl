__kernel void test1(int a, int b, __global int *c) {
	*c = a * b;
}

__kernel void test2(int a, int b, __global int *c) {
	*c = a / b;
}