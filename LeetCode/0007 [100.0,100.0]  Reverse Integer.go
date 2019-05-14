/*
Aeonni 2019.05
	Runtime:  0ms, faster than 100.00%
    Memory Usage:  2.2MB, less than 100.00%
*/

/*
Given a 32-bit signed integer, reverse digits of an integer.

Example 1:

Input: 123
Output: 321
Example 2:

Input: -123
Output: -321
Example 3:

Input: 120
Output: 21
Note:
Assume we are dealing with an environment which could only store integers within the 32-bit signed integer range: [−231,  231 − 1]. For the purpose of this problem, assume that your function returns 0 when the reversed integer overflows.
*/

func reverse(x int) int {
	y := 0
	m := 1
	for x/m != 0 {
		m *= 10
	}

	for i := 10; i <= m; i *= 10 {
		n := x % i / (i / 10)
		y *= 10
		y += n
	}

	if y <= -2147483648 || y >= 2147483647 {
		return 0
	}
	return y
}