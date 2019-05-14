/*
Aeonni 2019.05
	Runtime: 0 ms, faster than 100.00%%
    Memory Usage: 2.3 MB, less than 80.00%
*/

/*
Given an array with n objects colored red, white or blue, sort them in-place so that objects of the same color are adjacent, with the colors in the order red, white and blue.

Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.

Note: You are not suppose to use the library's sort function for this problem.

Example:

Input: [2,0,2,1,1,0]
Output: [0,0,1,1,2,2]
Follow up:

A rather straight forward solution is a two-pass algorithm using counting sort.
First, iterate the array counting number of 0's, 1's, and 2's, then overwrite array with total number of 0's, then 1's and followed by 2's.
Could you come up with a one-pass algorithm using only constant space?
*/

func sortColors(nums []int) {
	defer func() { recover() }()

	zero_e := 0
	two_s := len(nums) - 1
	for nums[two_s] == 2 {
		two_s--
	}
	for nums[zero_e] == 0 {
		zero_e++
	}

	for i := 0; i <= two_s && two_s >= zero_e; i++ {
		if i < zero_e {
			i = zero_e
		}

		if nums[i] == 2 {
			nums[i], nums[two_s] = nums[two_s], nums[i]
		}
		if nums[i] == 0 {
			nums[i], nums[zero_e] = nums[zero_e], nums[i]
		}

		for nums[two_s] == 2 {
			two_s--
		}
		for nums[zero_e] == 0 {
			zero_e++
		}

	}
}