/*
Aeonni 2019.05
	Runtime:  0ms, faster than 100.00%
    Memory Usage:  2.1MB, less than 100.00%
*/

/*
Implement pow(x, n), which calculates x raised to the power n (xn).

Example 1:

Input: 2.00000, 10
Output: 1024.00000
Example 2:

Input: 2.10000, 3
Output: 9.26100
Example 3:

Input: 2.00000, -2
Output: 0.25000
Explanation: 2-2 = 1/22 = 1/4 = 0.25
Note:

-100.0 < x < 100.0
n is a 32-bit signed integer, within the range [−231, 231 − 1]

 */

// 递归 0ms 2.1MB
func myPow(x float64, n int) float64 {

	if n == 0 {
		return 1
	} else if n == 1 {
		return x
	} else if n < 0 {
		x = myPow(x, -n)
		return 1 / x
	}

	if n%2 == 1 {
		x = myPow(x*x, n/2) * x
	} else {
		x = myPow(x*x, n/2)
	}

	return x
}

// 递归展开 0ms 2.1MB
func myPow(x float64, n int) float64 {
    mul := 1.0
    flag := n
    if n == 0 {
        return 1
    }else if n < 0 {
        n = -n
    }
    for n/2 > 0 {
        if n%2 > 0 {
            mul*=x
        }
        x *= x
        n/=2
    }
    if flag < 0 {
        return 1/(x*mul)
    }
    return x*mul
}


// math.Pow 0ms 2.0MB

import "math"
func myPow(x float64, n int) float64 {
    return math.Pow(x,float64(n))
}