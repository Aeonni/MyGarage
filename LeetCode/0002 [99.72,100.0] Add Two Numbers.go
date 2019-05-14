/*
Aeonni 2019.05
	Runtime: 8 ms, faster than 99.72%
	Memory Usage: 4.7 MB, less than 100.00%
*/

/*
You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example:

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.
*/

/**
 * Definition for singly-linked list.
 * type ListNode struct {
 *     Val int
 *     Next *ListNode
 * }
 */

func addTwoNumbers(l1 *ListNode, l2 *ListNode) *ListNode {
	l1_b := l1

	for {
		l1.Val += l2.Val

		if l2.Next == nil {
			break
		}

		if l1.Next == nil {
			l1.Next = l2.Next
			break
		}

		l1 = l1.Next
		l2 = l2.Next
	}

	l1 = l1_b
	y := 0
	for {
		l1.Val += y
		y = l1.Val / 10
		if l1.Val >= 10 {
			l1.Val %= 10
		}
		if l1.Next == nil {
			if y != 0 {
				l1.Next = &ListNode{
					Val:  y,
					Next: nil,
				}
			}
			break
		}
		l1 = l1.Next
	}

	return l1_b
}