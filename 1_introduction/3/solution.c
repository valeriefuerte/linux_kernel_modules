#include <linux/module.h>
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

int init_mdl(void) {
	CHECKER_MACRO;
	printk(KERN_INFO "kernel_mooc: init\n");

	short arr1[] = {1, 2, 3, 4, 5};
	short arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	short arr3[] = {9, 8, 7, 6, 5, 4};
	short arr4[] = {3, 2, 1, 9, 8, 7, 4, 5};
	short arr5[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	short arr6[] = {1, 9, 9, 1, 1, 9, 9, 1, 1, 9};
	short arr7[] = {2, 5, 3, 2, 5, 3, 2, 5, 3, 2, 5, 3, 2, 5, 3};
	short arr8[] = {4, 5, 6, 7, 8, 9, 4, 5, 6, 7, 8, 9};
	short arr9[] = {3, 2, 4, 3, 1, 2};
	short arr10[] = {6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
	short arr11[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	short arr12[] = {9, 8, 7, 6, 5, 4, 1, 1, 1, 1};
	short arr13[] = {1, 2};
	short arr14[] = {0, 1, 9, 8, 7, 6, 5, 5, 3, 3, 3, 3};
	short arr15[] = {5, 4, 1, 1, 1, 1, 5, 4, 2, 7, 2, 1, 3, 0};
	
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
