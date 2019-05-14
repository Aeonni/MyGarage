/*
Aeonni 2019.05
	Runtime: 1196 ms ms, faster than 24.68%
    Memory Usage: 205.5 MB, less than 94.85%
*/

/*
Given an array nums of n integers, are there elements a, b, c in nums such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.

Note:

The solution set must not contain duplicate triplets.

Example:

Given array nums = [-1, 0, 1, 2, -1, -4],

A solution set is:
[
  [-1, 0, 1],
  [-1, -1, 2]
]
*/

import "sort"

func threeSum(nums []int) [][]int {
	ret := make([][]int, 0)
	if len(nums) < 3 {
		return ret
	}

	sort.Sort(sort.IntSlice(nums))
	m := make(map[int]int, 10)

	num_zeros := 0
	for p := 0; p < len(nums); p++ {
		m[nums[p]] = p
		if nums[p] == 0 {
			num_zeros++
		}
	}

	if num_zeros >= 3 {
		ret = append(ret, []int{0, 0, 0})
	}

	pn, pp := 0, len(nums)-1
	need := 0

	if nums[pn] == nums[pp] && nums[pn] == 0 {
		return [][]int{[]int{0, 0, 0}}
	} else if nums[pn] > 0 || nums[pp] < 0 {
		return ret
	}

	for nums[pp] >= 0 {
	L1:
		for nums[pn] < 0 {
			need = 0 - nums[pn] - nums[pp]
			if v, flag := m[need]; flag && v != pn && v != pp {
				ans := []int{nums[pn], nums[pp], nums[v]}
				sort.Sort(sort.IntSlice(ans))
				for i := 0; i < len(ret); i++ {
					if ans[0] == ret[i][0] && ans[1] == ret[i][1] {
						pn++
						continue L1
					}
				}
				ret = append(ret, ans)
			}
			pn++
		}
		if pn >= pp {
			break
		}
		pn = 0
		pp--
	}
	return ret
}