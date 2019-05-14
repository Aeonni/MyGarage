/*
Aeonni 2019.05
	Runtime: 12 ms, faster than 99.33%
    Memory Usage: 5.6 MB, less than 12.50%
*/

/*
Given n non-negative integers a1, a2, ..., an , where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water.

Note: You may not slant the container and n is at least 2.





The above vertical lines are represented by array [1,8,6,2,5,4,8,3,7]. In this case, the max area of water (blue section) the container can contain is 49.



Example:

Input: [1,8,6,2,5,4,8,3,7]
Output: 49
*/

func maxArea(height []int) int {
	pl, pr := 0, len(height)-1
	maxarea := 0
	cuarea := 0

	for pl != pr {

		if height[pl] < height[pr] {
			cuarea = height[pl] * (pr - pl)
			if cuarea > maxarea {
				maxarea = cuarea
			}
			pl++
		} else {
			cuarea = height[pr] * (pr - pl)
			if cuarea > maxarea {
				maxarea = cuarea
			}
			pr--
		}
	}
	return maxarea
}