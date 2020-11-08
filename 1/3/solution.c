#include <linux/module.h>
#include <stdlib.h>
#include "checker.h"

int count_sum(short *arr, size_t n) {
	int sum = 0;
	int i;
	for (i = 0; i < n; i++) {
		sum += arr[i];
	}
	
	return(sum);
}

void result_from_array_sum(short *arr) {
	size_t n = sizeof(arr)/sizeof(arr[0]);
	int arrRealSum = count_sum(arr, n);
	int arrSum = array_sum(arr, n);
	char output[500];
	generate_output(arrSum, arr, n, output);
	if (arrRealSum == arrSum) {
		printk( KERN_INFO "kernel_mooc: %s\n", output );
	} else {
		printk( KERN_ERR "kernel_mooc: %s\n", output );
	}
}

short generate_array(int n)
{
	short my_array[n];
	int i;
	for (i = 0; i < n; i++) {
	    my_array[i] = rand();
	}
	return my_array;
}


int init_mdl(void) {
	CHECKER_MACRO;
	printk(KERN_INFO "kernel_mooc: init\n");

	short arr1 = generate_array(10);
	short arr2 = generate_array(5);
	short arr3 = generate_array(11);
	short arr4 = generate_array(8);
	short arr5 = generate_array(5);
	short arr6 = generate_array(10);
	short arr7 = generate_array(15);
	short arr8 = generate_array(9);
	short arr9 = generate_array(7);
	short arr10 = generate_array(20);
	short arr11 = generate_array(12);
	short arr12 = generate_array(13);
	short arr13 = generate_array(14);
	short arr14 = generate_array(15);
	short arr15 = generate_array(16);
	
	result_from_array_sum(arr1);
	result_from_array_sum(arr2);
	result_from_array_sum(arr3);
	result_from_array_sum(arr4);
	result_from_array_sum(arr5);
	result_from_array_sum(arr6);
	result_from_array_sum(arr7);
	result_from_array_sum(arr8);
	result_from_array_sum(arr9);
	result_from_array_sum(arr10);
	result_from_array_sum(arr11);
	result_from_array_sum(arr12);
	result_from_array_sum(arr13);
	result_from_array_sum(arr14);
	result_from_array_sum(arr15);

	return 0;
}

void cleanup_mdl(void) {
	CHECKER_MACRO;
	printk(KERN_INFO "kernel_mooc: cleanup\n");
}

module_init(init_mdl);
module_exit(cleanup_mdl);

MODULE_LICENSE("GPL");
