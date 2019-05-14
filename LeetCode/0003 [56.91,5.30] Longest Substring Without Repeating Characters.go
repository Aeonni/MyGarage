/*
Aeonni 2019.05
	Runtime: 12 ms, faster than 56.91%
	Memory Usage: 9 MB, less than 5.30%
*/

/*
Given a string, find the length of the longest substring without repeating characters.

Example 1:

Input: "abcabcbb"
Output: 3
Explanation: The answer is "abc", with the length of 3.
Example 2:

Input: "bbbbb"
Output: 1
Explanation: The answer is "b", with the length of 1.
Example 3:

Input: "pwwkew"
Output: 3
Explanation: The answer is "wke", with the length of 3.
             Note that the answer must be a substring, "pwke" is a subsequence and not a substring.
*/

func lengthOfLongestSubstring_1(arr []byte, win_size int) int {

	if len(arr) == 1 {
		return 1
	}
	if len(arr) < win_size {
		return len(arr)
	}

	for i := 0; i < len(arr)-win_size+1; i++ {
		if arr[i] == arr[i+win_size-1] {

			a := lengthOfLongestSubstring_1(arr[:i+win_size-1], win_size)
			b := lengthOfLongestSubstring_1(arr[i+1:], win_size)
			if a > b {
				return a
			} else {
				return b
			}

		}

	}

	return lengthOfLongestSubstring_1(arr, win_size+1)

}

func lengthOfLongestSubstring(s string) int {
	return lengthOfLongestSubstring_1([]byte(s), 2)
}