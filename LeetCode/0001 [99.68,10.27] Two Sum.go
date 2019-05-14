/*
Aeonni 2019.05
	Runtime: 4 ms, faster than 99.68%
    Memory Usage: 3.8 MB, less than 10.27%
*/

/*
Given an array of integers, return indices of the two numbers such that they add up to a specific target.

You may assume that each input would have exactly one solution, and you may not use the same element twice.

Example:

Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
*/

func twoSum(nums []int, target int) []int {
	m := make(map[int]int, 10)
	for i := 0; i < len(nums); i++ {
		m[nums[i]] = i
	}
	for i := 0; i < len(nums); i++ {
		c := target - nums[i]
		n, flag := m[c]
		if flag && n != i {
			return []int{i, n}
		}
	}
	return []int{}
}