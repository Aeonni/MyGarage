'''
Aeonni 2019.05
	Runtime: 32 ms, faster than 99.85%
    Memory Usage: 14.9 MB
'''

'''
Given an array of integers, return indices of the two numbers such that they add up to a specific target.

You may assume that each input would have exactly one solution, and you may not use the same element twice.

Example:

Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
'''

class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        m = {}
        for i, v in enumerate(nums):
            m[v] = i
        for i, each in enumerate(nums):
            c = target - each
            g = m.get(c) 
            if g is not None and g != i:
                return [i, g]
                