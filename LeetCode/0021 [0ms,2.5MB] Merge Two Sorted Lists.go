/*
Aeonni 2019.05
	Runtime:  0ms, faster than 100.00%
    Memory Usage: 2.5 MB, less than 100.00%
*/

/*
Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.

Example:

Input: 1->2->4, 1->3->4
Output: 1->1->2->3->4->4
*/

/**
 * Definition for singly-linked list.
 * type ListNode struct {
 *     Val int
 *     Next *ListNode
 * }
 */
func mergeTwoLists(l1 *ListNode, l2 *ListNode) *ListNode {
	ret := l1
	bak := l2
	if l1 == nil {
		return l2
	}
	for l2 != nil {

		if l2.Val >= l1.Val {
			if l1.Next != nil {
				if l2.Val <= l1.Next.Val {
					bak = l2.Next
					l2.Next = l1.Next
					l1.Next = l2
					l2 = bak
				}
				l1 = l1.Next
			} else {
				l1.Next = l2
				return ret
			}
		} else {
			bak = l2.Next
			l2.Next = l1
			if l1 == ret {
				ret = l2
			}
			l1 = l2
			l2 = bak

		}
	}
	return ret
}